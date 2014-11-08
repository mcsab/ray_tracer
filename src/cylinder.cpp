#include "cylinder.h"
#include "disc.h"

// ----------------------------------------------------------------------------
void Cylinder::photonMapIndexFromPos(Vector pos, int& x, int& y)
{
    Vector pos_center_dist = pos - m_center_point;

    float h     = pos_center_dist * m_axis_vec;
    float vCosA = pos_center_dist * m_local_x_vec;
    float vSinA = pos_center_dist * m_local_y_vec;

    if (vCosA < -1.0) vCosA = -1.0;
    if (vSinA < -1.0) vSinA = -1.0;
    if (vCosA >  1.0) vCosA = 1.0;
    if (vSinA >  1.0) vSinA = 1.0;

    float alfa = acos(vCosA) * 180.0 / PI;
    if (asin(vSinA) < 0.0) alfa = 360 - alfa;

    x = (int)(alfa*(CPMX) / 360.0);
    y = (int)(h*(CPMY) / m_height);
}

// ----------------------------------------------------------------------------
Intersection Cylinder::intersectBody(Ray ray)
{
    float a, b, c, d;
    Vector eye_center_dist = ray.eye - m_center_point;
    float axis_dot_eye = m_axis_vec * ray.eye;

    a = ray.v * ray.v - pow((m_axis_vec*ray.v), 2);

    b = ((eye_center_dist * ray.v) -
        m_axis_vec * ray.v * (m_axis_vec * ray.eye - m_axis_vec*m_center_point)) * 2;

    c = (eye_center_dist * eye_center_dist) -
        pow((axis_dot_eye - m_axis_vec * m_center_point), 2) - pow(m_r, 2);

    d = b*b - 4 * a*c;

    if (d<0) return Intersection();  //no intersection

    Intersection intersection;
    intersection.obj = this;
    if (d == 0)
    {
        // 1 intersection point
        // the object may intersect with itself
        if (-b / (2 * a) < EPS) return Intersection();
        else intersection.t = -b / 2 * a;
    }
    else
    {
        // 2 intersection point
        float t1 = (-b + sqrt(d)) / (2 * a);
        float t2 = (-b - sqrt(d)) / (2 * a);

        bool t1Val = true;
        bool t2Val = true;

        // check if the points are points of the cylinder
        if ((m_axis_vec*(ray.eye + ray.v*t1 - m_center_point) > m_height ||
            m_axis_vec*(ray.eye + ray.v*t1 - m_center_point) < 0 || t1 < EPS))
            t1Val = false;

        if ((m_axis_vec*(ray.eye + ray.v*t2 - m_center_point) > m_height ||
            m_axis_vec*(ray.eye + ray.v*t2 - m_center_point) < 0 || t2 < EPS))
            t2Val = false;

        if (!t1Val && !t2Val) return Intersection();

        if (t1Val && (t1<t2 || !t2Val))
            intersection.t = t1;
        else
            intersection.t = t2;
    } // d!=0
    
    intersection.pos = ray.eye + ray.v * intersection.t;
    intersection.mat = m_material;
    
    Vector parhuzamos = m_axis_vec * ((intersection.pos - m_center_point) * m_axis_vec);
    intersection.normal = intersection.pos - m_center_point - parhuzamos;

    intersection.normal = intersection.normal.normalize();

    return intersection;
} // intersectBody

// ----------------------------------------------------------------------------
Cylinder::Cylinder(Vector center, Vector axis_vec, float r, float height,
                        bool bottom, bool top, Material bm, Material tm)
{
    m_center_point = center;
    m_r            = r;
    m_height       = height;
    m_axis_vec     = axis_vec.normalize();

    // tricky axis generation
    m_local_x_vec = Vector(0.5*m_axis_vec.y*m_axis_vec.z, 
                           0.5*m_axis_vec.x*m_axis_vec.z, 
                           -1 * m_axis_vec.x*m_axis_vec.y);
    if (m_local_x_vec.length() == 0) 
        m_local_x_vec = Vector(m_axis_vec.y, m_axis_vec.z, m_axis_vec.x);

    m_local_y_vec = m_axis_vec % m_local_x_vec;
    m_local_x_vec = m_local_x_vec.normalize();
    m_local_y_vec = m_local_y_vec.normalize();

    if (bottom)
    {
        m_bottom = new Disc(center, axis_vec, r);
        m_bottom->setMaterial(bm);
    }
    else m_bottom = nullptr;

    if (top)
    {
        m_top = new Disc(center + m_axis_vec.normalize() * height, axis_vec, r);
        m_top->setMaterial(tm);
    }
    else m_top = nullptr;
    
    for (int i = 0; i<CPMX + 1; i++)
        for (int j = 0; j<CPMY + 1; j++)
            m_photon_map[i][j] = Color();

} // Cylinder

// ----------------------------------------------------------------------------
Intersection Cylinder::intersect(Ray ray)
{
    Intersection best_in;
    Intersection in;
    best_in.t = INFINITE;

    in = intersectBody(ray);
    if (in.t > EPS && in.t < best_in.t) best_in = in;

    if (m_bottom != nullptr)
    {
        in = m_bottom->intersect(ray);
        if (in.t > EPS && in.t < best_in.t) best_in = in;
    }

    if (m_top != nullptr)
    {
        in = m_top->intersect(ray);
        if (in.t > EPS && in.t < best_in.t) best_in = in;
    }

    if (fabs(best_in.t - INFINITE) < EPS) return Intersection();
    else return best_in;
} // intersect

// ----------------------------------------------------------------------------
void Cylinder::addToPhotonMap(Vector pos, Color power)
{
    int x, y;
    photonMapIndexFromPos(pos, x, y);
    m_photon_map[x][y] += power;
}

// ----------------------------------------------------------------------------
Color Cylinder::getPhotonMapColor(Vector pos)
{
    int x, y;
    photonMapIndexFromPos(pos, x, y);
    int elementNum = 0;
    Color c;
    for (int i = x - 10; i < x + 10; i++)
        for (int j = y - 10; j < y + 10; j++)
        {
            if (i >= 0 && i <= CPMX &&j >= 0 && j <= CPMY)
            {
                elementNum++;
                c = c + m_photon_map[i][j];
            }
        }
    if (elementNum > 0) c = c / elementNum;
    return c;
} // getPhotonMapColor

// ----------------------------------------------------------------------------
Color Cylinder::kd(Vector p)
{
    if (!m_material.textured)
        return m_material.kd;

    int x, y;
    photonMapIndexFromPos(p, x, y);
    int px = (x / (CPMX / 10)) % 2;
    int py = (y / (CPMY / 10)) % 2;

    if (px + py == 1)
        return Color(1, 1, 1);
    else return m_material.kd;
}

// ----------------------------------------------------------------------------
Color Cylinder::ka(Vector p)
{
    if (!m_material.textured)
        return m_material.ka;

    int x, y;
    photonMapIndexFromPos(p, x, y);
    int px = (x / (CPMX / 10)) % 2;
    int py = (y / (CPMY / 10)) % 2;

    if (px + py == 1)
        return Color(1, 1, 1);
    else return m_material.ka;
}

// ----------------------------------------------------------------------------
Cylinder::~Cylinder()
{
    if (m_bottom)
        delete m_bottom;
    if (m_top)
        delete m_top;
}
