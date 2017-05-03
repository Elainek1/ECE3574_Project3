// implement vtray main here
#include <vector>
#include <string>
#include <iostream>
#include <qstring.h>
#include <qimage.h>
//#include <algorithm>
//#include <pthread.h>
#include <thread>

#include "geometry.hpp"
#include "json.hpp"

void render(file * json, int thread);
//void do_join(std::thread& t);
//void do_leave(std::thread& t);

void render(file * json, int thread) {
	json->renderImage(thread);
}

//void do_join(std::thread& t) {
//	t.join();
//}

//void do_leave(std::thread& t) {
//	t.detach();
//}

int main(int argc, char *argv[]){
	std::string jsonFile, pngFile;
	unsigned int threadNum = 1;
	std::vector<std::string>  arguments;
	for (int i = 0; i < argc; ++i) {
		arguments.push_back(argv[i]);
	}
	if (argc == 3) { //if only provide the .json and .png then assume thread of 1
		jsonFile = arguments[1];
		pngFile = arguments[2];
		file json;
		json.setPngFilename(pngFile);
		json.setTotalThreadNum(1);
		bool ok = json.readJson(QString::fromStdString(jsonFile));
		if (!ok){
			std::cout << "Error: Unable to parse json";
			return EXIT_FAILURE;
		}
		json.renderImage(0);
	}
	else if (argc == 5) { //if -t is given then get the thread count
		if (arguments[1] == "-t") {
			try {
				threadNum = std::stoi(arguments[2]);
				jsonFile = arguments[3];
				pngFile = arguments[4];
				file * json = new file();
				json->setPngFilename(pngFile);
				json->setTotalThreadNum(threadNum);
				std::vector<std::thread> threads(threadNum);
				bool ok = json->readJson(QString::fromStdString(jsonFile));
				if (!ok) {
					std::cout << "Error: Unable to parse json";
					return EXIT_FAILURE;
				}
				for (int i = 0; i < threadNum; i++) {
					threads[i] = std::thread(&render, json, i);
				}
				for (int i = 0; i < threadNum; i++) {    //std::for_each(threads.begin(), threads.end(), this->std::thread::join);
					while (!threads[i].joinable()) {};
					threads[i].std::thread::join();  //do_join(threads[i]);
				}
				
				bool save = json->savePic();
				delete json;
				if (!save) {
					std::cout << "Error: Unable to save picture";
					return EXIT_FAILURE;
				}   //std::for_each(threads.begin(), threads.end(), do_leave);
			}
			catch(const std::invalid_argument&){
				std::cout << "Error: incorrect value for thread number\n";
				return EXIT_FAILURE;
			}
		}
	} else {
		std::cout << "Error: Invalid number of arguments";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
