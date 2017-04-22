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

#include "geometry.hpp"

class json
{
	//Q_OBJECT
public:
	bool readJson(QString filename);
	bool intersects(point fromPt, point toPt);
private:
	camera cameraVal;
	std::vector<light> lightsArray;
	std::vector<shape> objectsArray;
	bool parseCamera(QJsonObject cameraInput);
	bool parseLights(QJsonArray lightsInput);
	bool parseObjects(QJsonArray objectsInput);


};

#endif