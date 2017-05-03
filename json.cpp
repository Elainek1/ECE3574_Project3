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
#include <tuple>
#include <math.h>
#include <cmath>
#include <algorithm>

#include "geometry.hpp"

file::file() {
	image = nullptr;
	rgbMax = 0;
}

file::~file() {
	if (image != nullptr) {
		delete image;
		image = nullptr;
	}
}

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
	for (unsigned int i = 0; i < objectsInput.size(); i++)
	{
		shape shapeIn;
		QJsonObject shapeVal = objectsInput[i].toObject();
		QJsonObject center = shapeVal.value(QString("center")).toObject();
		shapeIn.center = point(center.value(QString("x")).toDouble(), center.value(QString("y")).toDouble(), center.value(QString("z")).toDouble());
		
		QJsonObject colorVal = shapeVal.value(QString("color")).toObject();
		shapeIn.color = colorClass(colorVal.value(QString("r")).toInt(), colorVal.value(QString("g")).toInt(), colorVal.value(QString("b")).toInt());
		if ((shapeIn.color.r > 255) || (shapeIn.color.g > 255) || (shapeIn.color.b > 255)){
			return false;
		}
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
	for (unsigned int i = 0; i < lightsInput.size(); i++)
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

colorClass file::findPixelVal(const point & fromPt, const point & toPt) {
	//unsigned int rgbVal = 0;
	unsigned int R = 0;
	unsigned int G = 0;
	unsigned int B = 0;
	double scale = 0;
	point closestPt = intersects(fromPt, toPt);
	point lightHits, shadowRayUnit;
	if (closestPt.objectIt >= 0) {
		closestPt.x += fromPt.x;
		closestPt.y += fromPt.y;
		closestPt.z += fromPt.z;
		for (unsigned int i = 0; i < lightsArray.size(); i++) {
			lightHits = intersects(closestPt, lightsArray[i].location);
			lightHits.x += closestPt.x;
			lightHits.y += closestPt.y;
			lightHits.z += closestPt.z;
			if((lightHits.z < cameraVal.center.z) || (lightHits.objectIt == -1)){
			/*point lightHits = intersects(lightsArray[i].location, closestPt);
			lightHits.x += lightsArray[i].location.x;
			lightHits.y += lightsArray[i].location.y;
			lightHits.z += lightsArray[i].location.z;
			if (lightHits.z < cameraVal.center.z) {
				point lightHits2 = intersects(lightHits, closestPt);
				lightHits.x += lightHits2.x;
				lightHits.y += lightHits2.y;
				lightHits.z += lightHits2.z;
			}
			if (((std::fabs(lightHits.x - closestPt.x) + std::fabs(lightHits.y - closestPt.y) + std::fabs(lightHits.z - closestPt.z) + std::fabs(lightHits.objectIt - closestPt.objectIt)) < 1e-6)) {
			*/
				//double scale = 0;
				if (objectsArray[closestPt.objectIt].type == "sphere"){
					shadowRayUnit = normal(closestPt, lightsArray[i].location);
					scale = dot(normal(objectsArray[closestPt.objectIt].center, closestPt), shadowRayUnit)*objectsArray[closestPt.objectIt].lambert;
				}
				else if (objectsArray[closestPt.objectIt].type == "plane") {
					shadowRayUnit = normal(closestPt, lightsArray[i].location);
					scale = dot(normalize(objectsArray[closestPt.objectIt].normal), shadowRayUnit)*objectsArray[closestPt.objectIt].lambert;
				}
				if (scale >= 0) {
					R += scale*lightsArray[i].intensity*objectsArray[closestPt.objectIt].color.r;
					G += scale*lightsArray[i].intensity*objectsArray[closestPt.objectIt].color.g;
					B += scale*lightsArray[i].intensity*objectsArray[closestPt.objectIt].color.b;
				}
			}
		}
	}
	return colorClass((int) R, (int) G, (int) B);
	//rgbVal = (R << 16) | (G << 8) | (B);
	// return (0xFF000000) | rgbVal;
}

point file::intersects(const point & fromPt, const point & toPt){
	point closestPt = point(0,0,0);
	//int closestNum = -1;
	unsigned int rgbVal = 0;
	for (unsigned int i = 0; i < objectsArray.size(); i++){
		if (objectsArray[i].type == "plane") {
			point rayDirection = point(toPt.x - fromPt.x, toPt.y - fromPt.y, toPt.z - fromPt.z);
			double magRay = mag(rayDirection);
			point rayUnit = point(rayDirection.x / magRay, rayDirection.y / magRay, rayDirection.z / magRay);
			double magNorm = mag(objectsArray[i].normal);
			point normUnit = point(objectsArray[i].normal.x / magNorm, objectsArray[i].normal.y / magNorm, objectsArray[i].normal.z / magNorm);
			double t = planeIntersect(fromPt, rayUnit, objectsArray[i].center, normUnit);
			if (t > 0) {
				if (closestPt.objectIt == -1){
					closestPt = point (t*rayUnit.x,t*rayUnit.y, t*rayUnit.z);
					closestPt.objectIt = i;
				}
				else{
					point newPt = point(t*rayUnit.x, t*rayUnit.y, t*rayUnit.z);
					if (mag(closestPt) > mag(newPt)){
						closestPt = newPt;
						closestPt.objectIt = i;
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
				if (t > 0) {
					if (closestPt.objectIt == -1) {
						closestPt = point(t*dx, t*dy, t*dz);
						closestPt.objectIt = i;
					}
					else {
						point newPt = point(t*dx, t*dy, t*dz);
						if (mag(closestPt) > mag(newPt)) {
							closestPt = newPt;
							closestPt.objectIt = i;
						}
					}
				}
				
			}
		}
	}

	/*if (closestPt.objectIt >= 0){
		rgbVal = (objectsArray[closestPt.objectIt].color.r << 16) | (objectsArray[closestPt.objectIt].color.g << 8) | (objectsArray[closestPt.objectIt].color.b);
	}*/
	return closestPt;
	//return (0xFF000000) | rgbVal;
}

void file::renderImage()
{
	image = new QImage(cameraVal.sizeX, cameraVal.sizeY, QImage::Format_RGB32);
	point focalPt = point(cameraVal.center.x - cameraVal.normal.x*cameraVal.focus, cameraVal.center.y - cameraVal.normal.y*cameraVal.focus, cameraVal.center.z - cameraVal.normal.z*cameraVal.focus);
	double yVal;
	int pixIt, pixVal;
	point pixPlace;
	for (int i = 0; i < cameraVal.sizeX; i++){
		double xVal = cameraVal.resolutionX * (i - cameraVal.sizeX / 2) - cameraVal.center.x;
		for (int j = 0; j < cameraVal.sizeY; j++){
			//image.fill(Qt::GlobalColor::black);
			yVal = cameraVal.resolutionY * (j - cameraVal.sizeY / 2) - cameraVal.center.y;
			//int pixColor = intersects(focalPt, point(xVal, yVal, cameraVal.center.z));
			pixIt = (i*cameraVal.sizeY) + (j);
			pixPlace = point(xVal, yVal, cameraVal.center.z);
			pixColorArray.push_back(findPixelVal(focalPt, pixPlace));
			if (rgbMax < pixColorArray[pixIt].r) {
				rgbMax = pixColorArray[pixIt].r;
			}
			if (rgbMax < pixColorArray[pixIt].b) {
				rgbMax = pixColorArray[pixIt].b;
			}
			if (rgbMax < pixColorArray[pixIt].g) {
				rgbMax = pixColorArray[pixIt].g;
			}
			//image->setPixel(i, j, pixColorArray[pixIt].r);
		}
	}

	if (rgbMax > 255)
	{
		for (unsigned int i = 0; i < pixColorArray.size(); i++) {
			pixColorArray[i].scaleBy((255.0) / ((double)rgbMax));
		}
	}
	for (int i = 0; i < cameraVal.sizeX; i++) {
		for (int j = 0; j < cameraVal.sizeY; j++) {
			pixVal = (0xFF000000) | (pixColorArray[(i*cameraVal.sizeY) + (j)].r << 16) | (pixColorArray[(i*cameraVal.sizeY) + (j)].g << 8) | (pixColorArray[(i*cameraVal.sizeY) + (j)].b);
			//QRgb pixVal = qRgb(pixColorArray[(i*cameraVal.sizeY) + (j)].r, pixColorArray[(i*cameraVal.sizeY) + (j)].g, pixColorArray[(i*cameraVal.sizeY) + (j)].b);
			image->setPixel(i, j, pixVal);
		}
	}
	
	bool ok = image->save(QString::fromStdString(pngFilename), "png");
	delete image;
	image = nullptr;
}

void file::setPngFilename(std::string file){
	pngFilename = "./" + file;
}

void file::setTotalThreadNum(int num) {
	threadNum = num;
}

void file::setCurThreadNum(int num) {
	curThreadNum = num;
}

double file::planeIntersect(const point & l0, const point & l, const point & p0, const point & n) {
	double denom = dot(n, l);
	if ((denom > 1e-6 )||(denom < -1e-6)){
		double t = dot(point(p0.x - l0.x, p0.y - l0.y, p0.z - l0.z), n) / denom;
		return t;
	}
	return -1;
}