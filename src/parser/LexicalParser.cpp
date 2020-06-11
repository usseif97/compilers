#include "header/First.h"
#include "header/LL1Grammar.h"
#include "header/ParsingTable.h"
#include "header/LexicalParser.h"
#include "header/Follow.h"

LexicalParser::LexicalParser(simulation *lexicalAnalyzer) :lexicalAnalyzer(lexicalAnalyzer) {
	// TODO Auto-generated constructor stub
	LL1Grammar ll1Grammar;
	ll1Grammar.generate_LL1_grammar();
	vector<string> all_lhs = ll1Grammar.get_all_lhs();
    multimap<string, string> prod_multimap = ll1Grammar.get_prod_multimap();
    cout << "el grammar "<<endl;
    cout << endl;
    ll1Grammar.print_disassembled_productions();

    cout << "el multimap "<<endl;
    cout << endl;
    //ll1Grammar.print_prod_multimap();


    First first;
    first.set_non_terminals_vector(all_lhs);
    first.set_multimap(prod_multimap);
    map<string, set<string>> all_first = first.get_all_first();


    first.print_all_first_map();


    Follow follow;
    follow.setProduction(prod_multimap);
    follow.setProductionOrg(all_lhs);
    follow.setFirstSets(all_first);
    follow.run();
    map<string, set<string>> all_follow = follow.getFollowSets();
    cout << endl;
    cout << endl;



    ParsingTable parser(this->lexicalAnalyzer, prod_multimap, all_lhs.at(0), all_first, all_follow);

}
LexicalParser::~LexicalParser() {
	// TODO Auto-generated destructor stub
}

