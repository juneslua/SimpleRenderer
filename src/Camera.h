#pragma once

#include "modules/LinearAlgebra.h"
#include "Transform.h"

#define INCH2MM 25.4

enum class CameraMode { Perspective, Orthographic };
enum class CameraScaling { Fill, Fit };

class Camera
{
    public:

        Transform transform;

        Camera();
        Camera(float fLength, std::pair<float,float> aperture, std::pair<int,int> resolution, float zNear, float zFar, CameraMode mode = CameraMode::Perspective, CameraScaling scaling = CameraScaling::Fill);


        inline Matrix4x4<float> CameraToWorld(bool transpose = false) { return transform.LocalToWorld(transpose); }
        inline Matrix4x4<float> WorldToCamera(bool transpose = false) { return transform.WorldToLocal(transpose); }

        inline Matrix4x4<float> ProjectionMatrix(bool transpose = false) { return transpose ? _projectionMatrix.Transposed() : _projectionMatrix; }

        inline float FilmAspectRatio() const { return _aperture.first / _aperture.second; }
        inline float PixelAspectRatio() const { return (float)_resolution.first / (float)_resolution.second; }

        inline float get_fLength() const { return _fLength; }
        inline float get_fovX() const { return _fovX; }
        inline std::pair<float, float> get_aperture() const { return _aperture; }
        inline std::pair<int, int> get_resolution() const { return _resolution; }

        inline Rect<float> get_canvasPlane() const { return _canvasPlane; }
        

        void set_fLength(const float flength);
        void set_fov(const float fovX);
        void set_aperture(const float x, const float y);
        void set_resolution(const int w, const int h);

    private:
        Matrix4x4<float> _projectionMatrix;

        float _fLength, _fovX;
        std::pair<float, float> _aperture;
        std::pair<int, int> _resolution;
        float _zNear, _zFar;

        CameraMode _mode;
        CameraScaling _scaling;

        Rect<float> _canvasPlane;

        Matrix4x4<float> PerspectiveProjection();
        Matrix4x4<float> OrthographicProjection();

        void OnPropertyChange();
        void RefreshProjectionMatrix();
        void CalculateCanvasPlane();

        void FixAspectRatio(float* xScalar, float* yScalar);
};