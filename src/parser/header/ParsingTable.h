#ifndef PT_PARSINGTABLE_H
#define PT_PARSINGTABLE_H

#include <map>
#include <string>
#include <set>
#include <iostream>
#include <vector>
#include <deque>
#include <fstream>
#include "../../lexical/header/simulation.h"


using namespace std;

/*
 * ParsingTable is responsible for constructing the Predictive Parsing Table,
 * and parse the input string with the help of the Lexical Analyzer
 *
 * Fields:
 * production: All the productions specified by the grammar.
 * startingSymbol: of the grammar.
 * first: All the first sets of the non-terminals and the RHS of all productions.
 * follow: All the follow sets of the non-terminals.
 * parsingTable: Predictive Parsing Table
 * lexicalAnalyzer: The Lexical Analyzer used to get the tokens of the input string.
 *
 * Data Structures: parsingTable is a map, so that it provides fast access (lg N),
 * and no space is wasted as in the case of using an array.
 *
 * Constructors:
 * (lexicalAnalyzer, &production, &first, &follow): Initialize the fields,
 * run the table construction process, and parse the string specified
 * by the lexical analyzer.
 *
 * Functions:
 * buildTable(): constructs the predictive parsing table.
 * addSynch(): adds the "synch" entries in the table.
 * printTable(): prints the predictive parsing table.
 * queryTable(string nonTerminal, string terminal):
 * 		return the entry specified by the nonTerminal and terminal.
 * parse(): parses the string input specified by the lexical analyzer.
 *
 */
class ParsingTable {
private:
    multimap<string, string> production;
    string startingSymbol;
    map<string,set<string>> first;
    map<string,set<string>> follow;
    map<string,string> parsingTable;
    simulation *lexicalAnalyzer;

    vector<string> split(string str, char around);
    bool isTerminal(string &s);

public:
    ParsingTable(simulation * lexicalAnalyzer, multimap<string, string> &production,
    		string startingSymbol, map<string,set<string>> &first, map<string,set<string>> &follow);
    void buildTable();
    void addSynch();
    void printTable();
    string queryTable(string nonTerminal, string terminal);
    void parse();


};


#endif //PT_PARSINGTABLE_H
