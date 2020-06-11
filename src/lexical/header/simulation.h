#ifndef LEXICAL_SIMULATION_H_
#define LEXICAL_SIMULATION_H_

#include "DFA.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

typedef struct token {
	string token_name;
	string token_value;
} token;

class simulation {


public:

	//simulation();
	simulation(DFA& d, vector<string>& keywords);
	virtual ~simulation();


	DFA dfa;
	fstream f;
	ofstream outputFile;
	map<string,string> symTable;
	vector<token> lexemes;



	token nextLexeme();
	void allLexemes();
	string move(string state, char c);
	string actionID(string lexeme);



};

#endif /* LEXICAL_SIMULATION_H_ */
