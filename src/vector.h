#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

#define INFINITE 3.402823466e+38F
#define EPS 0.001
#define RND (2.0f*(float)rand()/RAND_MAX-1.0)
#define PI 3.14159265f

struct Vector
{
    float x, y, z;

    Vector() 
    {
        x = y = z = 0; 
    }

    Vector(float x0, float y0, float z0 = 0) 
    {
        x = x0; y = y0; z = z0; 
    }

    Vector(int x0, int y0, int z0 = 0)
    {
        x = (float)x0; y = (float)y0; z = (float)z0;
    }

    Vector operator*(float a) 
    {
        return Vector(x * a, y * a, z * a);
    }

    void operator*=(float a)
    {
        x *= a;
        y *= a;
        z *= a;
    }

    Vector operator/(float a) 
    {
        return Vector(x / a, y / a, z / a);
    }

    Vector operator+(const Vector& v)
    {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector& v) 
    {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    // dot product
    float operator*(const Vector& v) 
    {
        return (x * v.x + y * v.y + z * v.z);
    }

    // cross product
    Vector operator%(const Vector& v) 
    {
        return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }

    float length() 
    {
        return sqrt(x * x + y * y + z * z);
    }

    Vector normalize()
    {
        return Vector(x / length(), y / length(), z / length());
    }

    bool operator==(const Vector& v)
    {
        float eps = 0.0001f;
        if (v.x - x < eps && v.y - y < eps && v.z < eps) return true;
        else return false;
    }
};

#endif
