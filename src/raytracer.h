#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "material.h"
#include "camera.h"

#include <list>

class RayTracer
{
private:
    Camera             m_camera;

    std::list<Object*> m_objects;
    std::list<Light>   m_lights;

    unsigned int       m_rec_level;

    bool               m_first_run;

    Intersection intersectAll(Ray ray);
    void         shoot(Color power, Ray ray, int level = 0);
    void         createPhotonMaps();
    Color        trace(Ray ray, int level = 0);
public:
    RayTracer(Camera cam, unsigned int rec_level = 5);
    ~RayTracer();

    void addLight(Light light);
    void addObject(Object* obj);

    void setCamera(Camera c) { m_camera = c; }

    void render(Color* render);
};

#endif
