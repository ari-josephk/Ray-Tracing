#pragma once
#include "hitable.h"

//implementation for a group of hitable objects. This is itself a hitable
class hitable_group: public hitable  {
    public:
        hitable **list;
        int list_size;
        hitable_group(hitable **l, int n) { 
            list = l; 
            list_size = n; 
        }
        
        //using hit implementation of each hittable, find the closest object that is hit from the origin of the ray
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
            hit_record temp_rec;
            bool hit_anything = false;
            double closest_so_far = t_max;
            for (int i = 0; i < list_size; i++) {
                if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }
            return hit_anything;
        }
};
