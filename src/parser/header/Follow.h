#ifndef PARSER_FOLLOW_H_
#define PARSER_FOLLOW_H_

#include <map>
#include <set>
#include <vector>

using namespace std;

class Follow {
public:
	Follow();
	virtual ~Follow();
	void computeFollow(vector<pair<bool, string>>, string);
	void setProduction(multimap<string, string>);
	void setProductionOrg(vector<string> prod);
	void setFirstSets(map<string, set<string>>);
	map<string, set<string>> getFollowSets();
	void displayFollow();
	void run();

private:
	map<string, set<string>> followSets;
	multimap<string, string> productions;
	vector<string> productionsOrg;
	map<string, set<string>> firstSets;
};

#endif /* PARSER_FOLLOW_H_ */
