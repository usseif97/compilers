#include "header/grammar.h"
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>

/**
 * Trimming functions
 */
std::string& lefttrim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& righttrim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& triim(std::string& str, const std::string& chars = "\t\n\v\f\r "){
    return lefttrim(righttrim(str, chars), chars);
}
grammar::grammar() {
	// TODO Auto-generated constructor stub
	std::cout<<"Reading Grammar ..."<<std::endl;
	char grammerFile[] = "grammarProductions.txt";
	readGrammar(grammerFile);
}

grammar::~grammar() {

}

void grammar::readGrammar(char* gfile){
	std::cout<<"Grammar file: "<<gfile<<std::endl;
	std::ifstream file(gfile);
	if (file.is_open()) {
		std::string line;
		while (getline(file, line)) {
			cout<<"Line: "<<line<<endl;
			parse(line);
			cout<<" "<<endl;
		}
		displayMap();
		file.close();
	}
}

void grammar::parse(std::string inputLine){
	int n = inputLine.length();

	if (inputLine[0] == '#'){
		std::string tempStr = inputLine.substr(1, inputLine.size());
		vector<std::string> nonTerminalAndProductions = split(tempStr, '=');
		std::string nonTerminal = triim(nonTerminalAndProductions[0]);
		std::string productions = triim(nonTerminalAndProductions[1]);

		currentNonTerminal = nonTerminal;
		nonTerminals.push_back(currentNonTerminal);
		parseProductions(productions);
	} else if(inputLine[0] == '|') {
		std::string productions = inputLine.substr(1, inputLine.size());
		parseProductions(productions);
	} else {
		cout<<"Error !"<<endl;
	}
}

void grammar::parseProductions(std::string production){
	std::string temp = "";
	for (int i = 0; i < production.length(); i++){
		if ('|' == production[i] && '\'' != production[i-1] && !(temp.empty())){
			temp = triim(temp);
			map.insert({currentNonTerminal, temp});
			temp = "";
		} else {
			temp += production[i];
		}
	}
	temp = triim(temp);
	map.insert({currentNonTerminal, temp});
}

vector<std::string> grammar::split(std::string s, char ch) {
	vector<std::string> result;
	string temp = "";

	for (int i = 0; i < s.size(); i++){
		if (ch == s[i] && s[i-1] != '\'' && !(temp.empty())){
			result.push_back(temp);
			temp = "";
		} else {
			temp += s[i];
		}
	}
	result.push_back(temp);
    return result;
}

void grammar::displayMap(){
	// Stores the range of key 1
	string nonTermiinals[13] = {"METHOD_BODY", "STATEMENT_LIST", "STATEMENT", "DECLARATION",
			"PRIMITIVE_TYPE", "IF", "WHILE", "ASSIGNMENT", "EXPRESSION", "SIMPLE_EXPRESSION",
			"TERM", "FACTOR", "SIGN"};

	for (int i = 0; i < 13; i++){
		auto it = map.equal_range(nonTermiinals[i]);
		cout << "The multimap elements of key "<<nonTermiinals[i]<<" is : \n";
		// Prints all the elements of key 1
		for (auto itr = it.first; itr != it.second; ++itr) {
			cout << itr->first
		             <<": "<< itr->second << '\n';
		}
		cout <<" "<<endl;;
	}

	cout<<"Non-Terminals"<<endl;
	for (int i = 0; i < nonTerminals.size(); i++){
		cout<<"nonTerminals: "<<nonTerminals[i]<<endl;
	}
}

multimap<std::string, std::string> grammar::getMultiMap(){
	return map;
}

vector<std::string> grammar::getNonTerminal(){
	return nonTerminals;
}






