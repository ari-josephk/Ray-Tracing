#include <iostream>
#include <fstream>
#include "vector.h"
#include "ray.h"
#include "hitable.h"
#include "hitable_group.h"
#include "sphere.h"
#include "camera.h"

using namespace std;

vector get_col(const ray& r, hitable *world){
    hit_record rec;
    if(world->hit(r, 0, 100, rec)){
        return 0.5*vector(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
    } else {
        //if ray hits nothing, background
        vector unit_dir = r.dir();
        unit_dir.make_unit_vector();

        float t = 0.5*(unit_dir.y() + 1.0);
        return (1.0-t)*vector(1.0, 1.0, 1.0) + t*vector(0.5, 0.7, 1.0);
    }

    
}

int main() {
    int nx = 1200;
    int ny = 800;
    int ns = 100;

    ofstream o;
    o.open("outimg.ppm", ofstream::out | ofstream::trunc);

    o << "P3\n" << nx << " " << ny << "\n255\n";

    //create stuff to "hit"
    hitable *list[2];
    list[0] = new sphere(vector(0,0, -1), 0.5, NULL);
    list[1] = new sphere(vector(0, -100.5, -1), 100, NULL);
    hitable_group *world = new hitable_group(list, 2);

    camera cam;
    for (int j = ny-1; j >= 0; j--){
        for(int i = 0; i < nx; i++){
            //antialiasing, add up colors for each sample, then divide by # samples
            vector col;
            for (int s = 0; s < ns; s++){
                float u = float(i + float(rand())/RAND_MAX) / float(nx);
                float v = float(j + float(rand())/RAND_MAX) / float(ny);
                ray r = cam.get_ray(u, v);
                
                col += get_col(r, world);
            }

            col /= float(ns);

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            o << ir << " " << ig << " " << ib << "\n";
            
        }
    }
    o.close();
}