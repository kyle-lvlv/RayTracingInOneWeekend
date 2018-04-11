#ifndef fCAMERA_H
#define fCAMERA_H

#include "ray.h"

class camera
{
public:
    camera(vec3 lookfrom, vec3 ori, vec3 up, float vfov, float asp, float Aperture, float Focus_dis) : fov(vfov), aspect(asp),
                                                                                                        origin(lookfrom), lookat(ori), vup(up),
                                                                                                        lens_radius(Aperture / 2),focus_dist(Focus_dis)
    {
        float theta = vfov * M_PI / 180;
        half_height = tan(theta / 2);
        half_width = asp * half_height;
        set_camera(origin, lookat, vup);
    }

    camera(vec3 l_l_c, vec3 h, vec3 v):origin(vec3(0,0,0)), lower_left_corner(l_l_c),horizontal(h),vertical(v){};
    ray get_ray(float s, float t)
    {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

    void set_fov(float vfov);
    void set_aspect(float asp);
    void set_camera(vec3 lookfrom, vec3 ori, vec3 up);
    void set_focus(float aperture, float Focus_dis);

    float fov;
    float aspect;
    float half_width;
    float half_height;

    vec3 origin;
    vec3 lookat;
    vec3 vup;

    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

    vec3 u, v, w;

    float lens_radius;
    float focus_dist;

};


void camera::set_fov(float vfov)
{
    fov = vfov;
    float theta = fov * M_PI / 180;
    half_height = tan(theta / 2);
    half_width = aspect * half_height;
    set_camera(origin, lookat, vup);
}
void camera::set_aspect(float asp)
{
    aspect = asp;
    float theta = fov * M_PI / 180;
    half_height = tan(theta / 2);
    half_width = asp * half_height;
    set_camera(origin, lookat, vup);
}

void camera::set_camera(vec3 lookfrom, vec3 ori, vec3 up)
{
    origin = lookfrom;
    lookat = ori;
    vup = up;

    w = unit_vector(origin - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);
    std::cout << focus_dist << "\n";
    lower_left_corner = origin - focus_dist * (half_width * u + half_height * v + w);
    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
}


void camera::set_focus(float aperture, float Focus_dis)
{
    lens_radius = aperture / 2;
    focus_dist = Focus_dis;
    set_camera(origin, lookat, vup);
}


#endif // fCAMERA_H
