#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

// IMPORTANT NOTE:
// These are just a few examples from my solution and **should be removed**.
// Depending on your code design your class and functions names would differ

#include "geometry.hpp"
#include "json.hpp"

#include <fstream>

TEST_CASE("Test Basic Geometry: point ", "[geometry]") {

	point a = point(2, 0, 0);
	point b = point(0, 4, 0);

	REQUIRE(a.x == Approx(2));
	REQUIRE(a.y == Approx(0));
	REQUIRE(a.z == Approx(0));

	REQUIRE(b.x == Approx(0));
	REQUIRE(b.y == Approx(4));
	REQUIRE(b.z == Approx(0));

	double adotb = dot(a, b);
	REQUIRE(adotb == Approx(0));

	point anorm = normal(a, b);
	REQUIRE(anorm.x == Approx(-2));
	REQUIRE(anorm.y == Approx(4));
	REQUIRE(anorm.z == Approx(0));

	double magA = mag(a);
	REQUIRE(magA == Approx(2));	
}

TEST_CASE("Test read json ", "[json]") {

	file json;
	json.setPngFilename("testMe.png");
	std::ofstream myfile("testMe.json");
	if (myfile.is_open())
	{
		myfile << "{\"camera\": {\"center\": {\"x\": 0,\"y\": 0,\"z\": 0},\"focus\": 10,\"normal\": {\"x\": 0,\"y\": 0,\"z\": 1},\"resolution\": [0.01,0.01],\"size\": [1024,1024]},\"lights\": [{\"intensity\": 0.6,\"location\": {\"x\": 0,\"y\": 0,\"z\": -10}},"
			<< "{\"intensity\": 0.5,\"location\": {\"x\": 100,\"y\": 0,\"z\": -100}},{\"intensity\": 0.5,\"location\": {\"x\": 99.50041652780257,\"y\": 9.983341664682815,\"z\": -100}}],"
			<< "\"objects\": [{\"center\": {\"x\": 2,\"y\": 0,\"z\": 5},\"color\": {\"b\": 100,\"g\": 100,\"r\": 200},\"lambert\": 1,\"radius\": 2,\"type\": \"sphere\"},"
			<< "{\"center\": {\"x\": -0.9999999999999996,\"y\": 1.7320508075688776,\"z\": 5},\"color\": {\"b\": 100,\"g\": 200,\"r\": 100},\"lambert\": 1,\"radius\": 2,\"type\": \"sphere\"},"
			<< "{\"center\": {\"x\": 0,\"y\": 0,\"z\": 40},\"color\": {\"b\": 100,\"g\": 100,\"r\": 100},\"lambert\": 1,\"normal\": {\"x\": 0,\"y\": 0,\"z\": -1},\"type\": \"plane\"},"
			<< "{\"center\": {\"x\": 0,\"y\": -10,\"z\": 0},\"color\": {\"b\": 200,\"g\": 0,\"r\": 0},\"lambert\": 1,\"normal\": {\"x\": 0,\"y\": 1,\"z\": 0},\"type\": \"plane\"}]}";
		myfile.close();
	}
	bool ok = json.readJson(QString::fromStdString("./testMe.json"));
	REQUIRE(ok);
	json.renderImage();
	std::ifstream inputPNG("testMe.png");
	REQUIRE(!inputPNG.fail());
}
/*
TEST_CASE( "Test Basic Geometry: Vec3d ", "[geometry]" ) {

  Vec3d a(2,0,0), b(0,4,0);
  
  REQUIRE(a.x == Approx(2));
  REQUIRE(a.y == Approx(0));
  REQUIRE(a.z == Approx(0));

  REQUIRE(b.x == Approx(0));
  REQUIRE(b.y == Approx(4));
  REQUIRE(b.z == Approx(0));

  double adotb = dot(a,b);
  REQUIRE(adotb == Approx(0));

  Vec3d anorm = norm(a);
  REQUIRE(anorm.x == Approx(1));
  REQUIRE(anorm.y == Approx(0));
  REQUIRE(anorm.z == Approx(0));

  Vec3d bnorm = norm(b);
  REQUIRE(bnorm.x == Approx(0));
  REQUIRE(bnorm.y == Approx(1));
  REQUIRE(bnorm.z == Approx(0));
}
*/