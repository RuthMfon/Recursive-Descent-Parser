//*****************************************************************************
// (part 4)
// purpose: node classes and interpret methods used while building a parse tree for
//              the arithmetic expression
// version: Fall 2022
//  author: Ruth Mfon-Uko
// NetID: rkm254
//*****************************************************************************

#include "parse_tree_nodes.h"
#include "parser.h"
#define EPSILON 0.001
static bool truth(float F) {
	return !((EPSILON > F) && (F > -EPSILON));
}
bool printDelete = true;   // shall we print deleting the tree?


// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
	for (int i = 0; i < level; i++)
		cout << ("| ");
}
// ---------------------------------------------------------------------
FactorNode::~FactorNode() {}
// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn) {
	os << endl; indent(fn._level); os << "(factor ";
	fn.printTo(os);
	os << endl; indent(fn._level); os << "factor) ";
	return os;
}
float FactorNode::interpret() {
	return this->interpret();
}
// ---------------------------------------------------------------------
IdNode::IdNode(int level, string name) {
	_level = level;
	id = new string(name);
}
IdNode::~IdNode() {
	if (printDelete)
		cout << "Deleting FactorNode:IdNode " << endl;
	delete id;
	id = nullptr;
}
void IdNode::printTo(ostream& os) {
	os << "( IDENT: " << *id << " ) ";
}

float IdNode::interpret() {
	symbolTableT::iterator variable = symbolTable.find(*id);
	return variable->second;
	
}
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int level, int value) {
	_level = level;
	int_literal = value;
}
IntLitNode::~IntLitNode() {
	if (printDelete)
		cout << "Deleting FactorNode:IntLitNode " << endl;
	// Nothing to do since the only members are not pointers
}
void IntLitNode::printTo(ostream& os) {
	os << "( INTLIT: " << int_literal << " ) ";
}
float IntLitNode::interpret() {
	return int_literal;
}
// ---------------------------------------------------------------------
FloatLitNode::FloatLitNode(int level, int value) {
	_level = level;
	float_literal = value;
}
FloatLitNode::~FloatLitNode() {
	if (printDelete)
		cout << "Deleting FactorNode:FloatLitNode " << endl;
}
void FloatLitNode::printTo(ostream& os) {
	os << "( FLOATLIT: " << float_literal << " ) ";
}
float FloatLitNode::interpret() {
	return float_literal;
}
//-----------------------------------------------------------------------
NestedExprNode::NestedExprNode(int level, ExprNode* en) {
	_level = level;
	exprPtr = en;
}
NestedExprNode::~NestedExprNode() {
	if (printDelete)
		cout << "Deleting FactorNode:NestedExprNode " << endl;
	delete exprPtr;
	exprPtr = nullptr;
}
void NestedExprNode::printTo(ostream& os) {
	os << "( " << *exprPtr << " ) ";
}
float NestedExprNode::interpret() {
	return exprPtr->interpret();
}
// ---------------------------------------------------------------------
NotFactorNode::NotFactorNode(int level, FactorNode* fn) {
	_level = level;
	FactorPtr = fn;
}
NotFactorNode::~NotFactorNode() {
	if (printDelete)
		cout << "Deleting FactorNode:NotNode " << endl;
	delete FactorPtr;
	FactorPtr = nullptr;
}
void NotFactorNode::printTo(ostream& os) {
	os << "( NOT" << *FactorPtr << " ) ";
}
float NotFactorNode::interpret() {
	if (truth(FactorPtr->interpret())) {
		return 0.0;
	}
	else
		return 1.0;
}
// ---------------------------------------------------------------------
MinusFactorNode::MinusFactorNode(int level, FactorNode* fn) {
	_level = level;
		FactorPtr = fn;
}
MinusFactorNode::~MinusFactorNode() {
	if (printDelete)
		cout << "Deleting FactorNode:MinusNode " << endl;
	delete FactorPtr;
	FactorPtr = nullptr;
}
void MinusFactorNode::printTo(ostream& os) {
	os << "( -" << *FactorPtr << " ) ";
}
float MinusFactorNode::interpret() {
	return 0 - FactorPtr->interpret();
}
//-------------------------------------------------------------------------------------
TermNode::TermNode(int level) {
	_level = level;
}
TermNode::~TermNode() {
	if (printDelete)
		cout << "Deleting TermNode " << endl;
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}
ostream& operator<<(ostream& os, TermNode& tn) {
	os << endl; indent(tn._level); os << "(term ";
	os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = tn.restFactorOps[i];
		if (op == TOK_MULTIPLY) {
			os << endl; indent(tn._level); os << "* ";
		}
		else if (op == TOK_DIVIDE) {
			os << endl; indent(tn._level); os << "/ ";
		}
		else {
			os << endl; indent(tn._level); os << "AND ";
		}
		os << *(tn.restFactors[i]);
	}
	os << endl; indent(tn._level); os << "term) ";
	return os;
}
float TermNode::interpret()
{

	float returnValue = firstFactor->interpret();

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		// get the value of the next Factor
		float nextValue = restFactors[i]->interpret();

		// perform the operation (* or /) that separates the Factors
		switch (restFactorOps[i]) {
		case TOK_MULTIPLY:
			returnValue = returnValue * nextValue;
		break;
		case TOK_DIVIDE:
			returnValue = returnValue / nextValue;
		break;
		case TOK_AND:
			if (truth(returnValue) && truth(nextValue)) {
				returnValue = 1.0;
			}
			else
				returnValue = 0.0;
		break;
		}
	}
	return returnValue;
}
// ---------------------------------------------------------------------
SimpExprNode::SimpExprNode(int level) {
	_level = level;
}
SimpExprNode::~SimpExprNode() {
	if (printDelete)
		cout << "Deleting SimpleExprNode " << endl;
	delete TermPtr;
	TermPtr = nullptr;

	int length = TermOps.size();
	for (int i = 0; i < length; ++i) {
		delete Terms[i];
		Terms[i] = nullptr;
	}

}
ostream& operator<<(ostream& os, SimpExprNode& sn) {
	os << endl; indent(sn._level); os << "(simple_exp ";
	os << *(sn.TermPtr);

	int length = sn.TermOps.size();
	for (int i = 0; i < length; ++i) {
		int op = sn.TermOps[i];
		if (op == TOK_PLUS) {
			os << endl; indent(sn._level); os << "+ ";
		}
		else if (op == TOK_MINUS) {
			os << endl; indent(sn._level); os << "- ";
		}
		else {
			os << endl; indent(sn._level); os << "OR ";
		}
		os << *(sn.Terms[i]);
	}
	os << endl; indent(sn._level); os << "simple_exp) ";
	return os;
}
float SimpExprNode::interpret() {
	float returnValue = TermPtr->interpret();

	int length = TermOps.size();
	for (int i = 0; i < length; ++i) {
		// get the value of the next Factor
		float nextValue = Terms[i]->interpret();

		// perform the operation (* or /) that separates the Factors
		switch (TermOps[i]) {
		case TOK_PLUS:
			returnValue = returnValue + nextValue;
		break;
		case TOK_MINUS:
			returnValue = returnValue - nextValue;
		break;
		case TOK_OR:
			if (truth(returnValue) || truth(nextValue)) {
				returnValue = 1.0;
			}
			else
				returnValue = 0.0;
		break;
		}
	}
	return returnValue;
}
// ---------------------------------------------------------------------

ExprNode::ExprNode(int level) {
	_level = level;
}
ostream& operator<<(ostream& os, ExprNode& en) {
	os << endl; indent(en._level); os << "(expr ";
	os << *(en.SimpExprPtr);

	int length = en.SimpOps.size();
	for (int i = 0; i < length; ++i) {
		int op = en.SimpOps[i];
		if (op == TOK_EQUALTO) {
			os << endl; indent(en._level); os << "= ";
		}
		else if (op == TOK_GREATERTHAN) {
			os << endl; indent(en._level); os << "> ";
		}
		else if (op == TOK_LESSTHAN) {
			os << endl; indent(en._level); os << "< ";
		}
		else {
			os << endl; indent(en._level); os << "<> ";
		}
		os << *(en.SimpExpr[i]);
	}
	os << endl; indent(en._level); os << "expr) ";
	return os;
}
ExprNode::~ExprNode() {
	if (printDelete)
		cout << "Deleting ExprNode " << endl;
	delete SimpExprPtr;
	SimpExprPtr = nullptr;

	int length = SimpOps.size();
	for (int i = 0; i < length; ++i) {
		delete SimpExpr[i];
		SimpExpr[i] = nullptr;
	}
}
float ExprNode::interpret()
{
	float firstValue = SimpExprPtr->interpret();
	float returnValue = 0.0;

	int length = SimpOps.size();
	if (length == 0) {
		return firstValue;
	}
	else {
		for (int i = 0; i < length; ++i) {
			// get the value of the next Term
			float nextValue = SimpExpr[i]->interpret();

			// perform the operation (+ or -) that separates the Terms
			switch (SimpOps[i]) {
			case TOK_EQUALTO:
				if (abs(firstValue - nextValue) <= EPSILON) {
					returnValue = 1.0;
				}
				else
					returnValue = 0.0;
			break;
			case TOK_LESSTHAN:
				if (firstValue < nextValue) {
					returnValue = 1.0;
				}
				else
					returnValue = 0.0;
			break;
			case TOK_GREATERTHAN:
				if (firstValue > nextValue) {
					returnValue = 1.0;
				}
				else
					returnValue = 0.0;
			break;
			case TOK_NOTEQUALTO:
				if (abs(firstValue - nextValue) > EPSILON) {
					returnValue = 1.0;
				}
				else
					returnValue = 0.0;
			break;
			}
		}
		return returnValue;
	}

}
// ---------------------------------------------------------------------
StatementNode::~StatementNode() {}
ostream& operator <<(ostream& os, StatementNode& sn) {
	sn.printTo(os);
	return os;
}
void StatementNode::interpret() {
	this->interpret();
}
// ---------------------------------------------------------------------
AssignNode::AssignNode(int level, string name) {
	_level = level;
	id = new string(name);
}
AssignNode::~AssignNode() {
	if (printDelete) {
		cout << "Deleting:StatementNode:AssignmentStmtNode" << endl;
	}
	delete id;
	id = nullptr;

	delete ExprPtr;
	ExprPtr = nullptr;
}
void AssignNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(assignment_stmt ";
	os << "( " << *id << " := )";
	os << *ExprPtr;
	os << endl; indent(_level); os << "assignment_stmt)";
}
void AssignNode::interpret() {
	symbolTableT::iterator variable = symbolTable.find(*id);
	variable->second = ExprPtr->interpret();

}
// ---------------------------------------------------------------------
CompoundNode::CompoundNode(int level) {
	_level = level;
}
CompoundNode::~CompoundNode() {
	if (printDelete)
		cout << "Deleting:StatementNode:CompoundStmtNode" << endl;
	delete StatementPtr;
	StatementPtr = nullptr;

	int length = CompoundOps.size();
	for (int i = 0; i < length; ++i) {
		delete Stmts[i];
		Stmts[i] = nullptr;
	}
}
void CompoundNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(compound_stmt ";
	os << *StatementPtr;

	int length = CompoundOps.size();
	for (int i = 0; i < length; i++) {
		os << *(Stmts[i]);
	}
	os << endl; indent(_level); os << "compound_stmt)";

}
void CompoundNode::interpret() {
	StatementPtr->interpret();

	int length = Stmts.size();
	for (int i = 0; i < length; i++) {
		Stmts[i]->interpret();
	}
}
// ---------------------------------------------------------------------
IfNode::IfNode(int level) {
	_level = level;
}
IfNode::~IfNode() {
	if (printDelete) {
		cout << "Deleting:StatementNode:IfStmtNode" << endl;
	}
	delete StmtPtr;
	StmtPtr = nullptr;

	delete ExprPtr;
	ExprPtr = nullptr;

	delete secondstmt;
	secondstmt = nullptr;
}
void IfNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(if_stmt ";
	os << *ExprPtr;
	os << endl; indent(_level); os << "(then ";
	os << *StmtPtr;
	os << endl; indent(_level); os << "then) ";


	if (IfOps.size() == 3) {
		os << endl; indent(_level); os << "(else ";
		os << *secondstmt;
		os << endl; indent(_level); os << "else) ";

	}
	os << endl; indent(_level); os << "if_stmt) ";
}
void IfNode::interpret(){
	cout << endl;
	if (truth(ExprPtr->interpret())) {
		StmtPtr->interpret();
	}
	else {
		secondstmt->interpret();
	}
	cout << endl;
}
// ----------------------------------------------------------------------
WhileNode::WhileNode(int level) {
	_level = level;
}
WhileNode::~WhileNode() {
	if (printDelete) {
		cout << "Deleting:StatementNode:WhileStmtNode" << endl;
	}
	delete StmtPtr;
	StmtPtr = nullptr;

	delete ExprPtr;
	ExprPtr = nullptr;
}
void WhileNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(while_stmt ";
	os << *ExprPtr;
	os << *StmtPtr;
	os << endl; indent(_level); os << "while_stmt)";
}
void WhileNode::interpret() {
	while (truth(ExprPtr->interpret())) {
		StmtPtr->interpret();
	}
}
// ----------------------------------------------------------------------
ReadNode::ReadNode(int level, string name) {
	_level = level;
	id = new string(name);
}
ReadNode::~ReadNode() {
	if (printDelete) {
		cout << "Deleting:StatementNode:ReadStmtNode" << endl;
	}
	delete id;
	id = nullptr;

}
void ReadNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(read_stmt ";
	os << "( " << *id << " )";
	os << endl; indent(_level); os << "read_stmt)";
}
void ReadNode::interpret() {
	symbolTableT::iterator variable = symbolTable.find(*id);
	cin >> variable->second;		
}
// ----------------------------------------------------------------------
WriteNode::WriteNode(int level, string name, int num) {
	_level = level;
	id = new string(name);
	token = num;
}
WriteNode::~WriteNode() {
	if (printDelete) {
		cout << "Deleting:StatementNode:WriteStmtNode" << endl;
	}
	delete id;
	id = nullptr;

}
void WriteNode::printTo(ostream& os) {
	os << endl; indent(_level); os << "(write_stmt ";
	os << "( " << *id << " )";
	os << endl; indent(_level); os << "write_stmt)";
}

void WriteNode::interpret() {
	
	if (token == TOK_STRINGLIT) {
		string output = *id;
		cout << output.substr(1, output.size() - 2) << endl;
	}
	else {
		symbolTableT::iterator variable = symbolTable.find(*id);
		cout << variable->second << endl;
	}
}

BlockNode::BlockNode(int level) {
	_level = level;
}
BlockNode::~BlockNode() {
	if (printDelete)
		cout << "Deleting BlockNode " << endl;
	delete CompoundPtr;
	CompoundPtr = nullptr;

}
ostream& operator<<(ostream& os, BlockNode& bn) {
	os << endl; indent(bn._level); os << "(block ";
	os << *(bn.CompoundPtr);
	os << endl; indent(bn._level); os << "block) ";
	return os;
}

void BlockNode::interpret() {
	CompoundPtr->interpret();
}

//------------------------------------------------------------------------
ProgramNode::ProgramNode(int level) {
	_level = level;
}
ProgramNode::~ProgramNode() {
	if (printDelete)
		cout << "Deleting ProgramNode " << endl;
	delete BlockPtr;
	BlockPtr = nullptr;

}
ostream& operator<<(ostream& os, ProgramNode& pn) {
	os << endl; indent(pn._level); os << "(program ";
	os << *(pn.BlockPtr);
	os << endl; indent(pn._level); os << "program) ";
	return os;
}

void ProgramNode::interpret() {
	BlockPtr->interpret();
}