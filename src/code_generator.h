#ifndef CODE_GENERATOR
#define CODE_GENERATOR


#include "header.h"


void codegen_builtinFunctions();

void codegen_variableDeclaration(Variable_Information&);

void codegen_procedureHeader(Symbol*);
void codegen_procedureEnd();

void codegen_beginMain();
void codegen_endMain();

// statements

void codegen_assignmentStatement(Variable_Access&, Expression&, TypeResolutionCase::TypeResolutionCase);
//void codegen_ifStatement();
std::pair<std::string/*else_block*/, std::string/*end_block*/> codegen_ifStatementCondition(const Resolution&); 
void codegen_ifStatementElse(std::string elseLabel, std::string endLabel);
void codegen_ifStatementEnd(std::string endLabel);
//void codegen_forStatement();
std::tuple<std::string/*cond_block*/, std::string/*body_block*/, std::string/*end_block*/> codegen_forStatementConditionStart();
void codegen_forStatementBodyStart(std::string bodyLabel, std::string endLabel, const Resolution&);
void codegen_forStatementEnd(std::string condLabel, std::string endLabel);
void codegen_returnStatement(Expression&, Typemark::Typemark);

// expressions

void codegen_factor(Factor&);
//void codegen_term(Term&); not needed
void codegen_termPrime(const Resolution&, const Resolution&, Resolution&, TermOperator::TermOperator);
//void codegen_relation(Relation&); not needed
void codegen_relationPrime(const Resolution&, const Resolution&, Resolution&, RelationOperator::RelationOperator);
//void codegen_arithOp();
void codegen_arithOpPrime(const Resolution&, const Resolution&, Resolution&, ArithmaticOperator::ArithmaticOperator);
//void codegen_expression();
void codegen_expressionBitwiseNot(Resolution&);
void codegen_expressionPrime(const Resolution&, const Resolution&, Resolution&, BinaryBitwise::BinaryBitwise);

//
void codegen_variableAccess(Variable_Access&);



#endif