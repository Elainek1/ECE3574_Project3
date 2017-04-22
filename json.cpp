#include "json.hpp"

#include <qstring.h>
#include <qfile.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include <qdebug.h>
#include <QTranslator>
#include <QObject>
#include <vector>

#include "geometry.hpp"

bool json::readJson(QString filename)
{
	bool ok;
	QString val;
	QFile file;
	file.setFileName(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	val = file.readAll();
	file.close();
	//qWarning() << val;

	QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
	if (d.isNull())
	{
		return false;
	}
	QJsonObject sett2 = d.object();
	QJsonObject camera = sett2.value(QString("camera")).toObject();
	if (!camera.isEmpty()){
		ok = parseCamera(camera);
	}
	else {
		return false;
	}
	
	QJsonValue value2 = sett2.value(QString("lights"));
	QJsonArray lights = value2.toArray();
	if (!lights.isEmpty()){
		ok = ok && parseLights(lights);
	}
	else {
		return false;
	}
	
	QJsonValue value3 = sett2.value(QString("objects"));
	QJsonArray objects = value3.toArray();
	if (!objects.isEmpty()) {
		ok = ok && parseObjects(objects);
	}
	else {
		return false;
	}
	
	return ok;
}

bool json::parseObjects(QJsonArray objectsInput) {
	for (int i = 0; i < objectsInput.size(); i++)
	{
		shape shapeIn;
		QJsonObject shapeVal = objectsInput[i].toObject();
		QJsonObject center = shapeVal.value(QString("center")).toObject();
		shapeIn.center = point(center.value(QString("x")).toDouble(), center.value(QString("y")).toDouble(), center.value(QString("z")).toDouble());
		
		QJsonObject colorVal = shapeVal.value(QString("color")).toObject();
		shapeIn.color = color(colorVal.value(QString("r")).toInt(), colorVal.value(QString("g")).toInt(), colorVal.value(QString("b")).toInt());
		
		QJsonObject normalVal = shapeVal.value(QString("normal")).toObject();
		shapeIn.normal = point(normalVal.value(QString("x")).toDouble(), normalVal.value(QString("y")).toDouble(), normalVal.value(QString("z")).toDouble());

		shapeIn.lambert = shapeVal.value(QString("lambert")).toDouble();
		shapeIn.radius = shapeVal.value(QString("radius")).toDouble();
		shapeIn.type = shapeVal.value(QString("type")).toString().toStdString();

		objectsArray.push_back(shapeIn);
	}
	return true;
}

bool json::parseLights(QJsonArray lightsInput){
	for (int i = 0; i < lightsInput.size(); i++)
	{
		light lightIn;
		QJsonObject lightVal = lightsInput[i].toObject();
		lightIn.intensity = lightVal.value(QString("intensity")).toDouble();
		lightVal = lightVal.value(QString("location")).toObject();
		lightIn.location = point(lightVal.value(QString("x")).toDouble(), lightVal.value(QString("y")).toDouble(), lightVal.value(QString("z")).toDouble());
		lightsArray.push_back(lightIn);
	}
	return true;
}

bool json::parseCamera(QJsonObject cameraInput)
{
	
	QJsonObject center = cameraInput.value(QString("center")).toObject();
	point centerIn = point(center.value(QString("x")).toDouble(), center.value(QString("y")).toDouble(), center.value(QString("z")).toDouble());
	cameraVal.center = centerIn;
	QJsonObject normal = cameraInput.value(QString("normal")).toObject();
	point normalIn = point(normal.value(QString("x")).toDouble(), normal.value(QString("y")).toDouble(), normal.value(QString("z")).toDouble());
	cameraVal.normal = normalIn;
	cameraVal.focus = cameraInput.value(QString("focus")).toDouble();
	QJsonArray resolution = cameraInput.value(QString("resolution")).toArray();
	if (resolution.size() == 2){
		cameraVal.resolutionX = resolution[0].toDouble();
		cameraVal.resolutionY = resolution[1].toDouble();
	}
	else{
		return false;
	}
	QJsonArray size = cameraInput.value(QString("size")).toArray();
	if (size.size() == 2){
		cameraVal.sizeX = size[0].toDouble();
		cameraVal.sizeY = size[1].toDouble();
	}
	else {
		return false;
	}
	return true;
}

bool json::intersects(point fromPt, point toPt)
{
	return true;
}