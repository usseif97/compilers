#ifndef COMPILERS_ZIAD_FIRST_H
#define COMPILERS_ZIAD_FIRST_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>

using namespace std;


class First {
private:
    multimap <string, string> prod;
    vector<string> non_terminals;
    set<string> get_first(string);
    set<string> get_single_first(string);
    bool single_term(string);
    string first_is_terminal(string);
    void adjust_reserved_symbols(string&);
    vector<string> get_components(string);
    vector<string> split(string, char);

public:
    First();
    virtual ~First();
    void set_multimap(multimap <string, string>);
    void set_non_terminals_vector(vector<string>);
    map<string, set<string>> get_all_first();
    void print_all_first_map();
    void print_prod_components(multimap<string, string> prod);
    void print_prod_terminals(multimap<string, string> prod);


};


#endif
