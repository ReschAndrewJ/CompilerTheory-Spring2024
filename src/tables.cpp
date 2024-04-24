#include "header.h"



Token* ConstantTable_string::lookup(std::string value) {
    if (!table.count(value)) {
        return &(table[value] = Token(value, TokenType::STRING_CONSTANT, value) );
    }
    return &table[value];
}

Token* ConstantTable_int::lookup(int value) {
    if (!table.count(value)) {
        return &(table[value] = Token(std::to_string(value), TokenType::INTEGER_CONSTANT, value) );
    }
    return &table[value];
}

Token* ConstantTable_float::lookup(float value) {
    if (!table.count(value)) {
        return &(table[value] = Token(std::to_string(value), TokenType::FLOAT_CONSTANT, value) );
    }
    return &table[value];
}




Symbol* SymbolTable::declare_global(std::string name) {
    if (!table_stack.front().table.count(name)) {
        Symbol new_symbol;
        new_symbol.name = name;
        return &(table_stack.front().table[name] = new_symbol);
    }
    return nullptr;
}

Symbol* SymbolTable::declare_local(std::string name) {
    if (!table_stack.back().table.count(name)) {
        Symbol new_symbol;
        new_symbol.name = name;
        return &(table_stack.back().table[name] = new_symbol);
    }
    return nullptr;
}

Symbol* SymbolTable::lookup(std::string name) {
    // commented out method for nested functions
    // nested functions in llvm would be difficult to implement
    /*for (unsigned i=table_stack.size()-1; i != -1; --i) { // iterate backwards through local scopes to global scope
        if (table_stack.at(i).count(name)) return &table_stack.at(i).at(name);
    }*/
    if (table_stack.back().table.count(name)) return &table_stack.back().table.at(name); // local scope
    if (table_stack.back().selfProcedureSymbol != nullptr) 
        if (table_stack.back().selfProcedureSymbol->name == name) // recursion
        return table_stack.back().selfProcedureSymbol; 
    if (table_stack.front().table.count(name)) return &table_stack.front().table.at(name); // global scope

    return nullptr;
}


void SymbolTable::enterScope(std::string scopeName, Symbol* self) {
    std::string fullScopeName = scopeName;
    if (table_stack.size() > 0) fullScopeName = table_stack.back().name + "." + scopeName;
    table_stack.push_back(Scope{fullScopeName, {}, self});
}

void SymbolTable::exitScope() {
    // moving the tokens themselves would invalidate any references to those tokens
    // so the map itself is moved
    dead_scopes.push_back(std::move(table_stack.back()));  
    table_stack.pop_back();
}


std::string SymbolTable::getCurrentScopeFullName() {
    if (table_stack.size() == 0) return "";
    return table_stack.back().name;
}