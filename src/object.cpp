#include "object.h"


// ----------------------------------------------------------------------------
Intersection::Intersection(Vector ipos, Vector inormal, float it, Material m)
{
    t      = it;
    pos    = ipos;
    normal = inormal;
    m      = mat;
}

// ----------------------------------------------------------------------------
void Intersection::operator=(const Intersection &other)
{
    t      = other.t;
    pos    = other.pos;
    normal = other.normal;
    mat    = other.mat;
    obj    = other.obj;
}

