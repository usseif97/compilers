#include "header/rules.h"
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


/**
 * Trimming functions
 */
std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    return ltrim(rtrim(str, chars), chars);
}

bool sortComparator (pair<string, string> i, pair<string, string> j) {
	return (i.first.length()>j.first.length());
}


rules::rules() {
	std::cout<<"initializing rules ..."<<std::endl;
}

rules::~rules() {
}

/**
 * read the rules file and call parseLine for each line
 */
void rules::readFile(char* rulesFile) {
	std::cout<<"rules file: "<<rulesFile<<std::endl;
	std::ifstream file(rulesFile);
	if (file.is_open()) {
	    std::string line;

	    // loop to read the file line by line
	    while (getline(file, line)) {
	        parseLine(line);
	    }
	    displayRulesData();
	    vector<pair<string, string>> finalDef = substituteDef();
	    displayDefinitionsData(finalDef);
	    substituteRules(finalDef);
	    displayRulesData();
	    file.close();
	}
}

/**
 * parse each line of the rules file
 */
void rules::parseLine(string line) {
	trim(line);
	int n = line.length();
	vector<string> tokens;
	string token = "";
	char special = ' ';
	string type = "";
	for(int i = 0; i < n; i++) {
		if (line[i] == ' ' || line[i] == '\t') {
			if (special == '{') {
				if (token != "") {
					tokens.push_back(trim(token));
					token = "";
				}
			} else if (special == '[') {
				if (token != "") {
					tokens.push_back(trim(token));
					token = "";
				}
			} else if (token != "") {
				token += line[i];
			}
		} else if (line[i] == '{') {
			if (i != 0) {
				if (type != "") {
					token += line[i];
					continue;
				}
				cout<<"error { not in the beginning"<<endl;
			} else {
				type = "keyword";
				special = line[i];
			}
		} else if (line[i] == '}') {
			if (type != "keyword" && i != 0) {
				token += line[i];
				continue;
			}
			if (token != "") {
				tokens.push_back(trim(token));
			}
			token = "";
			break;
		} else if (line[i] == '=') {
			if (type != "") {
				token += line[i];
				continue;
			}
			type = "definition";
			tokens.push_back(trim(token));
			token = "";
			if (i == 0) {
				cout<<"error = without preceding name"<<endl;
			}
		} else if (line[i] == ':') {
			type = "regex";
			tokens.push_back(trim(token));
			token = "";
			if (i == 0) {
				cout<<"error = without preceding name"<<endl;
			}
		} else if (line[i] == '[') {
			if (i != 0) {
				if (type != "") {
					token += line[i];
					continue;
				}
				cout<<"error [ not in the beginning"<<endl;
			}
			type = "special";
			special = line[i];
		} else if (line[i] == ']') {
			if (type != "special" && i != 0) {
				token += line[i];
				continue;
			}
			if (token != "") {
				tokens.push_back(trim(token));
			}
			token = "";
			break;
		} else {
			token += line[i];
		}
	}

	if (token != "") {
		tokens.push_back(trim(token));
	}

	if (type == "definition") {
		definitions.push_back(tokens);
	} else {
		rulesData.push_back(pair<string, vector<string>>(type, tokens));
	}
}

void rules::displayRulesData() {
	vector<pair<string, vector<string>>>::iterator itr;
	cout << "\nThe rules data is : \n";
	for (itr = rulesData.begin(); itr != rulesData.end(); ++itr) {
		cout << (*itr).first;
		cout << "\t";
		for(vector<string>::iterator it = (*itr).second.begin(); it != (*itr).second.end(); ++it) {
			if (it == (*itr).second.begin()) {
				cout << '\t' << *it << "\n";
				continue;
			}
			cout << "\t\t" << *it << "\n";
		}
		cout << endl;
	}
	cout << endl;
}

void rules::displayDefinitionsData(vector<pair<string, string>> finalDef) {
	vector<pair<string, string>>::iterator itr;
	cout << "\nThe definitions data is : \n";
	for (itr = finalDef.begin(); itr != finalDef.end(); ++itr) {
		cout << itr->first;
		cout << " = ";
		cout << itr->second;
		cout << endl;
	}
	cout << endl;
}

vector<pair<string, string>> rules::substituteDef() {
	vector<pair<string, string>> finalDef;
	for (vector<vector<string>>::iterator itr = definitions.begin(); itr != definitions.end(); ++itr) {
		if (itr->size() < 2) {
			cout<<"ERROR!! the definition is missing"<<endl;
			continue;
		}
		string first = itr->at(0);
		string secondTemp = itr->at(1);
		for (vector<pair<string, string>>::iterator it = finalDef.begin(); it != finalDef.end(); ++it) {
			// string to be searched in
			string temp = it->first;

			// Get the first occurrence
			size_t pos = secondTemp.find(temp);

			// Repeat till end is reached
			while( pos != std::string::npos) {
				// Replace this occurrence of Sub String
				string toReplace = "(" + it->second + ")";
				secondTemp.replace(pos, temp.size(), toReplace);

				// Get the next occurrence from the current position
				pos =secondTemp.find(temp, pos + toReplace.size());
			}
		}

		string second = secondTemp;
		cout<<first<<" = "<<second<<endl;

		// push the original or modified definition into new table
		pair<string, string> p(first, second);
		finalDef.push_back(p);
	}

	// sort the definitions using length
	sort(finalDef.begin(), finalDef.end(), sortComparator);

	return finalDef;
}

void rules::substituteRules(vector<pair<string, string>> finalDef) {
	cout<<"substituting definitions into regex ..."<<endl;
	for (vector<pair<string, vector<string>>>::iterator itr = rulesData.begin(); itr != rulesData.end(); ++itr) {
		string type = itr->first;
		string regex = "";
		if (type == "regex") {
			string originalRegex = (itr->second).at(1);
			cout<<"original regex: "<<originalRegex<<"\t";
			for (vector<pair<string, string>>::iterator it = finalDef.begin(); it != finalDef.end(); ++it) {
				string temp = it->first;

				// Get the first occurrence
				size_t pos = originalRegex.find(temp);

				// Repeat till end is reached
				while( pos != std::string::npos) {
					// Replace this occurrence of Sub String
					string toReplace = "(" + it->second + ")";
					originalRegex.replace(pos, temp.size(), toReplace);

					// Get the next occurrence from the current position
					pos = originalRegex.find(temp, pos + toReplace.size());
				}
			}
			itr->second.at(1) = originalRegex;
			cout<<"after substitution: "<<itr->second.at(1)<<endl;
		}
	}
}

vector<string> rules::getKeywords() {
	vector<string> result;
	for (auto itr = rulesData.begin(); itr != rulesData.end(); ++itr) {
		string first = itr->first;
		if (first == "keyword") {
			vector<string> second = itr->second;
			for (auto it = second.begin(); it != second.end(); ++it) {
				result.push_back(*it);
			}
		}
	}

	return result;
}

vector<pair<string, vector<string>>> rules::getRulesDate() {
	return rulesData;
}

vector<vector<string>> rules::getDefinitions() {
	return definitions;
}
