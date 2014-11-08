#include "camera.h"

// ----------------------------------------------------------------------------
Camera::Camera(Vector e, Vector l, Vector u, Vector r)
{
    m_eye       = e;
    m_look_at   = l;
    m_up        = u;
    m_right     = r;
}

// ----------------------------------------------------------------------------
void Camera::rotateAroundY(float a)
{
    a = a * PI / 180.0f;
    Vector rotated = m_eye;
    rotated.x = cos(a) * m_eye.x + sin(a) * m_eye.z;
    rotated.z = cos(a) * m_eye.z - sin(a) * m_eye.x;
    m_eye = rotated;

    rotated.y = m_look_at.y;
    rotated.x = cos(a) * m_look_at.x + sin(a) * m_look_at.z;
    rotated.z = cos(a) * m_look_at.z - sin(a) * m_look_at.x;
    m_look_at = rotated;

    m_right = m_up % (m_look_at - m_eye);
}

// ----------------------------------------------------------------------------
Ray Camera::getRay(float x, float y)
{
    Vector p = m_look_at + m_right * (2 * x / screen_width - 1) +
        m_up * (2 * y / screen_height - 1) + Vector(2.0f / screen_width, 2.0f / screen_height);
    return Ray(m_eye, p - m_eye);
}

