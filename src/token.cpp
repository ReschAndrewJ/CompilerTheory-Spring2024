#include "header.h"


Token::Token() {
    text = "";
    type = TokenType::UNASSIGNED;
    value = 0;
}

Token::Token(std::string text, TokenType::TokenType ttype, std::variant<int, float, std::string> value) {
    this->text = text;
    type = ttype;
    this->value = value; 
}



