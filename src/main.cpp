#include <stdlib.h>
#include <stdio.h>

#include "bmp/EasyBMP.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "raytracer.h"

#include "cylinder.h"
#include "disc.h"
#include "paraboloid.h"

#include <string>    

Color image[screen_width*screen_height];
Color temp_img[screen_width*screen_height];

RayTracer ray_tracer(Camera(Vector(0, 16.4, -14), Vector(0, 15.8, -13), Vector(0, 1, 0), Vector(1, 0, 0)));

// ----------------------------------------------------------------------------
void initScene()
{
    Paraboloid *p1 = new Paraboloid(Vector(0.0f, 1.0f, 0.0f), Vector(0.0f, 0.0f, 0.0f), 3.0f);
    p1->setMaterial(Material::gold);

    Paraboloid *p2 = new Paraboloid(Vector(0.0f, 6.0f, 0.0f), Vector(0.0f, 7.0f, 0.0f), 3.0f);
    p2->setMaterial(Material::gold);

    Cylinder *c = new Cylinder(Vector(0, -6, 0), Vector(0, 1, 0), 24, 32,
        true, false, Material::green, Material());
    c->setMaterial(Material::gold);

    Light l1(Vector(0.0f, 3.5f, 0.0f), Color(120, 120, 0));

    Paraboloid *p3 = new Paraboloid(Vector(15.0f, 14.0f, 15.0f), Vector(15.2f, 14.4f, 15.2f), 5.0f);
    p3->setMaterial(Material::silver);
    Light l2(Vector(15.1f,  13.8f,  15.1f),   Color(60, 0, 0));
    
    Paraboloid *p4 = new Paraboloid(Vector(15.0f, 14.0f, -15.0f), Vector(15.2f, 14.4f, -15.2f), 5.0f);
    p4->setMaterial(Material::silver);
    Light l3(Vector(15.1f,  13.8f, -15.1f),  Color(0, 60, 0));

    Paraboloid *p5 = new Paraboloid(Vector(-15.0f, 14.0f, 15.0f), Vector(-15.2f, 14.4f, 15.2f), 5.0f);
    p5->setMaterial(Material::silver);
    Light l4(Vector(-15.1f, 13.8f,  15.1f),  Color(60, 30, 0));
    
    Paraboloid *p6 = new Paraboloid(Vector(-15.0f, 14.0f, -15.0f), Vector(-15.2f, 14.4f, -15.2f), 5.0f);
    p6->setMaterial(Material::silver);
    Light l5(Vector(-15.1f, 13.8f, -15.1f), Color(30, 60, 0));

    Cylinder *ca_bottom = new Cylinder(Vector(1.5f, -3.0f, -0.866f), Vector(0.3f, 1.0f, 0.0f), 0.8f, 6,
                                true, true, Material::copper, Material::copper);
    ca_bottom->setMaterial(Material::copper);
    Cylinder *ca_top = new Cylinder(Vector(1.5f, 10.0f, -0.866f), Vector(0.3f, -1.0f, 0.0f), 0.8f, 6,
        true, true, Material::copper, Material::copper);
    ca_top->setMaterial(Material::copper);


    Cylinder *cb_bottom = new Cylinder(Vector(-1.5f, -3.0f, -0.866f), Vector(-0.3f, 1.0f, 0.0f), 0.8f, 6,
                                true, true, Material::silver, Material::silver);
    cb_bottom->setMaterial(Material::silver);
    Cylinder *cb_top = new Cylinder(Vector(-1.5f, 10.0f, -0.866f), Vector(-0.3f, -1.0f, 0.0f), 0.8f, 6,
        true, true, Material::silver, Material::silver);
    cb_top->setMaterial(Material::silver);

    Cylinder *cc_bottom = new Cylinder(Vector(0.0f, -3.0f, 1.732f), Vector(0.0f, 1.0f, 0.3f), 0.8f, 6,
                                true, true, Material::glass, Material::glass);
    cc_bottom->setMaterial(Material::glass);
    Cylinder *cc_top = new Cylinder(Vector(0.0f, 10.0f, 1.732f), Vector(0.0f, -1.0f, 0.3f), 0.8f, 6,
        true, true, Material::glass, Material::glass);
    cc_top->setMaterial(Material::glass);

    ray_tracer.addObject(c);

    ray_tracer.addObject(ca_top);
    ray_tracer.addObject(ca_bottom);
    ray_tracer.addObject(cb_top);
    ray_tracer.addObject(cb_bottom);
    ray_tracer.addObject(cc_top);
    ray_tracer.addObject(cc_bottom);

    ray_tracer.addObject(p1);
    ray_tracer.addObject(p2);
    ray_tracer.addObject(p3);
    ray_tracer.addObject(p4);
    ray_tracer.addObject(p5);
    ray_tracer.addObject(p6);

    ray_tracer.addLight(l1);
    ray_tracer.addLight(l2);
    ray_tracer.addLight(l3);
    ray_tracer.addLight(l4);
    ray_tracer.addLight(l5);
} // initScene

// ----------------------------------------------------------------------------
void onDisplay()
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(screen_width, screen_height, GL_RGB, GL_FLOAT, image);
    glutSwapBuffers();
}

// ----------------------------------------------------------------------------
void exportToFile(std::string file_name)
{
    BMP img;
    img.SetSize(screen_width, screen_height);
    img.SetBitDepth(32);

    for (int j = 0; j < screen_height; j++)
        for (int i = 0; i < screen_width; i++)
        {
            int r = (int)(255.0f * image[j*screen_width + i].r);
            int g = (int)(255.0f * image[j*screen_width + i].g);
            int b = (int)(255.0f * image[j*screen_width + i].b);
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;
            if (r < 0) r = 0;
            if (g < 0) g = 0;
            if (b < 0) b = 0;

            img(i, screen_height - j - 1)->Red   = r;
            img(i, screen_height - j - 1)->Blue  = b;
            img(i, screen_height - j - 1)->Green = g;
        }
    img.WriteToFile((file_name + ".bmp").c_str());
}

// ----------------------------------------------------------------------------
int main(int argc, char **argv) 
{

    glutInit(&argc, argv);
    glutInitWindowSize(screen_width, screen_height);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Elathan's first Ray Tracer");

    glViewport(0, 0, screen_width, screen_height);
    Material::initMaterials();
    
    initScene();
    ray_tracer.render(image);

    //exportToFile("test");
   

    //for (float i = 0.0f; i < 90.0f; i += 0.2f)
    //for (float i = 90.0f; i < 180.0f; i += 0.2f)
    //for (float i = 180.0f; i < 270.0f; i += 0.2f)
    //for (float i = 270.0f; i < 360.0f; i += 0.2f)
    //{
    //    Camera c(Vector(0, 16.4, -14), Vector(0, 15.8, -13), Vector(0, 1, 0), Vector(1, 0, 0));
    //    c.rotateAroundY(i);
    //    ray_tracer.setCamera(c);
    //    ray_tracer.render(image);
    //    exportToFile(std::to_string(i+100000));
    //    std::cout << i << " rendered" << std::endl;
    //}
    

    glutDisplayFunc(onDisplay);
    glutMainLoop();

    return 0;
}
