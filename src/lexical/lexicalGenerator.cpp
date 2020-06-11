#include "header/lexicalGenerator.h"
#include <bits/stdc++.h>
#include <iostream>
#include "header/rules.h"
#include "header/NFA.h"
#include "header/DFA.h"
#include "header/DFAMinimization.h"
#include "header/simulation.h"

#define ROW_MAX_LEN 500

using namespace std;

lexicalGenerator::lexicalGenerator() {
	std::cout<<"initializing lexical generator ..."<<std::endl;
	char rulesFile[] = "rulesSet.txt";

	// initializing rules and send the rules file
	rules rule;
	rule.readFile(rulesFile);

	// get the rules data
	vector<pair<string, vector<string>>> rules = rule.getRulesDate();
	vector<string> keywords = rule.getKeywords();

	cout << "All keywords:" << endl;
	for (auto it = keywords.begin(); it != keywords.end(); ++it) {
		cout << *it << endl;
	}

	// initializing NFA and send it the rules
	NFA nfa;
	NFA result = nfa.execute(rules);
	//result.display();

	// getting the initial state and final state of the NFA and fill the NFA transition table
	vector<vector<string>> NFA_TranstionTable;
	NFA_TranstionTable = nfa.get_results(result);
	string initialState = "1";
	string finalState = result.get_final_state();
	//std::cout<<"finalState: "<<finalState<<std::endl;

	//std::cout << "displaying tokens NFA:"<<endl;
	//nfa.display_tokens();

	// NFA TOKENS
	vector<pair<string, string>> NFAtokens = nfa.get_tokens();

	// initializing DFA
	DFA dfa;
	dfa.readParameters(initialState, finalState, NFA_TranstionTable, NFAtokens);
	dfa.convert();
	vector<vector<string>> minDFATable = dfa.getMinDFATable();
	vector<pair<string, vector<string>>> finalStatesWIthTokens =
			dfa.getFinalStatesWiithToken();
	string initState = dfa.getinState();
	vector<pair<string, string>> finalsWithToken =
			dfa.getMappedFinalWithTokens();

/*
	cout << "---------------------------------" << endl;
	for (auto it = finalsWithToken.begin(); it < finalsWithToken.end(); it++) {
		cout << (*it).first << "  " << (*it).second << endl;
	}
	cout << "---------------------------------" << endl;
	for (auto it = finalStatesWIthTokens.begin(); it < finalStatesWIthTokens.end(); it++) {
		cout << (*it).first << "  " << (*it).second[0] << endl;
	}
 */
	lexicalAnalyzer = new simulation(dfa, keywords);
	//lexicalAnalyzer.allLexemes();
}

lexicalGenerator::~lexicalGenerator(){

}

