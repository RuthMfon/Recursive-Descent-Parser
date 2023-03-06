//*****************************************************************************
// (part 4)
// purpose: driver file for interpreter
// author: Ruth Mfon-Uko
//*****************************************************************************
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstring>
#include "lexer.h"
#include "parser.h"
#include "parse_tree_nodes.h"

using namespace std;

extern "C"
{
  // Instantiate global variables
  extern FILE *yyin;   // input stream
  extern FILE *yyout;  // output stream
  extern int yyleng;   // length of current lexeme
  extern char *yytext; // text of current lexeme
  extern int yylex();  // the generated lexical analyzer
  extern int yylex_destroy(); // deletes memory allocated by yylex
}

int main(int argc, char* argv[])
{
  // Whether to print these items
  bool printTree = true; 
  bool printSymbolTable = true;
  bool printDelete = true;

  //* Process any command - line switches
  for(int i = 1; i < argc; i++) {
    // -p flag: if requested, print while parsing
    if(strcmp(argv[i], "-p") == 0) {
      printParse = true;
    }
    // -t flag: if requested, print parse tree
    if(strcmp(argv[i], "-t") == 0) {
      printTree = true;
    }
    // -s flag: if requested, print symbol table
    if(strcmp(argv[i], "-s") == 0) {
      printSymbolTable = true;
    }
    // -d flag: if requested, print while deleting parse tree
    if(strcmp(argv[i], "-d") == 0) {
      printDelete = true;
    }
  }

  if (argc > 1) {
      printf("INFO: Using the %s file for input\n", argv[argc - 1]);
      yyin = fopen(argv[argc - 1], "r");
  }
 

  // Create the root of the parse tree
  ProgramNode* root = nullptr;

  lex();  // prime the pump (get first token)
  do {
    root = program(); 
  } while(nextToken != TOK_EOF);

  if (yyin)
    fclose(yyin);
    
  yylex_destroy();

  // Printing, Interpreting, and Deleting the tree all result in 
  // the same in-order traversal of the tree as parsing.  All
  // use the call stack.

  if(printTree) {
    cout << endl << "*** Print the Tree ***" << endl;
    cout << *root << endl << endl;
  }

  cout << "*** Interpret the Tree ***" << endl;
  root->interpret();
  
  if(printSymbolTable)
  {
    cout << "*** Print the Symbol Table ***" << endl;
    symbolTableT::iterator it;
    for(it = symbolTable.begin(); it != symbolTable.end(); ++it )
      cout << setw(8) << it->first << ": " << it->second << endl;
  }
  
  if(printDelete)
    cout << "*** Delete the Tree ***" << endl;
  delete root;
  root = nullptr;
    
  return(EXIT_SUCCESS);
}
