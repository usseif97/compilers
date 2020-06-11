#ifndef PARSER_LEXICALPARSER_H_
#define PARSER_LEXICALPARSER_H_

#include "../../lexical/header/simulation.h"


class LexicalParser {
public:
	simulation *lexicalAnalyzer;
	LexicalParser(simulation *
			lexicalAnalyzer);
	virtual ~LexicalParser();
};

#endif /* PARSER_LEXICALPARSER_H_ */
