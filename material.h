#pragma once

#include "hitable.h"

class material {
    public:
        virtual bool scatter(const ray& r, const hit_record& rec, vector& attenuation, ray& scattered) const = 0;
};

vector random_point_in_sphere() {
    vector p;
    do{
        float x = float(rand())/RAND_MAX;
        float y = float(rand())/RAND_MAX;
        float z = float(rand())/RAND_MAX;
        p = 2 * vector(x, y, z) - vector(1, 1, 1);
    } while (p.length() > 1);
    return p;
}

vector reflect_vector(vector v, vector n){
    return v - 2*dot(v,n)*n;
}

float schlick(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

bool refract(vector v, vector n, float ni_over_nt, vector& refract_vector){
    vector uv = v;
    uv.make_unit_vector();

    float dt = dot(uv, n);
    float disc = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);

    if(disc > 0){
        refract_vector = ni_over_nt*(uv-n*dt) - n*sqrt(disc);
        return true;
    } else return false;
}

class lamb : public material {
    public: 
        vector albedo;

        lamb(vector a){
            albedo = a;
        }

        virtual bool scatter(const ray& r, const hit_record& rec, vector& attenuation, ray& scattered) const {
            vector reflect = rec.p + rec.normal + random_point_in_sphere();
            scattered = ray(rec.p, reflect-rec.p);
            attenuation = albedo;
            return true;
        }
};

class metal : public material {
    public: 
        vector albedo;
        float fuzz;

        metal(vector a, float f){
            albedo = a;
            fuzz = f;
        }

        virtual bool scatter(const ray& r, const hit_record& rec, vector& attenuation, ray& scattered) const {
            vector reflect = reflect_vector(r.dir(), rec.normal);
            reflect.make_unit_vector();
            scattered = ray(rec.p, reflect + fuzz*random_point_in_sphere());
            attenuation = albedo;
            return true;
        }
};

class dielectric : public material {
    public:
        float ref_idx;

        dielectric(float ri){
            ref_idx = ri;
        }
        virtual bool scatter(const ray& r_in, const hit_record& rec, vector& attenuation, ray& scattered) const  {
            vector outward_normal;
            vector reflected = reflect_vector(r_in.dir(), rec.normal);
            float ni_over_nt;
            attenuation = vector(1.0, 1.0, 1.0);
            vector refracted;
            float reflect_prob;
            float cosine;
            if (dot(r_in.dir(), rec.normal) > 0) {
                outward_normal = -(rec.normal);
                ni_over_nt = ref_idx;
                cosine = ref_idx * dot(r_in.dir(), rec.normal) / r_in.dir().length();
               //   cosine = dot(r_in.dir(), rec.normal) / r_in.dir().length();
               //   cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
             }
            else {
                outward_normal = rec.normal;
                ni_over_nt = 1.0 / ref_idx;
                cosine = -dot(r_in.dir(), rec.normal) / r_in.dir().length();
            }
            if (refract(r_in.dir(), outward_normal, ni_over_nt, refracted))
                reflect_prob = schlick(cosine, ref_idx);
            else
                scattered = ray(rec.p, reflected);
                reflect_prob = 1.0;
            if ((float(rand()) / RAND_MAX) < reflect_prob)
               scattered = ray(rec.p, reflected);
            else
               scattered = ray(rec.p, refracted);

            return true;
        }
};