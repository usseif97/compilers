#include <iostream>
#include <algorithm>
#include "header/Follow.h"
#include <stack>

using namespace std;

Follow::Follow() {
	cout << "initializing follow object ..." << endl;
}

/**
 * sets the productions after splitting them using | operator
 */
void Follow::setProduction(multimap<string, string> prod) {
	productions = prod;
}


/**
 * sets the original productions in their original order
 */
void Follow::setProductionOrg(vector<string> prod) {
	productionsOrg = prod;
}


/**
 * sets the first sets of the terminals and non terminals
 */
void Follow::setFirstSets(map<string, set<string>> firsts) {
	firstSets = firsts;
}


/**
 * gets the follow sets of the non terminals
 */
map<string, set<string>> Follow::getFollowSets() {
	return followSets;
}


/**
 * for each production it splits the RHS into tokens and send them to
 * computeFollow to compute the follow sets of the non terminals
 */
void Follow::run() {

	// loop 2 times to guarantee correctness
	for (int counter = 0; counter < 2; counter++) {

		// loop to get the LHS of the productions original form to access the multimap
		for (auto prodItr = productionsOrg.begin(); prodItr != productionsOrg.end(); ++prodItr) {
			string lhs = *prodItr;
			// cout << "\n\nLHS: " << lhs << endl;

			if (prodItr == productionsOrg.begin()) {
				followSets[lhs].insert("$");
			}

			// access multimap
			auto it = productions.equal_range(lhs);

			// cout << "The multimap elements of key " << lhs << " is : \n";
			// cout << "KEY\tELEMENT\n";

			// loop all the elements of the LHS
			for (auto itr = it.first; itr != it.second; ++itr) {
				// cout << itr->first << '\t' << itr->second << '\n';

				string rhs = itr->second;
				stack<char> quote; // stack to keep track of single quotes
				string terminal = "";
				string nonTerminal = "";
				bool escape = false;
				vector<pair<bool, string>> tokens;

				// loop the RHS and find the terminals and nonTerminals
				for (int i = 0; i < rhs.length(); i++) {
					char c = rhs[i];

					// checks for space separator and add the token to the tokens vector
					if (c == ' ') {
						if (nonTerminal != "") {
							tokens.push_back(pair<bool, string> (false, nonTerminal)); // false for non terminals
							nonTerminal = "";
						} else if (terminal != "") {
							tokens.push_back(pair<bool, string> (true, terminal)); // true for terminals
							terminal = "";
						}
						continue;
					}

					// check for escape character
					if (!escape && c == '\\') {
						escape = true;
						continue;
					}

					// check for epsilon
					if (escape && c =='L') {
					    escape = false;
                        continue;
					}

					// check for a single quote w/o escape before
					if (c == '\'' && !escape) {
						if (terminal == "") {
							quote.push(c);
						} else {
							quote.pop();
						}
						continue;
					}

					// checks for opened single quote and add to terminal
					if (quote.size() > 0) {
						terminal += c;
						continue;
					}

					nonTerminal += c;
					escape = false;
				}

				if (nonTerminal != "") {
					tokens.push_back(pair<bool, string> (false, nonTerminal)); // false for non terminals
					nonTerminal = "";
				} else if (terminal != "") {
					tokens.push_back(pair<bool, string> (true, terminal)); // true for terminals
					terminal = "";
				}

				computeFollow(tokens, lhs);

			}
		}
	}

	displayFollow();

}


/**
 * computes the follow set of the non terminals
 * @param tokens .. the tokens of the RHS of the production
 * @param lhs .. the LHS of the production
 */
void Follow::computeFollow(vector<pair<bool, string>> tokens, string lhs) {

	// loop the tokens of the sent production
	for (int i = 0; i < tokens.size(); i++) {
		bool type = tokens[i].first; // false: non terminal, true: terminal
		string token = tokens[i].second;

		// in case of terminals no computations needed
 		if (type) continue;

 		// checks for a following token
		if (i + 1 < tokens.size()) {
			bool typeNext = tokens[i + 1].first;
			string tokenNext = tokens[i + 1].second;

			// in case the following token is terminal its first is itself
			if (typeNext) {

				// union first(next token) with follow(current token)
				followSets[token].insert(tokenNext);
				followSets[token].erase("\\L");
				continue;

			}

			// union first(next token) with follow(current token)
			followSets[token].insert(firstSets[tokenNext].begin(),firstSets[tokenNext].end());
			followSets[token].erase("\\L");

			if (firstSets[tokenNext].find("\\L") != firstSets[tokenNext].end()) {

				// union follow(LHS of production) with follow(current token)
				followSets[token].insert(followSets[lhs].begin(),followSets[lhs].end());
			}

			continue;
		}

		// in case no following token
		// union follow(LHS of production) with follow(current token)
		followSets[token].insert(followSets[lhs].begin(),followSets[lhs].end());
	}

}


/**
 * displays the follow sets of the non terminals
 */
void Follow::displayFollow() {
	cout<< endl << endl << "Follow Sets:" << endl;
	for (auto itr = followSets.begin(); itr != followSets.end(); ++itr) {
		cout << itr->first << ": { ";
		set<string> s = itr->second;
		for (auto it = s.begin(); it != s.end(); ++it) {
			cout << *it << " ";
		}
		cout << "}" << endl;
	}
}

Follow::~Follow() {

}

