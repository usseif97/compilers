#ifndef COMPILERS_ZIAD_LL1GRAMMAR_H
#define COMPILERS_ZIAD_LL1GRAMMAR_H
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

class LL1Grammar {
private:
    vector<string> input;
    vector<pair<string, vector<string>>> disassembled_prod;
    vector<string> lhs;
    multimap <string, string> prod_multimap;
    void read_from_file();
    set<string> get_non_terminals(vector<string>);
    string get_non_terminal(string);
    void disassemble_productions();
    vector<string> get_terms(string);
    vector<string> split(string, char);
    void erase_SubStr(string &mainStr, const string &toErase);
    void substitute_eliminate_lr();
    void eliminate_left_recursion(int i);
    void left_factoring();
    void fill_output();


public:
    LL1Grammar();
    virtual ~LL1Grammar();
    void print_input();
    void print_all_lhs();
    void print_prod_multimap();
    void print_disassembled_productions();
    vector<pair<string, vector<string>>> get_LL1_products();
    void generate_LL1_grammar();
    vector<string> get_all_lhs();
    multimap <string, string> get_prod_multimap();
};


#endif //COMPILERS_ZIAD_LL1GRAMMAR_H
