//*****************************************************************************
// (part 4)
// purpose: Recursive descent parsing of arithmetic expressions
//          Builds the parse tree while parsing the input
// version: Spring 2022
//  author: Ruth Mfon-Uko 
// NetID: rkm254
//*****************************************************************************

#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
//#include "lexer.h"
#include "parse_tree_nodes.h"

using namespace std;

// Access the table that holds the symbols in the interpreted program
typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable;

extern int nextToken;        // next token returned by lexer
extern bool printParse;      // shall tree be printed while parsing?

extern "C" {
    extern FILE *yyin;       // input stream
    extern int   yylex();    // the generated lexical analyzer
    extern char *yytext;     // text of current lexeme
}

/* Function declarations */
int lex();                   // return the next token

ProgramNode* program();
BlockNode* block();
AssignNode* assignment();
StatementNode* statement();
CompoundNode* compound();
WhileNode* whileStmt();
IfNode* ifStmt();
ReadNode* read();
WriteNode* write();
ExprNode* expression();
SimpExprNode* simpleExpression();
TermNode* term();
FactorNode* factor();

#endif /* PARSER_H */