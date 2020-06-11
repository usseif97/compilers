#include "header/DFA.h"
#include "header/DFAMinimization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

#define ROW_MAX_LEN 500
#define COLOUMN_MAX_LEN 128

DFAMinimization mDFA;


DFA::DFA() {
	// std::cout<<"DFA Constructor ..."<<std::endl;
}

DFA::~DFA() {
}

void DFA::readParameters(string start ,string final ,vector< vector<string> > table,
											vector<pair<string, string>> finals){
	intialState = start;
	finalState = final;
	finalsTokens = finals;

	for (int i = 0; i < stoi(finalState) + 1; i++) {
		vector<string> temp;
		for (int j = 0; j < COLOUMN_MAX_LEN; j++) {
			temp.push_back("0");
		}
		DFA_TranstionTable.push_back(temp);
	}

	for (int i = 0; i < stoi(finalState) + 1; i++) {
		vector<string> temp;
		for (int j = 0; j < COLOUMN_MAX_LEN; j++) {
			temp.push_back("0");
		}
		NFA_TranstionTable.push_back(temp);
	}



	for (int i = 0; i < stoi(finalState) + 1; i++) {
		vector<string> temp = table.at(i);
		for (int j = 0; j < COLOUMN_MAX_LEN; j++){
			NFA_TranstionTable.at(i).at(j) = temp.at(j);
			/*if (NFA_TranstionTable.at(i).at(j) == "0"){
				continue;
			}*/
			//// std::cout<<"NFA"<<"["<<i<<"]"<<"["<<j<<"]: "<<NFA_TranstionTable[i][j]<<std::endl;

		}
	}


	//NFA_TranstionTable.at(428).at(0) = "0"; ////


	//// std::cout<<"NFA_TranstionTable[433][33]: "<<NFA_TranstionTable.at(433).at(33)<<std::endl;
	//// std::cout<<"NFA_TranstionTable[12][100]: "<<NFA_TranstionTable.at(1).at(100)<<std::endl;

	stringstream conv(finalState);
	conv >> statesCount;
	// std::cout<<"statesCount: "<<statesCount<<std::endl;

}

void DFA::convert(){
	// std::cout<<"** Convert **"<<std::endl;

	/* Get the Epsilon Closure for Each State */
	for (int i = 1; i <= statesCount; i++){
		epsilonClosure(i);
	}

	// std::cout<<"intialState: "<<intialState<<std::endl;

	std::string newIntial = "";
	//vector<std::string> intialEpsilonClosure = dfa.firstEpsilonClosure(intialState);
	vector<std::string> intialEpsilonClosure = closureStates[0].second;
    sort(intialEpsilonClosure.begin(), intialEpsilonClosure.end());




	for (int j = 0; j < intialEpsilonClosure.size(); j++){
		newIntial = newIntial + intialEpsilonClosure[j] + ",";
	}
	string newIntialTemp = newIntial.substr(0, newIntial.size()-1);


	intialState = newIntialTemp; //The New Intial State

	transition(intialEpsilonClosure);
	numberOfStates++;

	// std::cout<<" "<<std::endl;
	// std::cout<<"****** THE NEWSTATES ******"<<std::endl;
	int tempRow = tempDFA_row;
	// std::cout<<"tempRow: "<<tempRow<<std::endl;
	// std::cout<<"*>numberOfStatesTemp: "<<numberOfStatesTemp<<std::endl;

	while (numberOfStatesTemp > 0){
		//// std::cout<<" "<<std::endl;
		//// std::cout<<"tempRow: "<<tempRow<<std::endl;
		//// std::cout<<"The State: "<<markedStates[tempRow-1]<<std::endl;
		//// std::cout<<" "<<std::endl;


		if (markedStates[tempRow-1].empty()){
			//// std::cout<<"Break"<<std::endl;
			tempRow--;
			break;
		}

		for (int i = 1; i < 128; i++){
			if (DFA_TranstionTable.at(tempRow).at(i) == "0"){
				continue;
			}
			//// std::cout<<"tempState: "<<DFA_TranstionTable[tempRow][i]<<std::endl;

			std::string tempState = DFA_TranstionTable[tempRow][i];

			if (!(std::find(markedStates.begin(), markedStates.end(), tempState) != markedStates.end())){
				//// std::cout << "Element Not Found(Convert Function)"<<std::endl;
				numberOfStates++;
				vector <std::string> tempStateArray = conv_string_to_vector(tempState, ',');
				transition(tempStateArray);
				numberOfStatesTemp--;
				//// std::cout << "*>numberOfStatesTemp(Decrease): "<<numberOfStatesTemp<<std::endl;
			} else {
				//// std::cout << "Element Found(Convert Function)"<<std::endl;
				numberOfStatesTemp--;
				//// std::cout << "*>numberOfStatesTemp(Decrease): "<<numberOfStatesTemp<<std::endl;
			}

		}
		tempRow++;
		//numberOfStatesTemp--;
		//// std::cout << "*>numberOfStatesTemp(Decrease): "<<numberOfStatesTemp<<std::endl;
	}

	//// std::cout << "*OUT OF WHILE LOOP*"<<std::endl;
	//// std::cout << ">>tempRow: "<<tempRow<<std::endl;
	//// std::cout << "No.Of States: "<<markedStates.size()<<std::endl;


	//displayDFA_TransitionTAble();

	for (int h = 0; h < markedStates.size(); h++){
		for (int t = 0; t < finalsTokens.size(); t++){
			if (isBelong(finalsTokens[t].first, markedStates[h])){
				newfinalsTokens.push_back(make_pair(markedStates[h], finalsTokens[t].second));
				break;
			}
		}
	}

	// std::cout<<" "<<std::endl;
	// std::cout<<"*** THE MINIMIZATION ***"<<std::endl;
	//// std::cout<<"intialState: "<<intialState<<std::endl;
	//// std::cout<<"finalState: "<<finalState<<std::endl;
	mDFA.minimize(intialState, finalState, DFA_TranstionTable, markedStates, newfinalsTokens);//Minimization Call
	mDFATable = mDFA.getMinimizedDFATable();
	finalStatesWIthTokens = mDFA.getFinalStatesWithToken();
	intialStaaate = mDFA.getInitState();
	mapFinalsWithTokens = mDFA.getMappedFinalStatesWithToken();



}

void DFA::epsilonClosure(int i){
	//// std::cout<<" "<<std::endl;
	//// std::cout<<"**epsilonClosure Function**"<<std::endl;
	//// std::cout<<"CurrentState: "<<i<<std::endl;


	/* Convert int to String */
	ostringstream str1;
    str1 << i;
    string firstState = str1.str();

    stack <std::string> stack;
    pair<std::string, vector <std::string> > closure;

    closure.first = firstState;
    closure.second.push_back(firstState);

	stack.push(firstState);

	while(!stack.empty()){
		string currentState = stack.top();
		stack.pop();

		stringstream convert(currentState);
		int currentStateIndex;
		convert >> currentStateIndex;

		if(NFA_TranstionTable.at(currentStateIndex).at(0) != "0"){
			std::string nextState = NFA_TranstionTable.at(currentStateIndex).at(0);

			vector <std::string> nextStates = conv_string_to_vector(nextState, ',');

			for (int t = 0; t < nextStates.size(); t++){

			    closure.second.push_back(nextStates[t]);
				//// std::cout<<"charNextState: "<<nextStates[t]<<std::endl;
				stack.push(nextStates[t]);

			}
		}
	}
	remove(closure.second);
	/*for (int i = 0; i < closure.second.size(); i++){
		// std::cout<<"EpsilonClosure: "<<closure.second[i]<<std::endl;
	}*/
	closureStates.push_back(closure);
}

void DFA::transition(vector <std::string> set){

	//// std::cout<<" "<<std::endl;
	//// std::cout<<"Transition Function: "<<std::endl;
	/* Get the Transitions of each state in the Set */
	sort(set.begin(), set.end());
	/*for (int i = 0; i < set.size(); i++){
		// std::cout<<"set After Sortng: "<<set[i]<<std::endl;
	}*/
	std::string tempS = "";

	int f1 = 0;
	string st;
	for (int n = 0; n < set.size(); n++){
		f1 = 1;
		tempS = tempS + set[n] + ",";
	}
	if (f1 == 1){
		st = tempS.substr(0, tempS.size()-1);
	}

	//// std::cout<<">>"<<"Pushed in The MarkedStates: "<<st <<std::endl;
	markedStates.push_back(st);

	tempDFA_row++;
	for (int i = 1; i < 128; i++){
		vector <std::string> resultWithoutClosure;

		sort(set.begin(), set.end());

		for (int j = 0; j < set.size(); j++){

			string tempStat = set[j];

			stringstream convert(set[j]);
			int tempSymbolIndex;
			convert >> tempSymbolIndex;

			if (NFA_TranstionTable.at(tempSymbolIndex).at(i) != "0"){
				//// std::cout<<"TEMPSTATE: "<<tempStat<<std::endl;
				//// std::cout<<"NewSymbol: "<<i<<" (Ascii)<<"<<std::endl;
				//// std::cout<<"Value: "<<NFA_TranstionTable.at(tempSymbolIndex).at(i)<<std::endl;
				resultWithoutClosure.push_back(NFA_TranstionTable.at(tempSymbolIndex).at(i));
			} else {
				continue;
			}

			if (resultWithoutClosure.size() == 0){
				continue;
			}

			sort(resultWithoutClosure.begin(), resultWithoutClosure.end());
			std::string temp;
			int f2 = 0;
			string xy;
			for (int n = 0; n < resultWithoutClosure.size(); n++){
				f2 = 1;
				temp = temp + resultWithoutClosure[n] + ",";
			}
			if (f2 == 1){
				xy = temp.substr(0, temp.size()-1);
			}


			//// std::cout<<"resultWithoutClosure: "<<xy<<std::endl;
			std::string null = "0";
			if (xy == ""){
				DFA_TranstionTable.at(tempDFA_row).at(i) = null;
			} else {
				DFA_TranstionTable.at(tempDFA_row).at(i) = xy;
			}

			/* Union Closure */
			unionClosure(tempDFA_row, i);

			std::string checkExist = DFA_TranstionTable[tempDFA_row][i];
			//// std::cout << "checkExist: "<<checkExist<<std::endl;
			if(!(std::find(markedStates.begin(), markedStates.end(), checkExist) != markedStates.end())){
				//// std::cout << "Element Not Found"<<std::endl;
				numberOfStatesTemp++;
				//// std::cout << "*>numberOfStatesTemp(Increase): "<<numberOfStatesTemp<<std::endl;
				//// std::cout << "*>numberOfStatesTemp: "<<numberOfStatesTemp<<std::endl;
			} else {
				//// std::cout << "Element Found"<<std::endl;
			}
			//// std::cout<<" "<<std::endl;
		}
	}

}

/*void DFA::getNewStates(){
}*/

void DFA::unionClosure(int tempRow, int tempColumn){
	//// std::cout<<" "<<std::endl;
	//// std::cout<<"**unionClosure Function**"<<std::endl;

	std::string currentTransition = DFA_TranstionTable[tempRow][tempColumn];
	//// std::cout<<"currentTransition: "<<currentTransition<<std::endl;

	//std::string unionTrans;

	vector<std::string> currentTransitionArr = conv_string_to_vector(currentTransition, ',');
	currentTransition += ",";
	string currentTransitionTrim;

	for (int i = 0; i < currentTransitionArr.size(); i++){

		stringstream conv(currentTransitionArr[i]);
		int currentTransitionIndex;
		conv >> currentTransitionIndex;
		vector <std::string> stateClosure = closureStates[currentTransitionIndex - 1].second;

		int f = 0;
		for (int j = 0; j < stateClosure.size(); j++){
			f = 1;
			currentTransition = currentTransition + stateClosure[j] + ",";
		}
		if (f == 1){
			currentTransitionTrim = currentTransition.substr(0, currentTransition.size()-1);
		}
	}

	vector<std::string> unionArr = conv_string_to_vector(currentTransitionTrim, ',');
	//// std::cout<<"currentTransition: "<<currentTransitionTrim<<std::endl;
	remove(unionArr);
	sort(unionArr.begin(), unionArr.end());
	std::string str;

	for (int i = 0; i < unionArr.size(); i++){
		str = str + unionArr[i] + ",";
	}
	std::string strTrim = str.substr(0, str.size()-1);

	//// std::cout<<"Result With Closure: "<<strTrim<<std::endl;
	//// std::cout<<"*> Pushed in DFA_Table at Row: "<<tempRow<<std::endl;
	DFA_TranstionTable.at(tempRow).at(tempColumn) = strTrim;

}

std::string DFA::removeDuplicate(std::string str) {

	 // keeps track of visited characters
	 int counter = 0;
	 int i = 0;
	 int size = str.size();

	 // gets character value
	 int x;
	 // keeps track of length of resultant string
	 int length = 0;
	 while (i < size) {
		 x = str[i] - 97;
	     // check if Xth bit of counter is unset
	     if ((counter & (1 << x)) == 0) {
	    	 str[length] = 'a' + x;
	         // mark current character as visited
	         counter = counter | (1 << x);
	         length++;
	     }
	     i++;
	 }
	 return str.substr(0, length);
}

vector<std::string> DFA::conv_string_to_vector(std::string s, char ch) {
	vector<std::string> result;
	string temp = "";

	for (int i = 0; i < s.size(); i++){
		if (ch == s[i] && !(temp.empty())){
			result.push_back(temp);
			temp = "";
		} else {
			temp += s[i];
		}
	}
	result.push_back(temp);
    return result;
}

int DFA::isBelong(string s1, string s2) {

	vector<string> s2Arr = conv_string_to_vector(s2, ',');
	for (int i = 0; i < s2Arr.size(); i++){
		if (s2Arr[i] == s1){
			return 1;
		} else{
			continue;
		}
	}
	return 0;
}

void DFA::remove(std::vector<string> &v){
	auto end = v.end();
	for (auto it = v.begin(); it != end; ++it) {
		end = std::remove(it + 1, end, *it);
	}

	v.erase(end, v.end());
}

void DFA::displayDFA_TransitionTAble() {
	// std::cout<<"**DFA TRANSITION TABLE**"<<std::endl;

	for (int i = 0; i < markedStates.size()+1; i++){
		for (int j = 1; j < 128; j++){
			std::string x = DFA_TranstionTable[i][j];
			if (x == "0"){
				continue;
			} else {
				// std::cout<<"DFA"<<"["<<i<<"]"<<"["<<j<<"]: "<<DFA_TranstionTable[i][j]<<std::endl;
			}
		}
	}
	/*// std::cout<<"**THE STATES**"<<std::endl;
	for (int  i = 0; i < markedStates.size(); i++){
		// std::cout<<markedStates[i]<<std::endl;
	}*/
}


vector< vector<string> > DFA::getMinDFATable(){
	return mDFATable;
}

vector< pair<string, vector<string> > > DFA::getFinalStatesWiithToken(){
	return finalStatesWIthTokens;
}

string DFA::getinState(){
	return intialStaaate;
}

vector<pair<string, string>> DFA::getMappedFinalWithTokens(){
	return mapFinalsWithTokens;
}
