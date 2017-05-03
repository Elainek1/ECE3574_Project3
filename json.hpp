#ifndef JSON_H
#define JSON_H

#include <string>
#include <fstream>
#include <iostream>
#include <qstring.h>
#include <qobject.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <vector>
#include <QImage>

#include "geometry.hpp"

class file
{
	//Q_OBJECT
public:
	file();
	~file();
	bool readJson(QString filename);
	colorClass findPixelVal(const point & fromPt, const point & toPt);
	point intersects(const point & fromPt, const point & toPt);
	void renderImage(int threadNum);
	bool savePic();
	void setPngFilename(std::string file);
	void setTotalThreadNum(int num);
	void setCurThreadNum(int num);
	
private:
	int rgbMax;
	std::string pngFilename;
	camera cameraVal;
	std::vector<light> lightsArray;
	std::vector<shape> objectsArray;
	std::vector<colorClass> pixColorArray;
	bool parseCamera(QJsonObject cameraInput);
	bool parseLights(QJsonArray lightsInput);
	bool parseObjects(QJsonArray objectsInput);
	QImage * image;
	double planeIntersect(const point & l0, const point & l, const point & p0, const point & n);
	int totThreadNum;
	int curThreadNum;

};

#endif