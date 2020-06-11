/*
 * grammer.h
 *
 *  Created on: Apr 16, 2020
 *      Author: Usseif
 */

#ifndef PARSER_GRAMMER_H_
#define PARSER_GRAMMER_H_
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;



class grammar {
public:
	grammar();
	virtual ~grammar();
	multimap<std::string, std::string> getMultiMap();
	vector<std::string> getNonTerminal();


private:
	void readGrammar(char*);
	void parse(std::string);
	void parseProductions(std::string );
	vector<std::string> split(std::string , char );
	void displayMap();
    std::string currentNonTerminal;
    multimap<std::string, std::string> map;
    vector<std::string> nonTerminals;
};

#endif /* PARSER_GRAMMER_H_ */
