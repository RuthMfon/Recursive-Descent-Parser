/**************************************************************************
Author: Ruth Mfon-Uko K
NetID: rkm254
Course: CSE 4714
Programming Environment: Visual Studio Code
Contains file that: creates a recursive descent parser program for TIPS
**************************************************************************/

#include "parser.h"
#include "parse_tree_nodes.h"
#include "lexer.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

// Forward declarations of first_of functions
bool first_of_program();
bool first_of_block();
bool first_of_statement();
bool first_of_compound();
bool first_of_assignment();
bool first_of_while();
bool first_of_if();
bool first_of_read();
bool first_of_write();
bool first_of_expression();
bool first_of_simpleExpression();
bool first_of_term();
bool first_of_factor();

int nextToken = 0;            // hold nextToken returned by lex
bool printParse = true;      // shall we print the parse tree?

static int level = 0;

//*Handle syntax errors
void error() {
  cout << endl << "===========================" << endl;
  cout << "ERROR near: " << yytext;
  cout << endl << "===========================" << endl;
  if (yyin)
    fclose(yyin);
  exit(EXIT_FAILURE);
}

// Symbol table
symbolTableT symbolTable;
// Determine if a symbol is in the symbol table
bool inSymbolTable(string idName) {
  symbolTableT::iterator it;
  it = symbolTable.find( idName );
  // If idName is missing, will be set to the end
  return !(it == symbolTable.end());
}

//*****************************************************************************
// Print each level with appropriate indentation
void indent() {
  for (int i = 0; i<level; i++)
    cout << ("  ");
}

//*Announce what the lexical analyzer has found
void output() {
  indent();
  cout << "---> FOUND " << yytext << endl;
}
//*****************************************************************************
int lex() {
  nextToken = yylex();

  if (nextToken == TOK_EOF) {
    // save a "lexeme" into yytext
    yytext[0] = 'E';
    yytext[1] = 'O';
    yytext[2] = 'F';
    yytext[3] = 0;
  }
  if(printParse) {
    // Tell us about the token and lexeme
    indent();
    cout << "Next token is: ";
    switch(nextToken) {
    case TOK_INTLIT:      cout << "TOK_INTLIT";     break;
    case TOK_IDENT:        cout << "TOK_IDENT";       break;
    case TOK_PLUS:       cout << "TOK_PLUS";      break;
    case TOK_MINUS:       cout << "TOK_MINUS";      break;
    case TOK_MULTIPLY:      cout << "TOK_MULTIPLY";     break;
    case TOK_DIVIDE:       cout << "TOK_DIVIDE";      break;
    case TOK_OPENPAREN:   cout << "TOK_OPENPAREN";  break;
    case TOK_CLOSEPAREN:  cout << "TOK_CLOSEPAREN"; break;
    case TOK_OR:      cout << "TOK_OR";     break;
    case TOK_AND:       cout << "TOK_AND";      break;
    case TOK_EQUALTO:   cout << "TOK_EQUALTO";  break;
    case TOK_NOTEQUALTO:  cout << "TOK_NOTEQUALTO"; break;
    case TOK_LESSTHAN:      cout << "TOK_LESSTHAN";     break;
    case TOK_GREATERTHAN:       cout << "TOK_GREATERTHAN";      break;
    case TOK_MOD:   cout << "TOK_MOD";  break;
    case TOK_NOT:  cout << "TOK_NOT"; break;
    case TOK_INTEGER:      cout << "TOK_INTEGER";     break;
    case TOK_REAL:       cout << "TOK_REAL";      break;
    case TOK_WRITE:   cout << "TOK_WRITE";  break;
    case TOK_WHILE:        cout << "TOK_WHILE"; break;
    case TOK_IF:        cout << "TOK_IF"; break;
    case TOK_READ:        cout << "TOK_READ"; break;
    case TOK_BEGIN:        cout << "TOK_BEGIN"; break;
    case TOK_BREAK:        cout << "TOK_BREAK"; break;
    case TOK_CONTINUE:        cout << "TOK_CONTINUE"; break;
    case TOK_DOWNTO:        cout << "TOK_DOWNTO"; break;
    case TOK_ELSE:        cout << "TOK_ELSE"; break;
    case TOK_END:        cout << "TOK_END"; break;
    case TOK_FOR:        cout << "TOK_FOR"; break;
    case TOK_PROGRAM:        cout << "TOK_PROGRAM"; break;
    case TOK_LET:        cout << "TOK_LET"; break;
    case TOK_VAR:        cout << "TOK_VAR"; break;
    case TOK_SEMICOLON:        cout << "TOK_SEMICOLON"; break;
    case TOK_COLON:        cout << "TOK_COLON"; break;
    case TOK_ASSIGN:        cout << "TOK_ASSIGN"; break;
    case TOK_STRINGLIT:        cout << "TOK_STRINGLIT"; break;
    case TOK_FLOATLIT:        cout << "TOK_FLOATLIT"; break;
    case TOK_EOF:          cout << "TOK_EOF";         break;
    case TOK_UNKNOWN:      cout << "TOK_UNKNOWN";     break;
    default: error();
    }
    cout << ", Next lexeme is: " << yytext << endl;
  }
  return nextToken;
}

string psp(void) { // Stands for p-space, but I want the name short
    string str("");
    for (int i = 0; i < level; i++)
        str += "|  ";
    return str;
}
//*****************************************************************************
// Report what we found
void output(string what) {
    cout << psp() << "found |" << yytext << "| " << what << endl;
}


//*****************************************************************************
// Forward declarations of FIRST_OF functions.  These check whether the current 
// token is in the FIRST set of a production rule.
bool first_of_program();
bool first_of_block();
bool first_of_statement();
bool first_of_compound();
bool first_of_assignment();
bool first_of_while();
bool first_of_if();
bool first_of_read();
bool first_of_write();
bool first_of_expression();
bool first_of_simpleExpression();
bool first_of_term();
bool first_of_factor();


//*****************************************************************************
// Parses strings in the language generated by the rule:
// <program> → TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
ProgramNode* program()
{
    if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";

    if (printParse) {
        output("PROGRAM");
        cout << psp() << "enter <program>" << endl;
    }

    nextToken = yylex();
    ProgramNode* newProgramNode = new ProgramNode(level);
    ++level;
    if (nextToken == TOK_IDENT) {
        if (printParse)
            output("IDENTIFIER");
        nextToken = yylex();
    }
    else
        throw "2: identifier expected";

    if (nextToken == TOK_SEMICOLON) {
        if (printParse)
            output("SEMICOLON");
        nextToken = yylex();
    }
    else
        throw "4: ';' expected";

    newProgramNode->BlockPtr = block();
    --level;
    if (printParse)
        cout << psp() << "exit <program>" << endl;

    return newProgramNode;
}
BlockNode* block() {
    string var;
    string varType;
    //int value = 1.0;

    if (!first_of_block())
        throw "18: error in declaration part OR 17: 'BEGIN' expected";

    if (printParse) {
        output("BLOCK");
        cout << psp() << "enter <block>" << endl;
    }

    ++level;
    BlockNode* newBlockNode = new BlockNode(level - 1);
    if (nextToken == TOK_VAR) {
        nextToken = yylex();

        while (nextToken != TOK_BEGIN) {
            if (nextToken == TOK_IDENT) {
                if (printParse)
                    output("IDENTIFIER");
                var = yytext;
                nextToken = yylex();
            }
            else
                throw "2: identifier expected";

            if (nextToken == TOK_COLON) {
                if (printParse)
                    output("COLON");
                nextToken = yylex();
            }
            else
                throw "S: ':' expected";

            if (nextToken == TOK_INTEGER || nextToken == TOK_REAL) {
                if (printParse)
                    output("TYPE");
                varType = yytext;
                nextToken = yylex();
            }
            else
                throw "10: error in type";

            if (nextToken == TOK_SEMICOLON) {
                if (printParse)
                    output("SEMICOLON");
            }
            else
                throw "14: ';' expected";

            if (printParse)
                cout << psp() << "-- idName: |" << var << "| idType: |" << varType << "| --" << endl;
            
            if (!inSymbolTable(string(yytext)))
                symbolTable.insert(pair<string, float>(var, 1.0));
            else
                throw("101: identifier declared twice");
            
            nextToken = yylex();
        }
    }

    if (nextToken == TOK_BEGIN)
        newBlockNode->CompoundPtr = compound();
    else
        throw "error here";


    --level;
    if (printParse)
        cout << psp() << "exit <block>" << endl;

    return newBlockNode;
}
StatementNode* statement() {
    if (!first_of_statement)
        throw "900: illegal type of statement";
    StatementNode* newStatementNode = nullptr;

    if (nextToken == TOK_IDENT) {
        if (printParse)
            output("STATEMENT");
        newStatementNode = assignment();
    }
    else if (nextToken == TOK_BEGIN) {
        if (printParse)
            output("STATEMENT");
        newStatementNode = compound();
    }
    else if (nextToken == TOK_IF) {
        if (printParse)
            output("STATEMENT");
        newStatementNode = ifStmt();
    }
    else if (nextToken == TOK_WHILE) {
        if (printParse)
            output("STATEMENT");
        newStatementNode = whileStmt();
    }
    else if (nextToken == TOK_READ) {
        if (printParse)
            output("STATEMENT");
        newStatementNode = read();
    }
    else if (nextToken == TOK_WRITE) {
        if (printParse)
            output("STATEMENT");
        newStatementNode = write();
    }
    else
        throw "900: illegal type of statement";

    return newStatementNode;
}

AssignNode* assignment() {
    if (!first_of_assignment())
        throw "2: identifier expected";
    if (printParse)
        cout << psp() << "enter <assignment>" << endl;
    ++level;

    if (printParse) {
        output("IDENTIFIER");
        cout << psp() << yytext << endl;
    }
    string temp = yytext;

    nextToken = yylex();

    if (nextToken == TOK_ASSIGN) {
        if (printParse)
            output("ASSIGN");
        nextToken = yylex();
    }
    else
        throw "51: ':=' expected";


    AssignNode* newAssignNode = new AssignNode((level - 1), temp);
    newAssignNode->ExprPtr = expression();

    --level;

    if (printParse)
        cout << psp() << "exit <assignment>" << endl;

    return newAssignNode;
}

CompoundNode* compound() {
    if (!first_of_compound())
        throw "17: 'BEGIN' expected";
    if (printParse) {
        output("BEGIN");
        cout << psp() << "enter <compound_stmt>" << endl;
    }

    CompoundNode* newCompoundNode = new CompoundNode(level);

    nextToken = yylex();
    ++level;

    newCompoundNode->StatementPtr = statement();
    while (nextToken == TOK_SEMICOLON) {
        if (printParse)
            output("SEMICOLON");
        newCompoundNode->CompoundOps.push_back(nextToken);
        nextToken = yylex();
        newCompoundNode->Stmts.push_back(statement());
    }

    if (nextToken == TOK_END) {
        --level;
        if (printParse)
            output("END");
        nextToken = yylex();
    }

    else
        throw "13: 'END' expected";
    if (printParse)
        cout << psp() << "exit <compound_stmt>" << endl;

    return newCompoundNode;
}

IfNode* ifStmt() {
    if (!first_of_if())
        throw "900: illegal type of statement";

    if (printParse)
        cout << psp() << "enter <if>" << endl;
    ++level;

    IfNode* newIfNode = new IfNode(level - 1);
    if (nextToken == TOK_IF) {
        newIfNode->IfOps.push_back(nextToken);
        nextToken = yylex();
    }
    else
        throw "999: an error has occured";

    newIfNode->ExprPtr = expression();
    //expression();

    if (nextToken == TOK_THEN) {
        if (printParse)
            output("THEN");

        newIfNode->IfOps.push_back(nextToken);
        nextToken = yylex();
    }
    else
        throw "52: 'THEN' expected";

    newIfNode->StmtPtr = statement();

    if (nextToken == TOK_ELSE) {
        --level;
        if (printParse) {
            output("ELSE");
            cout << psp() << "enter <else>" << endl;
        }
        ++level;
        newIfNode->IfOps.push_back(nextToken);
        nextToken = yylex();
        newIfNode->secondstmt = statement();
    }
    --level;
    if (printParse)
        cout << psp() << "exit <if>" << endl;

    return newIfNode;
}

WhileNode* whileStmt() {
    if (!first_of_while())
        throw "900: illegal type of statement";
    nextToken = yylex();

    if (printParse)
        cout << psp() << "enter <while>" << endl;

    WhileNode* newWhileNode = new WhileNode(level);
    ++level;
    newWhileNode->ExprPtr = expression();
    newWhileNode->StmtPtr = statement();


    --level;
    if (printParse)
        cout << psp() << "exit <while>" << endl;
    return newWhileNode;
}

ReadNode* read() {
    if (!first_of_read())
        throw "900: illegal type of statement";

    nextToken = yylex();
    if (printParse)
        cout << psp() << "enter <read>" << endl;

    ReadNode* newReadNode = nullptr;
    ++level;

    if (nextToken == TOK_OPENPAREN) {
        if (printParse)
            output("OPENPAREN");
        nextToken = yylex();
    }
    else
        throw "9: '(' expected";

    if (nextToken == TOK_IDENT) {
        if (printParse) {
            output("IDENTIFIER");
            cout << psp() << yytext << endl;
        }
        newReadNode = new ReadNode((level - 1), string(yytext));
        nextToken = yylex();
    }
    else
        throw "2: identifier expected";

    if (nextToken == TOK_CLOSEPAREN) {
        if (printParse)
            output("CLOSEPAREN");
        nextToken = yylex();
    }
    else
        throw "4: ')' expected";

    --level;
    if (printParse)
        cout << psp() << "exit <read>" << endl;

    return newReadNode;
}

WriteNode* write() {
    if (!first_of_write())
        throw "900: illegal type of statement";

    nextToken = yylex();
    if (printParse)
        cout << psp() << "enter <write>" << endl;

    WriteNode* newWriteNode = nullptr;
    ++level;

    if (nextToken == TOK_OPENPAREN) {
        if (printParse)
            output("OPENPAREN");

        nextToken = yylex();
    }
    else
        throw "9: '(' expected";

    if (nextToken == TOK_IDENT || nextToken == TOK_STRINGLIT) {
        if (printParse) {
            output("WRITE");
            cout << psp() << yytext << endl;
        }

        newWriteNode = new WriteNode((level - 1), string(yytext),nextToken);
        nextToken = yylex();
    }
    else
        throw "134: illegal type of operand(s)";

    if (nextToken == TOK_CLOSEPAREN) {
        if (printParse)
            output("CLOSEPAREN");
        nextToken = yylex();
    }
    else
        throw "4: ')' expected";

    --level;
    if (printParse)
        cout << psp() << "exit <write>" << endl;

    return newWriteNode;
}

ExprNode* expression() {
    if (!first_of_expression)
        throw "144: illegal type of expression";

    if (printParse) {
        output("EXPRESSION");
        cout << psp() << "enter <expression>" << endl;
    }

    ExprNode* newExprNode = new ExprNode(level);
    ++level;
    newExprNode->SimpExprPtr = simpleExpression();

    //simpleExpression();

    switch (nextToken) {
    case TOK_EQUALTO:
        if (printParse) {
            output("EQUALTO");
        }
        newExprNode->SimpOps.push_back(nextToken);
        nextToken = yylex();
        newExprNode->SimpExpr.push_back(simpleExpression());
        break;
    case TOK_LESSTHAN:
        if (printParse) {
            output("LESSTHAN");
            cout << psp() << yytext << endl;
        }
        newExprNode->SimpOps.push_back(nextToken);
        nextToken = yylex();
        newExprNode->SimpExpr.push_back(simpleExpression());
        break;
    case TOK_GREATERTHAN:
        if (printParse) {
            output("GREATERTHAN");
            cout << psp() << yytext << endl;
        }
        newExprNode->SimpOps.push_back(nextToken);
        nextToken = yylex();
        newExprNode->SimpExpr.push_back(simpleExpression());
        break;
    case TOK_NOTEQUALTO:
        if (printParse) {
            output("NOTEQUALTO");
        }
        newExprNode->SimpOps.push_back(nextToken);
        nextToken = yylex();
        newExprNode->SimpExpr.push_back(simpleExpression());
        break;
    }

    --level;
    if (printParse)
        cout << psp() << "exit <expression>" << endl;

    return newExprNode;
}
SimpExprNode* simpleExpression() {
    if (!first_of_simpleExpression)
        throw "901: illegal type of simple expression";

    if (printParse) {
        output("SIMPLE_EXP");
        cout << psp() << "enter <simple_exp>" << endl;
    }
    SimpExprNode* newSimpExprNode = new SimpExprNode(level);
    ++level;
    newSimpExprNode->TermPtr = term();

    while (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR) {
        switch (nextToken) {
        case TOK_PLUS:
            if (printParse) {
                output("PLUS");
                cout << psp() << yytext << endl;
            }
            newSimpExprNode->TermOps.push_back(nextToken);
            nextToken = yylex();
            break;
        case TOK_MINUS:
            if (printParse) {
                output("MINUS");
                cout << psp() << yytext << endl;
            }
            newSimpExprNode->TermOps.push_back(nextToken);
            nextToken = yylex();
            break;
        case TOK_OR:
            if (printParse) {
                output("OR");
                cout << psp() << yytext << endl;
            }
            newSimpExprNode->TermOps.push_back(nextToken);
            nextToken = yylex();
            break;

        }
        newSimpExprNode->Terms.push_back(term());
    }


    --level;
    if (printParse)
        cout << psp() << "exit <simple_exp>" << endl;

    return newSimpExprNode;
}
TermNode* term() {
    if (!first_of_term)
        throw "902: illegal type of term";

    if (printParse) {
        output("TERM");
        cout << psp() << "enter <term>" << endl;
    }

    TermNode* newTermNode = new TermNode(level);
    ++level;
    newTermNode->firstFactor = factor();

    while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND) {
        switch (nextToken) {
        case TOK_MULTIPLY:
            if (printParse) {
                output("MULTIPLY");
                cout << psp() << yytext << endl;
            }
            newTermNode->restFactorOps.push_back(nextToken);
            nextToken = yylex();
            
        break;
        case TOK_DIVIDE:
            if (printParse) {
                output("DIVIDE");
                cout << psp() << yytext << endl;
            }
            newTermNode->restFactorOps.push_back(nextToken);
            nextToken = yylex();
        break;
        case TOK_AND:
            if (printParse) {
                output("AND");
                cout << psp() << yytext << endl;
            }
            newTermNode->restFactorOps.push_back(nextToken);
            nextToken = yylex();
        break;

        }
        newTermNode->restFactors.push_back(factor());
    }

    --level;
    if (printParse) {
        cout << psp() << "exit <term>" << endl;
    }
    return newTermNode;
}

FactorNode* factor() {
    if (!first_of_factor())
        throw "903: illegal type of factor";

    if (printParse) {
        output("FACTOR");
        cout << psp() << "enter <factor>" << endl;
    }

    ++level;
    FactorNode* newFactorNode = nullptr;
    //float value = 0.0;
    if (nextToken == TOK_INTLIT) {
        if (printParse) {
            output("INTLIT");
            cout << psp() << yytext << endl;
        }
        
        newFactorNode = new IntLitNode((level - 1), atoi(yytext));
        nextToken = yylex();
    }
    else if (nextToken == TOK_FLOATLIT) {
        if (printParse) {
            output("FLOATLIT");
            cout << psp() << yytext << endl;
        }
        
        newFactorNode = new FloatLitNode((level - 1), atof(yytext));
        nextToken = yylex();
    }
    else if (nextToken == TOK_IDENT) {
        if (printParse) {
            output("IDENTIFIER");
            cout << psp() << yytext << endl;
        }
        //if (!inSymbolTable(string(yytext)))
           // symbolTable.insert(pair<string, float>(string(yytext), 0.0));
        
        newFactorNode = new IdNode((level - 1), string(yytext));
        nextToken = yylex();
    }
    else if (nextToken == TOK_OPENPAREN) {
        if (printParse) {
            output("OPENPAREN");
            cout << psp() << yytext << endl;
        }
        nextToken = yylex();
        newFactorNode = new NestedExprNode((level - 1), expression());

        //expression();
        if (nextToken == TOK_CLOSEPAREN) {
            if (printParse) {
                output("CLOSEPAREN");
            }
            nextToken = yylex();
        }
        else
            throw "4: ')' expected";
    }
    else if (nextToken == TOK_NOT) {
        if (printParse) {
            output("NOT");
        }

        nextToken = yylex();
        newFactorNode = new NotFactorNode((level - 1), factor());
        //factor();
    }
    else if (nextToken == TOK_MINUS) {
        if (printParse) {
            output("MINUS");
            cout << psp() << yytext << endl;
        }
        nextToken = yylex();
        newFactorNode = new MinusFactorNode((level - 1), factor());
        // factor();
    }
    else
        throw "903: illegal type of factor";

    --level;
    if (printParse) {
        cout << psp() << "exit <factor>" << endl;
    }
    return newFactorNode;
}

// --------------------------------------------------------------------------------------
bool first_of_program() {
    return nextToken == TOK_PROGRAM;
}
bool first_of_statement() {
    return (nextToken == TOK_IDENT || nextToken == TOK_BEGIN || nextToken == TOK_IF || nextToken == TOK_WHILE || nextToken == TOK_READ || nextToken == TOK_WRITE);
}
bool first_of_block() {
    return (nextToken == TOK_VAR || nextToken == TOK_BEGIN);
}
bool first_of_assignment() {
    return nextToken == TOK_IDENT;
}
bool first_of_compound() {
    return nextToken == TOK_BEGIN;
}
bool first_of_if() {
    return nextToken == TOK_IF;
}
bool first_of_while() {
    return nextToken == TOK_WHILE;
}
bool first_of_read() {
    return nextToken == TOK_READ;
}
bool first_of_write() {
    return nextToken == TOK_WRITE;
}
bool first_of_factor() {
    return (nextToken == TOK_INTLIT || nextToken == TOK_FLOATLIT || nextToken == TOK_IDENT || nextToken == TOK_OPENPAREN || nextToken == TOK_NOT || nextToken == TOK_MINUS);
}
bool first_of_expression() {
    return first_of_factor();
}
bool first_of_simpleExpression() {
    return first_of_factor();
}
bool first_of_term() {
    return first_of_factor();
}