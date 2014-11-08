#ifndef OBJECT_H
#define OBJECT_H

#include "material.h"

#define CPMX 1024
#define CPMY 1024

class Object;

// ----------------------------------------------------------------------------
struct Ray
{
    Vector eye;
    Vector v;

    Ray(Vector ieye, Vector iv)  { eye = ieye;  v = iv.normalize(); }
    void operator=(const Ray& o) { eye = o.eye; v = o.v;            }
};

// ----------------------------------------------------------------------------
struct Intersection
{
    Vector   pos;
    Vector   normal;
    float    t;
    Material mat;
    Object   *obj;

    Intersection() { t = -1; }

    Intersection(Vector ipos, Vector inormal, float it, Material m);
    void operator=(const Intersection &other);
};

// ----------------------------------------------------------------------------
class Object
{
protected:
    Material m_material;
public:
    Object() {}
    
    virtual Intersection intersect(Ray ray) = 0;
    
    virtual void addToPhotonMap(Vector pos, Color power)    {}
    virtual Color getPhotonMapColor(Vector pos)             { return Color(); }

    void setMaterial(Material m) { m_material = m; }

    virtual Color kd(Vector p) { return m_material.kd; }
    virtual Color ka(Vector p) { return m_material.ka; }
};

#endif
