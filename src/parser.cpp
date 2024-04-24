#include "header.h"
#include "code_generator.h"

//
//
//



Program_Parse grammar__program();

Program_Header grammar__program_header();
Program_Body grammar__program_body();

Declaration grammar__declaration(bool inGlobal = false);
Statement grammar__statement(Typemark::Typemark procedureExpectedType);

Variable_Information grammar__variable_declaration(bool isGlobal);
Procedure_Information grammar__procedure_declaration(bool isGlobal);
Procedure_Body grammar__procedure_body(Typemark::Typemark expectedType);

Assignment_Statement grammar__assignment_statement();
If_Statement grammar__if_statement(Typemark::Typemark procedureExpectedType);
For_Statement grammar__for_statement(Typemark::Typemark procedureExpectedType);
Return_Statement grammar__return_statement(Typemark::Typemark expectedType);

Variable_Access grammar__variable_access();

Expression grammar__expression();
std::pair<ExpressionPrime, Resolution> grammar__expressionPrime(Resolution&);

ArithOp grammar__arithOp();
std::pair<ArithOpPrime, Resolution> grammar__arithOpPrime(Resolution&);

Relation grammar__relation();
std::pair<RelationPrime, Resolution> grammar__relationPrime(Resolution&);

Term grammar__term();
std::pair<TermPrime, Resolution> grammar__termPrime(Resolution&);

Factor grammar__factor();


//
//
//



Program_Parse parse() {
    return grammar__program();
}



Program_Parse grammar__program() {
    Program_Header header = grammar__program_header();
    symbol_table.enterScope("GLOBAL"); // enter global scope
    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_builtinFunctions(); // add builtin functions to the global scope
    }
    Program_Body body = grammar__program_body();
    symbol_table.exitScope(); // exit global scope

    Token period_expected_tok = scan();
    if (period_expected_tok.type != TokenType::PERIOD) {
        std::string message = "End of program '.' expected, encountered Token: '" +
            period_expected_tok.text + "', TokenType: " + 
            TokenTypeAsString(period_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token eof_expected_tok = scan();
    if (eof_expected_tok.type != TokenType::END_OF_FILE) {
        std::string message = "Token encountered after end of program, encountered Token: '" +
            eof_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(eof_expected_tok.type);
        error_reporting.ReportWarning(message);
    }
    
    Program_Parse prog;
    prog.header = header;
    prog.body = std::move(body);
    return prog;
}



Program_Header grammar__program_header() {

    Token program_expected_tok = scan();
    if (program_expected_tok.type != TokenType::PROGRAM) {
        std::string message = "Start of program 'program' expected, encountered Token: '" +
            program_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(program_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token identifier_expected_tok = scan();
    if (identifier_expected_tok.type != TokenType::IDENTIFIER) {
        std::string message = "Identifier expected, encountered Token: '" +
            identifier_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(identifier_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token is_expected_tok = scan();
    if (is_expected_tok.type != TokenType::IS) {
        std::string message = "'is' expected, encountered Token: '" +
            is_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(is_expected_tok.type);
        error_reporting.ReportError(message);
    }

    return Program_Header {(identifier_expected_tok.type == TokenType::IDENTIFIER) ? std::get<std::string>(identifier_expected_tok.value) : " "};
}

Program_Body grammar__program_body() {
    std::vector<Declaration> declarations;
    std::vector<Statement> statements;

    Token begin_expected_tok;
    while (1) {
        Token declaration_possible_tok = buffer_scan();
        if (
            declaration_possible_tok.type != TokenType::GLOBAL &&
            declaration_possible_tok.type != TokenType::PROCEDURE &&
            declaration_possible_tok.type != TokenType::VARIABLE
        ) {
            begin_expected_tok = scan(); // token not a declaration, consume the buffered token
            break;
        }

        declarations.push_back(grammar__declaration(true));

        while (error_reporting.AwaitingResync()) {
            if (buffer_scan().type == TokenType::SEMICOLON) error_reporting.Resync();
            else scan(); // consume non-semicolon token
        }
        
        Token semicolon_expected_tok = scan();
        if (semicolon_expected_tok.type != TokenType::SEMICOLON) {
            std::string message = "';' expected after declaration, encountered Token: '" +
                semicolon_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(semicolon_expected_tok.type);
            error_reporting.ReportError(message);
        }
    }
    if (begin_expected_tok.type != TokenType::BEGIN) {
        std::string message = "'begin' expected, encountered Token: '" +
            begin_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(begin_expected_tok.type);
        error_reporting.ReportError(message);
    }

    // generate llvm

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_beginMain();
    }

    // continue parse
    
    Token end_expected_tok;
    while (1) {
        Token statement_possible_tok = buffer_scan();
        if (
            statement_possible_tok.type != TokenType::IDENTIFIER &&
            statement_possible_tok.type != TokenType::IF &&
            statement_possible_tok.type != TokenType::FOR &&
            statement_possible_tok.type != TokenType::RETURN
        ) {
            end_expected_tok = scan(); // token not a statement, consume the buffered token
            break;
        }

        // return from main is unspecified behavior
        // setting expected type as Typemark::UNASSIGNED makes returning impossible without type-errors
        statements.push_back(grammar__statement(Typemark::UNASSIGNED)); 

        while (error_reporting.AwaitingResync()) {
            if (buffer_scan().type == TokenType::SEMICOLON) error_reporting.Resync();
            else scan(); // consume non-semicolon token
        }

        Token semicolon_expected_tok = scan();
        if (semicolon_expected_tok.type != TokenType::SEMICOLON) {
            std::string message = "';' expected after statement, encountered Token: '" +
                semicolon_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(semicolon_expected_tok.type);
            error_reporting.ReportError(message);
        } 
    }
    if (end_expected_tok.type != TokenType::END) {
        std::string message = "'end' expected, encountered Token: '" +
            end_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(end_expected_tok.type);
        error_reporting.ReportError(message);
    }


    Token program_expected_tok = scan();
    if (program_expected_tok.type != TokenType::PROGRAM) {
        std::string message = "'program' expected, encountered Token: '" +
            program_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(program_expected_tok.type);
        error_reporting.ReportError(message);
    }

    // generate llvm

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_endMain();
    }


    Program_Body body;
    body.declarations = std::move(declarations);
    body.statements = std::move(statements);
    return body;
}



Declaration grammar__declaration(bool inGlobal) {
    Declaration declaration;

    Token global_possible_tok = buffer_scan();
    bool declare_global = false;
    if (global_possible_tok.type == TokenType::GLOBAL) { 
        declare_global = true;
        scan(); // consume global    
    }
    declare_global = declare_global || inGlobal; // implicit global
    
    Token declaration_expected_tok = buffer_scan();
    if (declaration_expected_tok.type == TokenType::PROCEDURE) {
        Procedure_Information procedure = grammar__procedure_declaration(declare_global);
        declaration.declaration = std::move(procedure);

    } else if (declaration_expected_tok.type == TokenType::VARIABLE) {
        Variable_Information variable = grammar__variable_declaration(declare_global);
        declaration.declaration = variable;

    } else {
        std::string message = "Declaration 'procedure' or 'variable' expected, encountered Token: '" +
            declaration_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(declaration_expected_tok.type);
        error_reporting.ReportError(message);
    }

    return declaration;
}

Statement grammar__statement(Typemark::Typemark procedureExpectedType) {
    Statement statement;

    Token statement_expected_tok = buffer_scan();
    switch (statement_expected_tok.type)
    {
    case TokenType::IDENTIFIER:
        statement.statement = grammar__assignment_statement();
        break;
    case TokenType::IF:
        statement.statement = grammar__if_statement(procedureExpectedType);
        break;
    case TokenType::FOR:
        statement.statement = grammar__for_statement(procedureExpectedType);
        break;
    case TokenType::RETURN:
        statement.statement = grammar__return_statement(procedureExpectedType);
        break;
    default: // should never be called
        std::string message = "identifier, 'if', 'for', or 'return' expected, encountered Token: '" +
            statement_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(statement_expected_tok.type);
        error_reporting.ReportError(message);
        break;
    }

    return statement;
}



Variable_Information grammar__variable_declaration(bool isGlobal) {
    Variable_Information variable;
    variable.isGlobal = isGlobal;

    Token variable_expected_tok = scan(); 
    if (variable_expected_tok.type != TokenType::VARIABLE) { // should never occur
        std::string message = "'variable' expected, encountered Token: '" +
            variable_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(variable_expected_tok.type);
    }

    Token identifier_expected_tok = scan();
    if (identifier_expected_tok.type == TokenType::IDENTIFIER) {
        variable.identifier = std::get<std::string>(identifier_expected_tok.value);
    } else {
        std::string message = "Declaration identifier expected, encountered Token: '" +
            identifier_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(identifier_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token colon_expected_tok = scan();
    if (colon_expected_tok.type != TokenType::COLON) {
        std::string message = "':' expected after identifier, encountered Token: '" +
            colon_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(colon_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token typemark_expected_tok = scan();
    if (typemark_expected_tok.type == TokenType::TYPEMARK_INTEGER) {
        variable.type = Typemark::INTEGER;
    } else if (typemark_expected_tok.type == TokenType::TYPEMARK_FLOAT) {
        variable.type = Typemark::FLOAT;
    } else if (typemark_expected_tok.type == TokenType::TYPEMARK_BOOL) {
        variable.type = Typemark::BOOL;
    } else if (typemark_expected_tok.type == TokenType::TYPEMARK_STRING) {
        variable.type = Typemark::STRING;
    } else {
        std::string message = "Declaration typemark expected, encountered Token: '" +
            typemark_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(typemark_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token bound_possible_tok = buffer_scan();
    if (bound_possible_tok.type == TokenType::LEFT_BRACKET) {
        scan(); // consume left bracket
        
        Token integer_expected_tok = scan();
        if (integer_expected_tok.type != TokenType::INTEGER_CONSTANT) {
            std::string message = "Declaration array bounds integer expected, encountered Token: '" +
                integer_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(integer_expected_tok.type);
            error_reporting.ReportError(message);
        }
        variable.isArray = true;
        variable.bound = std::get<int>(integer_expected_tok.value);
        if (variable.bound == 0) {
            std::string message = "Declaration array bound is 0";
            error_reporting.ReportWarning(message);
        }

        Token rightBracket_expected_tok = scan();
        if (rightBracket_expected_tok.type != TokenType::RIGHT_BRACKET) {
            std::string message = "Declaration array bounds closing bracked expected, encountered Token: '" +
                rightBracket_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(rightBracket_expected_tok.type);
            error_reporting.ReportError(message);
        }

    }

    Symbol* variable_symbol = nullptr;
    if (variable.isGlobal) variable_symbol = symbol_table.declare_global(variable.identifier);
    else variable_symbol = symbol_table.declare_local(variable.identifier);
    
    if (variable_symbol == nullptr) { // symbol with name of variable already declared
        std::string message = "symbol already declared in scope with name: '" +
            variable.identifier + "'";
        error_reporting.ReportError(message);
    } else {
        variable_symbol->type = variable.type;

        if (variable.isArray) {
            variable_symbol->shape = TypeShape::ARRAY;
            variable_symbol->array_size = variable.bound;
        } else variable_symbol->shape = TypeShape::VALUE;
    }

    // generate llvm assembly

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_variableDeclaration(variable);    
    }

    return variable;
}

Procedure_Information grammar__procedure_declaration(bool isGlobal) {
    Procedure_Information procedure;
    procedure.isGlobal = isGlobal;
    
    Token procedure_expected_tok = scan(); 
    if (procedure_expected_tok.type != TokenType::PROCEDURE) { // should never occur
        std::string message = "'procedure' expected, encountered Token: '" +
            procedure_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(procedure_expected_tok.type);
    }

    Token identifier_expected_tok = scan();
    if (identifier_expected_tok.type == TokenType::IDENTIFIER) {
        procedure.identifier = std::get<std::string>(identifier_expected_tok.value);
    } else {
        std::string message = "Declaration identifier expected, encountered Token: '" +
            identifier_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(identifier_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token colon_expected_tok = scan();
    if (colon_expected_tok.type != TokenType::COLON) {
        std::string message = "':' expected after identifier, encountered Token: '" +
            colon_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(colon_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token typemark_expected_tok = scan();
    if (typemark_expected_tok.type == TokenType::TYPEMARK_INTEGER) {
        procedure.returnType = Typemark::INTEGER;
    } else if (typemark_expected_tok.type == TokenType::TYPEMARK_FLOAT) {
        procedure.returnType = Typemark::FLOAT;
    } else if (typemark_expected_tok.type == TokenType::TYPEMARK_BOOL) {
        procedure.returnType = Typemark::BOOL;
    } else if (typemark_expected_tok.type == TokenType::TYPEMARK_STRING) {
        procedure.returnType = Typemark::STRING;
    } else {
        std::string message = "Declaration typemark expected, encountered Token: '" +
            typemark_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(typemark_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token leftparen_expected_tok = scan();
    if (leftparen_expected_tok.type != TokenType::LEFT_PARENTHESIS) {
        std::string message = "'(' expected, encounterd Token: '" +
            leftparen_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(leftparen_expected_tok.type);
    }

    Token parameters_possible_tok = buffer_scan();
    if (parameters_possible_tok.type == TokenType::VARIABLE) {
        do {
            scan(); // consume buffered token, VARIABLE (start condition) or COMMA (while condition)
            if (parameters_possible_tok.type == TokenType::COMMA) {
                Token param_variable_expected_tok = scan(); 
                if (param_variable_expected_tok.type != TokenType::VARIABLE) {
                    std::string message = "'variable' expected, encountered Token: '" +
                        param_variable_expected_tok.text + "', TokenType: " +
                        TokenTypeAsString(param_variable_expected_tok.type);
                    error_reporting.ReportError(message);
                }
            }

            Parameter parameter;

            Token param_identifier_expected_tok = scan();
            if (param_identifier_expected_tok.type == TokenType::IDENTIFIER) {
                parameter.identifier = std::get<std::string>(param_identifier_expected_tok.value);
            } else {
                std::string message = "Parameter identifier expected, encountered Token: '" +
                    param_identifier_expected_tok.text + "', TokenType: " +
                    TokenTypeAsString(param_identifier_expected_tok.type);
                error_reporting.ReportError(message);
            }

            Token param_colon_expected_tok = scan();
            if (param_colon_expected_tok.type != TokenType::COLON) {
                std::string message = "':' expected after identifier, encountered Token: '" +
                    param_colon_expected_tok.text + "', TokenType: " +
                    TokenTypeAsString(param_colon_expected_tok.type);
                error_reporting.ReportError(message);
            }   

            Token param_typemark_expected_tok = scan();
            if (param_typemark_expected_tok.type == TokenType::TYPEMARK_INTEGER) {
                parameter.type = Typemark::INTEGER;
            } else if (param_typemark_expected_tok.type == TokenType::TYPEMARK_FLOAT) {
                parameter.type = Typemark::FLOAT;
            } else if (param_typemark_expected_tok.type == TokenType::TYPEMARK_BOOL) {
                parameter.type = Typemark::BOOL;
            } else if (param_typemark_expected_tok.type == TokenType::TYPEMARK_STRING) {
                parameter.type = Typemark::STRING;
            } else {
                std::string message = "Parameter typemark expected, encountered Token: '" +
                    param_typemark_expected_tok.text + "', TokenType: " +
                    TokenTypeAsString(param_typemark_expected_tok.type);
                error_reporting.ReportError(message);
            }

            Token param_bounds_possible_tok = buffer_scan();
            if (param_bounds_possible_tok.type == TokenType::LEFT_BRACKET) {
                scan(); // consume left bracket

                Token integer_expected_tok = scan();
                if (integer_expected_tok.type != TokenType::INTEGER_CONSTANT) {
                    std::string message = "Parameter array bounds integer expected, encountered Token: '" +
                        integer_expected_tok.text + "', TokenType: " +
                        TokenTypeAsString(integer_expected_tok.type);
                    error_reporting.ReportError(message);
                }
                parameter.isArray = true;
                parameter.bound = std::get<int>(integer_expected_tok.value);
                if (parameter.bound == 0) {
                    std::string message = "Parameter array bound is 0";
                    error_reporting.ReportWarning(message);
                }

                Token rightBracket_expected_tok = scan();
                if (rightBracket_expected_tok.type != TokenType::RIGHT_BRACKET) {
                    std::string message = "Parameter array bounds closing bracked expected, encountered Token: '" +
                        rightBracket_expected_tok.text + "', TokenType: " +
                        TokenTypeAsString(rightBracket_expected_tok.type);
                    error_reporting.ReportError(message);
                }

            }

            procedure.parameters.push_back(parameter);
            parameters_possible_tok = buffer_scan();
        } while (parameters_possible_tok.type == TokenType::COMMA);

    }

    Token rightparen_expected_tok = scan();
    if (rightparen_expected_tok.type != TokenType::RIGHT_PARENTHESIS) {
        std::string message = "')' expected, encounterd Token: '" +
            rightparen_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(rightparen_expected_tok.type);
    } 

    // add procedure to symbol table, step into scope, and add parameters to symbol table
    Symbol* procedure_symbol = nullptr;
    if (procedure.isGlobal) procedure_symbol = symbol_table.declare_global(procedure.identifier);
    else procedure_symbol = symbol_table.declare_local(procedure.identifier);

    if (procedure_symbol == nullptr) { // symbol with name of variable already declared
        std::string message = "symbol already declared in scope with name: '" +
            procedure.identifier + "'";
        error_reporting.ReportError(message);
    } else {
        procedure_symbol->shape = TypeShape::FUNCTION;
        procedure_symbol->type = procedure.returnType;
    }
    symbol_table.enterScope(procedure.identifier, procedure_symbol); 

    if (procedure_symbol != nullptr) {

        for (const Parameter& param : procedure.parameters) {
            Symbol* param_symbol = symbol_table.declare_local(param.identifier);
            if (param_symbol == nullptr) {
                std::string message = "symbol already declared in scope with name: '" +
                    param.identifier + "'";
                error_reporting.ReportError(message);
            } else {
                param_symbol->type = param.type;
                
                if (param.isArray) {
                    param_symbol->shape = TypeShape::ARRAY;
                    param_symbol->array_size = param.bound;
                } else param_symbol->shape = TypeShape::VALUE;
                
                procedure_symbol->function_parameters.push_back(param_symbol);
            }
        }
    }

    // generate llvm assembly

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_procedureHeader(procedure_symbol);
    }

    // continue parsing

    procedure.body = grammar__procedure_body(procedure.returnType);
    
    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_procedureEnd(); // close end of procedure
    }
    symbol_table.exitScope();

    return procedure;
}

Procedure_Body grammar__procedure_body(Typemark::Typemark expectedType) {
    std::vector<Declaration> declarations;
    std::vector<Statement> statements;

    Token begin_expected_tok;
    while (1) {
        Token declaration_possible_tok = buffer_scan();
        if (
            declaration_possible_tok.type != TokenType::GLOBAL &&
            declaration_possible_tok.type != TokenType::PROCEDURE &&
            declaration_possible_tok.type != TokenType::VARIABLE
        ) {
            begin_expected_tok = scan(); // token not a declaration, consume the buffered token
            break;
        }

        declarations.push_back(grammar__declaration());

        while (error_reporting.AwaitingResync()) {
            if (buffer_scan().type == TokenType::SEMICOLON) error_reporting.Resync();
            else scan(); // consume non-semicolon token
        }
        
        Token semicolon_expected_tok = scan();
        if (semicolon_expected_tok.type != TokenType::SEMICOLON) {
            std::string message = "';' expected after declaration, encountered Token: '" +
                semicolon_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(semicolon_expected_tok.type);
            error_reporting.ReportError(message);
        }
    }
    if (begin_expected_tok.type != TokenType::BEGIN) {
        std::string message = "'begin' expected, encountered Token: '" +
            begin_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(begin_expected_tok.type);
        error_reporting.ReportError(message);
    }

    
    Token end_expected_tok;
    while (1) {
        Token statement_possible_tok = buffer_scan();
        if (
            statement_possible_tok.type != TokenType::IDENTIFIER &&
            statement_possible_tok.type != TokenType::IF &&
            statement_possible_tok.type != TokenType::FOR &&
            statement_possible_tok.type != TokenType::RETURN
        ) {
            end_expected_tok = scan(); // token not a statement, consume the buffered token
            break;
        }

        statements.push_back(grammar__statement(expectedType));

        while (error_reporting.AwaitingResync()) {
            if (buffer_scan().type == TokenType::SEMICOLON) error_reporting.Resync();
            else scan(); // consume non-semicolon token
        }

        Token semicolon_expected_tok = scan();
        if (semicolon_expected_tok.type != TokenType::SEMICOLON) {
            std::string message = "';' expected after statement, encountered Token: '" +
                semicolon_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(semicolon_expected_tok.type);
            error_reporting.ReportError(message);
        } 
    }
    if (end_expected_tok.type != TokenType::END) {
        std::string message = "'end' expected, encountered Token: '" +
            end_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(end_expected_tok.type);
        error_reporting.ReportError(message);
    }


    Token procedure_expected_tok = scan();
    if (procedure_expected_tok.type != TokenType::PROCEDURE) {
        std::string message = "'procedure' expected, encountered Token: '" +
            procedure_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(procedure_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Procedure_Body body;
    body.declarations = std::move(declarations);
    body.statements = std::move(statements);
    return body;
}



Assignment_Statement grammar__assignment_statement() {

    Variable_Access variable = grammar__variable_access(); // identifier token still buffered
    
    Token assignment_expected_tok = scan();
    if (assignment_expected_tok.type != TokenType::ASSIGN) {
        std::string message = "assignment ':=' expected, encountered Token: '" +
            assignment_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(assignment_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Expression assignment = grammar__expression();

    // shape resolution

    if (variable.resolution.resolvedShape == TypeShape::ARRAY
    && assignment.resolution.resolvedShape == TypeShape::ARRAY) {

        if (variable.resolution.resolvedArraySize != assignment.resolution.resolvedArraySize) {
            std::string message = "arrays must be of the same size to perform assignment";
            error_reporting.ReportError(message);
        }

    }
    else if (variable.resolution.resolvedShape == TypeShape::ARRAY
    && assignment.resolution.resolvedShape == TypeShape::VALUE) {
    }
    else if (variable.resolution.resolvedShape == TypeShape::VALUE
    && assignment.resolution.resolvedShape == TypeShape::ARRAY) {
        std::string message = "a non-array variable can not be assigned to with an array";
        error_reporting.ReportError(message);
    }
    else if (variable.resolution.resolvedShape == TypeShape::VALUE
    && assignment.resolution.resolvedShape == TypeShape::VALUE) {
    }
    else {
        std::string message = "invalid assignment shape, left: '" +
            TypeShapeAsString(variable.resolution.resolvedShape) + "', right: '" +
            TypeShapeAsString(assignment.resolution.resolvedShape) + "'";
        error_reporting.ReportError(message);
    }

    // type resolution
    TypeResolutionCase::TypeResolutionCase typeResolution_cast = TypeResolutionCase::invalid;
    if (variable.resolution.resolvedType == assignment.resolution.resolvedType)
        typeResolution_cast = TypeResolutionCase::straight;
    if (variable.resolution.resolvedType == Typemark::BOOL
            && assignment.resolution.resolvedType == Typemark::INTEGER)
        typeResolution_cast = TypeResolutionCase::int_to_bool;
    if (variable.resolution.resolvedType == Typemark::INTEGER
            && assignment.resolution.resolvedType == Typemark::BOOL)
        typeResolution_cast = TypeResolutionCase::bool_to_int;
    if (variable.resolution.resolvedType == Typemark::INTEGER
            && assignment.resolution.resolvedType == Typemark::FLOAT)
        typeResolution_cast = TypeResolutionCase::float_to_int;
    if (variable.resolution.resolvedType == Typemark::FLOAT
            && assignment.resolution.resolvedType == Typemark::INTEGER)
        typeResolution_cast = TypeResolutionCase::int_to_float;
    
    if ( typeResolution_cast == TypeResolutionCase::invalid) {
        std::string message = "non-compatible assignment types, the only valid implicit casts are bool<->integer and integer<->float. left: '" +
            TypemarkAsString(variable.resolution.resolvedType) + "', right: '" +
            TypemarkAsString(assignment.resolution.resolvedType) + "'";
        error_reporting.ReportError(message);
    }

    // generate llvm assembly
    
    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_assignmentStatement(variable, assignment, typeResolution_cast);
    }

    return Assignment_Statement{std::move(variable), std::move(assignment)}; // both contain unique_ptr
}

If_Statement grammar__if_statement(Typemark::Typemark procedureExpectedType) {
    If_Statement if_statement;

    Token if_expected_tok = scan();
    if (if_expected_tok.type != TokenType::IF) { // should not occur
        std::string message = "'if' expected, encountered Token: '" +
            if_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(if_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token leftparen_expected_tok = scan();
    if (leftparen_expected_tok.type != TokenType::LEFT_PARENTHESIS) {
        std::string message = "'(' expected, encountered Token: '" +
            leftparen_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(leftparen_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Expression condition = grammar__expression();

    Token rightparen_expected_tok = scan();
    if (rightparen_expected_tok.type != TokenType::RIGHT_PARENTHESIS) {
        std::string message = "')' expected, encountered Token: '" +
            rightparen_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(rightparen_expected_tok.type);
        error_reporting.ReportError(message);
    }
    
    // condition type resolution
    if (condition.resolution.resolvedShape != TypeShape::VALUE) {
        std::string message = "invalid condition shape, the condition of an if statement must be a value. condition shape: '" +
            TypeShapeAsString(condition.resolution.resolvedShape) + "'";
        error_reporting.ReportError(message);
    }
    if (condition.resolution.resolvedType != Typemark::BOOL
    && condition.resolution.resolvedType != Typemark::INTEGER) {
        std::string message = "invalid condition type, the condition of an if statement must resolve to a bool. condition type: '" +
            TypemarkAsString(condition.resolution.resolvedType) + "'";
        error_reporting.ReportError(message);
    }

    std::string elseLabel;
    std::string endLabel;

    // codegen if_statement condition
    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        std::pair<std::string,std::string> labels = codegen_ifStatementCondition(condition.resolution);
        elseLabel = labels.first;
        endLabel = labels.second;
    }

    // resume parse

    Token then_expected_tok = scan();
    if (then_expected_tok.type != TokenType::THEN) {
        std::string message = "'then' expected, encountered Token: '" +
            then_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(then_expected_tok.type);
        error_reporting.ReportError(message);
    }
    
    std::vector<Statement> then_statements;
    Token then_statements_possible_tok = buffer_scan();
    while (
        then_statements_possible_tok.type == TokenType::IDENTIFIER ||
        then_statements_possible_tok.type == TokenType::IF ||
        then_statements_possible_tok.type == TokenType::FOR ||
        then_statements_possible_tok.type == TokenType::RETURN
        ) 
    {
        then_statements.push_back(grammar__statement(procedureExpectedType));

        Token semicolon_expected_tok = scan();
        if (semicolon_expected_tok.type != TokenType::SEMICOLON) {
            std::string message = "';' expected, encountered Token: '" +
                semicolon_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(semicolon_expected_tok.type);
            error_reporting.ReportError(message);
        }

        then_statements_possible_tok = buffer_scan();
    }
    
    // codegen end of then - start of else
    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_ifStatementElse(elseLabel, endLabel);
    }

    // resume parse

    std::vector<Statement> else_statements;
    
    Token else_possible_tok = buffer_scan();
    if (else_possible_tok.type == TokenType::ELSE) {
        scan(); // consume else token
        Token else_statements_possible_tok = buffer_scan();
        while (
            else_statements_possible_tok.type == TokenType::IDENTIFIER ||
            else_statements_possible_tok.type == TokenType::IF ||
            else_statements_possible_tok.type == TokenType::FOR ||
            else_statements_possible_tok.type == TokenType::RETURN
            ) 
        {
            else_statements.push_back(grammar__statement(procedureExpectedType));

            Token semicolon_expected_tok = scan();
            if (semicolon_expected_tok.type != TokenType::SEMICOLON) {
                std::string message = "';' expected, encountered Token: '" +
                    semicolon_expected_tok.text + "', TokenType: " +
                    TokenTypeAsString(semicolon_expected_tok.type);
                error_reporting.ReportError(message);
            }

            else_statements_possible_tok = buffer_scan();
        }
    }

    Token end_expected_tok = scan();
    if (end_expected_tok.type != TokenType::END) {
        std::string message = "'end' expected, encountered Token: '" +
            end_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(end_expected_tok.type);
        error_reporting.ReportError(message);
    }

    if_expected_tok = scan();
    if (if_expected_tok.type != TokenType::IF) {
        std::string message = "'if' expected, encountered Token: '" +
            if_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(if_expected_tok.type);
        error_reporting.ReportError(message);
    }

    // codegen end of else - begin of after block
    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_ifStatementEnd(endLabel);
    }
    
    if_statement.condition = std::move(condition);
    if_statement.then_body = std::move(then_statements);
    if_statement.else_body = std::move(else_statements);
    return if_statement;
}

For_Statement grammar__for_statement(Typemark::Typemark procedureExpectedType) {

    Token for_expected_tok = scan();
    if (for_expected_tok.type != TokenType::FOR) { // should not occur
        std::string message = "'for' expected, encountered Token: '" +
            for_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(for_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token leftparen_expected_tok = scan();
    if (leftparen_expected_tok.type != TokenType::LEFT_PARENTHESIS) { 
        std::string message = "'(' expected, encountered Token: '" +
            leftparen_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(leftparen_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Assignment_Statement assignment_statement = grammar__assignment_statement();

    Token semicolon_expected_tok = scan();
    if (semicolon_expected_tok.type != TokenType::SEMICOLON) { 
        std::string message = "';' expected, encountered Token: '" +
            semicolon_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(semicolon_expected_tok.type);
        error_reporting.ReportError(message);
    }

    // codegen loop condition

    std::string condLabel, bodyLabel, endLabel;
    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        std::tuple<std::string, std::string, std::string> labels = codegen_forStatementConditionStart();
        condLabel = std::get<0>(labels);
        bodyLabel = std::get<1>(labels);
        endLabel = std::get<2>(labels);
    }

    Expression condition = grammar__expression();

    // condition type resolution
    if (condition.resolution.resolvedShape != TypeShape::VALUE) {
        std::string message = "invalid condition shape, the condition of a for loop must be a value. condition shape: '" +
            TypeShapeAsString(condition.resolution.resolvedShape) + "'";
        error_reporting.ReportError(message);
    }
    if (condition.resolution.resolvedType != Typemark::BOOL
    && condition.resolution.resolvedType != Typemark::INTEGER) {
        std::string message = "invalid condition type, the condition of a for loop must resolve to a bool. condition type: '" +
            TypemarkAsString(condition.resolution.resolvedType) + "'";
        error_reporting.ReportError(message);
    }

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_forStatementBodyStart(bodyLabel, endLabel, condition.resolution);
    }

    // continue parse

    Token rightparen_expected_tok = scan();
    if (rightparen_expected_tok.type != TokenType::RIGHT_PARENTHESIS) { 
        std::string message = "')' expected, encountered Token: '" +
            rightparen_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(rightparen_expected_tok.type);
        error_reporting.ReportError(message);
    }

    std::vector<Statement> body_statements;
    Token statements_possible_tok = buffer_scan();
    while (
        statements_possible_tok.type == TokenType::IDENTIFIER ||
        statements_possible_tok.type == TokenType::IF ||
        statements_possible_tok.type == TokenType::FOR ||
        statements_possible_tok.type == TokenType::RETURN
        ) 
    {
        body_statements.push_back(grammar__statement(procedureExpectedType));

        Token semicolon_expected_tok = scan();
        if (semicolon_expected_tok.type != TokenType::SEMICOLON) {
            std::string message = "';' expected, encountered Token: '" +
                semicolon_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(semicolon_expected_tok.type);
            error_reporting.ReportError(message);
        }

        statements_possible_tok = buffer_scan();
    }


    Token end_expected_tok = scan();
    if (end_expected_tok.type != TokenType::END) {
        std::string message = "'end' expected, encountered Token: '" +
            end_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(end_expected_tok.type);
        error_reporting.ReportError(message);
    }

    for_expected_tok = scan();
    if (for_expected_tok.type != TokenType::FOR) {
        std::string message = "'for' expected, encountered Token: '" +
            for_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(for_expected_tok.type);
        error_reporting.ReportError(message);
    }

    // codegen end of for loop

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_forStatementEnd(condLabel, endLabel);
    }

    For_Statement for_statement;
    for_statement.initial = std::move(assignment_statement);
    for_statement.condition = std::move(condition);
    for_statement.body = std::move(body_statements);
    return for_statement;
}

Return_Statement grammar__return_statement(Typemark::Typemark expectedType) {

    Token return_expected_tok = scan();
    if (return_expected_tok.type != TokenType::RETURN) { // should not occur
        std::string message = "'return' expected, encountered Token: '" +
            return_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(return_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Expression expr = grammar__expression();
    if (expr.resolution.resolvedShape != TypeShape::VALUE) {
        std::string message = "invalid return shape, only values can be returned by procedures. return shape: '" +
            TypeShapeAsString(expr.resolution.resolvedShape) + "'";
        error_reporting.ReportError(message);
    }

    if (expr.resolution.resolvedType == expectedType) {}
    else if (expr.resolution.resolvedType == Typemark::INTEGER
    && expectedType == Typemark::FLOAT) {}
    else if (expr.resolution.resolvedType == Typemark::FLOAT
    && expectedType == Typemark::INTEGER) {}
    else if (expr.resolution.resolvedType == Typemark::INTEGER
    && expectedType == Typemark::BOOL) {}
    else if (expr.resolution.resolvedType == Typemark::BOOL
    && expectedType == Typemark::INTEGER) {}
    else {
        std::string message = "non-compatible return type, expected type: '" +
            TypemarkAsString(expectedType) + "', returned type: '" +
            TypemarkAsString(expr.resolution.resolvedType) + "'";
        error_reporting.ReportError(message);
    }

    // codegen

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_returnStatement(expr, expectedType);
    }

    return Return_Statement{std::move(expr)};
}



Variable_Access grammar__variable_access() {
    Variable_Access var_access;

    Token identifier_expected_tok = scan();
    if (identifier_expected_tok.type == TokenType::IDENTIFIER) {
        var_access.identifier = std::get<std::string>(identifier_expected_tok.value);
    } else {
        std::string message = "identifier expected, encountered Token: '" +
            identifier_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(identifier_expected_tok.type);
        error_reporting.ReportError(message);
    }

    Token bounds_possible_tok = buffer_scan();
    if (bounds_possible_tok.type == TokenType::LEFT_BRACKET) {
        scan();
        std::unique_ptr<Expression> exprPtr = std::make_unique<Expression>();
        *(exprPtr.get()) = grammar__expression();
        var_access.array_index = std::move(exprPtr);

        Token rightbracket_expected_tok = scan();
        if (rightbracket_expected_tok.type != TokenType::RIGHT_BRACKET) {
            std::string message = "']' expected, encountered Token: '" +
                rightbracket_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(rightbracket_expected_tok.type);
            error_reporting.ReportError(message);
        }
    }

    if (identifier_expected_tok.type == TokenType::IDENTIFIER) {
        Symbol* var_symbol = symbol_table.lookup(var_access.identifier);
        if (var_symbol == nullptr) {
            std::string message = "variable access on undeclared variable: '" +
                var_access.identifier + "'";
            error_reporting.ReportError(message);
        } 

        if (var_symbol != nullptr) { 

            if (bounds_possible_tok.type == TokenType::LEFT_BRACKET) {
                if (var_symbol->shape != TypeShape::ARRAY) {
                    std::string message = "variable array access on non-array symbol: '" +
                        var_access.identifier + "'";
                    error_reporting.ReportError(message);
                }

                if (var_access.array_index->get()->resolution.resolvedShape != TypeShape::VALUE ||
                    var_access.array_index->get()->resolution.resolvedType != Typemark::INTEGER) {
                    std::string message = "non-integer array access index";
                    error_reporting.ReportError(message);
                }

                var_access.resolution.resolvedShape = TypeShape::VALUE;
            }        
            else if (var_symbol->shape == TypeShape::VALUE) var_access.resolution.resolvedShape = TypeShape::VALUE;
            else if (var_symbol->shape == TypeShape::ARRAY) {
                var_access.resolution.resolvedShape = TypeShape::ARRAY;
                var_access.resolution.resolvedArraySize = var_symbol->array_size;    
            }
            else {
                std::string message = "variable access on non-variable symbol: '" +
                    var_access.identifier + "'";
                error_reporting.ReportError(message);
            }
        }

        if (var_symbol != nullptr) var_access.resolution.resolvedType = var_symbol->type;
    }

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_variableAccess(var_access);
    }

    return var_access;
}



Expression grammar__expression() {
    
    Expression expr;
    
    Token not_possible_tok = buffer_scan();
    if (not_possible_tok.type == TokenType::BITWISE_NOT) {
        expr.bitwise_not = true;
        scan();
    } else expr.bitwise_not = false;

    // typecheck self

    expr.arithOp = grammar__arithOp();
    if (expr.bitwise_not && (expr.arithOp.resolution.resolvedType != Typemark::INTEGER
    && expr.arithOp.resolution.resolvedType != Typemark::BOOL)) {
        std::string message = "invalid type, only integers and booleans can perform bitwise operations. type: '" +
            TypemarkAsString(expr.arithOp.resolution.resolvedType) + "'";
        error_reporting.ReportError(message);
    }

    Token exprPrime_possible_tok = buffer_scan();
    if (exprPrime_possible_tok.type == TokenType::BITWISE_AND || exprPrime_possible_tok.type == TokenType::BITWISE_OR) {
    
        // typecheck self // !expr.bitwise_not to prevent double error 
        if (!expr.bitwise_not && (expr.arithOp.resolution.resolvedType != Typemark::INTEGER
        && expr.arithOp.resolution.resolvedType != Typemark::BOOL)) {
            std::string message = "invalid type, only integers and booleans can perform bitwise operations. type: '" +
                TypemarkAsString(expr.arithOp.resolution.resolvedType) + "'";
            error_reporting.ReportError(message);
        }

        expr.prime = std::make_unique<ExpressionPrime>();
        std::pair<ExpressionPrime, Resolution> rightSide = grammar__expressionPrime(expr.arithOp.resolution);
        *(expr.prime.get()) = std::move(rightSide.first);
        expr.resolution = rightSide.second;
    } else {
        expr.resolution = expr.arithOp.resolution;
    }

    // generate llvm

    if (expr.bitwise_not && error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_expressionBitwiseNot(expr.resolution);
    }

    return expr;
}


std::pair<ExpressionPrime, Resolution> grammar__expressionPrime(Resolution& leftSideExpr) {
    ExpressionPrime exprPrime;
    
    Token bitwiseOp_expected_tok = scan();
    switch (bitwiseOp_expected_tok.type) {
    case TokenType::BITWISE_AND:
        exprPrime.bitwise_op = BinaryBitwise::AND;
        break;
    case TokenType::BITWISE_OR:
        exprPrime.bitwise_op = BinaryBitwise::OR;
        break;
    default: // should not occur
        std::string message = "'&' or '|' expected, encountered Token: '" +
            bitwiseOp_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(bitwiseOp_expected_tok.type);
        error_reporting.ReportError(message);
        break;
    }

    exprPrime.arithOp = grammar__arithOp();

    // typecheck self

    if (exprPrime.arithOp.resolution.resolvedType != Typemark::INTEGER
    && exprPrime.arithOp.resolution.resolvedType != Typemark::BOOL) {
        std::string message = "invalid type, only integers and booleans can perform bitwise operations. type: '" +
            TypemarkAsString(exprPrime.arithOp.resolution.resolvedType) + "'";
        error_reporting.ReportError(message);
    } else if (exprPrime.arithOp.resolution.resolvedType != leftSideExpr.resolvedType) {
        std::string message = "invalid type, types for bitwise operations must match exactly";
        error_reporting.ReportError(message);
    }
    
    // shape resolution

    if (leftSideExpr.resolvedShape == TypeShape::ARRAY
    && exprPrime.arithOp.resolution.resolvedShape == TypeShape::ARRAY) {
        // array on array, must be same size

        if (leftSideExpr.resolvedArraySize != exprPrime.arithOp.resolution.resolvedArraySize) {
            std::string message = "arrays must be of the same size to perform operations";
            error_reporting.ReportError(message);
        }

        exprPrime.resolution.resolvedShape = TypeShape::ARRAY;
        exprPrime.resolution.resolvedArraySize = exprPrime.arithOp.resolution.resolvedArraySize;
    }
    else if (leftSideExpr.resolvedShape == TypeShape::ARRAY
    && exprPrime.arithOp.resolution.resolvedShape == TypeShape::VALUE) {
        // value on array

        exprPrime.resolution.resolvedShape = TypeShape::ARRAY;
        exprPrime.resolution.resolvedArraySize = leftSideExpr.resolvedArraySize;
    }
    else if (leftSideExpr.resolvedShape == TypeShape::VALUE
    && exprPrime.arithOp.resolution.resolvedShape == TypeShape::ARRAY) {
        // value on array

        exprPrime.resolution.resolvedShape = TypeShape::ARRAY;
        exprPrime.resolution.resolvedArraySize = exprPrime.arithOp.resolution.resolvedArraySize;
    }
    else if (leftSideExpr.resolvedShape == TypeShape::VALUE
    && exprPrime.arithOp.resolution.resolvedShape == TypeShape::VALUE) {
        // value on value

        exprPrime.resolution.resolvedShape = TypeShape::VALUE;
    }
    else exprPrime.resolution.resolvedShape = TypeShape::UNASSIGNED;

    // type resolution

    exprPrime.resolution.resolvedType = leftSideExpr.resolvedType;

    // generate llvm

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_expressionPrime(leftSideExpr, exprPrime.arithOp.resolution, exprPrime.resolution, exprPrime.bitwise_op);
    }

    // continue parse

    std::pair<ExpressionPrime, Resolution> fullResolution;
    fullResolution.second = exprPrime.resolution;

    Token exprPrime_possible_tok = buffer_scan();
    if (exprPrime_possible_tok.type == TokenType::BITWISE_AND || exprPrime_possible_tok.type == TokenType::BITWISE_OR) {
        exprPrime.prime = std::make_unique<ExpressionPrime>();
        std::pair<ExpressionPrime, Resolution> rightSide = grammar__expressionPrime(exprPrime.resolution);
        *(exprPrime.prime.get()) = std::move(rightSide.first);
        fullResolution.second = rightSide.second; 
    }
    fullResolution.first = std::move(exprPrime);

    return fullResolution;
}



ArithOp grammar__arithOp() {
    ArithOp arithOp;
    
    arithOp.relation = grammar__relation();

    Token prime_possible_tok = buffer_scan();
    if (prime_possible_tok.type == TokenType::PLUS || prime_possible_tok.type == TokenType::MINUS) {

        // typecheck self
        if (arithOp.relation.resolution.resolvedType != Typemark::INTEGER 
        && arithOp.relation.resolution.resolvedType != Typemark::FLOAT) {
            std::string message = "invalid type, only integer and float types can perform arithmetic operations. type: " +
                TypemarkAsString(arithOp.relation.resolution.resolvedType) + "'";
            error_reporting.ReportError(message);
        }

        arithOp.prime = std::make_unique<ArithOpPrime>();
        std::pair<ArithOpPrime, Resolution> rightSide = grammar__arithOpPrime(arithOp.relation.resolution);
        *(arithOp.prime.get()) = std::move(rightSide.first);
        arithOp.resolution = rightSide.second;
    } else {
        arithOp.resolution = arithOp.relation.resolution;
    }

    return arithOp;
}

std::pair<ArithOpPrime, Resolution> grammar__arithOpPrime(Resolution& leftSideArith) {
    ArithOpPrime arithOpPrime;

    Token arithOp_expected_tok = scan();
    switch (arithOp_expected_tok.type) {
    case TokenType::PLUS:
        arithOpPrime.arithmatic_op = ArithmaticOperator::PLUS;
        break;
    case TokenType::MINUS:
        arithOpPrime.arithmatic_op = ArithmaticOperator::MINUS;
        break;
    default: // should not occur
        std::string message = "'+' or '-' expected, encountered Token: '" +
            arithOp_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(arithOp_expected_tok.type);
        error_reporting.ReportError(message);
        break;
    }

    arithOpPrime.relation = grammar__relation();
    
    // typecheck self
    if (arithOpPrime.relation.resolution.resolvedType != Typemark::INTEGER 
    && arithOpPrime.relation.resolution.resolvedType != Typemark::FLOAT) {
        std::string message = "invalid type, only integer and float types can perform arithmetic operations. type: " +
            TypemarkAsString(arithOpPrime.relation.resolution.resolvedType) + "'";
        error_reporting.ReportError(message);
    }

    // shape resolution

    if (leftSideArith.resolvedShape == TypeShape::ARRAY
    && arithOpPrime.relation.resolution.resolvedShape == TypeShape::ARRAY) {
        // array on array, must be same size

        if (leftSideArith.resolvedArraySize != arithOpPrime.relation.resolution.resolvedArraySize) {
            std::string message = "arrays must be of the same size to perform operations";
            error_reporting.ReportError(message);
        }

        arithOpPrime.resolution.resolvedShape = TypeShape::ARRAY;
        arithOpPrime.resolution.resolvedArraySize = arithOpPrime.relation.resolution.resolvedArraySize;
    }
    else if (leftSideArith.resolvedShape == TypeShape::ARRAY
    && arithOpPrime.relation.resolution.resolvedShape == TypeShape::VALUE) {
        // value on array

        arithOpPrime.resolution.resolvedShape = TypeShape::ARRAY;
        arithOpPrime.resolution.resolvedArraySize = leftSideArith.resolvedArraySize;
    }
    else if (leftSideArith.resolvedShape == TypeShape::VALUE
    && arithOpPrime.relation.resolution.resolvedShape == TypeShape::ARRAY) {
        // value on array

        arithOpPrime.resolution.resolvedShape = TypeShape::ARRAY;
        arithOpPrime.resolution.resolvedArraySize = arithOpPrime.relation.resolution.resolvedArraySize;
    }
    else if (leftSideArith.resolvedShape == TypeShape::VALUE
    && arithOpPrime.relation.resolution.resolvedShape == TypeShape::VALUE) {
        // value on value

        arithOpPrime.resolution.resolvedShape = TypeShape::VALUE;
    }
    else arithOpPrime.resolution.resolvedShape = TypeShape::UNASSIGNED;


    // type resolution

    if (leftSideArith.resolvedType != arithOpPrime.relation.resolution.resolvedType) {
        // one float, one integer. cast to float
        arithOpPrime.resolution.resolvedType = Typemark::FLOAT;
    } else arithOpPrime.resolution.resolvedType = arithOpPrime.relation.resolution.resolvedType;


    // generate llvm

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_arithOpPrime(leftSideArith, arithOpPrime.relation.resolution, arithOpPrime.resolution, arithOpPrime.arithmatic_op);
    }


    // continue parse

    std::pair<ArithOpPrime, Resolution> fullResolution;
    fullResolution.second = arithOpPrime.resolution;

    Token prime_possible_tok = buffer_scan();
    if (prime_possible_tok.type == TokenType::PLUS || prime_possible_tok.type == TokenType::MINUS) {
        arithOpPrime.prime = std::make_unique<ArithOpPrime>();
        std::pair<ArithOpPrime, Resolution> rightSide = grammar__arithOpPrime(arithOpPrime.resolution);
        *(arithOpPrime.prime.get()) = std::move(rightSide.first);
        fullResolution.second = rightSide.second; 
    }
    fullResolution.first = std::move(arithOpPrime);

    return fullResolution;
}



Relation grammar__relation() {
    Relation relation;

    relation.term = grammar__term();

    Token prime_possible_tok = buffer_scan();
    if (
        prime_possible_tok.type == TokenType::LESS_THAN || prime_possible_tok.type == TokenType::LESS_EQUAL ||
        prime_possible_tok.type == TokenType::GREATER_THAN || prime_possible_tok.type == TokenType::GREATER_EQUAL ||
        prime_possible_tok.type == TokenType::EQUAL_TO || prime_possible_tok.type == TokenType::NON_EQUAL
    ) {
        // all types are valid individually, type check not needed here

        relation.prime = std::make_unique<RelationPrime>();
        std::pair<RelationPrime, Resolution> rightSide = grammar__relationPrime(relation.term.resolution); 
        *(relation.prime.get()) = std::move(rightSide.first);
        relation.resolution = rightSide.second;
    } 
    else {
        relation.resolution = relation.term.resolution;
    }

    return relation;
}


std::pair<RelationPrime, Resolution> grammar__relationPrime(Resolution& leftSideRelation) {
    RelationPrime relationPrime;

    Token relationOp_expected_tok = scan();
    switch (relationOp_expected_tok.type) {
    case TokenType::LESS_THAN:
        relationPrime.relation_op = RelationOperator::LESS;
        break;
    case TokenType::LESS_EQUAL:
        relationPrime.relation_op = RelationOperator::LESS_EQUAL;
        break;
    case TokenType::GREATER_THAN:
        relationPrime.relation_op = RelationOperator::GREATER;
        break;
    case TokenType::GREATER_EQUAL:
        relationPrime.relation_op = RelationOperator::GREATER_EQUAL;
        break;
    case TokenType::EQUAL_TO:
        relationPrime.relation_op = RelationOperator::EQUAL;
        break;
    case TokenType::NON_EQUAL:
        relationPrime.relation_op = RelationOperator::NON_EQUAL;
        break;
    default: // should not occur
        std::string message = "comparison operator expected, encountered Token: '" +
            relationOp_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(relationOp_expected_tok.type);
        error_reporting.ReportError(message);
        break;
    }

    relationPrime.term = grammar__term();


    // shape resolution

    if (leftSideRelation.resolvedShape == TypeShape::ARRAY
    && relationPrime.term.resolution.resolvedShape == TypeShape::ARRAY) {
        // array on array, must be same size

        if (leftSideRelation.resolvedArraySize != relationPrime.term.resolution.resolvedArraySize) {
            std::string message = "arrays must be of the same size to perform operations";
            error_reporting.ReportError(message);
        }

        relationPrime.resolution.resolvedShape = TypeShape::ARRAY;
        relationPrime.resolution.resolvedArraySize = relationPrime.term.resolution.resolvedArraySize;
    }
    else if (leftSideRelation.resolvedShape == TypeShape::ARRAY
    && relationPrime.term.resolution.resolvedShape == TypeShape::VALUE) {
        // value on array

        relationPrime.resolution.resolvedShape = TypeShape::ARRAY;
        relationPrime.resolution.resolvedArraySize = leftSideRelation.resolvedArraySize;
    }
    else if (leftSideRelation.resolvedShape == TypeShape::VALUE
    && relationPrime.term.resolution.resolvedShape == TypeShape::ARRAY) {
        // value on array

        relationPrime.resolution.resolvedShape = TypeShape::ARRAY;
        relationPrime.resolution.resolvedArraySize = relationPrime.term.resolution.resolvedArraySize;
    }
    else if (leftSideRelation.resolvedShape == TypeShape::VALUE
    && relationPrime.term.resolution.resolvedShape == TypeShape::VALUE) {
        // value on value

        relationPrime.resolution.resolvedShape = TypeShape::VALUE;
    }
    else relationPrime.resolution.resolvedShape = TypeShape::UNASSIGNED;

    // type resolution

    relationPrime.resolution.resolvedType = Typemark::BOOL; // always returns bool
    if (leftSideRelation.resolvedType != relationPrime.term.resolution.resolvedType) {
        if (!(
            (leftSideRelation.resolvedType == Typemark::INTEGER
            && relationPrime.term.resolution.resolvedType == Typemark::BOOL) 
            ||
            (leftSideRelation.resolvedType == Typemark::BOOL
            && relationPrime.term.resolution.resolvedType == Typemark::INTEGER)
        )) {
            std::string message = "incompatible types, only integer and bool types are compatible with each other for relational operations";
            error_reporting.ReportError(message);
        }
    }
    else if (leftSideRelation.resolvedType == Typemark::STRING && !(
        relationPrime.relation_op == RelationOperator::EQUAL 
        || relationPrime.relation_op == RelationOperator::NON_EQUAL
    )) {
        std::string message = "incompatible relation, strings can only compare for equality/inequality";
        error_reporting.ReportError(message);
    }

    // generate llvm

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_relationPrime(leftSideRelation, relationPrime.term.resolution, relationPrime.resolution, relationPrime.relation_op);
    }

    // continue parse

    std::pair<RelationPrime, Resolution> fullResolution;
    fullResolution.second = relationPrime.resolution;

    Token prime_possible_tok = buffer_scan();
    if (
        prime_possible_tok.type == TokenType::LESS_THAN || prime_possible_tok.type == TokenType::LESS_EQUAL ||
        prime_possible_tok.type == TokenType::GREATER_THAN || prime_possible_tok.type == TokenType::GREATER_EQUAL ||
        prime_possible_tok.type == TokenType::EQUAL_TO || prime_possible_tok.type == TokenType::NON_EQUAL
    ) {
        relationPrime.prime = std::make_unique<RelationPrime>();
        std::pair<RelationPrime, Resolution> rightSide = grammar__relationPrime(relationPrime.resolution); // should always be bool 
        *(relationPrime.prime.get()) = std::move(rightSide.first);
        fullResolution.second = rightSide.second;
    }
    fullResolution.first = std::move(relationPrime);

    return fullResolution;
}



Term grammar__term() {
    Term term;

    term.factor = grammar__factor();
    
    Token prime_possible_tok = buffer_scan();
    if (prime_possible_tok.type == TokenType::TIMES || prime_possible_tok.type == TokenType::DIVIDE) {
        
        // typecheck self
        if (term.factor.resolution.resolvedType != Typemark::INTEGER 
        && term.factor.resolution.resolvedType != Typemark::FLOAT) {
            std::string message = "invalid type, only integer and float types can perform arithmetic operations. type: " +
                TypemarkAsString(term.factor.resolution.resolvedType) + "'";
            error_reporting.ReportError(message);
        }

        term.prime = std::make_unique<TermPrime>();
        std::pair<TermPrime, Resolution> rightSide = grammar__termPrime(term.factor.resolution);
        *(term.prime.get()) = std::move(rightSide.first);
        term.resolution = rightSide.second;
    } else {
        term.resolution = term.factor.resolution;
    }

    return term;
}


std::pair<TermPrime, Resolution> grammar__termPrime(Resolution& leftSideTerm) {
    TermPrime termPrime;

    Token termOp_expected_tok = scan();
    switch (termOp_expected_tok.type) {
    case TokenType::TIMES:
        termPrime.term_op = TermOperator::TIMES;
        break;
    case TokenType::DIVIDE:
        termPrime.term_op = TermOperator::DIVIDE;
        break;
    default: // should not occur
        std::string message = "'*' or '/' expected, encountered Token: '" +
            termOp_expected_tok.text + "', TokenType: " +
            TokenTypeAsString(termOp_expected_tok.type);
        error_reporting.ReportError(message);
        break;
    }

    termPrime.factor = grammar__factor();
    
    // typecheck self
    if (termPrime.factor.resolution.resolvedType != Typemark::INTEGER 
    && termPrime.factor.resolution.resolvedType != Typemark::FLOAT) {
        std::string message = "invalid type, only integer and float types can perform arithmetic operations. type: " +
            TypemarkAsString(termPrime.factor.resolution.resolvedType) + "'";
        error_reporting.ReportError(message);
    }

    // shape resolution

    if (leftSideTerm.resolvedShape == TypeShape::ARRAY
    && termPrime.factor.resolution.resolvedShape == TypeShape::ARRAY) {
        // array on array, must be same size

        if (leftSideTerm.resolvedArraySize != termPrime.factor.resolution.resolvedArraySize) {
            std::string message = "arrays must be of the same size to perform operations";
            error_reporting.ReportError(message);
        }

        termPrime.resolution.resolvedShape = TypeShape::ARRAY;
        termPrime.resolution.resolvedArraySize = termPrime.factor.resolution.resolvedArraySize;
    }
    else if (leftSideTerm.resolvedShape == TypeShape::ARRAY
    && termPrime.factor.resolution.resolvedShape == TypeShape::VALUE) {
        // value on array

        termPrime.resolution.resolvedShape = TypeShape::ARRAY;
        termPrime.resolution.resolvedArraySize = leftSideTerm.resolvedArraySize;
    }
    else if (leftSideTerm.resolvedShape == TypeShape::VALUE
    && termPrime.factor.resolution.resolvedShape == TypeShape::ARRAY) {
        // value on array

        termPrime.resolution.resolvedShape = TypeShape::ARRAY;
        termPrime.resolution.resolvedArraySize = termPrime.factor.resolution.resolvedArraySize;
    }
    else if (leftSideTerm.resolvedShape == TypeShape::VALUE
    && termPrime.factor.resolution.resolvedShape == TypeShape::VALUE) {
        // value on value

        termPrime.resolution.resolvedShape = TypeShape::VALUE;
    }
    else termPrime.resolution.resolvedShape = TypeShape::UNASSIGNED;


    // type resolution

    if (leftSideTerm.resolvedType != termPrime.factor.resolution.resolvedType) {
        // one float, one integer. cast to float
        termPrime.resolution.resolvedType = Typemark::FLOAT;
    } else termPrime.resolution.resolvedType = termPrime.factor.resolution.resolvedType;


    // generate llvm

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_termPrime(leftSideTerm, termPrime.factor.resolution, termPrime.resolution, termPrime.term_op);
    }


    // continue parse

    std::pair<TermPrime, Resolution> fullResolution;
    fullResolution.second = termPrime.resolution;

    Token prime_possible_tok = buffer_scan();
    if (prime_possible_tok.type == TokenType::TIMES || prime_possible_tok.type == TokenType::DIVIDE) {
        termPrime.prime = std::make_unique<TermPrime>();
        std::pair<TermPrime, Resolution> rightSide = grammar__termPrime(termPrime.resolution);
        *(termPrime.prime.get()) = std::move(rightSide.first);
        fullResolution.second = rightSide.second; 
    }
    fullResolution.first = std::move(termPrime);

    return fullResolution;
}



Factor grammar__factor() {
    Factor factor;

    Token factor_lead_tok = buffer_scan();

    switch (factor_lead_tok.type) {
    case TokenType::LEFT_PARENTHESIS: // expression
        {
        scan();
        factor.factor = std::make_unique<Expression>();
        *(std::get<std::unique_ptr<Expression>>(factor.factor).get()) = grammar__expression();
        factor.resolution.resolvedType = std::get<std::unique_ptr<Expression>>(factor.factor).get()->resolution.resolvedType;
        
        Token rightParen_expected_tok = scan();
        if (rightParen_expected_tok.type != TokenType::RIGHT_PARENTHESIS) {
            std::string message = "')' expected, encountered Token: '" +
                rightParen_expected_tok.text + "', TokenType: " +
                TokenTypeAsString(rightParen_expected_tok.type);
            error_reporting.ReportError(message);
        }
        break;
        }
    case TokenType::IDENTIFIER: // procedure call or variable access
        {
        Token identifier_tok = scan();
        Token leftparen_possible_tok = buffer_scan();
        if (leftparen_possible_tok.type == TokenType::LEFT_PARENTHESIS) { // procedure call
            scan(); // consume parenthesis
            Procedure_Call procCall;
            procCall.identifier = std::get<std::string>(identifier_tok.value);

            Token arguments_possible_tok = buffer_scan();
            if (arguments_possible_tok.type != TokenType::RIGHT_PARENTHESIS) {
                procCall.parameters.push_back(grammar__expression());
                
                arguments_possible_tok = buffer_scan();
                while (arguments_possible_tok.type == TokenType::COMMA) {
                    scan(); // consume comma
                    procCall.parameters.push_back(grammar__expression());
                    arguments_possible_tok = buffer_scan();
                }
            }

            Token rightparen_expected_tok = scan();
            if (rightparen_expected_tok.type != TokenType::RIGHT_PARENTHESIS) {
                std::string message = "')' expected, encountered Token: '" +
                    rightparen_expected_tok.text + "', TokenType: " +
                    TokenTypeAsString(rightparen_expected_tok.type);
                error_reporting.ReportError(message);
            }

            Symbol* procCall_symbol = symbol_table.lookup(procCall.identifier);
            if (procCall_symbol == nullptr) {
                std::string message = "call made to undeclared procedure: '" +
                    procCall.identifier + "'";
                error_reporting.ReportError(message);
            }
            else if (procCall_symbol->shape != TypeShape::FUNCTION) {
                std::string message = "procedure call on non-procedure symbol: '" +
                    procCall_symbol->name + "'";
                error_reporting.ReportError(message);
            } else {
                if (procCall_symbol->function_parameters.size() != procCall.parameters.size()) {
                    std::string message = "number of parameters passed to procedure: '" +
                        procCall_symbol->name + "' does not match required number of parameters";
                    error_reporting.ReportError(message);
                } else {
                    for (size_t i=0; i<procCall_symbol->function_parameters.size(); ++i) {
                        if (procCall_symbol->function_parameters[i]->shape != procCall.parameters[i].resolution.resolvedShape ||
                            procCall_symbol->function_parameters[i]->type != procCall.parameters[i].resolution.resolvedType) {
                            // no automatic type conversion
                            std::string message = "function input does not match required type of parameter: '" +
                                procCall_symbol->function_parameters[i]->name + "'";
                            error_reporting.ReportError(message);
                        }
                    }
                }
            }
 
            factor.resolution.resolvedShape = TypeShape::VALUE; // procedures always return a value, never an array
            if (procCall_symbol != nullptr) factor.resolution.resolvedType = procCall_symbol->type;

            factor.factor = std::move(procCall);
        } else { // variable access
            // identifier was already consumed to check for procedure call,
            // so variable access function can not be used

            Token bounds_possible_tok = leftparen_possible_tok;
            Factor_VariableAccess var;
            var.minus = false;

            Variable_Access& var_access = var.variable;
            var_access.identifier = std::get<std::string>(identifier_tok.value);

            Symbol* var_symbol = symbol_table.lookup(var_access.identifier);
            if (var_symbol == nullptr) {
                std::string message = "variable access on undeclared variable: '" +
                    var_access.identifier + "'";
                error_reporting.ReportError(message);
            } 

            if (bounds_possible_tok.type == TokenType::LEFT_BRACKET) { // array index access
                scan(); // consume left bracket
                std::unique_ptr<Expression> exprPtr = std::make_unique<Expression>();
                *(exprPtr.get()) = grammar__expression();
                var_access.array_index = std::move(exprPtr);

                Token rightbracket_expected_tok = scan();
                if (rightbracket_expected_tok.type != TokenType::RIGHT_BRACKET) {
                    std::string message = "']' expected, encountered Token: '" +
                        rightbracket_expected_tok.text + "', TokenType: " +
                        TokenTypeAsString(rightbracket_expected_tok.type);
                    error_reporting.ReportError(message);
                }

                if (var_symbol != nullptr) if (var_symbol->shape != TypeShape::ARRAY) {
                    std::string message = "variable array access on non-array symbol: '" +
                        var_access.identifier + "'";
                    error_reporting.ReportError(message);
                }

                if (var_access.array_index->get()->resolution.resolvedShape != TypeShape::VALUE ||
                    var_access.array_index->get()->resolution.resolvedType != Typemark::INTEGER) {
                    std::string message = "non-integer array access index";
                    error_reporting.ReportError(message);
                }

                var_access.resolution.resolvedShape = TypeShape::VALUE;
                factor.resolution.resolvedShape = TypeShape::VALUE;

            } else {
                if (var_symbol != nullptr) { 
                    if (var_symbol->shape == TypeShape::VALUE) {
                        var_access.resolution.resolvedShape = TypeShape::VALUE;
                        factor.resolution.resolvedShape = TypeShape::VALUE;
                    }
                    else if (var_symbol->shape == TypeShape::ARRAY) {
                        var_access.resolution.resolvedShape = TypeShape::ARRAY;
                        factor.resolution.resolvedShape = TypeShape::ARRAY;
                        var_access.resolution.resolvedArraySize = var_symbol->array_size;
                        factor.resolution.resolvedArraySize = var_symbol->array_size;
                    }
                    else {
                        std::string message = "variable value access on non-variable symbol: '" +
                            var_access.identifier + "'";
                        error_reporting.ReportError(message);
                    }
                }
            }

            if (var_symbol != nullptr) {
                var_access.resolution.resolvedType = var_symbol->type;
                factor.resolution.resolvedType = var_symbol->type;
            }

            // perform variableAccess codegen 

            if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
                codegen_variableAccess(var_access);
            }

            factor.factor = std::move(var);
        }
        break;
        }
    case TokenType::MINUS: // variable access or number constant
        {
        scan(); // consume minus
        Token afterMinus_tok = buffer_scan();
        if (afterMinus_tok.type == TokenType::INTEGER_CONSTANT) {
            scan();
            factor.factor = - std::get<int>(afterMinus_tok.value);
            factor.resolution.resolvedType = Typemark::INTEGER;
            factor.resolution.resolvedShape = TypeShape::VALUE;

        } else if (afterMinus_tok.type == TokenType::FLOAT_CONSTANT) {
            scan();
            factor.factor = - std::get<float>(afterMinus_tok.value);
            factor.resolution.resolvedType = Typemark::FLOAT;
            factor.resolution.resolvedShape = TypeShape::VALUE;
        
        } else if (afterMinus_tok.type == TokenType::IDENTIFIER) {
            Factor_VariableAccess var;
            var.minus = true;
            var.variable = grammar__variable_access();
            factor.factor = std::move(var);
            factor.resolution.resolvedShape = var.variable.resolution.resolvedShape;
            factor.resolution.resolvedType = var.variable.resolution.resolvedType;
            if (var.variable.resolution.resolvedShape == TypeShape::ARRAY) 
                factor.resolution.resolvedArraySize = var.variable.resolution.resolvedArraySize;

            if (factor.resolution.resolvedType != Typemark::INTEGER && 
            factor.resolution.resolvedType != Typemark::FLOAT) {
                std::string message = "'-' prefix operator can only be applied to variables of types integer and float";
                error_reporting.ReportError(message);
            }

        } else {
            std::string message = "number constant or identifier expected, encountered Token: '" +
                afterMinus_tok.text + "', TokenType: " +
                TokenTypeAsString(afterMinus_tok.type);
            error_reporting.ReportError(message);
        }
        break;
        }
    case TokenType::INTEGER_CONSTANT:
        {
        Token integer_tok = scan();
        factor.factor = std::get<int>(integer_tok.value);
        factor.resolution.resolvedType = Typemark::INTEGER;
        factor.resolution.resolvedShape = TypeShape::VALUE;

        break;
        }
    case TokenType::FLOAT_CONSTANT:
        {
        Token float_tok = scan();
        factor.factor = std::get<float>(float_tok.value);
        factor.resolution.resolvedType = Typemark::FLOAT;
        factor.resolution.resolvedShape = TypeShape::VALUE;

        break;
        }
    case TokenType::STRING_CONSTANT:
        {
        Token string_tok = scan();
        factor.factor = std::get<std::string>(string_tok.value);
        factor.resolution.resolvedType = Typemark::STRING;
        factor.resolution.resolvedShape = TypeShape::VALUE;

        break;
        }
    case TokenType::BOOL_CONSTANT:
        {
        Token bool_tok = scan();
        factor.factor = (bool)std::get<int>(bool_tok.value);
        factor.resolution.resolvedType = Typemark::BOOL;
        factor.resolution.resolvedShape = TypeShape::VALUE;

        break;
        }
    default:
        std::string message = "identifier, procedure call, or constant expected, encountered Token: '" +
            factor_lead_tok.text + "', TokenType: " +
            TokenTypeAsString(factor_lead_tok.type);
        error_reporting.ReportError(message);
        break;
    }

    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        codegen_factor(factor);
    }
    return factor;
}