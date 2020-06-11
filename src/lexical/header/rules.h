#ifndef LEXICAL_HEADER_RULES_H_
#define LEXICAL_HEADER_RULES_H_

#include <map>
#include <vector>

using namespace std;
class rules {
public:
	rules();
	virtual ~rules();
	void readFile(char*);
	void parseLine(string);
	void displayRulesData();
	void displayDefinitionsData(vector<pair<string, string>>);
	vector<pair<string, string>> substituteDef();
	void substituteRules(vector<pair<string, string>>);
	vector<pair<string, vector<string>>> getRulesDate();
	vector<vector<string>> getDefinitions();
	vector<string> getKeywords();

private:
	vector<pair<string, vector<string>>> rulesData;
	vector<vector<string>> definitions;
};

#endif /* LEXICAL_HEADER_RULES_H_ */
