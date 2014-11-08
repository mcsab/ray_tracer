#ifndef CYLINDER_H
#define CYLINDER_H

#include "object.h"

class Disc;

class Cylinder :public Object
{
private:
    Vector m_center_point;
    Vector m_axis_vec;
    float  m_r;
    float  m_height;
    Vector m_local_x_vec;
    Vector m_local_y_vec;
    Color  m_photon_map[CPMX + 1][CPMY + 1];

    Disc*  m_bottom;
    Disc*  m_top;

    void photonMapIndexFromPos(Vector pos, int& x, int& y);    
    Intersection intersectBody(Ray ray);

public:
    Cylinder(Vector center, Vector axis_vec, float r, float height,
        bool bottom, bool top, Material bm = Material(), Material bt = Material());
    
    Intersection intersect(Ray ray);

    void addToPhotonMap(Vector pos, Color intensity);

    Color getPhotonMapColor(Vector pos);

    Color kd(Vector p);
    Color ka(Vector p);

    ~Cylinder();
};

#endif
