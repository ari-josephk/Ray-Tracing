#pragma once
#include "vector.h"

//Simple implementation of a ray with a color
class ray
{
    public:
        vector e[2];

        ray() {}
        ray(const vector& origin, const vector& dir) { 
            e[0] = origin;
            e[1] = dir;
        }
        vector origin() const       { return e[0]; }
        vector dir() const    { return e[1]; }
        vector point_at_parameter(float t) const { return e[0] + t*e[1]; }
};