#ifndef HEADER
#define HEADER

#include "enums.h"
#include "parse_tree.h"

#include <string>
#include <stdio.h>
#include <unordered_map>
#include <variant>


extern struct InputFile {
    private:
        FILE* filePtr = nullptr;
        std::string filename;
        unsigned lineCount = 1;
    
    public:
        bool attachFile(const char*);
        char getChar();
        char peekChar() const;
        void incrementLineCount();
        unsigned getLineCount() const;
        std::string getFilename() const;
} inFile;


extern struct Reporting {
    private:
        ErrorStatus::ErrorStatus status = ErrorStatus::NO_ERROR;
        bool awaitingResync = false;
    public:
        void ReportError(std::string message);
        void ReportWarning(std::string message);
        void Resync();
        bool AwaitingResync();
        ErrorStatus::ErrorStatus GetErrorStatus() const;
} error_reporting;


// keywords sorted by length for fast lookup in lexer since strings can not be used in switch statements
// an unnecessary optimization, but fun
const std::tuple<std::string, TokenType::TokenType, std::variant<int, float, std::string>> keywords[] = {
    {"if", TokenType::IF, 0},                      //2, 0
    {"is", TokenType::IS, 0},                      //2, 0
    {"for", TokenType::FOR, 0},                    //3, 2
    {"end", TokenType::END, 0},                    //3, 2
    {"not", TokenType::BITWISE_NOT, 0},            //3, 2
    {"then", TokenType::THEN, 0},                  //4, 5
    {"else", TokenType::ELSE, 0},                  //4, 5
    {"bool", TokenType::TYPEMARK_BOOL, 0},         //4, 5
    {"true", TokenType::BOOL_CONSTANT, 1},         //4, 5
    {"false", TokenType::BOOL_CONSTANT, 0},        //5, 9
    {"begin", TokenType::BEGIN, 0},                //5, 9
    {"float", TokenType::TYPEMARK_FLOAT, 0},       //5, 9
    {"return", TokenType::RETURN, 0},              //6, 12
    {"string", TokenType::TYPEMARK_STRING, 0},     //6, 12
    {"global", TokenType::GLOBAL, 0},              //6, 12
    {"program", TokenType::PROGRAM, 0},            //7, 15
    {"integer", TokenType::TYPEMARK_INTEGER, 0},   //7, 15
    {"variable", TokenType::VARIABLE, 0},          //8, 17
    {"procedure", TokenType::PROCEDURE, 0},        //9, 18
};
const unsigned keyword_offsets[] = {
    0,  //2
    2,  //3
    5,  //4
    9,  //5
    12, //6
    15, //7
    17, //8
    18, //9
    19 //after-end
    };


struct Token {
    public:
        std::string text = "";
        TokenType::TokenType type = TokenType::UNASSIGNED;

        std::variant<int, float, std::string> value;
        
        Token();
        Token(std::string text, TokenType::TokenType ttype, std::variant<int, float, std::string> value=0);
};



struct Symbol { 
    std::string llvmName = "";
    std::string name = "";
    TypeShape::TypeShape shape = TypeShape::UNASSIGNED;
    Typemark::Typemark type = Typemark::UNASSIGNED;
    
    unsigned array_size = 0;
    std::vector<Symbol*> function_parameters; 
};

extern struct SymbolTable {
    private: 
        struct Scope {
            std::string name;
            std::unordered_map<std::string, Symbol> table;
            Symbol* selfProcedureSymbol = nullptr;
        };
        std::vector<Scope> table_stack {};
        std::vector<Scope> dead_scopes {};
    public:
        Symbol* declare_local(std::string); // symbol will be created but data must still be set 
        Symbol* declare_global(std::string); // symbol will be created but data must still be set
        Symbol* lookup(std::string);  
        std::string getCurrentScopeFullName();

        void enterScope(std::string scopeName, Symbol* self = nullptr);
        void exitScope();
} symbol_table;



extern struct ConstantTable_string {
    private:
        std::unordered_map<std::string, Token> table;
    public:
        Token* lookup(std::string);
} constantTable_string;
extern struct ConstantTable_int {
    private:
        std::unordered_map<int, Token> table;
    public:
        Token* lookup(int);
} constantTable_int;
extern struct ConstantTable_float {
    private:
        std::unordered_map<float, Token> table;
    public:
        Token* lookup(float);
} constantTable_float;


extern struct LlvmAssemblyStrings {
    std::string builtinFunctionDeclarations = "";
    std::string globalVariableDeclarations = "";
    private:
        unsigned next_global_unnamed_reg = 0;
    public:
        inline std::string GetNextGlobalUnnamedReg() {return "@" + std::to_string(next_global_unnamed_reg++);}
    struct FunctionString{
        std::string llvm = "";
        private:
            unsigned next_unnamed_reg = 0;
            unsigned next_block_label = 0;
        public:
            inline std::string GetNextUnnamedReg() {return "%" + std::to_string(next_unnamed_reg++);}
            inline std::string GetNextBlockLabel() {return "b" + std::to_string(next_block_label++);}
    };
    std::unordered_map<std::string, FunctionString> functionDefinitions; 
    // order of function definitions does not matter for llvm assembly
} llvmAssemblyStrings;

Token scan();
Token buffer_scan(); // scans the next token and leaves it in a buffer for the next scan or buffer_scan call; like peek
Program_Parse parse();



#endif