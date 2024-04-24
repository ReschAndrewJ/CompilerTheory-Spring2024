#ifndef ENUMS
#define ENUMS

#include <string>

namespace ErrorStatus { enum ErrorStatus {
    NO_ERROR,
    WARNING,
    ERROR
}; }

namespace Typemark { enum Typemark {UNASSIGNED, INTEGER, FLOAT, BOOL, STRING}; }
namespace TypeShape { enum TypeShape {UNASSIGNED, VALUE, ARRAY, FUNCTION}; }

namespace BinaryBitwise { enum BinaryBitwise {AND, OR}; }
namespace ArithmaticOperator { enum ArithmaticOperator {PLUS, MINUS}; }
namespace RelationOperator { enum RelationOperator {LESS, GREATER, LESS_EQUAL, GREATER_EQUAL, EQUAL, NON_EQUAL}; }
namespace TermOperator { enum TermOperator {TIMES, DIVIDE}; }

namespace TypeResolutionCase { enum TypeResolutionCase {
    invalid,
    straight,
    int_to_float,
    float_to_int,
    int_to_bool,
    bool_to_int
}; }

namespace TokenType { enum TokenType {
    UNASSIGNED,
    PLUS, MINUS,
    TIMES, DIVIDE,
    GREATER_THAN, LESS_THAN, GREATER_EQUAL, LESS_EQUAL, EQUAL_TO, NON_EQUAL,
    BITWISE_AND, BITWISE_OR, BITWISE_NOT,
    LEFT_PARENTHESIS, RIGHT_PARENTHESIS, 
    LEFT_BRACKET, RIGHT_BRACKET,
    SEMICOLON, COLON, PERIOD, COMMA,
    ASSIGN,
    IDENTIFIER,
    TYPEMARK_INTEGER, TYPEMARK_FLOAT, TYPEMARK_BOOL, TYPEMARK_STRING,
    INTEGER_CONSTANT, FLOAT_CONSTANT, BOOL_CONSTANT, STRING_CONSTANT,
    IF, THEN, ELSE, FOR, RETURN, BEGIN, END, 
    PROGRAM, PROCEDURE, VARIABLE, IS,
    GLOBAL,
    END_OF_FILE
}; }


std::string TypemarkAsString(Typemark::Typemark);
std::string TypeShapeAsString(TypeShape::TypeShape);

std::string TokenTypeAsString(TokenType::TokenType);

#endif