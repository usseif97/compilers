#ifndef LEXICAL_HEADER_NFA_H_
#define LEXICAL_HEADER_NFA_H_
#include <vector>
#include <string>

using namespace std;
class NFA {

private:
    struct trans {
        int from;
        int to;
        string trans_symbol;
    };
    vector<int> state;
    vector <trans> transitions;
    int final_state;
    vector<pair<string,string>> result_tokens;

public:
    NFA();
    virtual ~NFA();
    NFA execute(vector<pair<string, vector<string>>>);
    vector<vector<string>> get_results(NFA);
    void display();
    NFA input_to_NFAs(vector<pair<string, vector<string>>>);
    string adjustString(string);
    void reverseStr(string& str);
    string adjustStringReversed(string);
    int get_state_count();
    void set_state(int);
    void set_transition(int, int, string);
    void set_final_state(int);
    string get_final_state();
    int get_initial_state();
    NFA concat(NFA, NFA);
    NFA kleene(NFA);
    NFA or_selection(vector <NFA>, int, bool);
    NFA re_to_nfa(string);
    NFA keyword_to_nfa(string);
    string interval_substitute(string);
    void replaceAll(string&, const string&, const string&);
    string adjust_re(string);
    vector<trans> get_transitions();
    void set_tokens (vector<pair<string,string>>);
    vector<pair<string,string>> get_tokens();
    void display_tokens();
};

#endif /* LEXICAL_HEADER_NFA_H_ */
