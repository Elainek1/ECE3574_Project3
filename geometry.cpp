#include "geometry.hpp"

#include <math.h>

colorClass::colorClass()
{
}

colorClass::~colorClass()
{
}

colorClass::colorClass(int rIn, int gIn, int bIn)
{
	r = rIn;
	g = gIn;
	b = bIn;
}

void colorClass::scaleBy(double scale) {
	r = r*scale;
	g = g*scale;
	b = b*scale;
}

point::point(double xIn, double yIn, double zIn) {
	x = xIn;
	y = yIn;
	z = zIn;
	objectIt = -1;
}
point::point()
{
	objectIt = -1;
}

point::~point()
{

}

double dot(point a, point b)
{
	double value = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
	return value;
}
point normal(point center, point edge)
{
	//point norm = point(b.x - a.x, b.y - a.y, b.z - a.z);
	//return point(norm.x / mag(norm), norm.y / mag(norm), norm.z / mag(norm));
	return normalize(diff(center,edge));
}
double mag(point a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}

point diff(point start, point end)
{
	return point(end.x - start.x, end.y - start.y, end.z - start.z);
}
point normalize(point a)
{
	return point(a.x/mag(a), a.y/mag(a), a.z/mag(a));
}
camera::camera()
{
}

camera::~camera()
{
}
