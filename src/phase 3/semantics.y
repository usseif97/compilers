%{
	#include <stdio.h>
	#include <unistd.h>
	#include <fstream>
	#include <iostream>
	#include <map>
	#include <vector>

	using namespace std;

	#define GetCurrentDir getcwd

	// yacc usage
	extern  int yylex();
	extern  FILE *yyin;
	extern int lineCounter;
	void yyerror(const char * s);

	// output file
	ofstream fout("output.j");

	// default header and footer generators of the bytecode
	void generateHeader(void);
	void generateFooter(void);

	// variables and labels counters
	int variablesCount = 1;	//java starts with 1, 0 is 'this'
	int labelsCount = 0;
	string fileName;

	typedef enum {INTEGER_TYPE, FLOAT_TYPE, BOOLEAN_TYPE, VOID_TYPE, ERROR_TYPE} types;

    map<string, pair<int,types> > symbTab;
	//  java bytecode instruction listings
	map<string,string> byte_code_map = {
		{"+", "add"},
		{"-", "sub"},
		{"/", "div"},
		{"*", "mul"},
		{"|", "or"},
		{"&", "and"},
		{"%", "rem"},
		{"==", "if_icmpeq"},
		{"<=", "if_icmple"},
		{">=", "if_icmpge"},
		{"!=", "if_icmpne"},
		{">", "if_icmpgt"},
		{"<", "if_icmplt"}
	};

    void checkCast(int from , int to, string op);
    bool checkOp(string id);
    string getOp(string op);
    void defineVar(string name, int type);
    string generateLabel();
    string getLabel(int n);
    void backPatch(vector<int> *list, int num);
    vector<int> * merge (vector<int> *list1, vector<int>* list2);
    vector<string> bytecodeList;
    void writeCode(string x);
    void printCode(void);
    void printLineNo(int n);	// used in lexical.l

%}

%code requires {
	#include <vector>
	using namespace std;
}

// specifying starting symbol (production)
// used to generate header & footer
%start method_body

// every possible type of terminals and non terminals
%union{
	int ival;
	float fval;
	int bval;
	char * idval;
	char * aopval;
	struct {
		int sType;
	} expr_type;
	struct {
		vector<int> *trueList, *falseList;
	} bexpr_type;
	struct {
		vector<int> *nextList;
	} stmt_type;
	int sType;
}

// tokens specified in lexical.l
// data types tokens
%token <ival> INT
%token <fval> FLOAT
%token <bval> BOOLEAN 	// true or false words
%token <idval> IDENTIFIER
%token <aopval> ARITH_OP
%token <aopval> RELA_OP
%token <aopval> BOOL_OP
// symbols (words) tokens
%token IF_SYM
%token ELSE_SYM
%token WHILE_SYM
%token FOR_SYM
%token INT_SYM
%token FLOAT_SYM
%token BOOLEAN_SYM
%token SEMI_COLON
%token EQUAL
%token LEFT_BRACKET
%token RIGHT_BRACKET
%token LEFT_CURLY_BRACES
%token RIGHT_CURLY_BRACES
%token SYSTEM_OUT
// specifying types
%type <sType> primitive_type
%type <expr_type> expression
%type <bexpr_type> b_expression
%type <stmt_type> statement
%type <stmt_type> statement_list
%type <stmt_type> if
%type <stmt_type> while
%type <stmt_type> for
%type <ival> label
%type <ival> goto


%%

/*              java productions            */

method_body:
	{	generateHeader();	}
	statement_list
	label
	{   backPatch($2.nextList,$3);  generateFooter();   }
	;

statement_list:
	statement
	|
	statement
	label
	statement_list
	{   backPatch($1.nextList,$2);  $$.nextList = $3.nextList;  }
	;

label:
    {   $$ = labelsCount;   writeCode(generateLabel() + ":");    }
    ;

statement:
	declaration {vector<int> * v = new vector<int>(); $$.nextList =v;}
	|if {$$.nextList = $1.nextList;}
	|while 	{$$.nextList = $1.nextList;}
	|for {$$.nextList = $1.nextList;}
	| assignment {vector<int> * v = new vector<int>(); $$.nextList =v;}
	| system_print {vector<int> * v = new vector<int>(); $$.nextList =v;}
	;

declaration:
	primitive_type IDENTIFIER SEMI_COLON
	{   string str($2);
		if($1 == INTEGER_TYPE)
		{
			defineVar(str,INTEGER_TYPE);
		}else if ($1 == FLOAT_TYPE)
		{
			defineVar(str,FLOAT_TYPE);
		}
	}
	;

primitive_type:
	INT_SYM {$$ = INTEGER_TYPE;}
	| FLOAT_SYM {$$ = FLOAT_TYPE;}
	|BOOLEAN_SYM {$$ = BOOLEAN_TYPE;}
	;

goto:
    {   $$ = bytecodeList.size();   writeCode("goto "); }
    ;

if:
	IF_SYM LEFT_BRACKET
	b_expression
	RIGHT_BRACKET LEFT_CURLY_BRACES
	label
	statement_list
	goto
	RIGHT_CURLY_BRACES
	ELSE_SYM LEFT_CURLY_BRACES
	label
	statement_list
	RIGHT_CURLY_BRACES
	{
		backPatch($3.trueList,$6);
		backPatch($3.falseList,$12);
		$$.nextList = merge($7.nextList, $13.nextList);
		$$.nextList->push_back($8);
	}
	;

while:
	label
	WHILE_SYM LEFT_BRACKET
	b_expression
	RIGHT_BRACKET LEFT_CURLY_BRACES
	label
	statement_list
	RIGHT_CURLY_BRACES
	{
		writeCode("goto " + getLabel($1));
		backPatch($8.nextList,$1);
		backPatch($4.trueList,$7);
		$$.nextList = $4.falseList;
	}
	;

for:
	FOR_SYM LEFT_BRACKET assignment
	label b_expression SEMI_COLON
	label assignment goto RIGHT_BRACKET
	LEFT_CURLY_BRACES label statement_list goto RIGHT_CURLY_BRACES
	{
		backPatch($5.trueList,$12);
		vector<int> * v = new vector<int> ();
		v->push_back($9);
		backPatch(v,$4);
		v = new vector<int>();
		v->push_back($14);
		backPatch(v,$7);
		backPatch($13.nextList,$7);
		$$.nextList = $5.falseList;
	}
	;

assignment:
	IDENTIFIER EQUAL expression SEMI_COLON
	{
		string str($1);
		// store top of stack to the id
		if(checkOp(str))
		{
			if($3.sType == symbTab[str].second)
			{
				if($3.sType == INTEGER_TYPE)
				{
					writeCode("istore " + to_string(symbTab[str].first));
				}else if ($3.sType == FLOAT_TYPE)
				{
					writeCode("fstore " + to_string(symbTab[str].first));
				}
			}
			else
			{
				yyerror("type mismatch, cast required");
			}
		}else{
			string err = "identifier: "+str+" isn't declared in this scope";
			yyerror(err.c_str());
		}
	}
	;

expression:
	FLOAT 	{$$.sType = FLOAT_TYPE; writeCode("ldc "+to_string($1));}
	| INT 	{$$.sType = INTEGER_TYPE;  writeCode("ldc "+to_string($1));}
	| expression ARITH_OP expression	{checkCast($1.sType, $3.sType, string($2));}
	| IDENTIFIER {
		string str($1);
		if(checkOp(str))
		{
			$$.sType = symbTab[str].second;
			if(symbTab[str].second == INTEGER_TYPE)
			{
				writeCode("iload " + to_string(symbTab[str].first));
			}else if (symbTab[str].second == FLOAT_TYPE)
			{
				writeCode("fload " + to_string(symbTab[str].first));
			}
		}
		else
		{
			string err = "identifier: "+str+" isn't declared in this scope";
			yyerror(err.c_str());
			$$.sType = ERROR_TYPE;
		}
	}
	| LEFT_BRACKET expression RIGHT_BRACKET {$$.sType = $2.sType;}
	;

b_expression:
	BOOLEAN
	{
		if($1)	// "true"
		{
			$$.trueList = new vector<int> ();
			$$.trueList->push_back(bytecodeList.size());
			$$.falseList = new vector<int>();
			writeCode("goto ");
		}else
		{
			$$.trueList = new vector<int> ();
			$$.falseList= new vector<int>();
			$$.falseList->push_back(bytecodeList.size());
			writeCode("goto ");
		}
	}
	|b_expression BOOL_OP label b_expression
	{
		if(!strcmp($2, "&&"))
		{
			backPatch($1.trueList, $3);
			$$.trueList = $4.trueList;
			$$.falseList = merge($1.falseList,$4.falseList);
		}
		else if (!strcmp($2,"||"))
		{
			backPatch($1.falseList,$3);
			$$.trueList = merge($1.trueList, $4.trueList);
			$$.falseList = $4.falseList;
		}
	}
	| expression RELA_OP expression
	{
		string op ($2);
		$$.trueList = new vector<int>();
		$$.trueList ->push_back (bytecodeList.size());
		$$.falseList = new vector<int>();
		$$.falseList->push_back(bytecodeList.size()+1);
		writeCode(getOp(op)+ " ");
		writeCode("goto ");
	}
	;

	system_print:
	SYSTEM_OUT LEFT_BRACKET expression RIGHT_BRACKET SEMI_COLON
	{
		if($3.sType == INTEGER_TYPE)
		{
			// standard java bytecode for system out statement
			// expression is at top of stack now
			// save it at the predefined temp syso var
			writeCode("istore " + to_string(symbTab["1syso_int_var"].first));
			writeCode("getstatic      java/lang/System/out Ljava/io/PrintStream;");
			writeCode("iload " + to_string(symbTab["1syso_int_var"].first ));
			writeCode("invokevirtual java/io/PrintStream/println(I)V");

		}else if ($3.sType == FLOAT_TYPE)
		{
			writeCode("fstore " + to_string(symbTab["1syso_float_var"].first));
			writeCode("getstatic      java/lang/System/out Ljava/io/PrintStream;");
			writeCode("fload " + to_string(symbTab["1syso_float_var"].first ));
			writeCode("invokevirtual java/io/PrintStream/println(F)V");
		}
	}
	;
%%

/*              Main                */

main ()
{
	FILE *myfile;
	myfile = fopen("input.txt", "r");
	fileName = "input.txt";

	if (!myfile) {
		printf("file not open!\n");
		return -1;
	}

	yyin = myfile;
	yyparse();
	// write bytecode to file
	printCode();
}

// print error
void yyerror(const char * s)
{
	printf("error@%d: %s\n",lineCounter, s);
}

// standard java bytecode header & footer
void generateHeader()
{
	writeCode(".source " + fileName);
	writeCode(".class public test\n.super java/lang/Object\n"); //code for defining class
	writeCode(".method public <init>()V");
	writeCode("aload_0");
	writeCode("invokenonvirtual java/lang/Object/<init>()V");
	writeCode("return");
	writeCode(".end method\n");
	writeCode(".method public static main([Ljava/lang/String;)V");
	writeCode(".limit locals 100\n.limit stack 100");
	// generate temporal vars for syso
	defineVar("1syso_int_var",INTEGER_TYPE);
	defineVar("1syso_float_var",FLOAT_TYPE);

	writeCode(".line 1");
}

void generateFooter()
{
	writeCode("return");
	writeCode(".end method");
}

void checkCast(int from , int to, string op)
{
	if (from != to) {
		yyerror("type mismatch, cast required");
	} else
	{
		if(from == INTEGER_TYPE)
		{
			writeCode("i" + getOp(op));
		} else if (from == FLOAT_TYPE)
		{
			writeCode("f" + getOp(op));
		}
	}
}

// return java bytecode instruction if found
string getOp(string s)
{
	if(byte_code_map.find(s) != byte_code_map.end())
	{
		return byte_code_map[s];
	}
	return "";
}

// check if java bytecode instruction found
bool checkOp(string s)
{
	return (symbTab.find(s) != symbTab.end());
}

void defineVar(string name, int type)
{
	if(checkOp(name))
	{
		string err = "variable: "+name+" declared before";
		yyerror(err.c_str());
	}else
	{
		if(type == INTEGER_TYPE)
		{
			writeCode("iconst_0\nistore " + to_string(variablesCount));
		}
		else if ( type == FLOAT_TYPE)
		{
			writeCode("fconst_0\nfstore " + to_string(variablesCount));
		}
		symbTab[name] = make_pair(variablesCount++,(types)type);
	}
}

// create new label
string generateLabel()
{
	return "L_"+to_string(labelsCount++);
}

// create label for given index
string getLabel(int i)
{
	return "L_"+to_string(i);
}

// merge two vectors
vector<int> * merge(vector<int> *list1, vector<int> *list2)
{
	if(list1 && list2){
		vector<int> *list = new vector<int> (*list1);
		list->insert(list->end(), list2->begin(),list2->end());
		return list;
	}else if(list1)
	{
		return list1;
	}else if (list2)
	{
		return list2;
	}else
	{
		return new vector<int>();
	}
}

/*
    back patching used in jump statements
     to specify labels
*/
void backPatch(vector<int> *lists, int ind)
{
	if(! lists) return;

	for(int i =0 ; i < lists->size() ; i++)
	{
		bytecodeList[(*lists)[i]] = bytecodeList[(*lists)[i]] + getLabel(ind);
	}
}

// used to print line no (start of a new line)
void printLineNo(int n)
{
	writeCode(".line "+ to_string(n));
}

// add current bytecode line
void writeCode(string s)
{
	bytecodeList.push_back(s);
}

// write output file
void printCode(void)
{
	for ( int i = 0 ; i < bytecodeList.size() ; i++)
	{
		fout<<bytecodeList[i]<<endl;
	}
}