#pragma once
#include "ray.h"
#define M_PI 3.14159265358979323846

vector random_in_unit_disk() {
    vector p;
    do {
        p = 2.0*vector (float(rand())/RAND_MAX, float(rand())/RAND_MAX, 0) - vector(1,1,0);
    } while (dot(p,p) >= 1.0);
    return p;
}

//camera implementation that calculates the rays to each pixel
class camera {

    public:
        vector corner;
        vector dx;
        vector dy;
        vector origin;
        vector u, v, w;
        float lens_radius;

        camera(vector loc, vector lookat, vector vup, float vfov, float aspect_ratio, float aperture, float focus_dist) {
            float theta = vfov*M_PI/180;
            float half_height = tan(theta/2);
            float half_width = aspect_ratio * half_height;
            
            lens_radius = aperture / 2;

            w = loc - lookat;
            u = cross(vup, w);
            w.make_unit_vector();
            u.make_unit_vector();

            v = cross(w, u);

            origin = loc;

            corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
            dx = 2*half_width*focus_dist*u;
            dy = 2*half_height*focus_dist*v;       
        }

        ray get_ray(float s, float t){
            vector rd = lens_radius * random_in_unit_disk();
            vector offset = u*rd.x() + v*rd.y();

            return ray(origin+offset, corner + s*dx + t*dy-origin-offset);
        }
};