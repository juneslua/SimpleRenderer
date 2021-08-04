#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>


namespace fLoaders
{
    static const std::string GetFileExt(const char* path)
    {
        const std::string p(path);
        std::size_t extBegin = p.find_last_of(".");

        std::string ext;
        if (extBegin != std::string::npos) ext = p.substr(extBegin+1);
        else ext = "";

        return ext;
    }

    typedef std::map<std::string, unsigned int> AttribsIndex;

    static bool OBJLoader(const char* path, std::vector<float>* vertsPtr, std::vector<unsigned int>* trisPtr, unsigned int* vertexCount, unsigned int* triCount)
    {
        if (GetFileExt(path) != "obj")
        {
            std::cout << "[OBJLoader] The path doesn't correspond to a .obj file." << std::endl;
            return false;
        }

        std::ifstream obj(path); // FILE CONTENT
        if (!obj)
        {
            std::cout << "[OBJLoader] Couldn't load the expecify file (.\\" << path << ")." << std::endl;
            return false;
        }

        std::vector<float> coords;
        std::vector<float> uvs; 
        std::vector<float> normals;
        std::vector<std::string> faces;

        // Iterate over the content of the .obj file an extract vertex coords (v), UVs (vt), vertex normals (vn), and faces (f)
        // TODO - Get polygon/group name. Handle groups.
        std::string line;
        while (std::getline(obj, line))
        {
            if (line.substr(0,2) == "vt")
            {
                float u, v;
                sscanf(line.substr(3).c_str(), "%f %f", &u,&v);
                uvs.push_back(u); uvs.push_back(v);
            }
            else if (line.substr(0,2) == "vn")
            {
                float nx, ny, nz;
                sscanf(line.substr(3).c_str(), "%f %f %f", &nx,&ny,&nz);
                normals.push_back(nx); normals.push_back(ny); normals.push_back(nz);
            }
            else if (line.substr(0,1) == "v")
            {
                float x, y, z;
                sscanf(line.substr(2).c_str(), "%f %f %f", &x,&y,&z);
                coords.push_back(x); coords.push_back(y); coords.push_back(z);
            }
            else if (line.substr(0,1) == "f") faces.push_back(line.substr(2));
        }

        const unsigned int totalVerts = uvs.size() / 2;
        const unsigned int totalTris = faces.size();

        std::vector<float> verts(totalVerts * 8);           // VERTEX-BUFFER
        std::vector<unsigned int> tris(faces.size() * 3);   // INDEX-BUFFER

        unsigned int numIndeces = coords.size() / 3; // Also correspond to the number of unique vertex
        std::map<unsigned int, AttribsIndex*> parsedVerts;

        // LAMBDA -> VertexParser
        // Populate the 'verts' array (vbo) and return its index to be store in the 'tris' array (ibo).
        auto VertexParser = [&](char* vertexIndeces)
        {
            unsigned int v, vt, vn; // Attributes indeces
            sscanf(vertexIndeces, "%i/%i/%i", &v,&vt,&vn);
            v--; vt--; vn--;

            std::stringstream vertAttrib;
            vertAttrib << v << "/" << vt;

            unsigned int index = 0;
            AttribsIndex* uniqueVerts = new AttribsIndex;

            // Does a vertex with this coords exist?
            if (parsedVerts.find(v) == parsedVerts.end())
            {
                // NO, then insert a new list at this key (v),
                parsedVerts.insert({v, uniqueVerts});
                // and this vertex will keep its original index.
                index = v;
            }
            else 
            {
                // YES, then find the direction of the list that exist at key (v).
                uniqueVerts = parsedVerts[v];

                // Is there a vertex with the SAME attributes?
                if (uniqueVerts->find(vertAttrib.str()) != uniqueVerts->end())
                    // YES, then returns its index in the buffer....
                    return uniqueVerts->at(vertAttrib.str());

                // else put the new vertex at the end of the index buffer
                index = numIndeces++;
            }

            uniqueVerts->insert({vertAttrib.str(), index}); // New vertex with this attribute

            // Insert this vertex into the vertex buffer
            verts[index*8]   = (coords[v*3]);       //X 
            verts[index*8+1] = (coords[v*3+1]);     //Y
            verts[index*8+2] = (coords[v*3+2]);     //Z
            verts[index*8+3] = (uvs[vt*2]);         //U
            verts[index*8+4] = (uvs[vt*2+1]);       //V
            verts[index*8+5] = (normals[vn*3]);     //Normal - X
            verts[index*8+6] = (normals[vn*3+1]);   //Normal - Y
            verts[index*8+7] = (normals[vn*3+2]);   //Normal - Z

            // returns the index of this object in the vertex buffer
            return index;
        };

        // Get the vertex indeces of each faces, alters the index if necesarry
        // e.j - Two verts with same coords buts multiple UV coords (as in the UV seams).
        unsigned int tIndex = 0;
        for (auto const &face : faces)
        {
            char v1[24], v2[24], v3[24];
            sscanf(face.c_str(), "%s %s %s", v1,v2,v3);

            tris[tIndex*3]   = VertexParser(v1);
            tris[tIndex*3+1] = VertexParser(v2);
            tris[tIndex*3+2] = VertexParser(v3);

            tIndex++;
        }

        // Clean up
        for (auto &v : parsedVerts)
        {
            v.second->clear();
            delete v.second;
        }
        parsedVerts.clear();

        *vertsPtr = verts;
        *trisPtr = tris;
        *vertexCount = totalVerts;
        *triCount = totalTris;

        return true; // OBJ Loaded
    }
}