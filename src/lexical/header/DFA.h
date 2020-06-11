#ifndef LEXICAL_HEADER_DFA_H_
#define LEXICAL_HEADER_DFA_H_
#include <iostream>
#include <fstream>
#include <string>
#include <bits/stdc++.h>

using namespace std;
#define ROW_MAX_LEN 500
#define COLOUMN_MAX_LEN 128

class DFA {
public:
	DFA();
	virtual ~DFA();
	void readParameters(string, string, vector< vector<string> >, vector<pair<string, string>>);
	void convert();
	void epsilonClosure(int);
	vector<std::string> firstEpsilonClosure(char*);
	void transition(vector <std::string>);
	void unionClosure(int, int);
	std::string removeDuplicate(std::string);
	void remove(std::vector<std::string>&);
	vector<std::string> conv_string_to_vector(std::string, char);
	int isBelong(std::string , std::string );
	void remove(std::vector<int>&);
	void displayDFA_TransitionTAble();
	vector< vector<string> > getMinDFATable();
	vector< pair<string, vector<string> > > getFinalStatesWiithToken();
	string getinState();
	vector<pair<string, string>> getMappedFinalWithTokens();

	vector< vector<string> > NFA_TranstionTable;
	vector< vector<string> > DFA_TranstionTable;
	vector<std::string> markedStates;
	vector<vector<string>> mDFATable;
	vector<pair<string, vector<string>>> finalStatesWIthTokens;
	string intialStaaate;
	vector<pair<string, string>> finalsTokens;
	vector<pair<string, string>> newfinalsTokens;
	vector<pair<string, string>> mapFinalsWithTokens;

private:
	string intialState;
	string finalState;
	int statesCount = 0;
	int symbolsCount = 0;
	int epsilonIndex;
	int intialStateIndex;
	int tempDFA_row = 0;
	int numberOfStatesTemp = 0;
	int numberOfStates = 0;

	vector<pair<std::string, vector <std::string> > > closureStates;

};

#endif /* LEXICAL_HEADER_DFA_H_ */
