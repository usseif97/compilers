#include "header/lexicalGenerator.h"

#include <iostream>

#include "header/rules.h"

lexicalGenerator::lexicalGenerator() {
	std::cout<<"initializing lexical generator ..."<<std::endl;
	char rulesFile[] = "rulesSet.txt";

	rules rule;
	rule.readFile(rulesFile);
}

lexicalGenerator::~lexicalGenerator() {
}
