#include "header/rules.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

rules::rules() {
	std::cout<<"initializing rules ..."<<std::endl;
}

rules::~rules() {
	// TODO Auto-generated destructor stub
}

void rules::readFile(char* rulesFile) {
	std::cout<<"rules file: "<<rulesFile<<std::endl;
	std::ifstream file(rulesFile);
	std::cout<<file.is_open()<<std::endl;
	std::string str;
	while (std::getline(file, str)) {
		std::cout << str << "\n";
	}
}
