#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>

//A simple implementation of a 3-coordinate vector (for 2D rendering)
class vector {
    public:
        float e[3];
        //Construct using 3 points, default (0,0,0)
        vector() {
            e[0] = 0;
            e[1] = 0;
            e[2] = 0;
        } 
        vector(float x, float y, float z) {
            e[0] = x;
            e[1] = y;
            e[2] = z;
        } 

        float x() { return e[0]; }
        float y() { return e[1]; }
        float z() { return e[2]; }
        float r() { return e[0]; }
        float g() { return e[1]; }
        float b() { return e[2]; }

        vector operator-() { return vector(-e[0], -e[1], -e[2]); }
        float operator[](int i) const { return e[i]; }
        float& operator[](int i) { return e[i]; }

        vector operator+=(vector v2) {
            e[0] += v2.e[0];
            e[1] += v2.e[1];
            e[2] += v2.e[2];
            return *this;
        }

        vector& operator/=(float t) {
            e[0] /= t;
            e[1] /= t;
            e[2] /= t;
            return *this;
        }

        //Calculate 2-norm length
        float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }

        void make_unit_vector() {
            float len = length();
            e[0] /= len;
            e[1] /= len;
            e[2] /= len;
        };

};

//Only implemented the nessessary operators
vector operator+(const vector &v1, const vector &v2) {
    return vector(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

vector operator-(const vector &v1, const vector &v2) {
    return vector(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

vector operator-(const vector &v1) {
    return vector(-v1.e[0], -v1.e[1], -v1.e[2]);
}

vector operator*(const vector &v1, const vector &v2) {
    return vector(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

vector operator*(float t, const vector &v) {
    return vector(t*v.e[0], t*v.e[1], t*v.e[2]);
}

vector operator*(const vector &v, float t) {
    return vector(t*v.e[0], t*v.e[1], t*v.e[2]);
}

vector operator/(const vector v, float t) {
    return vector(v.e[0]/t, v.e[1]/t, v.e[2]/t);
}

float dot(const vector &v1, const vector &v2) {
    return v1.e[0] * v2.e[0]
         + v1.e[1] * v2.e[1]
         + v1.e[2] * v2.e[2];
}

vector cross(const vector &v1, const vector &v2){
    return vector(
        v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
        v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
        v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]
    );
}