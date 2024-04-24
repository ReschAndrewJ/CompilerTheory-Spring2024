#include "enums.h"


std::string TypemarkAsString(Typemark::Typemark mark) {
    std::string mark_string = "";
    switch (mark) {
        case Typemark::UNASSIGNED:
            mark_string = "UNASSIGNED";
            break;
        case Typemark::INTEGER:
            mark_string = "INTEGER";
            break;
        case Typemark::FLOAT:
            mark_string = "FLOAT";
            break;
        case Typemark::BOOL:
            mark_string = "BOOL";
            break;
        case Typemark::STRING:
            mark_string = "STRING";
            break;
        default:
            mark_string = "INVALID_TYPEMARK";
            break;
    }
    return mark_string;
}


std::string TypeShapeAsString(TypeShape::TypeShape shape) {
    std::string shape_string = "";
    switch (shape) {
        case TypeShape::UNASSIGNED:
            shape_string = "UNASSIGNED";
            break;
        case TypeShape::VALUE:
            shape_string = "VALUE";
            break;
        case TypeShape::ARRAY:
            shape_string = "ARRAY";
            break;
        case TypeShape::FUNCTION:
            shape_string = "FUNCTION";
            break;
        default:
            shape_string = "INVALID_TYPESHAPE";
            break;
    }  
    return shape_string;
}



std::string TokenTypeAsString(TokenType::TokenType ttype) {
    std::string ttype_string = "";
    switch (ttype)
        {
        case TokenType::UNASSIGNED:
            ttype_string = "UNASSIGNED";
            break;
        case TokenType::PLUS:
            ttype_string = "PLUS";
            break;
        case TokenType::MINUS:
            ttype_string = "MINUS";
            break;
        case TokenType::TIMES:
            ttype_string = "TIMES";
            break;
        case TokenType::DIVIDE:
            ttype_string = "DIVIDE";
            break;
        case TokenType::GREATER_THAN:
            ttype_string = "GREATER_THAN";
            break;
        case TokenType::LESS_THAN:
            ttype_string = "LESS_THAN";
            break;
        case TokenType::GREATER_EQUAL:
            ttype_string = "GREATER_EQUAL";
            break;
        case TokenType::LESS_EQUAL:
            ttype_string = "LESS_EQUAL";
            break;
        case TokenType::EQUAL_TO:
            ttype_string = "EQUAL_TO";
            break;
        case TokenType::NON_EQUAL:
            ttype_string = "NON_EQUAL";
            break;
        case TokenType::BITWISE_AND:
            ttype_string = "BITWISE_AND";
            break;
        case TokenType::BITWISE_OR:
            ttype_string = "BITWISE_OR";
            break;
        case TokenType::BITWISE_NOT:
            ttype_string = "BITWISE_NOT";
            break;
        case TokenType::LEFT_PARENTHESIS:
            ttype_string = "LEFT_PARENTHESIS";
            break;
        case TokenType::RIGHT_PARENTHESIS:
            ttype_string = "RIGHT_PARENTHESIS";
            break;
        case TokenType::LEFT_BRACKET:
            ttype_string = "LEFT_BRACKET";
            break;
        case TokenType::RIGHT_BRACKET:
            ttype_string = "RIGHT_BRACKET";
            break;
        case TokenType::SEMICOLON:
            ttype_string = "SEMICOLON";
            break;
        case TokenType::COLON:
            ttype_string = "COLON";
            break;
        case TokenType::PERIOD:
            ttype_string = "PERIOD";
            break;
        case TokenType::COMMA:
            ttype_string = "COMMA";
            break;
        case TokenType::ASSIGN:
            ttype_string = "ASSIGN";
            break;
        case TokenType::IDENTIFIER:
            ttype_string = "IDENTIFIER";
            break;
        case TokenType::TYPEMARK_INTEGER:
            ttype_string = "TYPEMARK_INTEGER";
            break;
        case TokenType::TYPEMARK_FLOAT:
            ttype_string = "TYPEMARK_FLOAT";
            break;
        case TokenType::TYPEMARK_BOOL:
            ttype_string = "TYPEMARK_BOOL";
            break;
        case TokenType::TYPEMARK_STRING:
            ttype_string = "TYPEMARK_STRING";
            break;
        case TokenType::INTEGER_CONSTANT:
            ttype_string = "INTEGER_CONSTANT";
            break;
        case TokenType::FLOAT_CONSTANT:
            ttype_string = "FLOAT_CONSTANT";
            break;
        case TokenType::BOOL_CONSTANT:
            ttype_string = "BOOL_CONSTANT";
            break;
        case TokenType::STRING_CONSTANT:
            ttype_string = "STRING_CONSTANT";
            break;
        case TokenType::IF:
            ttype_string = "IF";
            break;
        case TokenType::THEN:
            ttype_string = "THEN";
            break;
        case TokenType::ELSE:
            ttype_string = "ELSE";
            break;
        case TokenType::FOR:
            ttype_string = "FOR";
            break;
        case TokenType::RETURN:
            ttype_string = "RETURN";
            break;
        case TokenType::BEGIN:
            ttype_string = "BEGIN";
            break;
        case TokenType::END:
            ttype_string = "END";
            break;
        case TokenType::PROGRAM:
            ttype_string = "PROGRAM";
            break;
        case TokenType::PROCEDURE:
            ttype_string = "PROCEDURE";
            break;
        case TokenType::VARIABLE:
            ttype_string = "VARIABLE";
            break;
        case TokenType::IS:
            ttype_string = "IS";
            break;
        case TokenType::GLOBAL:
            ttype_string = "GLOBAL";
            break;
        case TokenType::END_OF_FILE:
            ttype_string = "END_OF_FILE";
            break;
        default:
            ttype_string = "INVALID_TOKEN_TYPE";
            break;
        }
    return ttype_string;
}