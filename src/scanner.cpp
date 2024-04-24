#include "header.h"

#include <cstring>


bool token_buffered = false;
Token buffer;

Token buffer_scan() {
    if (token_buffered) return buffer;
    buffer = scan();
    token_buffered = true;
    return buffer;
}


Token scan() {
    if (token_buffered) {
        token_buffered = false;
        return buffer;
    }

    char nextChar = inFile.getChar();

    // handle leading whitespace and comments
    while (
        nextChar == '\t' || nextChar == '\r' || nextChar == ' ' ||
        nextChar == '\n' || 
        (nextChar == '/' && inFile.peekChar() == '/') || 
        (nextChar == '/' && inFile.peekChar() == '*')
    ) {
        switch (nextChar) {
            case '\t': case '\r': case ' ':
                nextChar = inFile.getChar();
                break;
            
            case '\n':
                inFile.incrementLineCount();
                nextChar = inFile.getChar();
                break;
            
            case '/':
                if (inFile.peekChar() == '/') {
                    inFile.getChar();
                    nextChar = inFile.getChar();
                    while (nextChar != '\n' && nextChar != EOF) {
                        nextChar = inFile.getChar();
                    }
                    if (nextChar == '\n') {
                        inFile.incrementLineCount();
                        nextChar = inFile.getChar();
                    }
                }
                else { //inFile.peekChar() == '*'
                    unsigned comment_depth = 1;
                    inFile.getChar();
                    nextChar = inFile.getChar();

                    while (comment_depth && nextChar != EOF) {
                        if (nextChar == '/' && inFile.peekChar() == '*') {
                            ++comment_depth;
                            inFile.getChar();
                            nextChar = inFile.getChar();
                        }
                        else if (nextChar == '*' && inFile.peekChar() == '/') {
                            --comment_depth;
                            inFile.getChar();
                            nextChar = inFile.getChar();
                        }
                        else if (nextChar == '\n') {
                            inFile.incrementLineCount();
                            nextChar = inFile.getChar();
                        }
                        else {
                            nextChar = inFile.getChar();
                        }
                    }
                }
                break;
            default: // should be impossible to reach
                break;
        }        
    }
    
    
    switch (nextChar) {
        case '+':
            return Token("+", TokenType::PLUS);
        case '-':
            return Token("-", TokenType::MINUS);
        case '*':
            return Token("*", TokenType::TIMES);
        case '/':
            return Token("/", TokenType::DIVIDE);
        case '>':
            if (inFile.peekChar() == '=') {
                inFile.getChar();
                return Token(">=", TokenType::GREATER_EQUAL);
            }
            else { return Token(">", TokenType::GREATER_THAN); }
        case '<':
            if (inFile.peekChar() == '=') {
                inFile.getChar();
                return Token("<=", TokenType::LESS_EQUAL);
            }
            else { return Token("<", TokenType::LESS_THAN); }
        case '!':
            if (inFile.peekChar() == '=') {
                inFile.getChar();
                return Token("!=", TokenType::NON_EQUAL);
            }
            else { // error
                error_reporting.ReportError("Standalone Character '!' Encountered");
                return scan();
            } 
        case '=':
            if (inFile.peekChar() == '=') {
                inFile.getChar();
                return Token("==", TokenType::EQUAL_TO);
            }
            else { // error
                error_reporting.ReportError("Standalone Character '=' encountered");
                return scan();
            }
        case '&':
            return Token("&", TokenType::BITWISE_AND);
        case '|':
            return Token("|", TokenType::BITWISE_OR);
        case '(':
            return Token("(", TokenType::LEFT_PARENTHESIS);
        case ')':
            return Token(")", TokenType::RIGHT_PARENTHESIS);
        case '[':
            return Token("[", TokenType::LEFT_BRACKET);
        case ']':
            return Token("]", TokenType::RIGHT_BRACKET);
        case ';':
            return Token(";", TokenType::SEMICOLON);
        case ':':
            if (inFile.peekChar() == '=') {
                inFile.getChar();
                return Token(":=", TokenType::ASSIGN);
            }
            else { return Token(":", TokenType::COLON); }
        case '.':
            return Token(".", TokenType::PERIOD);
        case ',':
            return Token(",", TokenType::COMMA);
        case '"': {
            std::string value = "";
            if (inFile.peekChar() == EOF) {
                // error
                error_reporting.ReportError("End of File encountered before end of string");
                return Token(value, TokenType::STRING_CONSTANT, value);
            }
            nextChar = inFile.getChar();
            while (nextChar != '"') {
                value.push_back(nextChar);
                if (inFile.peekChar() == EOF) {
                    // error
                    error_reporting.ReportError("End of File encountered before end of string");
                    return Token(value, TokenType::STRING_CONSTANT, value);
                }
                nextChar = inFile.getChar();
            }

            return Token("\"" + value + "\"", TokenType::STRING_CONSTANT, value);
        }
        case EOF:
            return Token("EOF", TokenType::END_OF_FILE);
        default: 
            if (
                (nextChar >= 'a' && nextChar <='z') || 
                (nextChar >= 'A' && nextChar <= 'Z')
            ) {
                if (nextChar >= 'A' && nextChar <='Z') { nextChar += ('a' - 'A'); } // tolower
                std::string value = {nextChar};
                while (
                    (inFile.peekChar() >= 'a' && inFile.peekChar() <= 'z') ||
                    (inFile.peekChar() >= 'A' && inFile.peekChar() <= 'Z') ||
                    (inFile.peekChar() >= '0' && inFile.peekChar() <= '9') ||
                    (inFile.peekChar() == '_')
                ) {
                    nextChar = inFile.getChar();
                    if (nextChar >= 'A' && nextChar <='Z') { nextChar += ('a' - 'A'); } // tolower
                    value.push_back(nextChar);
                }
                
                unsigned length = value.length();
                if (length >= 2 && length <= 9) {
                    for (unsigned index = keyword_offsets[length-2]; index < keyword_offsets[length-1]; ++index) {
                        if (strcmp(std::get<0>(keywords[index]).c_str(), value.c_str()) == 0) {
                            return Token(value, std::get<1>(keywords[index]), std::get<2>(keywords[index]));
                        }
                    }
                }

                return Token(value, TokenType::IDENTIFIER, value);
            }
            else if ( 
                (nextChar >= '0' && nextChar <= '9') ||
                (nextChar == '_')
            ) {
                std::string valueString = "";
                if (nextChar != '_') { valueString = {nextChar}; }
                while (
                    (inFile.peekChar() >= '0' && inFile.peekChar() <= '9') ||
                    (inFile.peekChar() == '_')
                ) {
                    nextChar = inFile.getChar();
                    if (nextChar != '_') { valueString.push_back(nextChar); }
                }
                if (inFile.peekChar() != '.') {
                    int value = atoi(valueString.c_str());
                    return Token(valueString, TokenType::INTEGER_CONSTANT, value);
                } else {
                    nextChar = inFile.getChar();
                    valueString.push_back(nextChar);
                    while (
                        (inFile.peekChar() >= '0' && inFile.peekChar() <= '9') ||
                        (inFile.peekChar() == '_')
                    ) {
                        nextChar = inFile.getChar();
                        if (nextChar != '_') { valueString.push_back(nextChar); }
                    }
                    
                    float value = atof(valueString.c_str());
                    return Token(valueString, TokenType::FLOAT_CONSTANT, value);
                } 
            }
            else { // invalid character
                error_reporting.ReportError("Invalid character '" + std::string{nextChar} +"'");
                return scan();
            } 
    }


}