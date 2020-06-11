#include "header/DFAMinimization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;
#define ROW_MAX_LEN 500
#define COLOUMN_MAX_LEN 128

DFAMinimization minDFA;

DFAMinimization::DFAMinimization() {
	// TODO Auto-generated constructor stub

}

DFAMinimization::~DFAMinimization() {
	// TODO Auto-generated destructor stub
}

void DFAMinimization::minimize(string start ,string final ,vector< vector<string> > table
											,vector<string> st, vector<pair<string, string>> lastsWithTokens){


	initState = start;
	lastState = final;
	newMinfinalsTokens = lastsWithTokens;

	for (int i = 0; i < stoi(lastState) + 1; i++) {
		vector<string> temp;
		for (int j = 0; j < COLOUMN_MAX_LEN; j++) {
			temp.push_back("0");
		}
		DFA_Table.push_back(temp);
	}

	for (int i = 0; i < stoi(lastState) + 1; i++) {
		vector<string> temp = table.at(i);
		for (int j = 0; j < COLOUMN_MAX_LEN; j++){
			DFA_Table.at(i).at(j) = temp.at(j);
		}
	}

	for (int i = 0; i < stoi(lastState) + 1; i++) {
		vector<string> temp;
		for (int j = 0; j < COLOUMN_MAX_LEN; j++) {
			temp.push_back("0");
		}
		DFA_MinimizedTable.push_back(temp);
	}

	states = st;

	//mapping();  // Map The States, initialState, FinalState

	for (int i = 0; i < states.size(); i++){
		//// std::cout <<"states:" <<states[i]<<std::endl;
		int flag = 0;
		for (int j = 0; j < newMinfinalsTokens.size(); j++){
			if (states[i] == newMinfinalsTokens[j].first){
				flag = 1;
				tempStr2 = tempStr2 + states[i] + "/";
				break;
			} else {
				flag = 0;
				//tempStr1 = tempStr1 + states[i] + "/";
			}
		}
		if (flag == 0){
			tempStr1 = tempStr1 + states[i] + "/";
		}
	}

	c1 = tempStr1.substr(0, tempStr1.size()-1);
	c2 = tempStr2.substr(0, tempStr2.size()-1);


	tempClasses.push_back(c1);
	tempClasses.push_back(c2);



	minimizeTheStates();  // minimize the States

	minimizedTable();  // fill the MinDFA_Table

	display_MinimizedDFATable();  // display MinDFA_Table
	mapping();
	display_MinimizedDFATable();  // display MinDFA_Table
	finalsWithTokens();

}

void DFAMinimization::minimizeTheStates(){

	// std::cout <<" "<<std::endl;
	// std::cout <<"**THE Algorithm**"<<std::endl;
	int f = 0;
	while(true){
		// std::cout <<"New Iteration"<<std::endl;
		// std::cout <<" "<<std::endl;
		for (int j = 0; j < tempClasses.size(); j++){  // The Current Class
			/* Each Class */

			//std::string temp = tempClasses[j];
			vector<string> tempClass = conv_string_to_vector(tempClasses[j], '/');
			for (int t = 0; t < tempClass.size(); t++){
				// std::cout << "The Temp Class: "<<tempClass[t]<<" <<<<<<<<<<"<<std::endl;
			}

			vector< pair<string, int> > markedStates;
			for (int u = 0; u < tempClass.size(); u++){
				markedStates.push_back(make_pair(tempClass[u], 0)); // 0 notMarked
			}                                                  // 1 Marked

			for (int t = 0; t < tempClass.size(); t++){ // The Current State in The Class
				/* Contents of Each Class */

				string tempState1 = tempClass[t];
				int tempState1Index;
				// std::cout << "tempState1: "<<tempState1<<std::endl;

				int tempState1IndexInPair;
				for (int i = 0; i < markedStates.size(); i++){
					if (tempState1 == markedStates[i].first){
						tempState1IndexInPair = i;
					}
				}
				if (markedStates[tempState1IndexInPair].second == 1){
					continue;
				}

				string tempState2;
				int concatenateFlag = 0;
				for (int h = t+1; h < tempClass.size(); h++){
					tempState2 = tempClass[h];
					tempState1Index = getIndex(tempState1);
					int tempState2Index = getIndex(tempState2);
					// std::cout << "tempState2: "<<tempState2<<std::endl;
					// std::cout << "tempState2Index: "<<tempState2Index<<std::endl;

					int isbelong = checkBelongToTheSameClass(tempState1Index, tempState2Index,
															markedStates, tempState2);	// 0 not belong
					 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	   	// 1 belong
					// std::cout <<"isbelong: "<<isbelong<<std::endl;

					if (isbelong == 1){
						concatenateFlag = 1;
						tempState1 = tempState1 +"/"+ tempState2 ;
						// std::cout <<"Concatenated: "<<tempState1<<std::endl;
					}
				}
				string result;
				result = tempState1.substr(0, tempState1.size());
				// std::cout <<"result Pushed in The New Classes: "<<result<<std::endl;
				newClasses.push_back(result);
				// std::cout <<" "<<std::endl;
			}
		}
		// std::cout <<"*Finish the NewClasses*"<<std::endl;
		for (int r = 0; r < newClasses.size(); r++){
			// std::cout <<"newClasses: "<<newClasses[r]<<std::endl;
		}
		// std::cout <<"*Finish the TempClasses*"<<std::endl;
		for (int r = 0; r < tempClasses.size(); r++){
			// std::cout <<"newClasses: "<<tempClasses[r]<<std::endl;
		}

		if (isEqual(tempClasses, newClasses)){  // 0 not Equal
			break;                              // 1 Equal
			// std::cout <<"Equal"<<std::endl;
		} else {
			// std::cout <<"Not Equal"<<std::endl;
			tempClasses.clear();
			for (int r = 0; r < newClasses.size(); r++){
				tempClasses.push_back(newClasses[r]);
			}
			newClasses.clear();
		}
		// std::cout <<" "<<std::endl;
		f++;
	}

	for (int i = 0; i < newClasses.size(); i++){
		for (int j = 0; j < newMinfinalsTokens.size(); j++){
			if (isBelong(newMinfinalsTokens[j].first,newClasses[i])){
				finals.push_back(newClasses[i]);
				break;
			}
		}
		/*if (isBelong(lastState,newClasses[i])){
			finals.push_back(newClasses[i]);
		}*/
	}
	for (int i = 0; i < finals.size(); i++){
		// std::cout <<"finals: "<<finals[i]<<std::endl;
	}
	string tempInit = initState;
	for (int t = 0; t < newClasses.size(); t++){
		if (isBelong(tempInit,newClasses[t])){
			initState = newClasses[t];
		}
	}

	for (int y = 0; y < newClasses.size(); y++){
		for (int g = 0; g < newMinfinalsTokens.size(); g++){
			if (isBelongForFinals(newMinfinalsTokens[g].first, newClasses[y])){
				mappedNewMinfinalsTokens.push_back(make_pair(newClasses[y], newMinfinalsTokens[g].second));
				break;
			}
		}
	}




}

void DFAMinimization::mapping(){
	// std::cout << "**Mapping**"<<std::endl;
	// std::cout << " "<<std::endl;

	vector< vector<string> > tempTable;
	for (int i = 0; i < stoi(lastState) + 1; i++) {
		vector<string> temp;
		for (int j = 0; j < COLOUMN_MAX_LEN; j++) {
			temp.push_back("0");
		}
		tempTable.push_back(temp);
	}

	for (int i = 0; i < stoi(lastState) + 1; i++) {
		vector<string> temp = DFA_MinimizedTable.at(i);
		for (int j = 0; j < COLOUMN_MAX_LEN; j++){
			tempTable.at(i).at(j) = temp.at(j);
		}
	}

	for (int i = 0; i < newClasses.size(); i++){
		int x = i+1;
		ostringstream str;
		str << x;
		string mappedState = str.str();
		statesMapp.push_back(make_pair(newClasses[i], mappedState));
	}

	for (int t = 0; t < mappedNewMinfinalsTokens.size(); t++){
		for (int f = 0; f < statesMapp.size(); f++){
			if (mappedNewMinfinalsTokens[t].first == statesMapp[f].first){
				mappedNewMinfinalsTokens.at(t) = make_pair(statesMapp[f].second, mappedNewMinfinalsTokens[t].second);
			}
		}
	}

	ofstream myfile;
	myfile.open ("output.txt");
	myfile << "mappedNewMinfinalsTokens.sizessss: "<<mappedNewMinfinalsTokens.size()<< endl;
	for (int f = 0; f < mappedNewMinfinalsTokens.size(); f++){
		myfile << "newfinalsTokens.first: "<<mappedNewMinfinalsTokens[f].first<< endl;
		myfile << "newfinalsTokens.second: "<<mappedNewMinfinalsTokens[f].second<< endl;
		myfile << " "<< endl;
	}
	myfile.close();

	/*for (int i = 0; i < statesMapp.size(); i++){
		// std::cout <<"statesMapp.first: "<<statesMapp[i].first<<std::endl;
		// std::cout <<"statesMapp.second: "<<statesMapp[i].second<<std::endl;
	}*/

	for (int i = 1; i < newClasses.size()+1; i++){
		for (int j = 1; j < 128; j++){
			if (tempTable.at(i).at(j) == "0"){
				continue;
			} else {
				for (int u = 0; u < statesMapp.size(); u++){
					if (statesMapp[u].first == tempTable.at(i).at(j)){
						// std::cout <<"statesMapp[u].first: "<<statesMapp[u].first<<std::endl;
						// std::cout <<"statesMapp.second: "<<statesMapp[u].second<<std::endl;
						// std::cout <<"i: "<<i<<std::endl;
						// std::cout <<"j: "<<j<<std::endl;
						DFA_MinimizedTable.at(i).at(j) = statesMapp[u].second;
					}
				}
			}
		}
	}
	for (int i = 0; i < finalStates.size(); i++){
		string fState = finalStates.at(i).first;
		for (int j = 0; j < statesMapp.size(); j++){
			string oldState = statesMapp.at(j).first;
			if (fState == oldState){
				finalStates.at(i).first = statesMapp.at(j).second;
			}
		}
	}
	for (int t = 0; t < statesMapp.size(); t++){
		if (statesMapp[t].first == initState){
			initState = statesMapp[t].second;
		}
	}
}


int DFAMinimization::checkBelongToTheSameClass(int x, int y, vector< pair<string, int> >& marked, string yStr){

	int flag = 1;
	int flag2 = 0;
	int yStrIndexInPair;

	for (int k = 0; k < marked.size(); k++){
		if (yStr == marked[k].first){
			yStrIndexInPair = k;
		}
	}

	for (int i = 1; i < 128; i++){
		std::string xValue = DFA_Table[x][i];
		std::string yValue = DFA_Table[y][i];
		if (xValue != "0" && yValue != "0"){
			// std::cout << "xValue: "<<xValue<<std::endl;
			// std::cout << "yValue: "<<yValue<<std::endl;
			flag2 = 1;

			for (int j = 0; j < tempClasses.size(); j++){
				// std::cout << "tempClasses: "<<tempClasses[j]<<std::endl;
				//if ((tempClasses[j].find(xValue) != std::string::npos) &&
						//(tempClasses[j].find(yValue) != std::string::npos)){
				if ((isBelongForFinals(xValue, tempClasses[j])) &&
						(isBelongForFinals(yValue, tempClasses[j]))){
					flag = 1;
					break;
				} else {
					flag = 0;
				}
			}
			if (flag == 0){
				// std::cout << "Return Flag = 0"<<std::endl;
				return flag;
			}
		}
	}
	if (flag2 == 0 && yStr.find(lastState) != std::string::npos){
		marked[yStrIndexInPair].second = 1;
		return 1;
	}
	if (flag2 == 0){
		return flag2;
	}
	marked[yStrIndexInPair].second = 1;
	return 1;
}

void DFAMinimization::minimizedTable(){

	// std::cout <<" "<<std::endl;
	// std::cout <<"**Minimized Table Function**"<<std::endl;
	// std::cout <<" "<<std::endl;

	vector< pair<std::string , std::string> > classesMap;

	for (int i = 0; i < states.size(); i++){
		std::string state = states[i];
		//// std::cout <<"state: "<<states[i]<<std::endl;

		for (int j = 0; j < newClasses.size(); j++){
			std::string classState = newClasses[j];
			//// std::cout <<"classState: "<<classState<<std::endl;

			if (isBelong(state, classState)){
				//// std::cout <<"break"<<std::endl;
				classesMap.push_back(make_pair(state, classState));
				break;
			}
		}
	}

	//// std::cout <<"classesMap.size(): "<<classesMap.size()<<std::endl;

	/*for (int i = 0; i < classesMap.size(); i++){
		// std::cout <<"classesMap.first: "<<classesMap[i].first<<std::endl;
		// std::cout <<"classesMap.second: "<<classesMap[i].second<<std::endl;
		// std::cout <<" "<<std::endl;
	}*/




	for (int i = 0; i < newClasses.size(); i++){
		for (int j = 1; j < 128; j++){
			int f = 0;
			string newState = newClasses[i]; // New Class State
			string value;
			for (int u = 0; u < classesMap.size(); u++){
				if (newState == classesMap[u].second){
					std::string oldState = classesMap[u].first;
					value = DFA_Table[u+1][j];
					if (value == "0"){
						f = 1;
						DFA_MinimizedTable.at(i+1).at(j) = "0";
					}
					break;
				}
			}
			if (f == 1){
				continue;
			}
			string neew;
			for (int t = 0; t < classesMap.size(); t++){
				if (value == classesMap[t].first){
					neew = classesMap[t].second;
					break;
				}
			}
			/*string x;
			for (int r = 0; r < neew.size(); r++){
				x = x + neew[r] + ",";
			}
			string neewStr = x.substr(0, x.size()-1);*/
			DFA_MinimizedTable.at(i+1).at(j) = neew;
		}
	}



	/*for(int i = 0; i < newClasses.size(); i++){
		for (int j = 1; j < 128; j++){

			std::string newState = newClasses[i];
			//// std::cout << "newState: "<<newState<<std::endl;

			for (int u = 0; u < classesMap.size(); u++){
				if (newState == classesMap[u].second){
					string value = DFA_Table.at(u+1).at(j);
					if (value == "0"){
						DFA_MinimizedTable.at(i+1).at(j) = "0";
					} else {
						for (int f = 0; f < classesMap.size(); f++){
							if (value == classesMap[f].first){
								DFA_MinimizedTable.at(i+1).at(j) = classesMap[f].second;
							}
						}
					}
				}
			}
		}
	}*/
	for (int g = 0; g < finals.size(); g++){
		for (int h = 0; h < newClasses.size(); h++){
			for (int u = 1; u < 128; u++){
				string val = DFA_MinimizedTable.at(h+1).at(u);
				if (val == finals[g]){
					int token = u;
					ostringstream strToken;
					strToken << token;
					string tok = strToken.str();
					finalStates.push_back(make_pair(finals[g], tok));
				}
			}
		}
	}
}

void DFAMinimization::finalsWithTokens(){
	// std::cout <<" "<<std::endl;
	// std::cout <<"Finals With Tokens"<<std::endl;

	vector<int> marked;
	for (int i = 0; i < finalStates.size(); i++){
		marked.push_back(0);
	}

	for (int i = 0; i < finalStates.size(); i++){
		if (marked[i] == 1){
				continue;
		}
		marked[i] = 1;
		vector<string> tokens;
		tokens.push_back(finalStates.at(i).second);

		for (int j = i + 1; j < finalStates.size(); j++){
			if (finalStates.at(i).first == finalStates.at(j).first){
				marked[j] = 1;
				tokens.push_back(finalStates.at(j).second);
			}
		}
		remove(tokens);
		finalStatesWithTokens.push_back(make_pair(finalStates.at(i).first, tokens));
	}

	for (int i = 0; i < finalStatesWithTokens.size(); i++){
		// std::cout <<"finalState: "<<finalStatesWithTokens[i].first<<std::endl;
		for (int j = 0; j < finalStatesWithTokens[i].second.size(); j++){
			// std::cout <<"Tokens: "<<finalStatesWithTokens[i].second[j]<<std::endl;
		}
	}


}

void DFAMinimization::remove(std::vector<string> &v){
	auto end = v.end();
	for (auto it = v.begin(); it != end; ++it) {
		end = std::remove(it + 1, end, *it);
	}

	v.erase(end, v.end());
}

int DFAMinimization::isBelong(std::string s1, std::string s2) {

	//vector<string> arr = conv_string_to_vector(s2, ' ');
	std::replace( s2.begin(), s2.end(), '/', ','); // replace all '/' to ','

	vector<string> s2Arr = conv_string_to_vector(s2, ',');
	vector<string> s1Arr = conv_string_to_vector(s1, ',');
	for (int i = 0; i < s1Arr.size(); i++){
		if (std::count(s2Arr.begin(), s2Arr.end(), s1Arr[i])){
			// std::cout << "Element found"<<std::endl;
		} else{
			// std::cout << "Element not found"<<std::endl;
			return 0;
		}

	}

	return 1;
}

int DFAMinimization::isBelongForFinals(string s1, string s2) {

	vector<string> s2Arr = conv_string_to_vector(s2, '/');
	for (int i = 0; i < s2Arr.size(); i++){
		if (s2Arr[i] == s1){
			return 1;
		} else{
			continue;
		}
	}
	return 0;
}

int DFAMinimization::isSubstring(std::string s1, std::string s2) {

    int M = s1.length();
    int N = s2.length();

    /* A loop to slide pat[] one by one */
    for (int i = 0; i < N; i++){
		std::string s(1, s2[i]);
		char* c1 = const_cast<char*>(s.c_str());
    	for (int j = 0; j < M; j++){
    		std::string ss(1, s1[j]);
    		char* c2 = const_cast<char*>(ss.c_str());
    		if (!strcasecmp(c1, c2)){
    			return 1;
    		}
    	}
    }
    return 0;
}

int DFAMinimization::getIndex(string state){


	for (int i = 0; i < states.size(); i++){
		if(state == states[i]){
			return i+1;
		}
	}
}

int DFAMinimization::isEqual(vector<std::string> v1 ,vector<std::string> v2){

	int flag = 1;
	if (v1.size() == v2.size()){
	    for (int i = 0; i < v1.size(); i++){
	    	//Convert String to Char*
	    	char* v1Char = const_cast<char*>(v1[i].c_str());
	    	char* v2Char = const_cast<char*>(v2[i].c_str());
	    	if ( strcmp(v1Char, v2Char)){
	    		flag = 0;
	    		return flag;
	    	}

	    }
	    if (flag != 0){
	    	return flag;
	    }
	} else {
		return 0;
	}
}

vector<std::string> DFAMinimization::conv_string_to_vector(std::string s, char ch) {
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

void DFAMinimization::display_MinimizedDFATable(){
	ofstream outputFile;
	outputFile.open("transitionTable.txt");

	//std::cout << " "<<std::endl;
	//std::cout<<"**MINIMIZED DFA TRANSITION TABLE**"<<std::endl;
	outputFile<<"**MINIMIZED DFA TRANSITION TABLE**"<<std::endl;
	//std::string fState = lastState;
	//std::cout <<"No.Of States: "<<newClasses.size()<<std::endl;
	outputFile<<"No.Of States: "<<newClasses.size()<<std::endl;


	for (int i = 0; i < newClasses.size()+1; i++){
		for (int j = 1; j < 128; j++){
			if (DFA_MinimizedTable[i][j] == "0"){
				continue;
			} else {
				// std::cout<<"DFA_Minimized"<<"["<<i<<"]"<<"["<<j<<"]: "<<DFA_MinimizedTable[i][j]<<std::endl;
				outputFile<<"from "<<i<<" "<<" on getting "<<j<<" to "<<DFA_MinimizedTable[i][j]<<std::endl;
			}
		}
	}

	outputFile.close();

	for (int i = 0; i < finalStates.size(); i++){
		//std::cout <<"finalStates: "<<finalStates[i].first<<std::endl;
		//std::cout <<"finalStates: "<<finalStates[i].second<<std::endl;
	}
}


vector< vector<string> > DFAMinimization::getMinimizedDFATable(){
	return DFA_MinimizedTable;
}

vector< pair<string, vector<string> > > DFAMinimization::getFinalStatesWithToken(){
	return finalStatesWithTokens;
}

string DFAMinimization::getInitState(){
	return initState;
}

vector<pair<string, string>> DFAMinimization::getMappedFinalStatesWithToken(){
	return mappedNewMinfinalsTokens;
}
