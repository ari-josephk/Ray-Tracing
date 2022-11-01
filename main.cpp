#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "vector.h"
#include "ray.h"
#include "hitable.h"
#include "hitable_group.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

using namespace std;

//PARAMS
const int NUM_SPHERES = 50;
const int ANTIALIAS_SAMPLES = 25;
const float APERTURE = 0.05;
const vector CAMERA_LOC = vector(10, 1, 4);
const int MAX_REFLECTIONS = 20;


vector get_col(const ray& r, hitable *world, int reflections){
    hit_record rec;
    //minimum hit distance of 0.01 to avoid self-hitting bug
    if(world->hit(r, 0.0001, 100000000, rec)){
        ray reflect;
        vector attenuation;
        if(reflections < MAX_REFLECTIONS && rec.mat_ptr->scatter(r, rec, attenuation, reflect)){
            return attenuation*get_col(reflect, world, reflections + 1);
        }
        //prevent infinite bounces
        else return vector(0,0,0);
    } else {
        //if ray hits nothing, background
        vector unit_dir = r.dir();
        unit_dir.make_unit_vector();

        float t = 0.5*(unit_dir.y() + 1.0);
        return (1.0-t)*vector(1.0, 1.0, 1.0) + t*vector(0.5, 0.7, 1.0);
    }
}

hitable *create_scene(int n) {
    hitable **list = new hitable*[4 + n];
    list[0] =  new sphere(vector(0,-1000,0), 1000, new lamb(vector(0.5, 0.5, 0.5)));

    vector col(float(rand())/RAND_MAX, float(rand())/RAND_MAX, float(rand())/RAND_MAX);

    list[1] = new sphere(vector(0, 1, 0), 1.0, new dielectric(1.5));
    list[2] = new sphere(vector(-4, 1, 0), 1.0, new lamb(col));
    list[3] = new sphere(vector(4, 1, 0), 1.0, new metal(vector(0.7, 0.6, 0.5), 0.0));

    int i = 4;
    
    for(int j = 0; j < n; j++){
        vector center(14* float(rand())/RAND_MAX-7, 0.2, 8*float(rand())/RAND_MAX-4);
        vector col(float(rand())/RAND_MAX, float(rand())/RAND_MAX, float(rand())/RAND_MAX);
        float radius = 0.6*float(rand())/RAND_MAX;
        float mat = float(rand())/RAND_MAX;
        float fuzz = 0.5 * float(rand())/RAND_MAX;

        if(abs(center[0]) < 5 && abs(center[2]) < 1) continue;
        if(mat < 0.7) list[i++] = new sphere(center, radius, new lamb(col));
        else if(mat < 0.9) list[i++] = new sphere(center, radius, new metal(col, fuzz));
        else list[i++] = new sphere(center, radius, new dielectric(1.5));
        //printf("Added to list");
    }

    return new hitable_group(list, i);
}

int main() {
    //Set a random seed
    srand(time(0));

    int nx = 1200;
    int ny = 800;
    int ns = ANTIALIAS_SAMPLES;

    ofstream o;
    o.open("outimg.ppm", ofstream::out | ofstream::trunc);

    o << "P3\n" << nx << " " << ny << "\n255\n";

    //create a helper method to generate a random world of spheres
    hitable *world = create_scene(NUM_SPHERES);

    //set aspect ratio, aperture, location, look_at of camera
    vector camera_loc = CAMERA_LOC;
    vector lookat(2, 0.5, 0);
    float focus_dist = (camera_loc-lookat).length();
    float aperture = APERTURE;

    camera cam(camera_loc, lookat, vector(0, 1, 0), 40, float(nx)/float(ny), aperture, focus_dist);

    for (int j = ny-1; j >= 0; j--){
        for(int i = 0; i < nx; i++){ 
            //antialiasing, add up colors for each sample, then divide by # samples
            vector col;
            for (int s = 0; s < ns; s++){
                float u = float(i + float(rand())/RAND_MAX) / float(nx);
                float v = float(j + float(rand())/RAND_MAX) / float(ny);
                ray r = cam.get_ray(u, v);
                
                col += get_col(r, world, 0);
            }

            col /= float(ns);
            //account for gamma 2 correcting by taking sqrt
            col = vector(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));


            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            o << ir << " " << ig << " " << ib << "\n";
            
        }
    }
    o.close();
}