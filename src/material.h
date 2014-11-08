#ifndef MATERIAL_H
#define MATERIAL_H

#include "color.h"
#include "vector.h"

struct Light
{
    Vector pos;
    Color intensity;
    
    Light() {};
    Light(Vector p, Color i) { pos = p; intensity = i;                     }
    void operator=(Light &l) { pos = l.pos; intensity = l.intensity;       }
    Color getIntes(Vector v) { return intensity / pow((v-pos).length(),2); }
    Color getColor()         { return intensity;                           }
};

struct Material
{
    Color n;
    Color k;
    Color kd;
    Color ks;
    Color ka;
    int   shine;
    bool  is_reflective;
    bool  is_refractive;
    bool  rough;

    bool textured;

    static Material green;
    static Material blue;
    static Material gold;
    static Material silver;
    static Material copper;
    static Material glass;

    Material(Color i_n, Color i_k, Color i_kd, Color i_ks, Color i_ka, int i_shine, 
                                         bool i_is_reflective, bool i_is_refractive);

    Material() { rough = false; textured = false; }

    void operator=(const Material &other);

    Color fresnel(Vector& N, Vector& V);

    bool refractDir(Vector V, Vector N, Vector &newDir);

    static void initMaterials();
};

#endif
