#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "object.h"

const int screen_width = 1920;
const int screen_height = 1080;

class Camera
{
private:
    Vector m_eye;
    Vector m_look_at;
    Vector m_up;
    Vector m_right;
public:
    Camera() {}

    Camera(Vector e, Vector l, Vector u, Vector r);

    Ray getRay(float x, float y);

    void rotateAroundY(float a);
};

#endif
