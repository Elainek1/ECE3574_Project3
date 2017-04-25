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
#include <QImage>

#include "geometry.hpp"

bool file::readJson(QString filename)
{
	bool ok;
	QString val;
	QFile file;
	file.setFileName(filename);
	file.open(QIODevice::ReadOnly | QIODevice::Text);
	val = file.readAll();
	file.close();
	
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

bool file::parseObjects(QJsonArray objectsInput) {
	for (int i = 0; i < objectsInput.size(); i++)
	{
		shape shapeIn;
		QJsonObject shapeVal = objectsInput[i].toObject();
		QJsonObject center = shapeVal.value(QString("center")).toObject();
		shapeIn.center = point(center.value(QString("x")).toDouble(), center.value(QString("y")).toDouble(), center.value(QString("z")).toDouble());
		
		QJsonObject colorVal = shapeVal.value(QString("color")).toObject();
		shapeIn.color = colorClass(colorVal.value(QString("r")).toInt(), colorVal.value(QString("g")).toInt(), colorVal.value(QString("b")).toInt());
		
		QJsonObject normalVal = shapeVal.value(QString("normal")).toObject();
		shapeIn.normal = point(normalVal.value(QString("x")).toDouble(), normalVal.value(QString("y")).toDouble(), normalVal.value(QString("z")).toDouble());

		shapeIn.lambert = shapeVal.value(QString("lambert")).toDouble();
		shapeIn.radius = shapeVal.value(QString("radius")).toDouble();
		shapeIn.type = shapeVal.value(QString("type")).toString().toStdString();

		objectsArray.push_back(shapeIn);
	}
	return true;
}

bool file::parseLights(QJsonArray lightsInput){
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

bool file::parseCamera(QJsonObject cameraInput)
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

int file::intersects(point fromPt, point toPt)
{
	point closestPt;
	int closestNum = -1;
	unsigned int rgbVal = 0;
	for (int i = 0; i < objectsArray.size(); i++){
		if (objectsArray[i].type == "plane") {
			point rayDirection = point(toPt.x - fromPt.x, toPt.y - fromPt.y, toPt.z - fromPt.z);
			double magRay = mag(rayDirection);
			point rayUnit = point(rayDirection.x / magRay, rayDirection.y / magRay, rayDirection.z / magRay);
			double magNorm = mag(objectsArray[i].normal);
			point normUnit = point(objectsArray[i].normal.x / magNorm, objectsArray[i].normal.y / magNorm, objectsArray[i].normal.z / magNorm);
			double t = planeIntersect(fromPt, rayUnit, objectsArray[i].center, normUnit);
			if (t >= 0) {
				if (closestNum == -1){
					closestNum = i;
					closestPt = point (t*rayUnit.x,t*rayUnit.y, t*rayUnit.z);
				}
				else{
					point newPt = point(t*rayUnit.x, t*rayUnit.y, t*rayUnit.z);
					if (mag(closestPt) > mag(newPt)){
						closestNum = i;
						closestPt = newPt;
					}
				}
			}
		}
		else if (objectsArray[i].type == "sphere"){
			double dx = toPt.x - fromPt.x;
			double	dy = toPt.y - fromPt.y;
			double dz = toPt.z - fromPt.z;
			double a = dx*dx + dy*dy + dz*dz;
			double b = 2 * dx*(fromPt.x - objectsArray[i].center.x) + 2 * dy*(fromPt.y - objectsArray[i].center.y) + 2 * dz*(fromPt.z - objectsArray[i].center.z);
			double c = objectsArray[i].center.x*objectsArray[i].center.x + objectsArray[i].center.y*objectsArray[i].center.y + objectsArray[i].center.z*objectsArray[i].center.z + fromPt.x*fromPt.x + fromPt.y*fromPt.y + fromPt.z*fromPt.z + (-2 * (objectsArray[i].center.x*fromPt.x + objectsArray[i].center.y*fromPt.y + objectsArray[i].center.z*fromPt.z)) - objectsArray[i].radius*objectsArray[i].radius;
			double discrim = b*b - 4 * a*c;
			if (discrim >= 0){
				double t = (-1 * b - sqrt(discrim)) / (2 * a);
				if (closestNum == -1){
					closestNum = i;
					closestPt = point(t*dx, t*dy, t*dz);
				}
				else {
					point newPt = point(t*dx, t*dy, t*dz);
					if (mag(closestPt) > mag(newPt)) {
						closestNum = i;
						closestPt = newPt;
					}
				}
			}
		}
	}

	if (closestNum >= 0){
		rgbVal = (objectsArray[closestNum].color.r << 16) | (objectsArray[closestNum].color.g << 8) | (objectsArray[closestNum].color.b);
	}
	return (0xFF000000) | rgbVal;
}

void file::renderImage()
{
	image = new QImage(cameraVal.sizeX, cameraVal.sizeY, QImage::Format_RGB32);
	point focalPt = point(cameraVal.center.x - cameraVal.normal.x*cameraVal.focus, cameraVal.center.y - cameraVal.normal.y*cameraVal.focus, cameraVal.center.z - cameraVal.normal.z*cameraVal.focus);
	for (int i = 0; i < cameraVal.sizeX; i++){
		double xVal = cameraVal.resolutionX * (i - cameraVal.sizeX / 2) - cameraVal.center.x;
		for (int j = 0; j < cameraVal.sizeY; j++){
			if ((i == 511) && (j == 511))
			{
				std::cout << "Hi";
			}
			double yVal = cameraVal.resolutionY * (j - cameraVal.sizeY / 2) - cameraVal.center.y;
			int pixColor = intersects(focalPt, point(xVal, yVal, cameraVal.center.z));
			image->setPixel(i, j, pixColor);
		}
	}
	bool ok = image->save(QString::fromStdString(pngFilename), "png");
}

void file::setPngFilename(std::string file){
	pngFilename = "./" + file;
}

void file::setThreadNum(int num) {
	threadNum = num;
}

double file::planeIntersect(point l0, point l, point p0, point n) {
	double denom = dot(n, l);
	if ((denom > 1e-6 )||(denom < -1e-6)){
		double t = dot(point(p0.x - l0.x, p0.y - l0.y, p0.z - l0.z), n) / denom;
		return t;
	}
	return -1;
}