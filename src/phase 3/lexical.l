%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "y.tab.h"  // generated via yacc -d

	int lineCounter = 1;

	extern int printLineNo(int n);
%}

letter [a-zA-Z]
digit [0-9]
id {letter}({letter}|{digit})*
digits {digit}+
num "-"?{digits}
fNum "-"?{digits}.{digits}("E"{digits})?
relop "=="|"!="|">"|">="|"<"|"<="
boolop "&&"|"||"
op "+"|"-"|"*"|"/"|"%"|"&"|"|"
boolVal "true"|"false"

%%

"int"	{return INT_SYM;}
"float"	{return FLOAT_SYM;}
"boolean"	{return BOOLEAN_SYM;}
"if"	{return IF_SYM;}
"else"	{return ELSE_SYM;}
"while"	{return WHILE_SYM;}
"for"	{return FOR_SYM;}

";" {return SEMI_COLON;}
"(" {return LEFT_BRACKET;}
")" {return RIGHT_BRACKET;}
"{" {return LEFT_CURLY_BRACES;}
"}" {return RIGHT_CURLY_BRACES;}
"=" {return EQUAL;}
"System.out.println"	{return SYSTEM_OUT;}

\n {lineCounter++; printLineNo(lineCounter);}

{op}	{yylval.aopval = strdup(yytext); return ARITH_OP;}
{num}	{yylval.ival = atoi(yytext); return INT;}
{fNum}	{yylval.fval = atof(yytext); return FLOAT;}
{relop} {yylval.aopval = strdup(yytext); return RELA_OP;}
{boolop} {yylval.aopval = strdup(yytext); return BOOL_OP;}
{boolVal} {if(!strcmp(yytext,"true")){ yylval.bval = 1;} else { yylval.bval = 0;} return BOOLEAN;}
{id}	{yylval.idval = strdup(yytext);return IDENTIFIER;}

%%

int yywrap() {
   // handle IOF
   return -1;
}