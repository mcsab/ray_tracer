#include "paraboloid.h"


// ----------------------------------------------------------------------------
Paraboloid::Paraboloid(Vector focus, Vector center_of_mass, float height)
{
    m_focus_point    = focus;
    m_center_of_mass = center_of_mass;
    m_orientaion     = (focus - center_of_mass).normalize();
    m_height         = height;
}

// ----------------------------------------------------------------------------
Intersection Paraboloid::intersect(Ray ray)
{
    float a, b, c, d;
    Vector eye_focus_dist = ray.eye - m_focus_point;
    Vector eye_com_dist = ray.eye - m_center_of_mass;

    a = ray.v * ray.v - pow((ray.v * m_orientaion),2);
    
    b = 2 * (eye_focus_dist * ray.v - (eye_com_dist * m_orientaion) * (ray.v * m_orientaion));

    c = eye_focus_dist * eye_focus_dist - pow((eye_com_dist * m_orientaion),2);

    d = b*b - 4 * a*c;

    if (d<0) return Intersection();

    Intersection intersection;
    intersection.obj = this;
    if (d == 0) 
    {
        if (-b / (2 * a) < EPS) return Intersection();
        else intersection.t = -b / 2 * a;
    }
    else 
    {
        float t1 = (-b + sqrt(d)) / (2 * a);
        float t2 = (-b - sqrt(d)) / (2 * a);
        bool t1Val = true;
        bool t2Val = true;
        
        if ((ray.eye + ray.v * t1 - m_center_of_mass) * m_orientaion > m_height || t1 < EPS) 
            t1Val = false;
        if ((ray.eye + ray.v * t2 - m_center_of_mass) * m_orientaion > m_height || t2 < EPS) 
            t2Val = false;

        if (!t1Val && !t2Val) return Intersection();

        if (t1Val && (t1<t2 || !t2Val)) 
            intersection.t = t1;
        else 
            intersection.t = t2;
    } // d!=0

    intersection.pos = ray.eye + ray.v * intersection.t;
    intersection.mat = m_material;

    float x, y, z;
    x = intersection.pos.x;
    y = intersection.pos.y;
    z = intersection.pos.z;

    intersection.normal.x = x - m_focus_point.x - 
        m_orientaion.x * m_orientaion.x * (x - m_center_of_mass.x) -
        m_orientaion.x * m_orientaion.y*(y - m_center_of_mass.y)   -
        m_orientaion.x * m_orientaion.z*(z - m_center_of_mass.z);

    intersection.normal.y = y - m_focus_point.y - 
        m_orientaion.y * m_orientaion.y * (y - m_center_of_mass.y) - 
        m_orientaion.x * m_orientaion.y*(x - m_center_of_mass.x)   - 
        m_orientaion.y * m_orientaion.z*(z - m_center_of_mass.z);

    intersection.normal.z = z - m_focus_point.z -
        m_orientaion.z * m_orientaion.z * (z - m_center_of_mass.z) -
        m_orientaion.x * m_orientaion.z*(x - m_center_of_mass.x)   -
        m_orientaion.y * m_orientaion.z*(y - m_center_of_mass.y);

    intersection.normal = intersection.normal.normalize();

    return intersection;
} // intersect
