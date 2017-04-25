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
	bool readJson(QString filename);
	int intersects(point fromPt, point toPt);
	void renderImage();
	void setPngFilename(std::string file);
	void setThreadNum(int num);
	file();
	~file();
private:
	std::string pngFilename;
	camera cameraVal;
	std::vector<light> lightsArray;
	std::vector<shape> objectsArray;
	bool parseCamera(QJsonObject cameraInput);
	bool parseLights(QJsonArray lightsInput);
	bool parseObjects(QJsonArray objectsInput);
	QImage * image;
	double planeIntersect(point l0, point l, point p0, point n);
	int threadNum;

};

#endif