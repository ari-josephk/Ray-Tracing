#pragma once
#include "ray.h"

class material;

struct hit_record{
    float t;
    vector p;
    vector normal;
    material *mat_ptr;
};

//Parent class for all object rays can "hit"
class hitable  {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};