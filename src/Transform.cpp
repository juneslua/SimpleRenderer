#include "Transform.h"


using namespace std;


Transform::Transform() 
{
    _scale = Vector3<float>(1, 1, 1); 
    RefreshLocalMatrices(); 
}

Transform::Transform(Vector3<float> position, Vector3<float> rotation, Vector3<float> scale) : _position(position), _rotation(rotation), _scale(scale)  
{ 
    RefreshLocalMatrices(); 
}

Vector3<float> Transform::RotationMatrixToEuler(const Matrix4x4<float> &mat)
{
    Vector3<float> euler;

    euler.x = asinf(-mat[2][1]);
    if (cosf(euler.x) > 1e-6)
    {
        euler.y = atan2f(mat[2][0], mat[2][2]);
        euler.z = atan2f(mat[0][1], mat[1][1]);
    }
    else
    {
        euler.y = 0.0f;
        euler.z = atan2f(-mat[1][0], mat[0][0]);
    }

    euler *= RAD2DEG;

    return euler;
}

void Transform::set_position(const Vector3<float> position)
{
    this->_position = position;
    RefreshLocalMatrices();
}

void Transform::set_rotation(const Vector3<float> rotation)
{
    this->_rotation = rotation;
    RefreshLocalMatrices();
}

void Transform::set_scale(const Vector3<float> scale)
{
    this->_scale = scale;
    RefreshLocalMatrices();
}


void Transform::Translate(float deltaX, float deltaY, float deltaZ)
{
    _position.x += deltaX;
    _position.y += deltaY;
    _position.z += deltaZ;
    RefreshLocalMatrices();
}

void Transform::Rotate(float thetaX, float thetaY, float thetaZ)
{
    _rotation.x += thetaX;
    _rotation.y += thetaY;
    _rotation.z += thetaZ;
    RefreshLocalMatrices();
}

void Transform::Scale(float scaleX, float scaleY, float scaleZ)
{
    _scale.x *= scaleX;
    _scale.y *= scaleY;
    _scale.z *= scaleZ;
    RefreshLocalMatrices();
}


Matrix4x4<float> Transform::LookAt(Vector3<float> to, Vector3<float> tmpY)
{
    Vector3<float> zAxis = _position - to;
    zAxis.Normalize();

    tmpY.Normalize();
    Vector3<float> xAxis = Vector3<float>::Cross(tmpY, zAxis);
    Vector3<float> yAxis = Vector3<float>::Cross(zAxis, xAxis);

    Matrix4x4<float> lookAtMatrix(
            xAxis.x,     xAxis.y,     xAxis.z,  0,
            yAxis.x,     yAxis.y,     yAxis.z,  0,
            zAxis.x,     zAxis.y,     zAxis.z,  0,
        _position.x, _position.y, _position.z,  1
    );

    set_rotation(RotationMatrixToEuler(lookAtMatrix));

    return lookAtMatrix;
}

void Transform::RefreshLocalMatrices()
{
    Matrix4x4<float> scaling(S_MATRIX(_scale.x, _scale.y, _scale.z));
    _localToWorld = scaling;

    if (_rotation.x != 0)
    {
        Matrix4x4<float> rotX(RX_MATRIXf(_rotation.x * DEG2RAD));
        _localToWorld *= rotX;
    }
    if (_rotation.y != 0)
    {
        Matrix4x4<float> rotY(RY_MATRIXf(_rotation.y * DEG2RAD));
        _localToWorld *= rotY;
    }
    if (_rotation.z != 0)
    {
        Matrix4x4<float> rotZ(RZ_MATRIXf(_rotation.z * DEG2RAD));
        _localToWorld *= rotZ;
    }

    _localToWorld[3][0] = _position.x;
    _localToWorld[3][1] = _position.y;
    _localToWorld[3][2] = _position.z;

    _worldToLocal = _localToWorld.Inverted();
}