#include "raytracer.h"

// ----------------------------------------------------------------------------
Intersection RayTracer::intersectAll(Ray ray)
{
    Intersection optimal;
    Intersection current;
    optimal.t = INFINITE;
    std::list<Object*>::iterator it;
    for (it = m_objects.begin(); it != m_objects.end(); it++)
    {
        current = (*it)->intersect(ray);
        if (current.t > EPS && current.t < optimal.t)
            optimal = current;
    }
    if (fabs(optimal.t - INFINITE) < EPS) return Intersection();
    else
        return optimal;
}

// ----------------------------------------------------------------------------
void RayTracer::shoot(Color power, Ray ray, int level)
{
    if (level > m_rec_level) return;
    Intersection intersection = intersectAll(ray);

    if (intersection.t < 0) return;

    if (intersection.mat.is_reflective)
    {
        Ray reflectedRay = Ray(intersection.pos, ray.v - intersection.normal * (intersection.normal * ray.v) * 2);
        shoot(intersection.mat.fresnel(intersection.normal, reflectedRay.v) * power, reflectedRay, level + 1);
    }

    if (intersection.mat.is_refractive)
    {
        Ray refractRay(intersection.pos, Vector());
        if (intersection.mat.refractDir(ray.v, intersection.normal, refractRay.v))
            shoot((intersection.mat.fresnel(intersection.normal, refractRay.v) - 1)*power, refractRay, level + 1);
    }
    if (intersection.mat.rough) intersection.obj->addToPhotonMap(intersection.pos, power);
} // shoot

// ----------------------------------------------------------------------------
void RayTracer::createPhotonMaps()
{
    float x, y, z;
    std::list<Light>::iterator it;
    for (it = m_lights.begin(); it != m_lights.end(); it++)
    {
        for (int i = 0; i < 30000; i++)
        {
            do
            {
                x = RND; y = RND; z = RND;
            } while (x*x + y*y + z*z > 1);

            Ray ray(it->pos, Vector(x, y, z));
            shoot(it->getColor() / 30, ray);
        }
    }
}

// ----------------------------------------------------------------------------
Color RayTracer::trace(Ray ray, int level)
{
    if (level > m_rec_level) return Color(0.0, 0.0, 0.0);

    Intersection in = intersectAll(ray);
    if (in.t < EPS) return Color::getSkyColor();

    Color color = in.obj->ka(in.pos) * Color::getSkyColor() + (in.obj->getPhotonMapColor(in.pos));

    std::list<Light>::iterator it;
    for (it = m_lights.begin(); it != m_lights.end(); it++)
    {
        Ray shadow(in.pos, it->pos - in.pos);
        Intersection shadow_hit = intersectAll(shadow);
        if (shadow_hit.t < 0 || (in.pos - shadow_hit.pos).length() >(in.pos - it->pos).length())
        {
            Vector V, L, H;
            V = ray.v;
            L = shadow.v;
            H = (V + L).normalize();
            float ln = L * in.normal;
            float hn = H * in.normal;
            if (ln < 0) ln = 0;
            if (hn < 0) hn = 0;
            color += it->getIntes(in.pos) * (in.obj->kd(in.pos) * ln + in.mat.ks * pow(hn, in.mat.shine));
        }
    }

    if (in.mat.is_reflective)
    {
        Ray reflectedRay = Ray(in.pos, ((ray.v - in.normal * (in.normal * ray.v) * 2)));
        color += in.mat.fresnel(in.normal, reflectedRay.v) * trace(reflectedRay, level + 1);
    }

    if (in.mat.is_refractive)
    {
        Ray refractRay(in.pos, Vector());
        if (in.mat.refractDir(ray.v, in.normal, refractRay.v))
            color += (Color(1, 1, 1) - in.mat.fresnel(in.normal, refractRay.v)) * trace(refractRay, level + 1);
    }

    return color;
} // trace

// ----------------------------------------------------------------------------
RayTracer::RayTracer(Camera cam, unsigned int rec_level)
{
    m_camera = cam;
    m_rec_level = 5;
    m_first_run = true;
}

// ----------------------------------------------------------------------------
RayTracer::~RayTracer()
{
    std::list<Object*>::iterator it;
    for (it = m_objects.begin(); it != m_objects.end(); it++)
    {
        delete (*it);
    }
}

// ----------------------------------------------------------------------------
void RayTracer::addLight(Light light)
{
    m_lights.push_back(light);
}

// ----------------------------------------------------------------------------
void RayTracer::addObject(Object* obj)
{
    m_objects.push_back(obj);
}

// ----------------------------------------------------------------------------
void RayTracer::render(Color* image)
{
    if (m_first_run)
    {
        m_first_run = false;
        createPhotonMaps();
    }

    for (int i = 0; i < screen_width; i++)
        for (int j = 0; j < screen_height; j++)
            image[j*screen_width + i] = trace(m_camera.getRay(float(i), float(j)));
}
