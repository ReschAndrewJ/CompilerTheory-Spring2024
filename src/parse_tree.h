#ifndef PARSE_TREE_DESCRIPTION
#define PARSE_TREE_DESCRIPTION

#include "enums.h"

#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <memory>


struct Declaration; 
struct Statement;
struct Expression;


struct Resolution {
    std::string outputRegister;
    Typemark::Typemark resolvedType = Typemark::UNASSIGNED;
    TypeShape::TypeShape resolvedShape = TypeShape::UNASSIGNED; // VALUE or ARRAY, cannot be FUNCTION
    unsigned resolvedArraySize = 0; // only used if shape is ARRAY
};


struct Procedure_Call {
    std::string identifier;
    std::vector<Expression> parameters;
};
struct Variable_Access {
    Resolution resolution;
    std::string identifier;
    std::optional<std::unique_ptr<Expression>> array_index;
};


struct Factor_VariableAccess {
    bool minus=false;
    Variable_Access variable;
};


struct Factor { 
    Resolution resolution;
    std::variant<
        std::unique_ptr<Expression>,
        Procedure_Call, 
        Factor_VariableAccess, 
        int, float, 
        std::string, 
        bool 
    > factor;
};


struct TermPrime {
    Resolution resolution;
    TermOperator::TermOperator term_op;
    Factor factor;
    std::unique_ptr<TermPrime> prime=nullptr;
};
struct Term {
    Resolution resolution;
    Factor factor;
    std::unique_ptr<TermPrime> prime=nullptr;
};


struct RelationPrime {
    Resolution resolution;
    RelationOperator::RelationOperator relation_op;
    Term term;
    std::unique_ptr<RelationPrime> prime=nullptr;
};
struct Relation {
    Resolution resolution;
    Term term;
    std::unique_ptr<RelationPrime> prime=nullptr;
};


struct ArithOpPrime {
    Resolution resolution;
    ArithmaticOperator::ArithmaticOperator arithmatic_op;
    Relation relation;
    std::unique_ptr<ArithOpPrime> prime=nullptr;
};
struct ArithOp {
    Resolution resolution;
    Relation relation;
    std::unique_ptr<ArithOpPrime> prime=nullptr;
};


struct ExpressionPrime {
    Resolution resolution;
    BinaryBitwise::BinaryBitwise bitwise_op;
    ArithOp arithOp;

    std::unique_ptr<ExpressionPrime> prime=nullptr;
};
struct Expression {
    Resolution resolution;
    bool bitwise_not;
    ArithOp arithOp;
    
    std::unique_ptr<ExpressionPrime> prime=nullptr;
};


struct Assignment_Statement {
    Variable_Access variable;
    Expression assignment;
};
struct If_Statement {
    Expression condition;
    std::vector<Statement> then_body;
    std::vector<Statement> else_body;
};
struct For_Statement {
    Assignment_Statement initial;
    Expression condition;
    std::vector<Statement> body;
};
struct Return_Statement {
    Expression expression;
};


struct Procedure_Body {
    std::vector<Declaration> declarations;
    std::vector<Statement> statements;
};

struct Parameter {
    std::string identifier;
    Typemark::Typemark type;
    bool isArray=false;
    unsigned bound=0;
};
struct Variable_Information {
    std::string identifier;
    bool isGlobal;
    Typemark::Typemark type;
    bool isArray=false;
    unsigned bound=0;
};
struct Procedure_Information {
    std::string identifier;
    bool isGlobal;
    Typemark::Typemark returnType;
    std::vector<Parameter> parameters;
    Procedure_Body body;
};

struct Declaration {
    std::variant<Variable_Information, Procedure_Information> declaration;
};
struct Statement {
    std::variant<Assignment_Statement, If_Statement, For_Statement, Return_Statement> statement; 
};


struct Program_Header { std::string identifier; };
struct Program_Body {
    std::vector<Declaration> declarations;
    std::vector<Statement> statements;
};


struct Program_Parse {
    Program_Header header;
    Program_Body body;
};




#endif