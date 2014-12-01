#include <stdlib.h>

using namespace std;

//****************************************************
// vector class for basic arithmetic and operations
//****************************************************
class Vect {
	public:
		Vect()
			: x(0.0)
			, y(0.0)
			, z(0.0)
			{ }

    Vect(float, float, float);

    bool operator==(Vect v1);
    Vect operator+(Vect v1);
    Vect operator+(float s);
    Vect operator-(Vect v1);
    Vect operator-(float s);
	Vect operator*(float s);
	Vect operator/(float s);

    float length();
    Vect normalize();

    float x, y, z;
};

//****************************************************
// point class for basic arithmetic and operations
//****************************************************
class Point {
    public:
        Point()
            : x(0.0)
            , y(0.0)
            , z(0.0)
            { }

    Point(float, float, float);

    bool operator==(Point p1);
    Point operator+(Vect v1);
    Vect operator-(Point p1);
    Point operator-(Vect v1);
    Point operator*(float s);

    float x, y, z;
};

//****************************************************
// static methods involving vectors
//****************************************************
Vect cross(Vect v1, Vect v2);

float dot(Vect v1, Vect v2);

//****************************************************
// point and normal for pixel in scene
//****************************************************

class LocalGeo {
	public:
		LocalGeo()
			: pos(Vect())
			, norm(Vect())
			{ }

    LocalGeo(Vect, Vect);

    Vect pos;
    Vect norm;
};