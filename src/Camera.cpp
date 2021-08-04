#include "Camera.h"


using namespace std;

Camera::Camera() 
{
    _fLength = 35;
    _aperture = {0.980, 0.735};
    _resolution = {800, 600};

    _zNear = 0.01;
    _zFar = 1000;

    _mode = CameraMode::Perspective;
    _scaling = CameraScaling::Fill;

    _fovX = 2 * atan((_aperture.first * INCH2MM / 2) / _fLength) * DEG2RAD;

    OnPropertyChange();
}

Camera::Camera(float fLength, pair<float,float> aperture, pair<int,int> resolution, float zNear, float zFar, CameraMode mode, CameraScaling scaling) : 
_fLength(fLength), _aperture(aperture), _resolution(resolution), _zNear(zNear), _zFar(zFar), _mode(mode), _scaling(scaling)
{
    _fovX = 2 * atan((_aperture.first * INCH2MM / 2) / _fLength) * DEG2RAD;
    OnPropertyChange();
}

void Camera::set_fLength(const float fLength)
{
    _fLength = fLength;
    _fovX = 2 * atan((_aperture.first * INCH2MM / 2) / _fLength) * DEG2RAD;
    OnPropertyChange();
}

void Camera::set_fov(const float fovX)
{
    _fovX = fovX;
    _fLength = tan(_fovX * DEG2RAD / 2) * (_aperture.first * INCH2MM / 2);
    OnPropertyChange();
}

void Camera::set_aperture(const float x, const float y)
{
    _aperture = {x, y};
    _fLength = tan(_fovX * DEG2RAD / 2) * (_aperture.first * INCH2MM / 2);
    OnPropertyChange();
}

void Camera::set_resolution(const int w, const int h)
{
    _resolution = {w, h};
    OnPropertyChange();
}

Matrix4x4<float> Camera::PerspectiveProjection()
{
    float xScalar, yScalar;
    FixAspectRatio(&xScalar, &yScalar);

    float t = _canvasPlane.top * yScalar;
    float r = _canvasPlane.right * xScalar;
    float b = _canvasPlane.bottom * yScalar;
    float l = _canvasPlane.left * xScalar;
    
    float zN = _zNear, zF = _zFar;

    Matrix4x4<float> matProj(
         2 * zN / (r - l),                  0,                           0,   0,
                        0,   2 * zN / (t - b),                           0,   0,
        (r + l) / (r - l),  (t + b) / (t - b),    -((zF + zN) / (zF - zN)),  -1,
                        0,                  0,  -(2 * zF * zN / (zF - zN)),   0
    );

    return matProj;
}

Matrix4x4<float> Camera::OrthographicProjection()
{
    return Matrix4x4<float>();
}

void Camera::OnPropertyChange()
{
    CalculateCanvasPlane();
    RefreshProjectionMatrix();
}

void Camera::RefreshProjectionMatrix()
{
    if (_mode == CameraMode::Perspective) _projectionMatrix = PerspectiveProjection();
    else if ( _mode == CameraMode::Orthographic) _projectionMatrix = OrthographicProjection();
}

void Camera::CalculateCanvasPlane()
{
    float right = (_aperture.first * INCH2MM / 2) / _fLength * _zNear;
    float top = (_aperture.second * INCH2MM / 2) / _fLength * _zNear;

    _canvasPlane.top = top;
    _canvasPlane.bottom = -top;
    _canvasPlane.right = right;
    _canvasPlane.left = -right;
}

void Camera::FixAspectRatio(float* xScalar, float* yScalar)
{
    *xScalar = 1.0f;
    *yScalar = 1.0f;

    float fRatio = FilmAspectRatio(), pRatio = PixelAspectRatio();

    //std::cout << "Film Ratio | " << fRatio << "\n" << "Pixel Ratio | " << pRatio << std::endl;  

    if (_scaling == CameraScaling::Fill)
    {
        if (fRatio > pRatio) *xScalar = pRatio / fRatio;
        else *yScalar = fRatio / pRatio;
    }
    else if (_scaling == CameraScaling::Fit)
    {
        if (fRatio > pRatio) *yScalar = fRatio / pRatio;
        else *xScalar = pRatio / fRatio;
    }
}