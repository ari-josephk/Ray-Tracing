#pragma once
#include "ray.h"

//camera implementation that calculates the rays to each pixel
class camera {
    public:
        vector corner;
        vector dx;
        vector dy;
        vector origin;
        camera() {
            corner = vector(-2, -1, -1);
            dx = vector(4, 0, 0);
            dy = vector(0, 2, 0);
            origin = vector(0, 0, 0);
        }

        ray get_ray(float u, float v){
            return ray(origin, corner + u*dx + v*dy);
        }
};