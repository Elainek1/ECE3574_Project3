#include "geometry.hpp"

#include <math.h>

color::color()
{
}

color::~color()
{
}

color::color(int rIn, int gIn, int bIn)
{
	r = rIn;
	g = gIn;
	b = bIn;
}

point::point(double xIn, double yIn, double zIn) {
	x = xIn;
	y = yIn;
	z = zIn;
}
point::point()
{

}

point::~point()
{

}

double dot(point a, point b)
{
	double value = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return value;
}
point normal(point a, point b)
{
	return point(b.x - a.x, b.y - a.y, b.z - a.z);
}
double mag(point a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}
camera::camera()
{
}

camera::~camera()
{
}
