#include "header/NFA.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <stack>
#include <bits/stdc++.h>
#include <regex>

using namespace std;

NFA::NFA() {

}

NFA::~NFA() {
}

NFA NFA::execute(vector<pair<string, vector<string>>> rules) {
    return input_to_NFAs(rules);
}

// take input rules and return single combined nfa
NFA NFA::input_to_NFAs(vector<pair<string, vector<string>>> rules) {
    vector<NFA> result_NFAs;
    for (auto it = rules.begin(); it != rules.end(); ++it) {
        pair<string, vector<string>> curent_rule_pair = *it;
        if (curent_rule_pair.first == "keyword") {
            continue;
            /*vector<string> keywords = curent_rule_pair.second;
            for (string keyword : keywords) {
                result_NFAs.push_back(keyword_to_nfa(keyword));
            }*/
        } else if (curent_rule_pair.first == "special") {
            vector<string> specials = curent_rule_pair.second;
            for (string special : specials) {
                string token = special;
                if (special.size() > 1) {
                    token = special.at(special.size()-1);
                }
                string special_expression = adjust_re(special);
                NFA current_nfa = re_to_nfa(special_expression);
                int final_state = current_nfa.get_state_count() - 1;
                current_nfa.set_transition(final_state,final_state,token);
                result_NFAs.push_back(current_nfa);
            }
        } else if (curent_rule_pair.first == "regex") {
            vector<string> re_expressions = curent_rule_pair.second;
            string token = re_expressions.at(0);
            for (int i = 1; i < re_expressions.size(); i++) {
                string regex = adjustString(re_expressions[i]);
                //regex  = adjustStringReversed(regex);
                string current_regex = adjust_re(regex);
                NFA current_nfa = re_to_nfa(current_regex);
                int final_state = current_nfa.get_state_count() - 1;
                current_nfa.set_transition(final_state,final_state,token);
                result_NFAs.push_back(current_nfa);
            }
        }

    }
    NFA result = or_selection(result_NFAs, result_NFAs.size(), true);
    return result;
}

vector<NFA::trans> NFA::get_transitions() {
	return transitions;
}

// returns transitions table
vector<vector<string>> NFA::get_results(NFA result_nfa) {
    int rows_no = result_nfa.get_state_count() + 1;
    vector<vector<string>> result(rows_no,vector<string>(128));
    vector<pair<string,string>> tokens;

    for (int i = 0; i < rows_no; i++)
    {
        for (int j = 0; j < 128; j++)
        {
            result.at(i).at(j) = "0";
        }
    }
    vector <trans> transitions = result_nfa.get_transitions();
    for (trans current_transition : transitions) {
        string value = current_transition.trans_symbol;
        if (value.at(0) >= 128 || current_transition.from +1 >= rows_no) {
            // cout << "out of range now !!!" << endl;
            break;
        }
        if (current_transition.from +1 == current_transition.to+1) {
            pair<string,string> current_token (to_string(current_transition.to+1), value);
            tokens.push_back(current_token);
            continue;
        }
        if (value.size() > 1) {
            if (result.at(current_transition.from +1).at(0) == "0") {
                result.at(current_transition.from +1).at(0) = to_string(current_transition.to+1);
            } else {
                result.at(current_transition.from +1).at(0) += ",";
                result.at(current_transition.from +1).at(0) += to_string(current_transition.to+1);
            }

        } else {
            char c = value.at(0);
            if (result.at(current_transition.from +1).at(c) == "0") {
                result.at(current_transition.from +1).at(c) = to_string(current_transition.to+1);
            } else {
                result.at(current_transition.from +1).at(c) += ",";
                result.at(current_transition.from +1).at(c) += to_string(current_transition.to+1);
            }
        }
    }

    set_tokens(tokens);
    return result;
}

void NFA::display_tokens() {
    vector<pair<string,string>> tokens = get_tokens();
    for (int i = 0; i < tokens.size(); i++) {
        cout << "final state: " << tokens.at(i).first << " token: " << tokens.at(i).second << endl;
    }
}

string NFA::adjustString(string input) {
    stack<int> pars;
    stack<int> ors;
    int counter = 0;
    bool firstOr = true;
    string result = "";
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '|') {
            if (firstOr && counter == 0) {
                firstOr = false;
            }
            if (counter == 0) {
                result += ')';
                if (ors.size() != 0) {
                    ors.pop();
                }
            }
            ors.push(i);
            result += '|';
            result += '(';
        } else if (input[i] == '(') {
            if (i != 0) {
                if (input[i-1] == '\\') {
                    result += input[i];
                    continue;
                }
            }
            pars.push(i);
            counter++;
        } else if (input[i] == ')') {
            counter--;
            if (ors.size() == 0) {
                result += input[i];
                continue;
            }
            int orPos = ors.top();
            int parsPos = pars.top();
            if (orPos < parsPos) {
                pars.pop();
            } else {
                result += ')';
                ors.pop();
            }
        }

        if (input[i] != '|') {
            result += input[i];
        }
    }

    while(ors.size() != 0) {
        result += ')';
        ors.pop();
    }

    if (firstOr) {
        return "(" + result + ")";
    } else {
        return "((" + result + ")";
    }
}

void NFA::reverseStr(string& str) {
    int n = str.length();

    // Swap character starting from two
    // corners
    for (int i = 0; i < n / 2; i++)
        swap(str[i], str[n - i - 1]);
}

string NFA::adjustStringReversed(string input) {
    reverseStr(input);
    stack<int> pars;
    stack<int> ors;
    int counter = 0;
    bool firstOr = true;
    string result = "";
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '|') {
            if (firstOr && counter == 0) {
                firstOr = false;
            }
            if (counter == 0) {
                result += '(';
                if (ors.size() != 0) {
                    ors.pop();
                }
            }
            ors.push(i);
            result += '|';
            result += ')';
        } else if (input[i] == ')') {
            if (i != 0) {
                if (input[i-1] == '\\') {
                    result += input[i];
                    continue;
                }
            }
            pars.push(i);
            counter++;
        } else if (input[i] == '(') {
            counter--;
            if (ors.size() == 0) {
                result += input[i];
                continue;
            }
            int orPos = ors.top();
            int parsPos = pars.top();
            if (orPos > parsPos) {
                pars.pop();
            } else {
                result += '(';
                ors.pop();
            }
        }

        if (input[i] != '|') {
            result += input[i];
        }
    }

    while(ors.size() != 0) {
        result += '(';
        ors.pop();
    }

    if (firstOr) {
        reverseStr(result);
    } else {
        result = ")" + result;
        reverseStr(result);
    }
    return result;
}

int NFA::get_state_count() {
    return state.size();
}

void NFA::set_state(int no_state) {
    for (int i = 0; i < no_state; i++) {
        state.push_back(i);
    }
}

void NFA::set_transition(int from, int to, string trans_symbol) {
    trans new_trans;
    new_trans.from = from;
    new_trans.to = to;
    if ((int) trans_symbol[0] == -66 ) {
    	trans_symbol = ";";
    }
    new_trans.trans_symbol = trans_symbol;
    transitions.push_back(new_trans);
}

void NFA::set_final_state(int fs) {
    final_state = fs;
}

string NFA::get_final_state() {
    return to_string(final_state+1);
}

int NFA::get_initial_state() {
    return 0;
}

void NFA::set_tokens (vector<pair<string,string>> tokens) {
    result_tokens = tokens;
}

vector<pair<string,string>> NFA::get_tokens() {
    return result_tokens;
}

void NFA::display() {
    cout<<"transitions size: "<<transitions.size()<<endl;
    trans new_trans;
    cout << "\n";
    for (int i = 0; i < transitions.size(); i++) {
        new_trans = transitions.at(i);
        cout << "q" << new_trans.from << " --> q" << new_trans.to << " : Symbol - "
             << new_trans.trans_symbol << " " << (int) new_trans.trans_symbol[0] << endl;
    }
    cout << "\nThe final state is q" << get_final_state() << endl;
}


// concatenate two nfas and return result nfa
NFA NFA::concat(NFA a, NFA b) {
    NFA result;
    result.set_state(a.get_state_count() + b.get_state_count() - 1);
    int i;
    trans new_trans;

    for (i = 0; i < a.transitions.size(); i++) {
        new_trans = a.transitions.at(i);
        result.set_transition(new_trans.from, new_trans.to, new_trans.trans_symbol);
    }

    //result.set_transition(a.get_final_state(), a.get_state_count(), "^");

    int n = a.get_state_count();
    for (i = 0; i < b.transitions.size(); i++) {
        new_trans = b.transitions.at(i);
        result.set_transition(new_trans.from + n - 1, new_trans.to + n - 1, new_trans.trans_symbol);
    }

    result.set_final_state(result.get_state_count() - 1);

    return result;
}

// return kleene of given nfa
NFA NFA::kleene(NFA a) {
    NFA result;
    int i;
    trans new_trans;

    // add new start and end states
    result.set_state(a.get_state_count() + 2);
    // initial state
    result.set_transition(0, 1, "\\L");

    // copy original states but increment old states no. by 1
    for (i = 0; i < a.transitions.size(); i++) {
        new_trans = a.transitions.at(i);
        result.set_transition(new_trans.from + 1, new_trans.to + 1, new_trans.trans_symbol);
    }
    // final state
    result.set_transition(a.get_state_count(), a.get_state_count() + 1, "\\L");
    result.set_transition(a.get_state_count(), 1, "\\L");
    result.set_transition(0, a.get_state_count() + 1, "\\L");

    result.set_final_state(a.get_state_count() + 1);

    return result;
}

// take multiple nfas and return one nfa which is the or selection of the given nfas
NFA NFA::or_selection(vector <NFA> selections, int no_of_selections, bool combine) {
	// cout << no_of_selections << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
	NFA result;
	int state_count = 2;
	if (combine)
		state_count = 1;
	int i, j;
	NFA med;
	trans new_trans;

	for (i = 0; i < no_of_selections; i++) {
		state_count += selections.at(i).get_state_count();
	}

	result.set_state(state_count);

	int adder_track = 1;

	for (i = 0; i < no_of_selections; i++) {
		result.set_transition(0, adder_track, "\\L");
		med = selections.at(i);
		for (j = 0; j < med.transitions.size(); j++) {
			new_trans = med.transitions.at(j);
			result.set_transition(new_trans.from + adder_track, new_trans.to + adder_track,
								  new_trans.trans_symbol);
		}
		adder_track += med.get_state_count();
		if (!combine)
			result.set_transition(adder_track - 1, state_count - 1, "\\L");
	}

	result.set_final_state(state_count - 1);

	return result;
}

// produce nfa for a given single string
NFA NFA::re_to_nfa(string re) {
    stack<char> operators;
    stack <NFA> operands;
    char op_sym;
    int op_count;
    char cur_sym;
    NFA *new_sym;
    bool back_slash = false;

    for (string::iterator it = re.begin(); it != re.end(); ++it) {
        cur_sym = *it;
        if (!back_slash && cur_sym == '\\') {
            back_slash = true;
            continue;
        }
        if (back_slash) {
            back_slash = false;
            new_sym = new NFA();
            new_sym->set_state(2);
            if (cur_sym == 'L') {
                new_sym->set_transition(0, 1, "\\L");
            } else {
                new_sym->set_transition(0, 1, string(1, cur_sym));
            }
            new_sym->set_final_state(1);
            operands.push(*new_sym);
            delete new_sym;
            continue;
        }

        if (cur_sym != '(' && cur_sym != ')' && cur_sym != '*' && cur_sym != '+' && cur_sym != '|' && cur_sym != ' ') {
            new_sym = new NFA();
            new_sym->set_state(2);
            new_sym->set_transition(0, 1, string(1, cur_sym));
            new_sym->set_final_state(1);
            operands.push(*new_sym);
            delete new_sym;
        } else {
            if (cur_sym == '*') {
                NFA star_sym = operands.top();
                operands.pop();
                operands.push(kleene(star_sym));
            } else if (cur_sym == '+') {
                NFA plus_sym = operands.top();
                operands.pop();
                operands.push(concat(plus_sym, kleene(plus_sym)));
            }else if (cur_sym == ' ') {
                operators.push(cur_sym);
            } else if (cur_sym == '|') {
                operators.push(cur_sym);
            } else if (cur_sym == '(') {
                operators.push(cur_sym);
            } else {
                op_count = 0;
                op_sym = operators.top();
                if (op_sym == '(') {
                    operators.pop();
                    continue;
                }
                do {
                    operators.pop();
                    op_count++;
                } while (operators.top() != '(');
                operators.pop();
                NFA op1;
                NFA op2;
                vector <NFA> selections;
                if (op_sym == ' ') {
                    for (int i = 0; i < op_count; i++) {
                        op2 = operands.top();
                        operands.pop();
                        op1 = operands.top();
                        operands.pop();
                        operands.push(concat(op1, op2));
                    }
                } else if (op_sym == '|') {
                    selections.assign(op_count + 1, NFA());
                    int tracker = op_count;
                    for (int i = 0; i < op_count + 1; i++) {
                        if (operands.size() == 0) break;
                        selections.at(tracker) = operands.top();
                        tracker--;
                        operands.pop();
                    }
                    operands.push(or_selection(selections, op_count + 1, false));
                }
            }
        }
    }
    return operands.top();
}

NFA NFA::keyword_to_nfa(string keyword) {
    int n = keyword.length();
    string result = "(";
    for (int i = 0; i < n; i++) {
        result += keyword.at(i);
        if (i != n - 1)
            result += ' ';
    }
    result += ')';
    // cout << result << endl;
    return re_to_nfa(result);
}

// substitute by interval value ( as 0-2 -> 0|1|2 )
string NFA::interval_substitute(string interval) {
    int n = interval.length();
    char char_array[n + 1];
    strcpy(char_array, interval.c_str());

    string result = "";
    for (int i = char_array[0]; i <= char_array[2]; i++) {
        result += i;
        if (i != char_array[2])
            result += '|';
    }
    return result;
}

void NFA::replaceAll(string& str, const string& from, const string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        // In case 'to' contains 'from', like replacing 'x' with 'yx'
        start_pos += to.length();
    }
}

string NFA::adjust_re(string re) {
    string s = re;
    replaceAll(s, " | ", "|");
    replaceAll(s, " |", "|");
    replaceAll(s, "| ", "|");
    replaceAll(s, " - ", "-");
    replaceAll(s, "\\", " \\");
    regex g("\\(\\s");
    smatch match;
    if (regex_search(s, match, g)) {
        replaceAll(s,"\\( ","\\(  ");
    }
    replaceAll(s, "( ", "(");
    replaceAll(s, " )", ")");
    //replaceAll(s, "\\", "\\\\");

    regex r("[a-z]-[a-z]|[0-9]-[0-9]|[A-Z]-[A-Z]");
    string subject = s;
    set<string> intervals;

    int i = 1;
    while (regex_search(subject, match, r)) {
        intervals.insert(match.str(0));
        // cout << "\nMatched string is " << match.str(0) << endl;
        i++;
        // suffix to find the rest of the string.
        subject = match.suffix().str();
    }

    //regex r("[]\\[]");

    // replace interval by its value
    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        string current_interval = *it;
        // cout << current_interval << endl;
        replaceAll(s, current_interval, interval_substitute(current_interval));
    }

    // cout << s << endl;
    return s;
}
