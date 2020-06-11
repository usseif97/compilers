#include "header/First.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <regex>
#include <map>
#include <iterator>

using namespace std;

First::First() {

}

First::~First() {
}

void First::set_multimap(multimap <string, string> productions) {
    prod = productions;
}

void First::set_non_terminals_vector(vector<string> nonterminals) {
    non_terminals = nonterminals;
}

// get map of first of all terms
map<string, set<string>> First::get_all_first() {
    map<string, set<string>> result;
    for (string lhs: non_terminals) {
        auto it = prod.equal_range(lhs);
        result.insert({lhs, get_first(lhs)});
        for (auto itr = it.first; itr != it.second; ++itr) {
            auto founder = result.find(itr->second);
            if(founder == result.end())
            {
                // element not found
                result.insert({itr->second, get_first(itr->second)});
            }
        }
    }
    return result;
}

// print all first map
void First::print_all_first_map() {
    map<string, set<string>> result = get_all_first();
    for (auto elem : result) {
        cout << "First(" << elem.first << ") = { ";
        set<string> f = elem.second;
        auto it = f.begin();
        while (it != f.end())
        {
            cout << (*it);
            it++;
            if (it != f.end())
                cout << " , ";
        }
            cout << " }" << endl;
    }
}

// get first of single term
set<string> First::get_single_first(string input) {
    set<string> first;
    if (!first_is_terminal(input).empty()) {
        first.insert(first_is_terminal(input));
    } else if (input == "\\L") {
        first.insert("\\L");
    } else {
        // products of current term
        auto it = prod.equal_range(input);
        for (auto itr = it.first; itr != it.second; ++itr) {
            vector<string> components = get_components(itr->second);
            bool epsilon = true;
            for (string s: components) {
                set<string> current_first = get_first(s);
                // check if epsilon found
                auto it = std::find(current_first.begin(), current_first.end(), "\\L");
                if (it == current_first.end()) { //not found
                    epsilon = false;
                    first.insert(current_first.begin(), current_first.end());
                    break;  // no need to check next element
                } else {
                    for (string s2 : current_first) {
                        if (s2 != "\\L")
                            first.insert(s2);
                    }
                }
            }
            if (epsilon) {
                first.insert("\\L");
            }
        }
    }
    return first;
}

// get first of input
// uses get components and prod
set<string> First::get_first(string input) {
    if (single_term(input)) {
        return get_single_first(input);
    }
    // has multiple terms
    set<string> first;
    if (!first_is_terminal(input).empty()) {
        first.insert(first_is_terminal(input));
    } else if (input == "\\L") {
        first.insert("\\L");
    } else {
        vector<string> components = get_components(input);
        bool epsilon = true;
        for (string s: components) {
            //cout << s << endl;
            // products of current s
            set<string> current_first = get_first(s);
            // check if epsilon found
            auto it = std::find(current_first.begin(), current_first.end(), "\\L");
            if (it == current_first.end()) { //not found
                epsilon = false;
                first.insert(current_first.begin(), current_first.end());
                break;  // no need to check next element
            } else {
                for (string s2 : current_first) {
                    if (s2 != "\\L")
                        first.insert(s2);
                }
            }
        }
        if (epsilon) {
            first.insert("\\L");
        }
    }
    return first;
}

// check if string consists of one term
bool First::single_term(string s) {
    for (auto x : s)
    {
        if (x == ' ')
            return false;
    }
    return true;
}

string First::first_is_terminal(string s) {
    smatch match;
    regex r("^[']([\\S]+)[']");
    string subject = s;
    if (regex_search(subject, match, r)) {
        //cout << match.str(1) << endl;
        string terminal = match.str(1);
        adjust_reserved_symbols(terminal);
        return terminal;
    }
    return "";
}

void First::adjust_reserved_symbols(string& str) {
    size_t index = 0;
    while (true) {
        /* Locate the substring to replace. */
        index = str.find("\\'", 0);
        if (index != std::string::npos) {
            /* Make the replacement. */
            // length of substring to be replaced is 2
            str.replace(index, 2, "'");
            continue;
        }
        index = str.find("\\|", 0);
        if (index != std::string::npos) {
            str.replace(index, 2, "|");
            continue;
        }
        index = str.find("\\#", 0);
        if (index != std::string::npos) {
            str.replace(index, 2, "#");
            continue;
        }
        index = str.find("\\=", 0);
        if (index != std::string::npos) {
            str.replace(index, 2, "=");
            continue;
        }
        break;
    }
}

// split elements of rhs into a vector
vector<string> First::get_components(string rhs) { //rhs -> right hand section
    vector<string> components = split(rhs, ' ');
    return components;
}

// split string around single character
vector<string> First::split(string str, char around)
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

// print productions with components
void First::print_prod_components(multimap<string, string> prod) {
    multimap <string, string> :: iterator itr;
    cout << "\tKEY\tELEMENT\t\tComponents\n";
    for (itr = prod.begin(); itr != prod.end(); ++itr)
    {
        cout  <<  '\t' << itr->first
              <<  '\t' << itr->second;
        vector<string> comp = get_components(itr->second);
        for (int i = 0; i < comp.size(); i++) {
            cout << "\t\t" << comp.at(i);
        }
        cout << "\n";
    }
    cout << endl;
}

// print productions with terminals
void First::print_prod_terminals(multimap<string, string> prod) {
    multimap <string, string> :: iterator itr;
    cout << "\nThe multimap prod is : \n";
    cout << "\tKEY\tELEMENT\t\tFirst Terminal\n";
    for (itr = prod.begin(); itr != prod.end(); ++itr)
    {
        cout  <<  '\t' << itr->first
              <<  '\t' << itr->second;

        string terminal = first_is_terminal(itr->second);
        if (!terminal.empty()) {
            cout << "\t\t" << terminal;
        }
        cout << "\n";
    }
    cout << endl;
}


