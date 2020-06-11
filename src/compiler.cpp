#include <iostream>
#include "lexical/header/lexicalGenerator.h"
#include "parser/header/LexicalParser.h"

using namespace std;

int main() {
	// phase 1
	lexicalGenerator lex;

	// phase 2
	LexicalParser parser(lex.lexicalAnalyzer);

	return 0;
}
