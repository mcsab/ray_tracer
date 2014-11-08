#ifndef PARABOLOID_H
#define PARABOLOID_H

#include "object.h"

class Paraboloid :public Object
{
    Vector m_focus_point;
    Vector m_center_of_mass;
    Vector m_orientaion;
    float m_height;
public:
    Paraboloid(Vector focus, Vector center_of_mass, float height);
    
    Paraboloid() {};

    Intersection intersect(Ray ray);
};

#endif
