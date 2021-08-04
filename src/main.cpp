#include <iostream>
#include <vector>

#include <GL/glew.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL3_IMPLEMENTATION
#include "modules/3rd_party/nuklear/nuklear.h"
#include "modules/3rd_party/nuklear/nuklear_sdl_gl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "modules/3rd_party/stb/stb_image.h"

#define WINDOW_WIDTH 1100
#define WINDOW_HEIGHT 700

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define UI_MENUS

#ifdef UI_MENUS
    #include "UI/TransformUI.c"
#endif

#include "Camera.h"

#include "modules/LinearAlgebra.h"
#include "modules/FileLoaders.h"


using namespace std;

static void FatalError(const string &msg)
{
    cout << "[FATAL ERROR] " << msg << "\n"
         << "Press any key to exit... " << endl;

    char s;
    cin >> s ;

    SDL_Quit();
    exit(1);
}

#define GLCheck(x) GLClearErrors(); x; GLPrintErrors(__FILE__, #x, __LINE__);

static void GLClearErrors() { while (glGetError() != GL_NO_ERROR); }

static void GLPrintErrors(const char* file, const char* func, int line)
{
    bool onError = false;

    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        cout << "[OPENGL ERROR] " << file << " - line: " << line << " " << func << " (Error code " << error << ")." << endl;
        onError = true;
    }

    if (onError) FatalError("OpenGL error detected");
}

static void CompileShader(const char* src, unsigned int id)
{
    GLCheck(glShaderSource(id, 1, &src, nullptr));
    GLCheck(glCompileShader(id));

    int status;
    GLCheck(glGetShaderiv(id, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE)
    {
        int len;
        GLCheck(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));

        vector<char> log(len);
        GLCheck(glGetShaderInfoLog(id, len, &len, &log[0]));

        cout << "[SHADER INFO LOG] " << &log[0] << endl;
        FatalError("Shader compilation failed.");
    }
}

static unsigned int LoadTexture(const char* path, unsigned short slot = 0)
{
    stbi_set_flip_vertically_on_load(1);
    int imgW, imgH, comp;
    unsigned char* image = stbi_load(path, &imgW, &imgH, &comp, STBI_rgb_alpha);
    cout << "\n" << imgW << "x" << imgH << "\n" << *image << endl;

    GLCheck(glActiveTexture(GL_TEXTURE0 + slot));

    unsigned int texID = 0;
    
    GLCheck(glGenTextures(1, &texID));
    GLCheck(glBindTexture(GL_TEXTURE_2D, texID));
    
    GLCheck(glTextureParameteri(texID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCheck(glTextureParameteri(texID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCheck(glTextureParameteri(texID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCheck(glTextureParameteri(texID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imgW, imgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, image));

    stbi_image_free(image);
    return texID;
}


int main()
{
    int width, height;
    SDL_Window* window;
    SDL_GLContext context;
    struct nk_context *ctx;
    bool running = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) FatalError("SDL INIT - FAILED");

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    window = SDL_CreateWindow("OpenGL 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) FatalError("Window missing.");

    SDL_GetWindowSize(window, &width, &height);
    
    context = SDL_GL_CreateContext(window);
    if (!context) FatalError("GLContext missing.");

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) FatalError("GLEW INIT - FAILED");

    ctx = nk_sdl_init(window);
    {struct nk_font_atlas *atlas;
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();}

    if (SDL_GL_SetSwapInterval(1) == 0) 
        cout << "Vsync: ON" << endl;
    else 
        cout << "Vsync: OFF" << endl;

    GLCheck(glClearColor(0.4, 0.1, 0.7, 1.0));
    GLCheck(glEnable(GL_DEPTH_TEST));

    std::vector<float> v;
    std::vector<unsigned int> t;
    unsigned int numVerts, numTris;
    fLoaders::OBJLoader("objs/buso.obj", &v, &t, &numVerts, &numTris);

    unsigned int vaoID = 0, vboID = 0, iboID = 0;

    GLCheck(glGenVertexArrays(1, &vaoID));
    GLCheck(glBindVertexArray(vaoID));

    GLCheck(glGenBuffers(1, &vboID));
    GLCheck(glBindBuffer(GL_ARRAY_BUFFER, vboID));
    GLCheck(glBufferData(GL_ARRAY_BUFFER, 8  * numVerts * sizeof(float), &v[0], GL_STATIC_DRAW));

    GLCheck(glEnableVertexAttribArray(0));
    GLCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0));

    GLCheck(glEnableVertexAttribArray(1));
    GLCheck(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float))));

    GLCheck(glGenBuffers(1, &iboID));
    GLCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID));
    GLCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * numTris * sizeof(unsigned int), &t[0], GL_STATIC_DRAW));

    const char* vertexSrc = R"glsl(
        #version 330
        layout (location=0) in vec3 position;
        layout (location=1) in vec2 uv;

        uniform mat4 u_Model;
        uniform mat4 u_View;
        uniform mat4 u_Proj;

        out vec2 v_UV;

        void main()
        {
            gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);
            v_UV = uv;
        }
    )glsl";

    const char* fragmentSrc = R"glsl(
        #version 330
        in vec2 v_UV;

        uniform vec4 u_Color;
        uniform sampler2D u_Diffuse;

        out vec4 outColor;

        void main()
        {
            vec4 diff = texture(u_Diffuse, v_UV);
            outColor = diff;
        }
    )glsl";

    unsigned int vertexID, fragmentID, glProgramID;

    vertexID = glCreateShader(GL_VERTEX_SHADER);
    CompileShader(vertexSrc, vertexID);

    fragmentID =glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(fragmentSrc, fragmentID);

    glProgramID = glCreateProgram();
    GLCheck(glAttachShader(glProgramID, vertexID));
    GLCheck(glAttachShader(glProgramID, fragmentID));
    GLCheck(glLinkProgram(glProgramID));

    int status;
    GLCheck(glGetProgramiv(glProgramID, GL_LINK_STATUS, &status));
    if (status == GL_FALSE)
    {
        int len;
        GLCheck(glGetProgramiv(glProgramID, GL_INFO_LOG_LENGTH, &len));
        
        vector<char> log(len);
        GLCheck(glGetProgramInfoLog(glProgramID, len, &len, &log[0]));

        cout << "[PROGRAM INFO LOG]" << &log[0] << endl;

        GLCheck(glDeleteProgram(glProgramID));
        GLCheck(glDeleteShader(vertexID));
        GLCheck(glDeleteShader(fragmentID));

        FatalError("Program shader failed at stratup.");
    }
    
    GLCheck(glDetachShader(glProgramID, vertexID));
    GLCheck(glDeleteShader(vertexID));

    GLCheck(glDetachShader(glProgramID, fragmentID));
    GLCheck(glDeleteShader(fragmentID));

    GLCheck(glUseProgram(glProgramID));

    Camera cam(35, {0.980, 0.735}, {width, height}, 0.1, 1000);
    //cam.transform.set_position({1.5,0,1.5});
    //cam.transform.LookAt({0,0,0});
    cam.transform.set_position({2.5,2.3,3.5});
    cam.transform.LookAt({0.0,0.8,0.0});

    Transform transform;

    
    int modelLocation = glGetUniformLocation(glProgramID, "u_Model");
    if (modelLocation == -1) cout << "No matching uniform" << endl;
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, transform.LocalToWorld().toPtr());

    int viewLocation = glGetUniformLocation(glProgramID, "u_View");
    if (viewLocation == -1) cout << "No matching unifrom" << endl;
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, cam.WorldToCamera().toPtr());

    int projLocation = glGetUniformLocation(glProgramID, "u_Proj");
    if (projLocation == -1) cout << "No matching unifrom" << endl;
    glUniformMatrix4fv(projLocation, 1, GL_FALSE, cam.ProjectionMatrix().toPtr());


    unsigned int texID = LoadTexture("imgs/Buso_Diff.png");

    int colorLocation = glGetUniformLocation(glProgramID, "u_Color");
    if (colorLocation == -1) cout << "No matching uniform" << endl;
    GLCheck(glUniform4f(colorLocation, 0.6, 0.5, 0.2, 1.0));

    int diffLocation = glGetUniformLocation(glProgramID, "u_Diffuse");
    if (diffLocation == -1) cout << "No matching uniform" << endl;
    GLCheck(glUniform1i(diffLocation, 0));


    unsigned int lastTime = 0, currentTime = 0;
    float deltaTime = 0;

    while (running)
    {
        lastTime = currentTime;
        currentTime = SDL_GetPerformanceCounter();
        deltaTime = (currentTime - lastTime) / (float)SDL_GetPerformanceFrequency();

        SDL_Event event;
        nk_input_begin(ctx);
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    running = false;
                    break;
            }
            nk_sdl_handle_event(&event);
        }
        nk_input_end(ctx);

        transform.Rotate(0, 15 *  deltaTime, 0);
        //cout << transform.get_rotation() << endl;

        GLCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GLCheck(glBindVertexArray(vaoID));
        GLCheck(glGenBuffers(1, &iboID));
        GLCheck(glUseProgram(glProgramID));
        GLCheck(glBindTexture(GL_TEXTURE_2D, texID));
        GLCheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, transform.LocalToWorld().toPtr()));
        GLCheck(glEnable(GL_DEPTH_TEST));

        GLCheck(glDrawElements(GL_TRIANGLES, numTris * 3,  GL_UNSIGNED_INT, nullptr));

        #ifdef UI_MENUS
            TransformUI(ctx);
        #endif
        nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}