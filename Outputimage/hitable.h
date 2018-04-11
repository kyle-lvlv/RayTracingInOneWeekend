#ifndef fHITABLE_H
#define fHITABLE_H

#include "ray.h"

class material;

struct hit_record
{
    vec3 hit_point;
    vec3 normal;
    float t;

    material* mat_ptr;
};

float random_in_range(float t_min, float t_max)
{
    if(t_max <= t_min)
        return 0.0;
    return (rand() % 100) / 100.0 * (t_max - t_min) + t_min;
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do{
        p = 2.0 * vec3(random_in_range(0,1) , random_in_range(0,1), random_in_range(0,1)) - vec3(1,1,1);
    } while (p.squared_length() >= 1.);
    return p;
}
vec3 random_in_unit_disk()
{
    vec3 p;
    do{
        p = 2.0 * vec3(random_in_range(0,1) , random_in_range(0,1), 0) - vec3(1,1,0);
    } while (dot(p, p) >= 1.);
    return p;
}

class hitable
{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

class sphere : public hitable
{
public:
    sphere();
    sphere(vec3 c, float r, material* m) : center(c), radius(r),mat(m){}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    vec3 center;
    float radius;
    material* mat;
};


class hitable_list : public hitable
{
public:
    hitable_list(){}
    hitable_list(hitable** l, int size){list = l; list_size = size;}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    hitable** list;
    int list_size;
};



bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;

    float disciminant = b * b - 4 * a * c;
    if(disciminant > 0)
    {
        float t = (-b - sqrt(disciminant)) / (2 * a);
        if(t > t_min && t < t_max)
        {
            rec.t = t;
            rec.hit_point = r.point_at_parameter(rec.t);
            rec.normal = (rec.hit_point - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }
        t = (-b + sqrt(disciminant)) / (2 * a);
        if(t > t_min && t < t_max)
        {
            rec.t = t;
            rec.hit_point = r.point_at_parameter(rec.t);
            rec.normal = (rec.hit_point - center) / radius;
            rec.mat_ptr = mat;
            return true;
        }
    }
    return false;
}

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for(int i = 0; i < list_size; i++)
    {
        if(list[i]->hit(r, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
#endif // fHITABLE_H
