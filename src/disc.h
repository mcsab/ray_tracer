#ifndef DISC_H
#define DISC_H

#include "object.h"

class Disc :public Object
{
    Vector m_center;
    Vector m_norm_vec;
    Vector m_local_x_vec;
    Vector m_local_y_vec;
    Color  m_photon_map[CPMX + 1][CPMY + 1];
    float  m_r;

    void photonMapIndexFromPos(Vector pos, int& x, int& y);

public:
    Disc(Vector center, Vector norm, float r);

    Intersection intersect(Ray ray);

    void addToPhotonMap(Vector pos, Color power);

    Color getPhotonMapColor(Vector pos);

    Color kd(Vector p);
    Color ka(Vector p);
};


#endif
