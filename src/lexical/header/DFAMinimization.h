#ifndef LEXICAL_DFAMINIMIZATION_H_
#define LEXICAL_DFAMINIMIZATION_H_
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;
#define ROW_MAX_LEN 500
#define COLOUMN_MAX_LEN 128

class DFAMinimization {
public:

	DFAMinimization();
	virtual ~DFAMinimization();
	void minimize(string, string, vector< vector<string> >, vector<string>, vector<pair<string, string>>);
	void mapping();
	void minimizeTheStates();
	int checkBelongToTheSameClass(int ,int ,vector< pair<string, int> >&, string);
	void minimizedTable();
	int isBelong(std::string , std::string );
	int isBelongForFinals(string , string );
	int isSubstring(string , string );
	int getIndex(string );
	int isEqual(vector<std::string> ,vector<std::string> );
	void remove(std::vector<string> &v);
	void mappingMinimizedTable();
	void finalsWithTokens();
	vector<std::string> conv_string_to_vector(std::string s, char ch);
	void display_MinimizedDFATable();
	vector< vector<string> > getMinimizedDFATable();
	vector< pair<string, vector<string> > > getFinalStatesWithToken();
	string getInitState();
	vector<pair<string, string>> getMappedFinalStatesWithToken();

	vector< vector<string> > DFA_Table;
	vector< vector<string> > DFA_MinimizedTable;
	vector< pair<string, vector<string> > > finalStatesWithTokens;// will be vector of pair
	vector<pair<string, string>> newMinfinalsTokens;
	vector<pair<string, string>> mappedNewMinfinalsTokens;



private:
	string initState;
	string lastState;
	int numberrOfStates;
	int numberOfSymbols;
	std::string tempStr1;
	std::string tempStr2;
	string c1;
	string c2;

	vector<std::string> tempClasses;
	vector<std::string> newClasses;
	vector<string> states;
	vector< pair<string, string> > statesMapp;
	vector<std::string> finals;
	vector< pair<string, string> > finalStates;// will be vector of pair

};

#endif /* LEXICAL_DFAMINIMIZATION_H_ */
