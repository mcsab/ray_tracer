#ifndef COLOR_H
#define COLOR_H

struct Color 
{
    float r, g, b;

    Color() 
    {
        r = g = b = 0.0;
    }

    Color(float r0, float g0, float b0) 
    {
        r = r0; g = g0; b = b0;
    }

    static Color getSkyColor()
    {
        return Color(0.529f, 0.809f, 0.98f);
        //return Color(0.25, 0.4, 0.49);
    }

    Color operator*(float a)
    {
        return Color(r * a, g * a, b * a);
    }

    Color operator/(float a) 
    {
        return Color(r / a, g / a, b / a);
    }

    Color operator*(const Color& c)
    {
        return Color(r * c.r, g * c.g, b * c.b);
    }

    Color operator/(const Color& c)
    {
        return Color(r / c.r, g / c.g, b / c.b);
    }

    Color operator+(const Color& c)
    {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator-(const Color& c)
    {
        return Color(r - c.r, g - c.g, b - c.b);
    }

    Color operator+(const float c)
    {
        return Color(r + c, g + c, b + c);
    }

    Color operator-(const float c)
    {
        return Color(r - c, g - c, b - c);
    }

    void operator +=(const Color& c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
    }

    void operator=(const Color& c)
    {
        r = c.r;
        g = c.g;
        b = c.b;
    }
};

#endif
