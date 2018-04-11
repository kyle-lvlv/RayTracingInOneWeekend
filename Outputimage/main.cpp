#include <iostream>
#include <fstream>
#include <limits>
#include <stdlib.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "camera.h"
#include "material.h"

#define MAX_FLOAT (numeric_limits<float>::max)()

using namespace std;

hitable* random_scene()
{
    int n = 500;
    hitable** g_list = new hitable*[n+1];
    g_list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(.5,.5,.5)));
    int i = 1;
    for(int a = -11; a < 11; a++)
    {
        for(int b = -11; b < 11; b++)
        {
            float choose_mat = random_in_range(0,1);
            vec3 center(a + 0.9 * random_in_range(0,1), 0.2, b + 0.9 * random_in_range(0,1));
            if((center - vec3(4,0.2,0)).length() > .9)
            {
                if(choose_mat < 0.8)
                {
                    g_list[i++] = new sphere(center, .2,
                                        new lambertian(vec3(random_in_range(0,1) * random_in_range(0,1), random_in_range(0,1) * random_in_range(0,1), random_in_range(0,1) * random_in_range(0,1))));
                }
                else if(choose_mat < 0.8)
                {
                    g_list[i++] = new sphere(center, .2,
                                        new metal( vec3( 0.5 * (1 + random_in_range(0,1)), 0.5 * (1 + random_in_range(0,1)), 0.5 * (1 + random_in_range(0,1)) ), 0.5 * random_in_range(0,1) ));
                }
                else
                {
                    g_list[i++] = new sphere(center, .2, new dielectric(1.5));
                }
            }
        }
    }
    g_list[i++] = new sphere(vec3(0,1,0), 1.0, new dielectric(1.5));
    g_list[i++] = new sphere(vec3(-4,1,0), 1.0, new lambertian(vec3(.4,.2,.1)));
    g_list[i++] = new sphere(vec3(4,1,0), 1.0, new metal(vec3(.7,.6,.5),.0));

    return new hitable_list(g_list, i);
}

vec3 color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
    if(world->hit(r, 0.001, MAX_FLOAT, rec))
    {
        ray scattered;
        vec3 attenuation;
        if(depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * color(scattered, world, depth + 1);
        else
            return vec3(.0,.0,.0);
    }
    else
    {
        vec3 dir = unit_vector(r.direction());
        float t = 0.5 * (dir.y()+ 1.0);
        return (1.0 - t) * vec3(1.0,1.0,1.0) + t * vec3(0.5,0.7,1.0);
    }
}


int main()
{
    int nx = 1200;
    int ny = 800;
    int ns = 10;

    ofstream outfile("out.txt", ios_base::out);
    outfile << "P3\n" << nx << " " << ny << "\n255\n";
    cout << "P3\n" << nx << " " << ny << "\n255\n";

//    vec3 lower_left_corner(-2.0,-1.0,-1.0);
//    vec3 horizontal(4.0,0.0,0.0);
//    vec3 vertical(0.0,2.0,0.0);
//    vec3 origin(0.0,0.0,0.0);
    vec3 cam_origin = vec3(11,3,2);
    vec3 cam_ori = vec3(0,0.6,0);
    float focus_dist = (cam_origin - cam_ori).length();
    std::cout << focus_dist << "\n";
    camera c(cam_origin, cam_ori, vec3(0,1,0), 20, float(nx) / float(ny), 0, 0.7 * focus_dist);
    //c.set_fov(20);
    hitable *list[5];
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1,.2,.5)));
    list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8,0.8,0.0)));
    list[2] = new sphere(vec3(1,0,-1), .5, new metal(vec3(0.8,0.6,0.2),.0));
    list[3] = new sphere(vec3(-1,0,-1), .5, new dielectric(1.5f));
    list[4] = new sphere(vec3(-1,0,-1), -.45, new dielectric(1.5f));
    hitable* world = new hitable_list(list, 5);
    world = random_scene();

    float u = 0;
    float v = 0;
    float r = 0;
    vec3 col ;
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            col = vec3(0,0,0);
            for(int s = 0; s < ns; s++)
            {
                r = random_in_range(0,1);
                u = float(i + r)  / float(nx);
                v = float(j + r) / float(ny);
                ray r = c.get_ray(u, v);
                col += color(r, world,0);
            }
            col /= float(ns);
            col[0] = sqrt(col[0]);
            col[1] = sqrt(col[1]);
            col[2] = sqrt(col[2]);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            outfile << ir << " " << ig << " " << ib << "\n";
            //cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}
