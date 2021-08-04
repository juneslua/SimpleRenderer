#pragma once

#include <iostream>
#include <iomanip>

#include "modules/LinearAlgebra.h"

class Transform
{
    public:
        Transform();
        Transform(const Vector3<float> position, const Vector3<float> rotation, const Vector3<float> scale = Vector3<float>(1, 1, 1));

        static Vector3<float> RotationMatrixToEuler(const Matrix4x4<float> &mat);

        Matrix4x4<float> LocalToWorld(bool transpose = false) { return transpose ? _localToWorld.Inverted() : _localToWorld; }
        Matrix4x4<float> WorldToLocal(bool transpose = false) { return transpose ? _worldToLocal.Inverted() : _worldToLocal; }

        inline Vector3<float> get_position() const { return _position; }
        inline Vector3<float> get_rotation() const { return _rotation; }
        inline Vector3<float> get_scale() const { return _scale; }

        void set_position(const Vector3<float> position);
        void set_rotation(const Vector3<float> rotation);
        void set_scale(const Vector3<float> scale);

        friend std::ostream& operator<<(std::ostream &s, const Transform &t) 
        {  
            return std::cout << "Position | " << t._position  << "\n" 
                             << "Rotation | " << t._rotation  << "\n" 
                             << "Scale    | " << t._scale     << "\n" 
                             << "\n"
                             << "LocalToWorld" << "\n" 
                             << t._localToWorld << std::endl; 
        }

        void Translate(float deltaX, float deltaY, float deltaZ);
        void Rotate(float thetaX, float thetaY, float thetaZ);
        void Scale(float scaleX, float scaleY, float scaleZ);

        Matrix4x4<float> LookAt(Vector3<float> to, Vector3<float> tmpY = Vector3<float>(0, 1, 0));

    private:
        Matrix4x4<float> _localToWorld;
        Matrix4x4<float> _worldToLocal;

        Vector3<float> _position, _rotation, _scale;

        void RefreshLocalMatrices();
};