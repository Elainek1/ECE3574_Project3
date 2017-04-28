#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <fstream>
#include <iostream>



class colorClass
{
public:
	int b;
	int g;
	int r;
	colorClass();
	~colorClass();
	colorClass(int rIn, int gIn, int bIn);
	void scaleBy(double scale);
private:
};
//
//class Vec3d
//{
//public:
//
//	Vec3d norm();
//	Vec3d dot();
//private:
//
//
//};

class point
{
public:
	double x;
	double y;
	double z;
	int objectIt;
	point();
	~point();
	point(double xIn, double yIn, double zIn);
private:
};

class shape
{
public:
	std::string type;
	point center;
	colorClass color;
	float lambert;
	float radius;
	point normal;

private:


};

class camera
{
public:
	point center;
	double focus;
	point normal;
	double resolutionX;
	double resolutionY;
	int sizeX;
	int sizeY;
	camera();
	~camera();
private:
};

class light
{
public:
	point location;
	double intensity;

private:
};


double dot(point a, point b);

point normal(point center, point edge);
double mag(point a);
point diff(point start, point end);
point normalize(point a);


#endif

