
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 66 "semantics.y"

	#include <vector>
	using namespace std;



/* Line 1676 of yacc.c  */
#line 47 "y.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     FLOAT = 259,
     BOOLEAN = 260,
     IDENTIFIER = 261,
     ARITH_OP = 262,
     RELA_OP = 263,
     BOOL_OP = 264,
     IF_SYM = 265,
     ELSE_SYM = 266,
     WHILE_SYM = 267,
     FOR_SYM = 268,
     INT_SYM = 269,
     FLOAT_SYM = 270,
     BOOLEAN_SYM = 271,
     SEMI_COLON = 272,
     EQUAL = 273,
     LEFT_BRACKET = 274,
     RIGHT_BRACKET = 275,
     LEFT_CURLY_BRACES = 276,
     RIGHT_CURLY_BRACES = 277,
     SYSTEM_OUT = 278
   };
#endif
/* Tokens.  */
#define INT 258
#define FLOAT 259
#define BOOLEAN 260
#define IDENTIFIER 261
#define ARITH_OP 262
#define RELA_OP 263
#define BOOL_OP 264
#define IF_SYM 265
#define ELSE_SYM 266
#define WHILE_SYM 267
#define FOR_SYM 268
#define INT_SYM 269
#define FLOAT_SYM 270
#define BOOLEAN_SYM 271
#define SEMI_COLON 272
#define EQUAL 273
#define LEFT_BRACKET 274
#define RIGHT_BRACKET 275
#define LEFT_CURLY_BRACES 276
#define RIGHT_CURLY_BRACES 277
#define SYSTEM_OUT 278




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 76 "semantics.y"

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



/* Line 1676 of yacc.c  */
#line 130 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


