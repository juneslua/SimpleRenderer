#pragma once

#include <iostream>
#include <iomanip>
#include <cmath>

#define DEG2RAD 0.01745329251994329576923690768489
#define RAD2DEG 57.295779513082320876798154814105

template<typename T> 
class Vector2
{
    public:
        T x, y;

        Vector2() : x(T(0)), y(T(0)) { }
        Vector2(T x, T y) : x(x), y(y) { }

        inline static T Dot(const Vector2 &vecA, const Vector2 &vecB) { return vecA.x*vecB.x + vecA.y*vecB.y; }

        inline T Magnitud() const { return sqrtf(x * x + y * y); }
        Vector2 Normalized() const
        { 
            T mag = Magnitud(); 
            return Vector2(x / mag, y / mag); 
        }

        inline Vector2 operator+(const Vector2 &vec) const { return Vector2(x + vec.x, y + vec.y); }
        inline Vector2 operator-(const Vector2 &vec) const { return Vector2(x - vec.x, y - vec.y); }
        inline Vector2 operator*(T scalar) const { return Vector2(x * scalar, y * scalar); }
        inline Vector2 operator/(T scalar) const { return Vector2(x / scalar, y / scalar); }

        void operator+=(const Vector2 &vec) { x += vec.x; y += vec.y; }
        void operator-=(const Vector2 &vec) { x -= vec.x; y -= vec.y; }
        void operator*=(T scalar) { x *= scalar; y *= scalar; }
        void operator/=(T scalar) { x /= scalar; y /= scalar; }

        inline friend std::ostream& operator<<(std::ostream &s, const Vector2<T> &v) { return s << "[" << v.x << ", " << v.y << "]"; }

        void Normalize() 
        { 
            T mag = Magnitud();
            x /= mag; y /= mag;
        }
};

template<typename T>
struct Rect
{
    public:

        T top, right, bottom, left;

        Rect() : top(0), right(0), bottom(0), left(0) { }
        Rect(T top, T right, T bottom, T left) : top(top), right(right), bottom(bottom), left(left) { }
        Rect(const Vector2<T> &bottomLeftCorner, T width, T height)
        {   
            bottom = bottomLeftCorner.y;
            left = bottomLeftCorner.x;
            top = bottom + height;
            right = left + width;
        }

        inline Vector2<T> Center() const { return { (right + left) / 2, (top + bottom) / 2}; }
        inline std::pair<float, float> Size() const { return {right- left, top - bottom}; }

        friend std::ostream& operator<<(std::ostream &s, const Rect &r)
        {
            return s << " Top    | " << r.top    << "\n"
                     << " Right  | " << r.right  << "\n"
                     << " Bottom | " << r.bottom << "\n"
                     << " Left   | " << r.left   << std::endl;
        }
};

template<typename T> 
class Vector3
{
    public:
        T x, y, z;

        Vector3() : x(T(0)), y(T(0)), z(T(0)) { }
        Vector3(T x, T y) : x(x), y(y), z(T(0)) { }
        Vector3(T x, T y, T z) : x(x), y(y), z(z) { }

        inline static T Dot(const Vector3 &vecA, const Vector3 &vecB) { return vecA.x*vecB.x + vecA.y*vecB.y + vecA.z*vecB.z; }
        inline static Vector3 Cross(const Vector3 &vecA, const Vector3 &vecB)
        { 
            return Vector3(vecA.y * vecB.z - vecA.z * vecB.y, vecA.z * vecB.x - vecA.x * vecB.z, vecA.x * vecB.y - vecA.y * vecB.x);
        }

        inline T Magnitud() const { return sqrtf(x * x + y * y + z * z); }
        Vector3 Normalized() const 
        { 
            T mag = Magnitud(); 
            return Vector3(x / mag, y / mag, z / mag); 
        }

        inline Vector3 operator+(const Vector3 &vec) const { return Vector3(x + vec.x, y + vec.y, z + vec.z); }
        inline Vector3 operator-(const Vector3 &vec) const { return Vector3(x - vec.x, y - vec.y, z - vec.z); }
        inline Vector3 operator*(T scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
        inline Vector3 operator/(T scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

        void operator+=(const Vector3 &vec) { x += vec.x; y += vec.y; z += vec.z; }
        void operator-=(const Vector3 &vec) { x -= vec.x; y -= vec.y; z -= vec.z; }
        void operator*=(T scalar) { x *= scalar; y *= scalar; z *= scalar; }
        void operator/=(T scalar) { x /= scalar; y /= scalar; z /= scalar; }

        inline friend std::ostream& operator<<(std::ostream &s, const Vector3<T> &v) { return s << "[" << v.x << ", " << v.y << ", " << v.z << "]"; }

        void Normalize() 
        { 
            T mag = Magnitud(); 
            x /= mag; y /= mag; z /= mag;
        }
};



#define T_MATRIX(X, Y, Z) 1,0,0,0, 0,1,0,0, 0,0,1,0, X,Y,Z,1

#define RX_MATRIXf(theta) 1,0,0,0,  0,cosf(theta),sinf(theta),0,  0,-sinf(theta),cosf(theta),0,  0,0,0,1
#define RY_MATRIXf(theta) cosf(theta),0,-sinf(theta),0,  0,1,0,0,  sinf(theta),0,cosf(theta),0,  0,0,0,1
#define RZ_MATRIXf(theta) cosf(theta),sinf(theta),0,0,  -sinf(theta),cosf(theta),0,0,  0,0,1,0,  0,0,0,1

#define S_MATRIX(sX, sY, sZ) sX,0,0,0,  0,sY,0,0,  0,0,sZ,0,  0,0,0,1


template<typename T>
class Matrix4x4
{
    public:
        T m[4][4] = { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} };

        Matrix4x4() { }

        Matrix4x4(T scalar)
        {
            m[0][0] *= scalar;
            m[1][1] *= scalar;
            m[2][2] *= scalar;
        }

        Matrix4x4(
            T c00, T c01, T c02, T c03, 
            T c10, T c11, T c12, T c13,
            T c20, T c21, T c22, T c23,
            T c30, T c31, T c32, T c33)
        {
            m[0][0] = c00; m[0][1] = c01; m[0][2] = c02; m[0][3] = c03;
            m[1][0] = c10; m[1][1] = c11; m[1][2] = c12; m[1][3] = c13;
            m[2][0] = c20; m[2][1] = c21; m[2][2] = c22; m[2][3] = c23;
            m[3][0] = c30; m[3][1] = c31; m[3][2] = c32; m[3][3] = c33;
        }

        static Matrix4x4 Multiply(const Matrix4x4 &matA, const Matrix4x4 &matB)
        {
            Matrix4x4 nMat;

            nMat[0][0] = matA[0][0] * matB[0][0] + matA[0][1] * matB[1][0] + matA[0][2] * matB[2][0] + matA[0][3] * matB[3][0];
            nMat[0][1] = matA[0][0] * matB[0][1] + matA[0][1] * matB[1][1] + matA[0][2] * matB[2][1] + matA[0][3] * matB[3][1];
            nMat[0][2] = matA[0][0] * matB[0][2] + matA[0][1] * matB[1][2] + matA[0][2] * matB[2][2] + matA[0][3] * matB[3][2];
            nMat[0][3] = matA[0][0] * matB[0][3] + matA[0][1] * matB[1][3] + matA[0][2] * matB[2][3] + matA[0][3] * matB[3][3];

            nMat[1][0] = matA[1][0] * matB[0][0] + matA[1][1] * matB[1][0] + matA[1][2] * matB[2][0] + matA[1][3] * matB[3][0];
            nMat[1][1] = matA[1][0] * matB[0][1] + matA[1][1] * matB[1][1] + matA[1][2] * matB[2][1] + matA[1][3] * matB[3][1];
            nMat[1][2] = matA[1][0] * matB[0][2] + matA[1][1] * matB[1][2] + matA[1][2] * matB[2][2] + matA[1][3] * matB[3][2];
            nMat[1][3] = matA[1][0] * matB[0][3] + matA[1][1] * matB[1][3] + matA[1][2] * matB[2][3] + matA[1][3] * matB[3][3];

            nMat[2][0] = matA[2][0] * matB[0][0] + matA[2][1] * matB[1][0] + matA[2][2] * matB[2][0] + matA[2][3] * matB[3][0];
            nMat[2][1] = matA[2][0] * matB[0][1] + matA[2][1] * matB[1][1] + matA[2][2] * matB[2][1] + matA[2][3] * matB[3][1];
            nMat[2][2] = matA[2][0] * matB[0][2] + matA[2][1] * matB[1][2] + matA[2][2] * matB[2][2] + matA[2][3] * matB[3][2];
            nMat[2][3] = matA[2][0] * matB[0][3] + matA[2][1] * matB[1][3] + matA[2][2] * matB[2][3] + matA[2][3] * matB[3][3];

            nMat[3][0] = matA[3][0] * matB[0][0] + matA[3][1] * matB[1][0] + matA[3][2] * matB[2][0] + matA[3][3] * matB[3][0];
            nMat[3][1] = matA[3][0] * matB[0][1] + matA[3][1] * matB[1][1] + matA[3][2] * matB[2][1] + matA[3][3] * matB[3][1];
            nMat[3][2] = matA[3][0] * matB[0][2] + matA[3][1] * matB[1][2] + matA[3][2] * matB[2][2] + matA[3][3] * matB[3][2];
            nMat[3][3] = matA[3][0] * matB[0][3] + matA[3][1] * matB[1][3] + matA[3][2] * matB[2][3] + matA[3][3] * matB[3][3];

            return nMat;
        }

        float* toPtr() const
        { 
            float* mat = new T[16]{ 
                m[0][0], m[0][1], m[0][2], m[0][3],
                m[1][0], m[1][1], m[1][2], m[1][3],
                m[2][0], m[2][1], m[2][2], m[2][3],
                m[3][0], m[3][1], m[3][2], m[3][3],
            };

            return mat;
        }

        inline Matrix4x4 Transposed() const
        {
            return {
                m[0][0], m[1][0], m[2][0], m[3][0],
                m[0][1], m[1][1], m[2][1], m[3][1],
                m[0][2], m[1][2], m[2][2], m[3][2],
                m[0][3], m[1][3], m[2][3], m[3][3],
            };
        }

        Matrix4x4 Inverted() const
        {
            int i, j, k;
            Matrix4x4 s;
            Matrix4x4 t (*this);

            // Forward elimination
            for (i = 0; i < 3 ; i++)
            {
                int pivot = i;
                T pivotsize = t[i][i];

                if (pivotsize < 0) pivotsize = -pivotsize;

                for (j = i + 1; j < 4; j++) 
                {
                    T tmp = t[j][i];

                    if (tmp < 0) tmp = -tmp;

                    if (tmp > pivotsize) 
                    {
                        pivot = j;
                        pivotsize = tmp;
                    }
                }

                // Cannot invert singular matrix
                if (pivotsize == 0) return Matrix4x4();

                if (pivot != i) 
                    for (j = 0; j < 4; j++) 
                    {
                        T tmp;

                        tmp = t[i][j];
                        t[i][j] = t[pivot][j];
                        t[pivot][j] = tmp;

                        tmp = s[i][j];
                        s[i][j] = s[pivot][j];
                        s[pivot][j] = tmp;
                    }

                for (j = i + 1; j < 4; j++)
                {
                    T f = t[j][i] / t[i][i];

                    for (k = 0; k < 4; k++)
                    {
                        t[j][k] -= f * t[i][k];
                        s[j][k] -= f * s[i][k];
                    }
                }
            }

            // Backward substitution
            for (i = 3; i >= 0; --i) 
            {
                T f;

                // Cannot invert singular 
                if ((f = t[i][i]) == 0) return Matrix4x4();

                for (j = 0; j < 4; j++)
                {
                    t[i][j] /= f;
                    s[i][j] /= f;
                }

                for (j = 0; j < i; j++) 
                {
                    f = t[j][i];

                    for (k = 0; k < 4; k++)
                    {
                        t[j][k] -= f * t[i][k];
                        s[j][k] -= f * s[i][k];
                    }
                }
            }

            return s;
        }

        inline const T* operator[](uint8_t i) const { return(m[i]); }
        inline T* operator[](uint8_t i) { return(m[i]); }

        template<typename S>
        Vector3<S> operator*(const Vector3<S> &vec)
        {
            Vector3<S> nVec;
            nVec.x = vec.x * m[0][0] + vec.y * m[1][0] + vec.z * m[2][0];
            nVec.y = vec.x * m[0][1] + vec.y * m[1][1] + vec.z * m[2][1];
            nVec.z = vec.x * m[0][2] + vec.y * m[1][2] + vec.z * m[2][2];

            return nVec;
        }
        inline Matrix4x4 operator*(const Matrix4x4 &mat) { return Multiplicar(*this, mat); }

        void operator*=(const Matrix4x4 &matB)
        {
            Matrix4x4<float> matA = *this;

            m[0][0] = matA[0][0] * matB[0][0] + matA[0][1] * matB[1][0] + matA[0][2] * matB[2][0] + matA[0][3] * matB[3][0];
            m[0][1] = matA[0][0] * matB[0][1] + matA[0][1] * matB[1][1] + matA[0][2] * matB[2][1] + matA[0][3] * matB[3][1];
            m[0][2] = matA[0][0] * matB[0][2] + matA[0][1] * matB[1][2] + matA[0][2] * matB[2][2] + matA[0][3] * matB[3][2];
            m[0][3] = matA[0][0] * matB[0][3] + matA[0][1] * matB[1][3] + matA[0][2] * matB[2][3] + matA[0][3] * matB[3][3];

            m[1][0] = matA[1][0] * matB[0][0] + matA[1][1] * matB[1][0] + matA[1][2] * matB[2][0] + matA[1][3] * matB[3][0];
            m[1][1] = matA[1][0] * matB[0][1] + matA[1][1] * matB[1][1] + matA[1][2] * matB[2][1] + matA[1][3] * matB[3][1];
            m[1][2] = matA[1][0] * matB[0][2] + matA[1][1] * matB[1][2] + matA[1][2] * matB[2][2] + matA[1][3] * matB[3][2];
            m[1][3] = matA[1][0] * matB[0][3] + matA[1][1] * matB[1][3] + matA[1][2] * matB[2][3] + matA[1][3] * matB[3][3];

            m[2][0] = matA[2][0] * matB[0][0] + matA[2][1] * matB[1][0] + matA[2][2] * matB[2][0] + matA[2][3] * matB[3][0];
            m[2][1] = matA[2][0] * matB[0][1] + matA[2][1] * matB[1][1] + matA[2][2] * matB[2][1] + matA[2][3] * matB[3][1];
            m[2][2] = matA[2][0] * matB[0][2] + matA[2][1] * matB[1][2] + matA[2][2] * matB[2][2] + matA[2][3] * matB[3][2];
            m[2][3] = matA[2][0] * matB[0][3] + matA[2][1] * matB[1][3] + matA[2][2] * matB[2][3] + matA[2][3] * matB[3][3];

            m[3][0] = matA[3][0] * matB[0][0] + matA[3][1] * matB[1][0] + matA[3][2] * matB[2][0] + matA[3][3] * matB[3][0];
            m[3][1] = matA[3][0] * matB[0][1] + matA[3][1] * matB[1][1] + matA[3][2] * matB[2][1] + matA[3][3] * matB[3][1];
            m[3][2] = matA[3][0] * matB[0][2] + matA[3][1] * matB[1][2] + matA[3][2] * matB[2][2] + matA[3][3] * matB[3][2];
            m[3][3] = matA[3][0] * matB[0][3] + matA[3][1] * matB[1][3] + matA[3][2] * matB[2][3] + matA[3][3] * matB[3][3];
        }

        friend std::ostream& operator<<(std::ostream &s, const Matrix4x4 &m)
        {
            std::ios_base::fmtflags prevFormat = s.flags();
            int width = 12;
            s.precision(5);
            s.setf(std::ios_base::fixed);

            s << "|" << std::setw(width) << m[0][0] << " "
                     << std::setw(width) << m[0][1] << " "
                     << std::setw(width) << m[0][2] << " "
                     << std::setw(width) << m[0][3] << "|" << "\n"

              << "|" << std::setw(width) << m[1][0] << " "
                     << std::setw(width) << m[1][1] << " "
                     << std::setw(width) << m[1][2] << " "
                     << std::setw(width) << m[1][3] << "|" << "\n"

              << "|" << std::setw(width) << m[2][0] << " "
                     << std::setw(width) << m[2][1] << " "
                     << std::setw(width) << m[2][2] << " "
                     << std::setw(width) << m[2][3] << "|" << "\n"

              << "|" << std::setw(width) << m[3][0] << " "
                     << std::setw(width) << m[3][1] << " "
                     << std::setw(width) << m[3][2] << " "
                     << std::setw(width) << m[3][3] << "|" << std::endl;

            s.flags(prevFormat);
            return s;
        }

        template<typename S>
        Vector3<S> TransformVector(const Vector3<S> &vec) const
        {
            Vector3<S> nVec;
            S w;

            nVec.x = vec.x * m[0][0] + vec.y * m[1][0] + vec.z * m[2][0] + m[3][0];
            nVec.y = vec.x * m[0][1] + vec.y * m[1][1] + vec.z * m[2][1] + m[3][1];
            nVec.z = vec.x * m[0][2] + vec.y * m[1][2] + vec.z * m[2][2] + m[3][2];
            w = vec.x * m[0][3] + vec.y * m[1][3] + vec.z * m[2][3] + m[3][3];

           nVec.x /= w;
           nVec.y /= w;
           nVec.z /= w;

           return nVec;
        }

        void Transpose()
        {
            Matrix4x4 nMat = {
                m[0][0], m[1][0], m[2][0], m[3][0],
                m[0][1], m[1][1], m[2][1], m[3][1],
                m[0][2], m[1][2], m[2][2], m[3][2],
                m[0][3], m[1][3], m[2][3], m[3][3],
            };
            *this = nMat; 
        }

        void Inverse() { *this = Inverted(); }
};