#include "header/LL1Grammar.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <regex>
#include <map>
#include <iterator>

using namespace std;

LL1Grammar::LL1Grammar() {

}

LL1Grammar::~LL1Grammar() {
}

// return products
vector<pair<string, vector<string>>> LL1Grammar::get_LL1_products() {
    return disassembled_prod;
}

// return all lhs
vector<string> LL1Grammar::get_all_lhs() {
    return lhs;
}

// return final output multimap
multimap <string, string> LL1Grammar::get_prod_multimap() {
    return prod_multimap;
}

// read productions from the file & store lines in vector input
void LL1Grammar::read_from_file() {
    string line;
    ifstream myFile ("productions.txt");
    if (myFile.is_open()) {
        // loop to read the file line by line
        while (getline(myFile, line)) {
            //cout << line << endl;
            if (line.at(0) != '#') {
                input.at(input.size()-1).append(" " + line);
                continue;
            }
            input.push_back(line);
        }

        myFile.close();
    } else {
        cout << "file not opened !!!" << endl;
    }
}

// print productions and non terminals
void LL1Grammar::print_input(){
    string line;
    for (string s: input) {
        cout << s << endl;
    }
    set<string> non_terminals = get_non_terminals(input);
    for (string s: non_terminals) {
        cout << s << endl;
    }
}

// return non terminals of input
set<string> LL1Grammar::get_non_terminals(vector<string> lines) {
    smatch match;
    regex r("^[#]\\s(.+)\\s[=]");
    set<string> non_terminals;

    int i = 0;
    while (i < lines.size()) {
        string subject = lines.at(i);
        if (regex_search(subject, match, r)) {
            non_terminals.insert(match.str(1));
            //cout << "\nnon terminal is " << match.str(1) << endl;
        }
        i++;
    }
    return non_terminals;
}

// return non terminal from single production
string LL1Grammar::get_non_terminal(string line) {
    smatch match;
    regex r("^[#]\\s+(.+)\\s+[=]");
    string subject = line;
    if (regex_search(subject, match, r)) {
        return match.str(1);
    }
    return NULL;
}

// disassemble input for each part (ex: lhs , rhs elements as vector)
// and store output in vector disassembled_prod
void LL1Grammar::disassemble_productions() {
    for (string s : input) {
        string lhs = get_non_terminal(s);
        smatch match;
        regex r("^[#]\\s+(.+)\\s+[=]\\s+(.+)");
        string subject = s;
        if (regex_search(subject, match, r)) {
            string rhs = match.str(2);
            vector<string> terms = get_terms(rhs);
            disassembled_prod.push_back(make_pair(lhs,terms));
        } else {
            cout << "error found in input" << endl;
        }
    }
}

// print every single part in the productions on its own
void LL1Grammar::print_disassembled_productions() {
    for (pair<string, vector<string>> p : disassembled_prod) {
        cout << "lhs : " << p.first << " rhs terms ";
        for (string s : p.second) {
            cout << " : " << s ;
        }
        cout << endl;
    }
}

// return vector of terms of rhs ( split around or )
vector<string> LL1Grammar::get_terms(string rhs) {
    vector<string> terms;
    string s = rhs;
    size_t pos = 0;
    string token;
    string delimiter = " | ";
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        terms.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    terms.push_back(s);
    return terms;
}

// split string around single character
vector<string> LL1Grammar::split(string str, char around)
{
    vector<string> result;
    string word;
    for (auto x : str)
    {
        if (x == around)
        {
            result.push_back(word);
            word = "";
        }
        else
        {
            word += x;
        }
    }
    if (!word.empty())
        result.push_back(word);
    return result;
}

// delete substring in a string
void LL1Grammar::erase_SubStr(string &mainStr, const string &toErase) {
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);

    if (pos != string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

// eliminate left recursion
void LL1Grammar::substitute_eliminate_lr() {
    for (int i = 0; i < disassembled_prod.size(); i++) {
        // current production
        string lhs = disassembled_prod.at(i).first;
        vector<string> &terms = disassembled_prod.at(i).second;
        for (int s = 0; s < terms.size(); s++) {
            vector<string> current_rhs = split(terms.at(s), ' ');
            for (int j = 0; j <=  i-1; j++) {
                // sub by previous non terminals in current non terminal
                string prev_lhs = disassembled_prod.at(j).first;
                // first in rhs equal previous lhs
                if (current_rhs.at(0) == prev_lhs) {
                    // then substitute by it
                    vector<string> prev_terms = disassembled_prod.at(j).second;
                    // remove non terminal from current term
                    erase_SubStr(terms.at(s), prev_lhs);
                    // sub by non terminal terms
                    string tail = terms.at(s);
                    terms.at(s) = prev_terms.at(0) + tail;
                    int k;
                    for (k = 1; k < prev_terms.size(); k++) {
                        terms.insert(terms.begin()+s+k ,prev_terms.at(k) + tail);
                    }
                    s += k-1;
                }
            }
        }
        // eliminate left recursion here
        eliminate_left_recursion(i);
    }
}

// eliminate immediate left recursion of a given production
void LL1Grammar::eliminate_left_recursion(int i) {
    string lhs = disassembled_prod.at(i).first;
    vector<string> &terms = disassembled_prod.at(i).second;
    vector<string> new_rhs_terms;
    string new_lhs_name = lhs + "1";
    bool left_recursive = false;
    for (int s = 0; s < terms.size(); s++) {
        vector<string> current_rhs = split(terms.at(s), ' ');
        if (current_rhs.at(0) == lhs) { // left recursive
            left_recursive = true;
            erase_SubStr(terms.at(s), lhs + " ");
            // add to new production
            new_rhs_terms.push_back(terms.at(s));
            // remove from current production
            terms.erase(terms.begin()+s);
            s--;
        }
    }
    if (left_recursive) {
        // add new term
        for (int s = 0; s < terms.size(); s++) {
            terms.at(s) = terms.at(s) + " " + new_lhs_name;
        }
        for (int s = 0; s < new_rhs_terms.size(); s++) {
            new_rhs_terms.at(s) = new_rhs_terms.at(s) + " " + new_lhs_name;
        }
        new_rhs_terms.push_back("\\L");
        disassembled_prod.insert(disassembled_prod.begin()+i+1, make_pair(new_lhs_name, new_rhs_terms));
    }
}

// remove left factor from grammar
void LL1Grammar::left_factoring() {
    for (int i = 0; i < disassembled_prod.size(); i++) {
        // current production
        string lhs = disassembled_prod.at(i).first;
        vector<string> &terms = disassembled_prod.at(i).second;

        for (int s = 0; s < terms.size(); s++) {
            vector<string> current_rhs = split(terms.at(s), ' ');
            string factor = current_rhs.at(0);
            vector<string> current_refactored;
            bool common = false;
            for (int j = s+1; j < terms.size(); j++) {
                vector<string> other_rhs = split(terms.at(j), ' ');
                if (current_rhs.at(0) == other_rhs.at(0)) {
                    // have left common
                    common = true;
                    // remove factor from second term
                    if (terms.at(j).size() > factor.size())
                        erase_SubStr(terms.at(j), factor + " ");
                    else
                        erase_SubStr(terms.at(j), factor);
                    if (!terms.at(j).empty())
                        current_refactored.push_back(terms.at(j));
                    else
                        current_refactored.push_back("\\L");
                    // remove from current production
                    terms.erase(terms.begin()+j);
                    j--;
                }
            }
            if (common) {
                // remove factor from second term
                if (terms.at(s).size() > factor.size())
                    erase_SubStr(terms.at(s), factor + " ");
                else
                    erase_SubStr(terms.at(s), factor);
                if (!terms.at(s).empty())
                    current_refactored.insert(current_refactored.begin() ,terms.at(s));
                else
                    current_refactored.insert(current_refactored.begin() ,"\\L");
                // remove from current production
                terms.erase(terms.begin()+s);
                // add common factor with its assembled value
                terms.insert(terms.begin()+s, factor + " " +lhs+factor);
                disassembled_prod.insert(disassembled_prod.begin()+i+1, make_pair(lhs+factor, current_refactored));
            }
        }
    }
}

// fill the lhs vector and the productions multimap
void LL1Grammar::fill_output() {
    for (pair<string, vector<string>> p : disassembled_prod) {
        lhs.push_back(p.first);
        for (string s : p.second) {
            prod_multimap.insert({p.first, s});
        }
    }
}

void LL1Grammar::print_all_lhs() {
    cout << "LHS" << endl;
    for (string s : lhs) {
        cout << s << endl;
    }
}

void LL1Grammar::print_prod_multimap() {
    for (string s : lhs) {
        auto it = prod_multimap.equal_range(s);
        cout << "The multimap elements of key "<<s<<" is : \n";
        // Prints all the elements of key 1
        for (auto itr = it.first; itr != it.second; ++itr) {
            cout << itr->first
                 <<": "<< itr->second << '\n';
        }
        cout <<" "<<endl;;
    }
}

void LL1Grammar::generate_LL1_grammar() {
    // 1) read input from file
    read_from_file();
    // 2) disassemble productions as ex:
    // lhs string , rhs terms vector
    disassemble_productions();
    // 3) eliminate left recursion
    substitute_eliminate_lr();
    // 4) remove left factor
    left_factoring();
    // 5) fill vector and multimap output
    fill_output();
}


