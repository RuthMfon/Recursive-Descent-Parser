//*****************************************************************************
// (part 4)
// purpose: node classes and interpret methods used while building a parse tree for
//              the arithmetic expression
// version: Fall 2022
//  author: Ruth Mfon-Uko 
// NetID: rkm254
//*****************************************************************************

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
//#include "parser.h"
using namespace std;

extern bool printDelete;      // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Forward declaration of node types
class ProgramNode;
class BlockNode;
class ExprNode;
class SimpExprNode;
class StatementNode;
class AssignNode;
class CompoundNode;
class IfNode;
class WhileNode;
class ReadNode;
class WriteNode;
class TermNode;
class FactorNode;
//<program> ->TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
class ProgramNode {
public:
    int _level = 0;

    BlockNode* BlockPtr = nullptr;
    ProgramNode(int level);
    ~ProgramNode();
    void interpret();
};
ostream& operator<<(ostream&, ProgramNode&);// Node print operator

//
class BlockNode {
public:
    int _level = 0;

    CompoundNode* CompoundPtr = nullptr;

    BlockNode(int level);
    ~BlockNode();
    void interpret();
};
ostream& operator<<(ostream&, BlockNode&);// Node print operator

class StatementNode {
public:
    int _level = 0;

    virtual void printTo(ostream& os) = 0;
    virtual void interpret() = 0;
    virtual ~StatementNode();
   
};
ostream& operator<<(ostream&, StatementNode&);// Node print operator

class AssignNode : public StatementNode {
public:
    friend StatementNode;
    string* id = nullptr;
    ExprNode* ExprPtr = nullptr;

    AssignNode(int level, string name);
    ~AssignNode();
    void printTo(ostream& os);
    void interpret();
};

class CompoundNode : public StatementNode {
public:
    friend StatementNode;

    StatementNode* StatementPtr = nullptr;
    vector<int> CompoundOps;
    vector<StatementNode*> Stmts;


    CompoundNode(int level);
    ~CompoundNode();
    void printTo(ostream& os);
    void interpret();
};
class IfNode : public StatementNode {
public:

    ExprNode* ExprPtr = nullptr;
    StatementNode* StmtPtr = nullptr;
    StatementNode* secondstmt = nullptr;

    vector<int> IfOps;

    IfNode(int level);
    ~IfNode();
    void printTo(ostream& os);
    void interpret();

};
class WhileNode : public StatementNode {
public:
    ExprNode* ExprPtr = nullptr;
    StatementNode* StmtPtr = nullptr;

    WhileNode(int level);
    ~WhileNode();
    void printTo(ostream& os);
    void interpret();
};
class ReadNode : public StatementNode {
public:
    string* id = nullptr;

    ReadNode(int level, string name);
    ~ReadNode();
    void printTo(ostream& os);
    void interpret();
};
class WriteNode : public StatementNode {
public:
    string* id = nullptr;
    int token;

    WriteNode(int level, string name, int num);
    ~WriteNode();
    void printTo(ostream& os);
    void interpret();

};
// ---------------------------------------------------------------------
// <expr> -> <simpexpr> [( =| < |> | <> ) <simpexpr> ]
class ExprNode {
public:
    int _level = 0;          // recursion level of this node
    SimpExprNode* SimpExprPtr = nullptr;
    vector<int> SimpOps;
    vector<SimpExprNode*> SimpExpr;

    ExprNode(int level);
    ~ExprNode();
    float interpret();
};
ostream& operator<<(ostream&, ExprNode&); // Node print operator
// ---------------------------------------------------------------------
//<simpexpr> -> <term> {(+ | - | OR ) <term>}
class SimpExprNode {
public:
    int _level = 0;          // recursion level of this node
    TermNode* TermPtr = nullptr;
    vector<int> TermOps;
    vector<TermNode*> Terms;

    SimpExprNode(int level);
    ~SimpExprNode();
    float interpret();
};
ostream& operator<<(ostream&, SimpExprNode&);
// <term> -> <factor> {{ (( * || / )) <factor> }}
class TermNode {
public:
    int _level = 0;              // recursion level of this node
    FactorNode* firstFactor = nullptr;
    vector<int> restFactorOps;   // TOK_MULT_OP or TOK_DIV_OP
    vector<FactorNode*> restFactors;

    TermNode(int level);
    ~TermNode();
    float interpret();
};
ostream& operator<<(ostream&, TermNode&); // Node print operator
// ---------------------------------------------------------------------
// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
// <factor> -> ID || INTLIT || ( <expr> )
class FactorNode {
public:
    int _level = 0;                        // recursion level of this node

    virtual void printTo(ostream& os) = 0; // pure virtual method, makes the class Abstract
    virtual ~FactorNode();                 // labeling the destructor as virtual allows 
    virtual float interpret() = 0;                                      // the subclass destructors to be called
};
ostream& operator<<(ostream&, FactorNode&); // Node print operator
// ---------------------------------------------------------------------
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
    string* id = nullptr;

    IdNode(int level, string name);
    ~IdNode();
    void printTo(ostream& os);
    float interpret();
};
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int level, int value);
    ~IntLitNode();
    void printTo(ostream& os);
    float interpret();
};
// ---------------------------------------------------------------------
// class IntLitNode (float Literal Node)
class FloatLitNode : public FactorNode {
public:
    int float_literal = 0.0;

    FloatLitNode(int level, int value);
    ~FloatLitNode();
    void printTo(ostream& os);
    float interpret();
};
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(int level, ExprNode* en);
    void printTo(ostream& os);
    ~NestedExprNode();
    float interpret();
};
// class NotfactorNode (Not Node)
class NotFactorNode : public FactorNode {
public:

    FactorNode* FactorPtr = nullptr;

    NotFactorNode(int level, FactorNode* fn);
    ~NotFactorNode();
    void printTo(ostream& os);
    float interpret();
};

// class MinusFactorNode (Minus Node)
class MinusFactorNode : public FactorNode {
public:

    FactorNode* FactorPtr = nullptr;

    MinusFactorNode(int level, FactorNode* fn);
    ~MinusFactorNode();
    void printTo(ostream& os);
    float interpret();
};

#endif /* PARSE_TREE_NODES_H */
