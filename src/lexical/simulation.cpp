#include "header/simulation.h"

//simulation::simulation() {}

simulation::simulation(DFA &d, vector<string>& keywords) :
		dfa(d) {

	for (auto it = keywords.begin(); it < keywords.end(); it++) {
		symTable[*it] = "keyword";
	}
	f.open("input.txt");
	outputFile.open("output.txt");
}

token simulation::nextLexeme() {


	string lexeme;
	stack<string> seq;
	string state = dfa.getinState();

	seq.push(state);

	char c;
	while (f.get(c), !f.eof()) {
		if (seq.size() == 1 && (c == ' ' || c == '\n' || c == '\t')) {
			continue;
		}
		state = move(state, c);
		if (state != "0") {
			seq.push(state);
			lexeme += c;
		} else {
			break;
		}
	}
	if (seq.size() > 1 && !f.eof()) {
		f.putback(c);
	} else {
	}
	if (seq.size() == 1 && f.eof() ) {
		token token = { "$", "" };
		//cout << "<" << token.token_name << ", " << token.token_value << ">" << endl;
		return token;
	}
	bool found = false;
	string tk;
	while (!seq.empty()) {
		string final = seq.top();
		seq.pop();

		for (unsigned int i = 0; i < dfa.getMappedFinalWithTokens().size();
				i++) {
			if (dfa.getMappedFinalWithTokens()[i].first == final) {
				found = true;
				tk = dfa.getMappedFinalWithTokens()[i].second;
				break;
			}
		}
		if (found) break;
	}

	if (found) {
		token result = { tk, lexeme };
		if (tk == "id") {
			string token_temp = actionID(lexeme);
			result.token_name = token_temp;

		}
		//cout << "<" << result.token_name << ", " << result.token_value << ">" << endl;
		return result;
	} else {
		token result = { "Error", "" };
		//cout << "<" << result.token_name << ", " << result.token_value << ">" << endl;
		return result;
	}

}

void simulation::allLexemes(){
	while (!f.eof()) {
		token token = nextLexeme();
		lexemes.push_back(token);
		cout << "<" << token.token_name << ", " << token.token_value << ">" << endl;
		outputFile << token.token_value << " \t " << token.token_name << endl;
	}
	outputFile.close();
}

string simulation::actionID(string lexeme) {
	auto it = symTable.find(lexeme);
	if (it == symTable.end()) {
		symTable[lexeme] = "id";
		return "id";
	} else {
		return (symTable[lexeme] == "keyword") ? lexeme : "id";
	}
}

string simulation::move(string state, char c) {
	string nextState;

	nextState = dfa.getMinDFATable()[stoi(state)][c];
	return nextState;

}

simulation::~simulation() {

}

