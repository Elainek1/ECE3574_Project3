// implement vtray main here
#include <vector>
#include <string>
#include <iostream>
#include <qstring.h>
#include <qimage.h>

#include "geometry.hpp"
#include "json.hpp"
#include "png.hpp"

int main(int argc, char *argv[]){
	std::string jsonFile, pngFile;
	int threadNum = 1;
	std::vector<std::string>  arguments;
	for (int i = 0; i < argc; ++i) {
		arguments.push_back(argv[i]);
	}
	if (argc == 3) { //if only provide the .json and .png then assume thread of 1
		jsonFile = arguments[1];
		pngFile = arguments[2];
		file json;
		json.setPngFilename(pngFile);
		json.setThreadNum(1);
		bool ok = json.readJson(QString::fromStdString(jsonFile));
		if (!ok){
			std::cout << "Error: Unable to parse json";
			return EXIT_FAILURE;
		}
		json.renderImage();
	}
	//if -t is given then get the thread count
	else if (argc == 5) {
		if (arguments[1] == "-t") {
			try {
				threadNum = std::stoi(arguments[2]);
				jsonFile = arguments[1];
				pngFile = arguments[2];
				file json;
				json.setPngFilename(pngFile);
				json.setThreadNum(1);
				bool ok = json.readJson(QString::fromStdString(jsonFile));
				if (!ok) {
					std::cout << "Error: Unable to parse json";
					return EXIT_FAILURE;
				}
				json.renderImage();
			}
			catch(const std::invalid_argument&){
				std::cout << "Error: incorrect value for thread number\n";
				return EXIT_FAILURE;
			}
		}
	}
	else {
		std::cout << "Error: Invalid number of arguments";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}