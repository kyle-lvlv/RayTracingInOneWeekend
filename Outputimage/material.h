#ifndef fMATERIAL_H
#define fMATERIAL_H

#include "hitable.h"
#include "vec3.h"

vec3 reflect(const vec3& v, const vec3& n)
{
    return v - 2*dot(v, n)*n;
}

float schlick(float cosine, float ref_indx)
{
    float r0 = (1 - ref_indx) / (1 + ref_indx);
    r0 *= r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool refract(const vec3& v, const vec3& n, float e, vec3& refracted)
{
    vec3 uv = unit_vector(v);
    float cosi1 = dot(-uv, n);
    float cosi2s = 1 - e * e * (1 - cosi1 * cosi1);
    if(cosi2s > 0)
    {
        refracted = e * uv + n * (e * cosi1 - sqrt(cosi2s));
        return true;
    }
    else
        return false;
}

class material
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const vec3& a) : albedo(a){}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        scattered = ray(rec.hit_point, rec.normal + random_in_unit_sphere());
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

class metal : public material
{
public:
    metal(const vec3& a, float f) : albedo(a), fuzz(f){}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
    {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.hit_point, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 albedo;
    float fuzz;
};

class dielectric : public material
{
public:
    dielectric(float ri) : ref_idx(ri){}
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const;

    float ref_idx;
};

bool dielectric::scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
{
    vec3 outward_normal;
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    float e;
    attenuation = vec3(1,1,1);

    vec3 refracted;
    float reflcet_prob, cose;

    if(dot(r_in.direction(), rec.normal) > 0)
    {
        e = ref_idx;
        outward_normal = - rec.normal;
        cose = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
    }
    else
    {
        e = 1.0 / ref_idx;
        outward_normal = rec.normal;
        cose = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
    }

    if(refract(r_in.direction(), outward_normal, e, refracted))
        reflcet_prob = schlick(cose, ref_idx);
    else
        reflcet_prob = 1.;


    if(random_in_range(0, 1) < reflcet_prob)
        scattered = ray(rec.hit_point, reflected);
    else
        scattered = ray(rec.hit_point, refracted);

    return true;
}


#endif // fMATERIAL_H
