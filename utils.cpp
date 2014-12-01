#include <cmath>
#include <algorithm>
#include <math.h>
#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

#include "utils.h"

inline float sqr(float x) { return x*x; }

//****************************************************
// vector class for basic arithmetic and operations
//****************************************************
Vect::Vect(float a, float b, float c) {
	x = a;
	y = b;
	z = c;
}

bool Vect::operator==(Vect v1) {
	return (x == v1.x && y == v1.y && z == v1.z);
}

Vect Vect::operator+(Vect v1) {
	return Vect(x+v1.x, y+v1.y, z+v1.z);
}

Vect Vect::operator+(float s) {
	return Vect(x+s, y+s, z+s);
}

Vect Vect::operator-(Vect v1) {
	return Vect(x-v1.x, y-v1.y, z-v1.z);
}

Vect Vect::operator-(float s) {
	return Vect(x-s, y-s, z-s);
}

Vect Vect::operator*(float s) {
	return Vect(x*s, y*s, z*s);
}

Vect Vect::operator/(float s) {
	return Vect(x/s, y/s, z/s);
}

float Vect::length() {
	return sqrt(sqr(x) + sqr(y) + sqr(z));
}

Vect Vect::normalize() {
	float mag = this->length();
	return Vect(x/mag, y/mag, z/mag);
}

//****************************************************
// point class for basic arithmetic and operations
//****************************************************

Point::Point(float a, float b, float c) {
	x = a;
	y = b;
	z = c;
}

bool Point::operator==(Point p1) {
	return (x == p1.x && y == p1.y && z == p1.z);
}

Point Point::operator+(Vect v1) {
	return Point(x + v1.x, y + v1.y, z + v1.z);
}

Vect Point::operator-(Point p1) {
	return Vect(x - p1.x, y - p1.y, z - p1.z);
}

Point Point::operator-(Vect v1) {
	return Point(x - v1.x, y - v1.y, z - v1.z);
}

Point Point::operator*(float s) {
	return Point(x*s, y*s, z*s);
}

//****************************************************
// static methods involving vectors
//****************************************************
Vect cross(Vect v1, Vect v2) {
  return Vect(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

float dot(Vect v1, Vect v2) {
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

//****************************************************
// point and normal for pixel in scene
//****************************************************

LocalGeo::LocalGeo(Vect p, Vect n) {
	pos = p;
	norm = n;
}