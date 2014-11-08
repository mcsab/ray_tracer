#include "disc.h"

// ----------------------------------------------------------------------------
void Disc::photonMapIndexFromPos(Vector pos, int& x, int& y)
{
    x = (int)((pos - m_center) * m_local_x_vec * CPMX / (2 * m_r)) + CPMX / 2;
    y = (int)((pos - m_center) * m_local_y_vec * CPMY / (2 * m_r)) + CPMY / 2;
}

// ----------------------------------------------------------------------------
Disc::Disc(Vector center, Vector norm, float r)
{
    norm = norm.normalize();

    m_center      = center;
    m_r           = r;
    m_norm_vec    = norm;

    m_local_x_vec = Vector(0.5f * norm.y * norm.z, 0.5f * norm.x * norm.z, -1.0f * norm.x * norm.y);
    if (m_local_x_vec.length() == 0) 
        m_local_x_vec = Vector(norm.y, norm.z, norm.x);

    m_local_y_vec = norm % m_local_x_vec;
    m_local_x_vec = m_local_x_vec.normalize();
    m_local_y_vec = m_local_y_vec.normalize();
}


// ----------------------------------------------------------------------------
Intersection Disc::intersect(Ray ray)
{
    if (fabs(ray.v * m_norm_vec)<EPS) return Intersection();

    Intersection intersection;
    intersection.obj = this;
    intersection.t = (m_center - ray.eye) * m_norm_vec / (ray.v * m_norm_vec);
    intersection.pos = ray.eye + ray.v * intersection.t;

    if ((m_center - intersection.pos).length() > m_r) return Intersection();

    intersection.mat    = m_material;
    intersection.normal = m_norm_vec;

    return intersection;
}

// ----------------------------------------------------------------------------
void Disc::addToPhotonMap(Vector pos, Color power)
{
    int x, y;
    photonMapIndexFromPos(pos, x, y);
    m_photon_map[x][y] += power;
}

// ----------------------------------------------------------------------------
Color Disc::getPhotonMapColor(Vector pos)
{
    int x, y;
    photonMapIndexFromPos(pos, x, y);
    float element_num = 0;
    Color c;

    for (int i = x - 10; i<x + 10; i++)
        for (int j = y - 10; j<y + 10; j++)
        {
            if (i >= 0 && i < CPMX && y >= 0 && y < CPMY && Vector(x - i, y - j).length() < 5)
            {
                element_num++;
                c = c + m_photon_map[i][j];
            }
        }
    if (element_num > 0) c = c / element_num;
    return c;
}

// ----------------------------------------------------------------------------
Color Disc::kd(Vector p)
{
    if (!m_material.textured)
        return m_material.kd;

    int x, y;
    photonMapIndexFromPos(p, x, y);
    int px = (x / (CPMX / 10)) % 2;
    int py = (y / (CPMY / 10)) % 2;

    if (px+py == 1)
       return Color(0, 0.8, 0);
    else return m_material.kd;
}

// ----------------------------------------------------------------------------
Color Disc::ka(Vector p)
{
    if (!m_material.textured)
        return m_material.ka;

    int x, y;
    photonMapIndexFromPos(p, x, y);
    int px = (x / (CPMX / 10)) % 2;
    int py = (y / (CPMY / 10)) % 2;

    if (px + py == 1)
        return Color(0, 0.7, 0);
    else return m_material.ka;
}
