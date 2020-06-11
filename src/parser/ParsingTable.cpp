#include "header/ParsingTable.h"


ParsingTable::ParsingTable(simulation *lexicalAnalyzer,
		multimap<string, string> &production, string startingSymbol, map<string, set<string>> &first,
		map<string, set<string>> &follow) :
		lexicalAnalyzer(lexicalAnalyzer), startingSymbol(startingSymbol),
		follow(follow), first(first), production(production) {
	buildTable();
	parse();
}

/*
 * constructs the predictive parsing table.
 *
 * The algorithm follows directly from [Dragon's Book], Algorithm 4.31.
 *
 */
void ParsingTable::buildTable() {
	parsingTable.clear();
	for (pair<string, string> p : production) {
		auto fir = first.find(p.second);
		if (fir == first.end()) {
			cout << "ERROR -> Can't find a first set for: " << p.second << endl;
			return;
		} else {
			/* A -> x */
			bool epsilon = false;
			set<string> &firstSet = (*fir).second; /* First(x) */
			for (string terminal : firstSet) { /* for each terminal a in First(x) */
				if (terminal != "\\L") {
					if (!parsingTable.count(p.first + "." + terminal)) {
						parsingTable[p.first + "." + terminal] = p.second; /* table[A,a] = x */
					} else {
						cout << "ERROR: NOT LL1 GRAMMAR !"
								<< endl;
						parsingTable.clear();
					}
				} else {
					epsilon = true; /* First(x) has epsilon */
				}
			}
			if (epsilon) { /* if First(x) has epsilon */
				auto fol = follow.find(p.first);
				if (fol == follow.end()) {
					cout << "ERROR -> Can't find a follow set for: " << p.first
							<< endl;
					parsingTable.clear();
				} else {
					set<string> &followSet = (*fol).second; /* Follow(A) */
					for (string terminal : followSet) { /* for each terminal b in follow(A) */
						if (!parsingTable.count(p.first + "." + terminal)) {
							parsingTable[p.first + "." + terminal] = p.second; /* table[A,a] = X */
						} else {
							cout << "ERROR: NOT LL1 GRAMMAR !"
									<< endl;
							parsingTable.clear();
						}
					}
				}
			}
		}
	}
	addSynch();
	printTable();
}

/*
 * adds the "synch" entries in the table.
 * We choose the follow sets to be the synchronizing sets.
 */
void ParsingTable::addSynch() {
	for (auto p : follow) { /* for each Follow(A) */
		const string &nonTerminal = p.first;
		set<string> &followSet = p.second;
		for (string terminal : followSet) { /* for each terminal a in follow(A) */
			if (!parsingTable.count(nonTerminal + "." + terminal)) { /* if table[A,a] = error */
				parsingTable[nonTerminal + "." + terminal] = "synch"; /* table[A,a] = synch */
			}
		}
	}
}

/*
 * prints the predictive parsing table in the form:
 * non-terminal.terminal --> entry
 */
void ParsingTable::printTable() {
	ofstream tableFile;
	tableFile.open("table.txt", ios::trunc);
	if (!tableFile.is_open()) {
		cout << "CANNOT OPEN OUTPUT FILE table.txt" << endl;
		return;
	}
	for (auto it = parsingTable.begin(); it != parsingTable.end(); it++) {
		tableFile << it->first << " --> " << it->second << endl;
	}
}

/*
 * queryTable(string nonTerminal, string terminal):
 */
string ParsingTable::queryTable(string nonTerminal, string terminal) {
	auto it = parsingTable.find(nonTerminal + "." + terminal);
	if (it == parsingTable.end()) {
		return ""; // "" respresents error
	} else {
		return (*it).second;
	}
}

/*
 * parses the string input specified by the lexical analyzer.
 * Decisions: '$' represents the end of a string.
 * Data Structures:
 * 	deque derivationStack: We choose a deque, so that we can directly
 * 	access the first element -just like a normal stack-, in addition
 * 	to be able to print the content of the list.
 *
 * 	Algorithm:
 * 	follows directly from [Dragon's Book], Algorithm 4.34.
 * 	The next token is provided by lexicalAnalyzer::nextLexeme().
 */
void ParsingTable::parse() {
	ofstream derivationFile;
	derivationFile.open("derivation.txt", ios::trunc);
	if (!derivationFile.is_open()) {
		cout << "CANNOT OPEN OUTPUT FILE derivation.txt" << endl;
		return;
	}
	deque<string> derivationStack;
	vector<string> tokensFound;
	derivationStack.push_front("$");
	derivationStack.push_front(startingSymbol);
	string next = lexicalAnalyzer->nextLexeme().token_name;

	while (derivationStack.front() != "$") {
		/* prints the terminals found until now */
		for (string s : tokensFound) {
			derivationFile << s << " ";
		}
		/* prints the stack */
		//derivationFile << "\"";
		for (auto it = derivationStack.begin(); it < derivationStack.end() - 1;
				it++) {
			derivationFile << *it << " ";
		}
		/* prints the next token provided by the lexical analyzer */
		//derivationFile << " \"\t\t" << next;
		derivationFile << endl;
		string stackTop = string(derivationStack.front());
		if (isTerminal(stackTop)) {
			if (stackTop == next) {
				tokensFound.push_back(next);
				next = lexicalAnalyzer->nextLexeme().token_name;
				derivationStack.pop_front();
			} else {
				derivationFile << "Error parsing: " << stackTop << " is missing. Inserting" << endl;
				tokensFound.push_back(stackTop);
				derivationStack.pop_front();
			}
		} else {
			string tableEntry = queryTable(derivationStack.front(), next);
			if (tableEntry == "") {
				derivationFile << "Error parsing: Illegal " << derivationStack.front() <<
						". Discarding " << next <<"." << endl;
				if (next == "$") {
					break;
				} else {
					next = lexicalAnalyzer->nextLexeme().token_name;
				}
			} else if (tableEntry == "synch") {
				derivationFile << "Error parsing: SYNCH entry. Popping " << derivationStack.front() << endl;
				derivationStack.pop_front();
			} else {
				derivationStack.pop_front();
				vector<string> nonTerminals = split(tableEntry, ' ');
				for (auto it = nonTerminals.rbegin(); it != nonTerminals.rend();
						it++) {
					if (*it != "\\L")
						derivationStack.push_front(*it);
				}
			}
		}
	}
	for (string s : tokensFound) {
		derivationFile << s << " ";
	}
}

vector<string> ParsingTable::split(string str, char around) {
	vector<string> result;
	string word;
	for (auto x : str) {
		if (x == around) {
			result.push_back(word);
			word = "";
		} else {
			word += x;
		}
	}
	if (!word.empty())
		result.push_back(word);
	return result;
}

bool ParsingTable::isTerminal(string &s) {
	if (s[0] == '\'') {
		s.erase(s.begin());
		s.pop_back();
		return true;
	} else {
		return false;
	}
}
