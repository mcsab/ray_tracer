#include "material.h"

Material Material::green;
Material Material::blue;
Material Material::gold;
Material Material::silver;
Material Material::copper;
Material Material::glass;

// ----------------------------------------------------------------------------
Material::Material(Color i_n, Color i_k, Color i_kd, Color i_ks, Color i_ka, int i_shine,
                                              bool i_is_reflective, bool i_is_refractive)
{
    n             = i_n;
    k             = i_k;
    kd            = i_kd;
    ks            = i_ks;
    ka            = i_ka;
    shine         = i_shine;
    is_reflective = i_is_reflective;
    is_refractive = i_is_refractive;
    rough         = false;
}

// ----------------------------------------------------------------------------
void Material::operator=(const Material &other)
{
    n             = other.n;
    kd            = other.kd;
    ks            = other.ks;
    ka            = other.ka;
    shine         = other.shine;
    is_reflective = other.is_reflective;
    is_refractive = other.is_refractive;
    rough         = other.rough;
    textured      = other.textured;
}

// ----------------------------------------------------------------------------
Color Material::fresnel(Vector& N, Vector& V)
{
    float cosa = fabs(N * V);
    Color F0 = ((n - 1) * (n - 1) + (k * k)) / ((n + 1)*(n + 1) + (k * k));
    return  F0 + (F0 * (-1) + 1) * pow(1 - cosa, 5);
}

// ----------------------------------------------------------------------------
bool Material::refractDir(Vector V, Vector N, Vector &newDir)
{
    float cosa = N * V * (-1);
    float cn = n.r;
    if (cosa < 0)
    {
        cosa *= -1;
        N = N * (-1);
        cn = 1 / cn;
    }
    float disc = 1 - (1 - cosa * cosa) / cn / cn;
    if (disc < 0) return false;

    newDir = V / cn + N * (cosa / cn - sqrt(disc));
    return true;
}


// ----------------------------------------------------------------------------
void Material::initMaterials()
{
    green.n              = Color(0.2f, 0.2f, 0.2f);
    green.k              = Color(0.2f, 0.2f, 0.2f);
    green.kd             = Color(0.8f, 0.05f, 0.05f);
    green.ks             = Color(0.15f, 0.65f, 0.15f);
    green.ka             = Color(0.3f, 0.0f, 0.0f);
    green.shine          = 30;
    green.is_reflective  = false;
    green.is_refractive  = false;
    green.rough          = true;
    green.textured       = true;
                         
    blue.n               = Color(0.2f, 1.1f, 1.2f);
    blue.k               = Color(0.2f, 1.1f, 1.2f);
    blue.kd              = Color(0.06f, 0.9f, 0.99f);
    blue.ks              = Color(0.06f, 0.34f, 0.54f);
    blue.ka              = Color(0.00f, 0.5f, 0.6f);
    blue.shine           = 30;
    blue.is_reflective   = false;
    blue.is_refractive   = false;
    blue.rough           = true;
    blue.textured        = false;

    gold.n               = Color(0.17f, 0.35f, 1.5f);
    gold.k               = Color(3.1f, 0.35f, 1.9f);
    gold.kd              = Color(0.4,0.4,0.0f);
    gold.ks              = Color(0,0,0);
    gold.ka              = Color(0.1,0.1,0);
    gold.shine           = 30;
    gold.is_reflective   = true;
    gold.is_refractive   = false;
    gold.rough           = false;

    silver.n             = Color(0.14f, 0.16f, 0.13f);
    silver.k             = Color(4.1f, 2.3f, 3.1f);
    silver.kd            = Color(0.50754f, 0.50754f, 0.50754f);
    silver.ks            = Color(0.508273f, 0.508273f, 0.508273f);
    silver.ka            = Color(0.19225f, 0.19225f, 0.19225f);
    silver.shine         = 30;
    silver.is_reflective = true;
    silver.is_refractive = false;

    copper.n             = Color(0.2f, 1.1f, 1.2f);
    copper.k             = Color(3.6f, 2.6f, 2.3f);
    copper.kd            = Color(0.7038f, 0.27048f, 0.0828f);
    copper.ks            = Color(0.256777f, 0.137622f, 0.086014f);
    copper.ka            = Color(0.19125f, 0.0735f, 0.0225f);
    copper.shine         = 30;
    copper.is_reflective = true;
    copper.is_refractive = false;

    glass.n             = Color(1.4f, 2.4f, 1.4f);
    glass.k             = Color(0.0f, 0.0f, 0.0f);
    glass.kd            = Color(0.2038f, 0.67048f, 0.0828f);
    glass.ks            = Color(0.256777f, 0.637622f, 0.086014f);
    glass.ka            = Color(0.09125f, 0.2735f, 0.0225f);
    glass.shine         = 30;
    glass.is_reflective = true;
    glass.is_refractive = true;
}
