#include "header.h"
#include "code_generator.h"


std::string typemarkToLLVMType(Typemark::Typemark ty) {
    switch (ty) {
        case Typemark::INTEGER:
            return "i32";
        case Typemark::FLOAT:
            return "float";
        case Typemark::BOOL:
            return "i1";
        case Typemark::STRING:
            return "i8*";
        default:
            return "ERRTYPE";
    }
}


void codegen_builtinFunctions() {
    std::string llvmBuiltinsString = "";

    // getters

    const std::string GetBool = "getbool";
    const std::string GetBoolExtern = "getbool_impl"; 
    Symbol* getBool_symbol = symbol_table.declare_global(GetBool);
    getBool_symbol->shape = TypeShape::FUNCTION;
    getBool_symbol->type = Typemark::BOOL;
    getBool_symbol->llvmName = "@" + GetBool;
    llvmBuiltinsString += "declare i8 @" + GetBoolExtern + "()\n";
    llvmBuiltinsString += "define i1 " + getBool_symbol->llvmName + "() {\n";
    llvmBuiltinsString += "%val = call i8 @" + GetBoolExtern + "()\n";
    llvmBuiltinsString += "%cast = icmp ne i8 0, %val\n";
    llvmBuiltinsString += "ret i1 %cast\n";
    llvmBuiltinsString += "}\n\n";

    const std::string GetInteger = "getinteger";
    const std::string GetIntegerExtern = "getinteger_impl";
    Symbol* getInteger_symbol = symbol_table.declare_global(GetInteger);
    getInteger_symbol->shape = TypeShape::FUNCTION;
    getInteger_symbol->type = Typemark::INTEGER;
    getInteger_symbol->llvmName = "@" + GetInteger;
    llvmBuiltinsString += "declare i32 @" + GetIntegerExtern + "()\n";
    llvmBuiltinsString += "define i32 " + getInteger_symbol->llvmName + "() {\n";
    llvmBuiltinsString += "%val = call i32 @" + GetIntegerExtern + "()\n";
    llvmBuiltinsString += "ret i32 %val\n";
    llvmBuiltinsString += "}\n\n";

    const std::string GetFloat = "getfloat";
    const std::string GetFloatExtern = "getfloat_impl";
    Symbol* getFloat_symbol = symbol_table.declare_global(GetFloat);
    getFloat_symbol->shape = TypeShape::FUNCTION;
    getFloat_symbol->type = Typemark::FLOAT;
    getFloat_symbol->llvmName = "@" + GetFloat;
    llvmBuiltinsString += "declare float @" + GetFloatExtern + "()\n";
    llvmBuiltinsString += "define float " + getFloat_symbol->llvmName + "() {\n";
    llvmBuiltinsString += "%val = call float @" + GetFloatExtern + "()\n";
    llvmBuiltinsString += "ret float %val\n";
    llvmBuiltinsString += "}\n\n";

    const std::string GetString = "getstring";
    const std::string GetStringExtern = "getstring_impl";
    Symbol* getString_symbol = symbol_table.declare_global(GetString);
    getString_symbol->shape = TypeShape::FUNCTION;
    getString_symbol->type = Typemark::STRING;
    getString_symbol->llvmName = "@" + GetString;
    llvmBuiltinsString += "declare i8* @" + GetStringExtern + "()\n";
    llvmBuiltinsString += "define i8* " + getString_symbol->llvmName + "() {\n";
    llvmBuiltinsString += "%val = call i8* @" + GetStringExtern + "()\n";
    llvmBuiltinsString += "ret i8* %val\n";
    llvmBuiltinsString += "}\n\n";

    // putters

    const std::string PutBool = "putbool";
    const std::string PutBoolExtern = "putbool_impl";
    Symbol* putBool_symbol = symbol_table.declare_global(PutBool);
    putBool_symbol->shape = TypeShape::FUNCTION;
    putBool_symbol->type = Typemark::BOOL;
    putBool_symbol->llvmName = "@" + PutBool;
    putBool_symbol->function_parameters.push_back(new Symbol({"","",TypeShape::VALUE, Typemark::BOOL}));
    llvmBuiltinsString += "declare i8 @" + PutBoolExtern + "(i8)\n";
    llvmBuiltinsString += "define i1 " + putBool_symbol->llvmName + "(i1 %val) {\n";
    llvmBuiltinsString += "%cast = zext i1 %val to i8\n";
    llvmBuiltinsString += "%tmp = call i8 @" + PutBoolExtern + "(i8 %cast)\n";
    llvmBuiltinsString += "ret i1 1\n";
    llvmBuiltinsString += "}\n\n";

    const std::string PutInteger = "putinteger";
    const std::string PutIntegerExtern = "putinteger_impl";
    Symbol* putInteger_symbol = symbol_table.declare_global(PutInteger);
    putInteger_symbol->shape = TypeShape::FUNCTION;
    putInteger_symbol->type = Typemark::BOOL;
    putInteger_symbol->llvmName = "@" + PutInteger;
    putInteger_symbol->function_parameters.push_back(new Symbol({"","",TypeShape::VALUE, Typemark::INTEGER}));
    llvmBuiltinsString += "declare i8 @" + PutIntegerExtern + "(i32)\n";
    llvmBuiltinsString += "define i1 " + putInteger_symbol->llvmName + "(i32 %val) {\n";
    llvmBuiltinsString += "%tmp = call i8 @" + PutIntegerExtern + "(i32 %val)\n";
    llvmBuiltinsString += "ret i1 1\n";
    llvmBuiltinsString += "}\n\n";

    const std::string PutFloat = "putfloat";
    const std::string PutFloatExtern = "putfloat_impl";
    Symbol* putFloat_symbol = symbol_table.declare_global(PutFloat);
    putFloat_symbol->shape = TypeShape::FUNCTION;
    putFloat_symbol->type = Typemark::BOOL;
    putFloat_symbol->llvmName = "@" + PutFloat;
    putFloat_symbol->function_parameters.push_back(new Symbol({"","",TypeShape::VALUE, Typemark::FLOAT}));
    llvmBuiltinsString += "declare i8 @" + PutFloatExtern + "(float)\n";
    llvmBuiltinsString += "define i1 " + putFloat_symbol->llvmName + "(float %val) {\n";
    llvmBuiltinsString += "%tmp = call i8 @" + PutFloatExtern + "(float %val)\n";
    llvmBuiltinsString += "ret i1 1\n";
    llvmBuiltinsString += "}\n\n";

    const std::string PutString = "putstring";
    const std::string PutStringExtern = "putstring_impl";
    Symbol* putString_symbol = symbol_table.declare_global(PutString);
    putString_symbol->shape = TypeShape::FUNCTION;
    putString_symbol->type = Typemark::BOOL;
    putString_symbol->llvmName = "@" + PutString;
    putString_symbol->function_parameters.push_back(new Symbol({"","",TypeShape::VALUE, Typemark::STRING}));
    llvmBuiltinsString += "declare i8 @" + PutStringExtern + "(i8*)\n";
    llvmBuiltinsString += "define i1 " + putString_symbol->llvmName + "(i8* %val) {\n";
    llvmBuiltinsString += "%tmp = call i8 @" + PutStringExtern + "(i8* %val)\n";
    llvmBuiltinsString += "ret i1 1\n";
    llvmBuiltinsString += "}\n\n";

    // sqrt

    const std::string Sqrt = "sqrt";
    const std::string SqrtExtern = "sqrt_impl";
    Symbol* sqrt_symbol = symbol_table.declare_global(Sqrt);
    sqrt_symbol->shape = TypeShape::FUNCTION;
    sqrt_symbol->type = Typemark::FLOAT;
    sqrt_symbol->llvmName = "@" + Sqrt;
    llvmBuiltinsString += "declare float @" + SqrtExtern + "(float)\n";
    llvmBuiltinsString += "define float " + sqrt_symbol->llvmName + "(float %val) {\n";
    llvmBuiltinsString += "%res = call float @" + SqrtExtern + "(float %val)\n";
    llvmBuiltinsString += "ret float %res\n";
    llvmBuiltinsString += "}\n\n";

    // exception handling
    llvmBuiltinsString += "declare i8 @exception_block()\n";
    llvmBuiltinsString += "declare void @throw_arrayoutofbounds()\n"; 


    llvmAssemblyStrings.builtinFunctionDeclarations += llvmBuiltinsString;
}



void codegen_variableDeclaration(Variable_Information& variable) {
    Symbol* variable_symbol = symbol_table.lookup(variable.identifier);
    if (variable.isGlobal) variable_symbol->llvmName = "@" + variable_symbol->name;
    else variable_symbol->llvmName = "%" + variable_symbol->name;

    std::string declaration_assembly = variable_symbol->llvmName;  
    
    declaration_assembly += " = ";

    if (variable.isGlobal) declaration_assembly += "global ";
    else declaration_assembly += "alloca ";

    if (variable.isArray) {
        declaration_assembly += "[";
        declaration_assembly += std::to_string(variable.bound);
        declaration_assembly += " x ";
    }
    declaration_assembly += typemarkToLLVMType(variable.type);

    if (variable.isArray) {
        declaration_assembly += "]";
    }

    if (variable.isGlobal) declaration_assembly += " zeroinitializer";
    declaration_assembly += "\n";

    if (variable.isGlobal) {
        llvmAssemblyStrings.globalVariableDeclarations += declaration_assembly;
    }
    else {
        llvmAssemblyStrings.functionDefinitions.at(symbol_table.getCurrentScopeFullName()
            ).llvm += declaration_assembly;
    }
}



void codegen_procedureHeader(Symbol* procedure_symbol) {

    std::string functionScopeName = symbol_table.getCurrentScopeFullName();
    std::string functionLlvmName = "@" + functionScopeName;
    procedure_symbol->llvmName = functionLlvmName;
    LlvmAssemblyStrings::FunctionString functionDefinition {};
    
    std::string declaration_assembly = "define ";
    
    declaration_assembly += typemarkToLLVMType(procedure_symbol->type) + " ";

    declaration_assembly += functionLlvmName;
    declaration_assembly += "(";

    std::string parameter_declarations = "";

    bool first = true;
    for (Symbol* param : procedure_symbol->function_parameters) {
        
        if (!first) declaration_assembly += ", ";
        first = false;

        param->llvmName = "%" + param->name;

        if (param->shape == TypeShape::VALUE) { // copy value to a variable
            std::string paramName = functionDefinition.GetNextUnnamedReg();

            declaration_assembly += typemarkToLLVMType(param->type);
            declaration_assembly += " " + paramName;

            parameter_declarations += param->llvmName + " = alloca " + typemarkToLLVMType(param->type) + "\n";
            parameter_declarations += "store " + typemarkToLLVMType(param->type) + " " + paramName;
            parameter_declarations += ", ptr " + param->llvmName + "\n";
        } 
        else { // array is already a ptr
            declaration_assembly += "[" + std::to_string(param->array_size) + " x ";
            declaration_assembly += typemarkToLLVMType(param->type) + "]";
            declaration_assembly += " " + param->llvmName;
        }

    }

    declaration_assembly += ") {\n";
    declaration_assembly += functionDefinition.GetNextBlockLabel() + ":\n";

    declaration_assembly += parameter_declarations;

    functionDefinition.llvm = declaration_assembly;

    llvmAssemblyStrings.functionDefinitions.insert({functionScopeName, functionDefinition});
}

void codegen_procedureEnd() {
    std::string functionScopeName = symbol_table.getCurrentScopeFullName();
    llvmAssemblyStrings.functionDefinitions.at(functionScopeName).llvm += "}\n\n";
}



void codegen_assignmentStatement(Variable_Access& variable, Expression& rightSideExpr, TypeResolutionCase::TypeResolutionCase typeResolution_cast) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);
    
    std::string assign_assembly = "";
    std::string cast_register = "";
    std::string cast_assembly = "";
    std::string store_assembly = "";

    bool fromIsArray = rightSideExpr.resolution.resolvedShape == TypeShape::ARRAY;
    bool toIsArray = variable.resolution.resolvedShape == TypeShape::ARRAY;
    unsigned len = variable.resolution.resolvedArraySize;

    switch (typeResolution_cast) {
        case TypeResolutionCase::straight:
            cast_register = rightSideExpr.resolution.outputRegister; // no cast
            break;
        case TypeResolutionCase::int_to_bool: 
            // icmp needs to handle array case to compare against 0
            cast_register = currentFunctionLlvm.GetNextUnnamedReg();
            if (fromIsArray) {
                cast_assembly += cast_register + " = alloca [" + std::to_string(len) + " x i1]\n";
                for (size_t i=0 ; i<len; ++i) {
                    std::string elemAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                    cast_assembly += elemAccessReg + " = getelementptr [" + std::to_string(len) + " x i32]";
                    cast_assembly += ", ptr " + rightSideExpr.resolution.outputRegister;
                    cast_assembly += ", i32 0, i32 " + std::to_string(i) + "\n";

                    std::string elemLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                    cast_assembly += elemLoadReg + " = load i32, ptr " + elemAccessReg + "\n";

                    std::string compReg = currentFunctionLlvm.GetNextUnnamedReg();
                    cast_assembly += compReg + " = icmp ne i32 ";
                    cast_assembly += elemLoadReg + ", 0\n";

                    std::string elemTargetReg = currentFunctionLlvm.GetNextUnnamedReg();\
                    cast_assembly += elemTargetReg + " = getelementptr [" + std::to_string(len) + " x i1]";
                    cast_assembly += ", ptr " + cast_register;
                    cast_assembly += ", i32 0, i32 " + std::to_string(i) + "\n";

                    cast_assembly += "store i1 " + compReg + ", ptr " + elemTargetReg + "\n";
                }
            } else {
                cast_assembly = cast_register + " = icmp ne i32 ";
                cast_assembly += rightSideExpr.resolution.outputRegister + ", 0\n";    
            }
            break;
        case TypeResolutionCase::bool_to_int:
            cast_register = currentFunctionLlvm.GetNextUnnamedReg();

            cast_assembly = cast_register + " = zext ";
            if (fromIsArray) cast_assembly += "[ " + std::to_string(len) + " x i1]";
            else cast_assembly += "i1";
            cast_assembly += rightSideExpr.resolution.outputRegister + " to ";
            if (fromIsArray) cast_assembly += "[ " + std::to_string(len) + " x i32]\n";
            else cast_assembly += "i32\n";
            break;
        case TypeResolutionCase::int_to_float:
            cast_register = currentFunctionLlvm.GetNextUnnamedReg();

            cast_assembly = cast_register + " = sitofp ";
            if (fromIsArray) cast_assembly += "[ " + std::to_string(len) + " x i32]";
            else cast_assembly += "i32";
            cast_assembly += rightSideExpr.resolution.outputRegister + " to ";
            if (fromIsArray) cast_assembly += "[ " + std::to_string(len) + " x float]\n";
            else cast_assembly += "float\n";
            break;
        case TypeResolutionCase::float_to_int:
            cast_register = currentFunctionLlvm.GetNextUnnamedReg();

            cast_assembly = cast_register + " = fptosi ";
            if (fromIsArray) cast_assembly += "[ " + std::to_string(len) + " x float]";
            else cast_assembly += "float";
            cast_assembly += rightSideExpr.resolution.outputRegister + " to ";
            if (fromIsArray) cast_assembly += "[ " + std::to_string(len) + " x i32]\n";
            else cast_assembly += "i32\n";
            break;
    }

    if (toIsArray == fromIsArray) {
        store_assembly = "store ";
        switch (variable.resolution.resolvedType) {
            case Typemark::INTEGER:
                if (toIsArray) store_assembly += "[" + std::to_string(len) + " x i32] ";
                else store_assembly += "i32 ";
                break;
            case Typemark::FLOAT:
                if (toIsArray) store_assembly += "[" + std::to_string(len) + " x float] ";
                else store_assembly += "float ";
                break;
            case Typemark::BOOL:
                if (toIsArray) store_assembly += "[" + std::to_string(len) + " x i1] ";
                else store_assembly += "i1 ";
                break;
            case Typemark::STRING:
                if (toIsArray) store_assembly += "[" + std::to_string(len) + " x i8*] ";
                else store_assembly += "i8* ";
                break;
        }
        store_assembly += cast_register + ", ptr " + variable.resolution.outputRegister + "\n";
    } 
    else for (size_t i=0; i<len; ++i) {
        // insert the resolved value at each index of the store target
        std::string index_register = currentFunctionLlvm.GetNextUnnamedReg();

        std::string indexLoad_assembly = "";
        std::string indexStore_assembly = "";

        indexLoad_assembly = index_register + " = getelementptr [" + std::to_string(len) + " x ";
        switch (variable.resolution.resolvedType) {
            case Typemark::INTEGER:
                indexLoad_assembly += "i32";
                break;
            case Typemark::FLOAT:
                indexLoad_assembly += "float";
                break;
            case Typemark::BOOL:
                indexLoad_assembly += "i1";
                break;
            case Typemark::STRING:
                indexLoad_assembly += "i8*";
                break;
        }
        indexLoad_assembly += "], ptr " + variable.resolution.outputRegister;
        indexLoad_assembly += + ", i32 0, i32 " + std::to_string(i) + "\n";

        indexStore_assembly = "store ";
        switch (variable.resolution.resolvedType) {
            case Typemark::INTEGER:
                indexStore_assembly += "i32";
                break;
            case Typemark::FLOAT:
                indexStore_assembly += "float";
                break;
            case Typemark::BOOL:
                indexStore_assembly += "i1";
                break;
            case Typemark::STRING:
                indexStore_assembly += "i8*";
                break;
        }
        indexStore_assembly += " " + cast_register + ", ptr " + index_register + "\n";

        store_assembly += indexLoad_assembly + indexStore_assembly;
    }
    assign_assembly = cast_assembly + store_assembly;
    currentFunctionLlvm.llvm += assign_assembly;
}






void codegen_factor(Factor& factor) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string factor_assembly = "";

    switch (factor.factor.index()) {
        case 0: {
            Expression& expr = *std::get<0>(factor.factor);
            factor.resolution = expr.resolution; // no additional assembly needed
            break;
        }
        case 1: {
            Procedure_Call& proc = std::get<1>(factor.factor);
            Symbol* procSymbol = symbol_table.lookup(proc.identifier);
            std::string callTarget_register = currentFunctionLlvm.GetNextUnnamedReg();

            factor_assembly += callTarget_register + " = call ";
            switch (procSymbol->type) {
                case Typemark::INTEGER:
                    factor_assembly += "i32 ";
                    break;
                case Typemark::FLOAT:
                    factor_assembly += "float ";
                    break;
                case Typemark::BOOL:
                    factor_assembly += "i1 ";
                    break;
                case Typemark::STRING:
                    factor_assembly += "i8* ";
                    break;
            }
            factor_assembly += procSymbol->llvmName + "(";
            
            bool first = true;
            for (Expression& param : proc.parameters) { // type checking was already done by the parser, types must match
                if (!first) {
                    factor_assembly += ", ";
                    first = false;
                }

                bool isArray = param.resolution.resolvedShape == TypeShape::ARRAY;
                switch (param.resolution.resolvedType) {
                    case Typemark::INTEGER:
                        if (isArray) factor_assembly += "[ " + std::to_string(param.resolution.resolvedArraySize) + " x i32] "; 
                        else factor_assembly += "i32 ";
                        break;
                    case Typemark::FLOAT:
                        if (isArray) factor_assembly += "[ " + std::to_string(param.resolution.resolvedArraySize) + " x float] "; 
                        else factor_assembly += "float ";
                        break;
                    case Typemark::BOOL:
                        if (isArray) factor_assembly += "[ " + std::to_string(param.resolution.resolvedArraySize) + " x i1] "; 
                        else factor_assembly += "i1 ";
                        break;
                    case Typemark::STRING:
                        if (isArray) factor_assembly += "[ " + std::to_string(param.resolution.resolvedArraySize) + " x i8*] "; 
                        else factor_assembly += "i8* ";
                        break;
                }
                factor_assembly += param.resolution.outputRegister;
            }
            factor_assembly += ")\n";
            
            factor.resolution.outputRegister = callTarget_register;
            break;
        }
        case 2: {
            Factor_VariableAccess& var = std::get<2>(factor.factor);
            if (var.variable.resolution.resolvedShape == TypeShape::VALUE && !var.minus) {
                std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
                factor_assembly += outReg + " = load " + typemarkToLLVMType(var.variable.resolution.resolvedType);
                factor_assembly += ", ptr " + var.variable.resolution.outputRegister + "\n";
                
                factor.resolution.outputRegister = outReg;
            } 
            else if (var.variable.resolution.resolvedShape == TypeShape::ARRAY && !var.minus) {
                factor.resolution.outputRegister = var.variable.resolution.outputRegister;

            }
            if (var.variable.resolution.resolvedShape == TypeShape::VALUE && var.minus) {
                
                std::string valReg = currentFunctionLlvm.GetNextUnnamedReg();
                if (var.variable.resolution.resolvedType == Typemark::INTEGER) {
                    factor_assembly += valReg + " = load i32, ptr " + var.variable.resolution.outputRegister + "\n";
                } else {
                    factor_assembly += valReg + " = load float, ptr " + var.variable.resolution.outputRegister + "\n";
                }

                std::string subtract_register = currentFunctionLlvm.GetNextUnnamedReg();

                if (var.variable.resolution.resolvedType == Typemark::INTEGER) {
                    factor_assembly += subtract_register + " = sub i32 0, " + valReg + "\n";
                } else {
                    factor_assembly += subtract_register + " = sub float 0.0, " + valReg + "\n";
                }

                factor.resolution.outputRegister = subtract_register;
            } 
            else if (var.variable.resolution.resolvedShape == TypeShape::ARRAY && var.minus) {
                unsigned len = var.variable.resolution.resolvedArraySize;

                std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
                if (var.variable.resolution.resolvedType == Typemark::INTEGER) {
                    factor_assembly += outReg + " = alloca [" + std::to_string(len) + " x i32]\n";
                } else {
                    factor_assembly += outReg + " = alloca [" + std::to_string(len) + " x float]\n";
                }

                for (size_t i=0; i<len; ++i) {
                    // subtract each value in the array from 0
                    std::string subtract_assembly = "";

                    // get pointer to index in source array
                    std::string elemAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                    if (var.variable.resolution.resolvedType == Typemark::INTEGER) {
                        subtract_assembly += elemAccessReg + " = getelementptr [" + std::to_string(len) + " x i32]";
                    } else {
                        subtract_assembly += elemAccessReg + " = getelementptr [" + std::to_string(len) + " x float]";
                    }
                    subtract_assembly += ", ptr " + var.variable.resolution.outputRegister;
                    subtract_assembly += ", i32 0, i32 " + std::to_string(i) + "\n";

                    // load value from index
                    std::string loadReg = currentFunctionLlvm.GetNextUnnamedReg();
                    if (var.variable.resolution.resolvedType == Typemark::INTEGER) {
                        subtract_assembly += loadReg + " = load i32, ptr " + elemAccessReg + "\n";
                    } else {
                        subtract_assembly += loadReg + " = load float, ptr " + elemAccessReg + "\n";
                    }

                    // subtract value from zero
                    std::string negReg = currentFunctionLlvm.GetNextUnnamedReg();
                    if (var.variable.resolution.resolvedType == Typemark::INTEGER) {
                        subtract_assembly += negReg + " = sub i32 0, " + loadReg + "\n";
                    } else {
                        subtract_assembly += negReg + " = sub float 0.0, " + loadReg + "\n";
                    }

                    // get pointer to index in target array
                    std::string elemTargetReg = currentFunctionLlvm.GetNextUnnamedReg();
                    if (var.variable.resolution.resolvedType == Typemark::INTEGER) {
                        subtract_assembly += elemTargetReg + " = getelementptr [" + std::to_string(len) + " x i32]";
                    } else {
                        subtract_assembly += elemTargetReg + " = getelementptr [" + std::to_string(len) + " x float]";
                    }
                    subtract_assembly += ", ptr " + outReg + ", i32 0, i32 " + std::to_string(i) + "\n";

                    // store value at index
                    if (var.variable.resolution.resolvedType == Typemark::INTEGER) {
                        subtract_assembly += "store i32 " + negReg + ", ptr " + elemTargetReg + "\n";
                    } else {
                        subtract_assembly += "store float " + negReg + ", ptr " + elemTargetReg + "\n";
                    }

                    factor_assembly += subtract_assembly;
                }

                factor.resolution.outputRegister = outReg;
            }
            
            break;
        }
        case 3: {
            int val = std::get<3>(factor.factor);

            std::string valReg = currentFunctionLlvm.GetNextUnnamedReg();
            factor_assembly += valReg + " = add i32 0, " + std::to_string(val) + "\n";

            factor.resolution.outputRegister = valReg;
            break;
        }
        case 4: {
            float val = std::get<4>(factor.factor);

            std::string valReg = currentFunctionLlvm.GetNextUnnamedReg();
            factor_assembly += valReg + " = add float 0.0, " + std::to_string(val) + "\n";

            factor.resolution.outputRegister = valReg;
            break;
        }
        case 5: {
            std::string val = std::get<5>(factor.factor);

            // add value as string constant
            std::string strConstReg = llvmAssemblyStrings.GetNextGlobalUnnamedReg();
            
            std::string strConstant_assembly = strConstReg + " = constant [";
            strConstant_assembly += std::to_string(val.size()+1); // std::string drops the null terminator
            strConstant_assembly += " x i8] c\"";
            for (char c : val) {
                if (c=='\\') strConstant_assembly += "\\\\";
                else strConstant_assembly += c;
            }
            strConstant_assembly += "\\00\"\n";
            llvmAssemblyStrings.globalVariableDeclarations += strConstant_assembly;

            // get pointer to string constant
            std::string ptrReg = currentFunctionLlvm.GetNextUnnamedReg();
            factor_assembly += ptrReg + " = getelementptr [" + std::to_string(val.size()+1) + " x i8]";
            factor_assembly += ", i8* " + strConstReg + ", i32 0\n";

            factor.resolution.outputRegister = ptrReg;
            break;
        }
        case 6: {
            bool val = std::get<6>(factor.factor);

            std::string valReg = currentFunctionLlvm.GetNextUnnamedReg();
            factor_assembly += valReg + " = add i1 0, " + std::to_string((unsigned)val) + "\n";

            factor.resolution.outputRegister = valReg;
            break;
        }
    }

    currentFunctionLlvm.llvm += factor_assembly;
}



void codegen_termPrime(const Resolution& leftSideTerm, const Resolution& rightSideTerm, Resolution& output, TermOperator::TermOperator op) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string termP_assembly = "";

    std::string leftSideReg = leftSideTerm.outputRegister;
    std::string rightSideReg = rightSideTerm.outputRegister;

    // cast if type mismatch

    if (leftSideTerm.resolvedType == Typemark::INTEGER
    && output.resolvedType == Typemark::FLOAT) {

        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
        termP_assembly += castReg + " = sitofp ";

        if (leftSideTerm.resolvedShape == TypeShape::ARRAY) {
            termP_assembly += "[" + std::to_string(leftSideTerm.resolvedArraySize) + " x i32] ";
        } else {
            termP_assembly += "i32 ";
        }

        termP_assembly += leftSideTerm.outputRegister + " to ";

        if (leftSideTerm.resolvedShape == TypeShape::ARRAY) {
            termP_assembly += "[" + std::to_string(leftSideTerm.resolvedArraySize) + " x float] ";
        } else {
            termP_assembly += "float ";
        }

        leftSideReg = castReg;
    }
    else if (rightSideTerm.resolvedType == Typemark::INTEGER
    && output.resolvedType == Typemark::FLOAT) {

        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
        termP_assembly += castReg + " = sitofp ";

        if (rightSideTerm.resolvedShape == TypeShape::ARRAY) {
            termP_assembly += "[" + std::to_string(leftSideTerm.resolvedArraySize) + " x i32] ";
        } else {
            termP_assembly += "i32 ";
        }

        termP_assembly += rightSideTerm.outputRegister + " to ";

        if (rightSideTerm.resolvedShape == TypeShape::ARRAY) {
            termP_assembly += "[" + std::to_string(rightSideTerm.resolvedArraySize) + " x float] ";
        } else {
            termP_assembly += "float ";
        }

        rightSideReg = castReg;
    }

    // perform operation

    if (leftSideTerm.resolvedShape == rightSideTerm.resolvedShape) {
        
        std::string termReg = currentFunctionLlvm.GetNextUnnamedReg();

        termP_assembly += termReg + " = ";

        if (output.resolvedType == Typemark::FLOAT) {
            termP_assembly += "f";
        } else if (op == TermOperator::DIVIDE) { // integer divide
            termP_assembly += "s";
        }

        if (op == TermOperator::TIMES) termP_assembly += "mul ";
        else termP_assembly += "div ";

        if (leftSideTerm.resolvedShape == TypeShape::ARRAY) {
            termP_assembly += "[" + std::to_string(leftSideTerm.resolvedArraySize) + " x ";
            if (output.resolvedType == Typemark::FLOAT) {
                termP_assembly += "float] ";
            } else termP_assembly += "i32] ";
        } else {
            if (output.resolvedType == Typemark::FLOAT) {
                termP_assembly += "float ";
            } else termP_assembly += "i32 ";
        }

        termP_assembly += leftSideReg + ", " + rightSideReg + "\n";

        output.outputRegister = termReg;
    } else {

        std::string arrayReg;
        std::string valReg;
        unsigned arrayLen = output.resolvedArraySize;

        if (leftSideTerm.resolvedShape == TypeShape::ARRAY) {
            arrayReg = leftSideReg;
            valReg = rightSideReg;
        } else {
            arrayReg = rightSideReg;
            valReg = leftSideReg;
        }

        std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
        termP_assembly += outReg + " = alloca ";

        if (output.resolvedType == Typemark::INTEGER) {
            termP_assembly += "[" + std::to_string(arrayLen) + " x i32]\n";
        } else {
            termP_assembly += "[" + std::to_string(arrayLen) + " x float]\n";
        }

        for (size_t i=0; i<arrayLen; ++i) {

            std::string elemAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
            termP_assembly += elemAccessReg + " = getelementptr ";
            if (output.resolvedType == Typemark::INTEGER) {
                termP_assembly += "[" + std::to_string(arrayLen) + " x i32]";
            } else {
                termP_assembly += "[" + std::to_string(arrayLen) + " x float]";
            }
            termP_assembly += ", ptr " + arrayReg + " i32 0, i32 " + std::to_string(i) + "\n";

            std::string loadReg = currentFunctionLlvm.GetNextUnnamedReg();
            termP_assembly += loadReg + " = load ";
            if (output.resolvedType == Typemark::INTEGER) {
                termP_assembly += "i32, ";
            } else {
                termP_assembly += "float, ";
            }
            termP_assembly += elemAccessReg + "\n";

            std::string resultReg = currentFunctionLlvm.GetNextUnnamedReg();
            termP_assembly += resultReg + " = ";
            if (output.resolvedType == Typemark::FLOAT) {
                termP_assembly += "f";
            } else if (op == TermOperator::DIVIDE) { 
                termP_assembly += "s";
            }
            if (op == TermOperator::TIMES) termP_assembly += "mul ";
            else termP_assembly += "div ";

            if (output.resolvedType == Typemark::INTEGER) {
                termP_assembly += "i32 ";
            } else termP_assembly += "float ";

            termP_assembly += loadReg + ", " + valReg + "\n";

            std::string elemTargetReg = currentFunctionLlvm.GetNextUnnamedReg();
            termP_assembly += elemTargetReg + " = getelementptr ";
            if (output.resolvedType == Typemark::INTEGER) {
                termP_assembly += "[" + std::to_string(arrayLen) + " x i32]";
            } else {
                termP_assembly += "[" + std::to_string(arrayLen) + " x float]";
            }
            termP_assembly += ", ptr " + outReg + " i32 0, i32 " + std::to_string(i) + "\n";

            termP_assembly += "store ";
            if (output.resolvedType == Typemark::INTEGER) {
                termP_assembly += "i32, ";
            } else {
                termP_assembly += "float, ";
            }
            termP_assembly += resultReg + ", ptr " + elemTargetReg + "\n";
        }

        output.outputRegister = outReg;
    }

    currentFunctionLlvm.llvm += termP_assembly;
}


void codegen_relationPrime(const Resolution& leftSideRelation, const Resolution& rightSideRelation, Resolution& output, RelationOperator::RelationOperator op) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string RelationP_assembly = "";

    std::string leftSideReg = leftSideRelation.outputRegister;
    std::string rightSideReg = rightSideRelation.outputRegister;

    // cast if type mismatch

    // specification does not state whether to cast the left or right value
    // so I will cast the right value to that of the left

    if (leftSideRelation.resolvedType == Typemark::INTEGER
    && rightSideRelation.resolvedType == Typemark::BOOL) {

        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
        RelationP_assembly += castReg + " = zext ";
        if (rightSideRelation.resolvedShape == TypeShape::ARRAY) {
            RelationP_assembly += "[" + std::to_string(rightSideRelation.resolvedArraySize) + " x i1]";
        } else RelationP_assembly += "i1";
        RelationP_assembly += " to ";
        if (rightSideRelation.resolvedShape == TypeShape::ARRAY) {
            RelationP_assembly += "[" + std::to_string(rightSideRelation.resolvedArraySize) + " x i32]\n";
        } else RelationP_assembly += "i32\n";

        rightSideReg = castReg;
    }
    else if (leftSideRelation.resolvedType == Typemark::BOOL
    && rightSideRelation.resolvedType == Typemark::INTEGER) {
        // icmp needs to handle array case to compare against 0
        if (rightSideRelation.resolvedShape == TypeShape::ARRAY) {
            unsigned arrLen = rightSideRelation.resolvedArraySize;
            std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += castReg + " = alloca [" + std::to_string(arrLen) + " x i1]\n"; 
            for (size_t i=0; i<arrLen; ++i) {
                std::string elemAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += elemAccessReg + " = getelementptr [" + std::to_string(arrLen) + " x i32]";
                RelationP_assembly += ", ptr " + rightSideReg;
                RelationP_assembly += ", i32 0, i32 " + std::to_string(i) + "\n";

                std::string elemLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += elemLoadReg + " = load i32, ptr " + elemAccessReg + "\n";

                std::string compReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += compReg + " = icmp ne i32 ";
                RelationP_assembly += elemLoadReg + ", 0\n";

                std::string elemTargetReg = currentFunctionLlvm.GetNextUnnamedReg();\
                RelationP_assembly += elemTargetReg + " = getelementptr [" + std::to_string(arrLen) + " x i1]";
                RelationP_assembly += ", ptr " + castReg;
                RelationP_assembly += ", i32 0, i32 " + std::to_string(i) + "\n";

                RelationP_assembly += "store i1 " + compReg + ", ptr " + elemTargetReg + "\n";
            }
            rightSideReg = castReg;
        } else {
            std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += castReg + " = icmp ne i32 ";
            RelationP_assembly += rightSideReg + ", 0\n";
            rightSideReg = castReg;
        }
    }

    // perform operation

    Typemark::Typemark ty = leftSideRelation.resolvedType;
    if (leftSideRelation.resolvedShape == rightSideRelation.resolvedShape) {
        
        if (ty == Typemark::INTEGER) {

            std::string outputReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outputReg + " = icmp ";

            if (op == RelationOperator::EQUAL) RelationP_assembly += "eq ";
            else if (op == RelationOperator::NON_EQUAL) RelationP_assembly += "ne ";
            else if (op == RelationOperator::GREATER) RelationP_assembly += "sgt ";
            else if (op == RelationOperator::GREATER_EQUAL) RelationP_assembly += "sge ";
            else if (op == RelationOperator::LESS) RelationP_assembly += "slt ";
            else if (op == RelationOperator::LESS_EQUAL) RelationP_assembly += "sle ";
            
            if (output.resolvedShape == TypeShape::ARRAY) {
                RelationP_assembly += "[" + std::to_string(output.resolvedArraySize) + " x i32]";
            }
            else RelationP_assembly += "i32";

            RelationP_assembly += " " + leftSideReg + ", " + rightSideReg + "\n";
            output.outputRegister = outputReg;
        }
        else if (ty == Typemark::BOOL) {

            std::string outputReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outputReg + " = icmp ";

            if (op == RelationOperator::EQUAL) RelationP_assembly += "eq ";
            else if (op == RelationOperator::NON_EQUAL) RelationP_assembly += "ne ";
            else if (op == RelationOperator::GREATER) RelationP_assembly += "ugt ";
            else if (op == RelationOperator::GREATER_EQUAL) RelationP_assembly += "uge ";
            else if (op == RelationOperator::LESS) RelationP_assembly += "ult ";
            else if (op == RelationOperator::LESS_EQUAL) RelationP_assembly += "ule ";
            
            if (output.resolvedShape == TypeShape::ARRAY) {
                RelationP_assembly += "[" + std::to_string(output.resolvedArraySize) + " x i1]";
            }
            else RelationP_assembly += "i1";

            RelationP_assembly += " " + leftSideReg + ", " + rightSideReg + "\n";
            output.outputRegister = outputReg;
        }
        else if (ty == Typemark::FLOAT) {

            std::string outputReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outputReg + " = fcmp ";

            if (op == RelationOperator::EQUAL) RelationP_assembly += "oeq ";
            else if (op == RelationOperator::NON_EQUAL) RelationP_assembly += "one ";
            else if (op == RelationOperator::GREATER) RelationP_assembly += "ogt ";
            else if (op == RelationOperator::GREATER_EQUAL) RelationP_assembly += "oge ";
            else if (op == RelationOperator::LESS) RelationP_assembly += "olt ";
            else if (op == RelationOperator::LESS_EQUAL) RelationP_assembly += "ole ";
            
            if (output.resolvedShape == TypeShape::ARRAY) {
                RelationP_assembly += "[" + std::to_string(output.resolvedArraySize) + " x float]";
            }
            else RelationP_assembly += "float";

            RelationP_assembly += " " + leftSideReg + ", " + rightSideReg + "\n";
            output.outputRegister = outputReg;
        }
        else if (ty == Typemark::STRING) {
            // must loop through each value in the strings until either a terminator '\00' or a non-match

            if (leftSideRelation.resolvedShape == TypeShape::ARRAY) {
                // arrays of strings

                unsigned arrLen = rightSideRelation.resolvedArraySize;
                std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += outReg + " = alloca [" + std::to_string(arrLen) + " x i1]\n"; 

                // array iterator
                std::string outerIterReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += outerIterReg + " = alloca i32\n";
                RelationP_assembly += "store i32 0, ptr " + outerIterReg + "\n";

                // per-index relation
                std::string bodyLabel = currentFunctionLlvm.GetNextBlockLabel();
                RelationP_assembly += "br label %" + bodyLabel + "\n\n";
                RelationP_assembly += bodyLabel + ":\n";

                // get string pointers from arrays
                std::string outerIterLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += outerIterLoadReg + " = load i32, ptr " + outerIterReg + "\n";
                std::string leftStringAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += leftStringAccessReg + " = getelementptr [" + std::to_string(arrLen) + " x i8*], ptr " + leftSideReg;
                RelationP_assembly += ", i32 0, i32 " + outerIterLoadReg + "\n";
                std::string leftStringReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += leftStringReg + " = load i8*, ptr " + leftStringAccessReg + "\n";

                std::string rightStringAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += rightStringAccessReg + " = getelementptr [" + std::to_string(arrLen) + " x i8*], ptr " + rightSideReg;
                RelationP_assembly += ", i32 0, i32 " + outerIterLoadReg + "\n";
                std::string rightStringReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += rightStringReg + " = load i8*, ptr " + rightStringAccessReg + "\n";

                { // perform relation
                    std::string iterReg = currentFunctionLlvm.GetNextUnnamedReg();
                    RelationP_assembly += iterReg + " = alloca i32\n";
                    RelationP_assembly += "store i32 0, ptr " + iterReg + "\n";

                    std::string resultReg = currentFunctionLlvm.GetNextUnnamedReg();
                    RelationP_assembly += resultReg + " = alloca i1\n";
                    RelationP_assembly += "store i1 true, ptr " + resultReg + "\n";

                    std::string ifHeadLabel = currentFunctionLlvm.GetNextBlockLabel();
                    std::string ifLoopLabel = currentFunctionLlvm.GetNextBlockLabel();
                    std::string ifFalseLabel = currentFunctionLlvm.GetNextBlockLabel();
                    std::string afterIfLabel = currentFunctionLlvm.GetNextBlockLabel();

                    RelationP_assembly += "br label %" + ifHeadLabel + "\n\n";

                    RelationP_assembly += ifHeadLabel + ":\n";
                    std::string iterLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                    std::string leftAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                    std::string leftLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                    std::string rightAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                    std::string rightLoadReg = currentFunctionLlvm.GetNextUnnamedReg();

                    RelationP_assembly += iterLoadReg + " = load i32, ptr " + iterReg + "\n";
                    // use array of size 0 ([0 x i8]) to represent the pointer
                    RelationP_assembly += leftAccessReg + " = getelementptr [0 x i8], ptr " + leftStringReg;
                    RelationP_assembly += ", i32 0, i32 " + iterLoadReg + "\n";
                    RelationP_assembly += leftLoadReg + " = load i8, ptr " + leftAccessReg + "\n";

                    RelationP_assembly += rightAccessReg + " = getelementptr [0 x i8], ptr " + rightStringReg;
                    RelationP_assembly += ", i32 0, i32 " + iterLoadReg + "\n";
                    RelationP_assembly += rightLoadReg + " = load i8, ptr " + rightAccessReg + "\n";

                    std::string compReg = currentFunctionLlvm.GetNextUnnamedReg();
                    RelationP_assembly += compReg + " = icmp ne i8 " + leftLoadReg + ", " + rightLoadReg + "\n";
                    RelationP_assembly += "br i1 " + compReg + ", label %" + ifFalseLabel + ", label %" + ifLoopLabel + "\n\n";

                    RelationP_assembly += ifLoopLabel + ":\n";
                    std::string contReg = currentFunctionLlvm.GetNextUnnamedReg();
        
                    RelationP_assembly += contReg + " = icmp ne i8 " + leftLoadReg + ", 0\n";
                    std::string iterPlusReg = currentFunctionLlvm.GetNextUnnamedReg();
                    RelationP_assembly += iterPlusReg + " = add i32 1, " + iterLoadReg + "\n";
                    RelationP_assembly += "store i32 " + iterPlusReg + ", ptr " + iterReg + "\n";
                    RelationP_assembly += "br i1 " + contReg + ", label %" + ifHeadLabel + ", label %" + afterIfLabel + "\n\n";

                    RelationP_assembly += ifFalseLabel + ":\n";
                    RelationP_assembly += "store i1 false, ptr " + resultReg + "\n";
                    RelationP_assembly += "br label %" + afterIfLabel + "\n\n";

                    RelationP_assembly += afterIfLabel + ":\n";

                    // store string relation result
                    std::string loadOutReg = currentFunctionLlvm.GetNextUnnamedReg();
                    RelationP_assembly += loadOutReg + " = getelementptr [" + std::to_string(arrLen) + " x i1], ptr " + outReg;
                    RelationP_assembly += ", i32 0, i32 " + outerIterLoadReg + "\n";
                    
                    std::string resultValueReg = currentFunctionLlvm.GetNextUnnamedReg();
                    RelationP_assembly += resultValueReg + " = load i1, ptr " + resultReg + "\n";
                    RelationP_assembly += "store i1 " + resultValueReg + ", ptr " + loadOutReg + "\n"; 
                }

                // iterate for next string pair
                std::string iterLabel = currentFunctionLlvm.GetNextBlockLabel();
                RelationP_assembly += "br label %" + iterLabel + "\n\n";
                RelationP_assembly += iterLabel + ":\n";
                std::string outerIterLoadReg2 = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += outerIterLoadReg2 + " = load i32, ptr " + outerIterReg + "\n";
                std::string nextIterReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += nextIterReg + " = add i32 " + outerIterLoadReg2 + ", 1\n";
                std::string compReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += compReg + " = icmp slt i32 " + nextIterReg + ", " + std::to_string(arrLen) + "\n";

                // continue or end iterating
                std::string endLabel = currentFunctionLlvm.GetNextBlockLabel();
                RelationP_assembly += "br i1 " + compReg + ", label %" + bodyLabel + ", label %" + endLabel + "\n\n";
                RelationP_assembly += endLabel + ":\n";

                output.outputRegister = outReg;
            } else {
                // just two strings

                std::string iterReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += iterReg + " = alloca i32\n";
                RelationP_assembly += "store i32 0, ptr " + iterReg + "\n";

                std::string resultReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += resultReg + " = alloca i1\n";
                RelationP_assembly += "store i1 true, ptr " + resultReg + "\n";
                
                std::string ifHeadLabel = currentFunctionLlvm.GetNextBlockLabel();
                std::string ifLoopLabel = currentFunctionLlvm.GetNextBlockLabel();
                std::string ifFalseLabel = currentFunctionLlvm.GetNextBlockLabel();
                std::string afterIfLabel = currentFunctionLlvm.GetNextBlockLabel();

                RelationP_assembly += "br label %" + ifHeadLabel + "\n\n";

                RelationP_assembly += ifHeadLabel + ":\n";
                std::string iterLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                std::string leftAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                std::string leftLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                std::string rightAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                std::string rightLoadReg = currentFunctionLlvm.GetNextUnnamedReg();

                RelationP_assembly += iterLoadReg + " = load i32, ptr " + iterReg + "\n";
                // use array of size 0 ([0 x i8]) to represent the pointer
                RelationP_assembly += leftAccessReg + " = getelementptr [0 x i8], ptr " + leftSideReg;
                RelationP_assembly += ", i32 0, i32 " + iterLoadReg + "\n";
                RelationP_assembly += leftLoadReg + " = load i8, ptr " + leftAccessReg + "\n";

                RelationP_assembly += rightAccessReg + " = getelementptr [0 x i8], ptr " + rightSideReg;
                RelationP_assembly += ", i32 0, i32 " + iterLoadReg + "\n";
                RelationP_assembly += rightLoadReg + " = load i8, ptr " + rightAccessReg + "\n";

                std::string compReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += compReg + " = icmp ne i8 " + leftLoadReg + ", " + rightLoadReg + "\n";
                RelationP_assembly += "br i1 " + compReg + ", label %" + ifFalseLabel + ", label %" + ifLoopLabel + "\n\n";

                RelationP_assembly += ifLoopLabel + ":\n";
                std::string contReg = currentFunctionLlvm.GetNextUnnamedReg();
    
                RelationP_assembly += contReg + " = icmp ne i8 " + leftLoadReg + ", 0\n";
                std::string iterPlusReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += iterPlusReg + " = add i32 1, " + iterLoadReg + "\n";
                RelationP_assembly += "store i32 " + iterPlusReg + ", ptr " + iterReg + "\n";
                RelationP_assembly += "br i1 " + contReg + ", label %" + ifHeadLabel + ", label %" + afterIfLabel + "\n\n";

                RelationP_assembly += ifFalseLabel + ":\n";
                RelationP_assembly += "store i1 false, ptr " + resultReg + "\n";
                RelationP_assembly += "br label %" + afterIfLabel + "\n\n";

                RelationP_assembly += afterIfLabel + ":\n";
                std::string outputReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += outputReg + " = load i1, ptr " + resultReg + "\n";
                output.outputRegister = outputReg;
            }
        }

    } else {
        // value on array

        std::string valReg;
        std::string arrReg;
        unsigned arrLen = output.resolvedArraySize;

        if (leftSideRelation.resolvedShape == TypeShape::ARRAY) {
            valReg = rightSideReg;
            arrReg = leftSideReg;
        } else {
            valReg = leftSideReg;
            arrReg = rightSideReg;
        }

        if (ty == Typemark::INTEGER) {
            std::string copyReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += copyReg + " = alloca [" + std::to_string(arrLen) + " x i32]\n";

            for (size_t i=0; i<arrLen; ++i) {
                std::string copyAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += copyAccessReg + " = getelementptr [" + std::to_string(arrLen) + " x i32]";
                RelationP_assembly += ", ptr " + copyReg + ", i32 0, i32 " + std::to_string(i) + "\n";

                RelationP_assembly += "store i32 " + valReg + ", ptr " + copyAccessReg + "\n";
            }

            std::string outputReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outputReg + " = icmp ";

            if (op == RelationOperator::EQUAL) RelationP_assembly += "eq ";
            else if (op == RelationOperator::NON_EQUAL) RelationP_assembly += "ne ";
            else if (op == RelationOperator::GREATER) RelationP_assembly += "sgt ";
            else if (op == RelationOperator::GREATER_EQUAL) RelationP_assembly += "sge ";
            else if (op == RelationOperator::LESS) RelationP_assembly += "slt ";
            else if (op == RelationOperator::LESS_EQUAL) RelationP_assembly += "sle ";
            
            RelationP_assembly += "[" + std::to_string(arrLen) + " x i32] ";
            RelationP_assembly += arrReg + ", " + copyReg + "\n";
            output.outputRegister = outputReg;
        }
        else if (ty == Typemark::BOOL) {
            std::string copyReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += copyReg + " = alloca [" + std::to_string(arrLen) + " x i1]\n";

            for (size_t i=0; i<arrLen; ++i) {
                std::string copyAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += copyAccessReg + " = getelementptr [" + std::to_string(arrLen) + " x i1]";
                RelationP_assembly += ", ptr " + copyReg + ", i32 0, i32 " + std::to_string(i) + "\n";

                RelationP_assembly += "store i1 " + valReg + ", ptr " + copyAccessReg + "\n";
            }

            std::string outputReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outputReg + " = icmp ";

            if (op == RelationOperator::EQUAL) RelationP_assembly += "eq ";
            else if (op == RelationOperator::NON_EQUAL) RelationP_assembly += "ne ";
            else if (op == RelationOperator::GREATER) RelationP_assembly += "ugt ";
            else if (op == RelationOperator::GREATER_EQUAL) RelationP_assembly += "uge ";
            else if (op == RelationOperator::LESS) RelationP_assembly += "ult ";
            else if (op == RelationOperator::LESS_EQUAL) RelationP_assembly += "ule ";
            
            RelationP_assembly += "[" + std::to_string(arrLen) + " x i1] ";
            RelationP_assembly += arrReg + ", " + copyReg + "\n";
            output.outputRegister = outputReg;
        }
        else if (ty == Typemark::FLOAT) {
            std::string copyReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += copyReg + " = alloca [" + std::to_string(arrLen) + " x float]\n";

            for (size_t i=0; i<arrLen; ++i) {
                std::string copyAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += copyAccessReg + " = getelementptr [" + std::to_string(arrLen) + " x float]";
                RelationP_assembly += ", ptr " + copyReg + ", i32 0, i32 " + std::to_string(i) + "\n";

                RelationP_assembly += "store float " + valReg + ", ptr " + copyAccessReg + "\n";
            }

            std::string outputReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outputReg + " = fcmp ";

            if (op == RelationOperator::EQUAL) RelationP_assembly += "oeq ";
            else if (op == RelationOperator::NON_EQUAL) RelationP_assembly += "one ";
            else if (op == RelationOperator::GREATER) RelationP_assembly += "ogt ";
            else if (op == RelationOperator::GREATER_EQUAL) RelationP_assembly += "oge ";
            else if (op == RelationOperator::LESS) RelationP_assembly += "olt ";
            else if (op == RelationOperator::LESS_EQUAL) RelationP_assembly += "ole ";

            RelationP_assembly += "[" + std::to_string(arrLen) + " x float] ";
            RelationP_assembly += arrReg + ", " + copyReg + "\n";
            output.outputRegister = outputReg;
        }
        else if (ty == Typemark::STRING) {
            // same as two string arrays but only iterate on one side

            unsigned arrLen = rightSideRelation.resolvedArraySize;
            std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outReg + " = alloca [" + std::to_string(arrLen) + " x i1]\n"; 

            // array iterator
            std::string outerIterReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outerIterReg + " = alloca i32\n";
            RelationP_assembly += "store i32 0, ptr " + outerIterReg + "\n";

            // per-index relation
            std::string bodyLabel = currentFunctionLlvm.GetNextBlockLabel();
            RelationP_assembly += "br label %" + bodyLabel + "\n\n";
            RelationP_assembly += bodyLabel + ":\n";

            // get string pointers from arrays
            // left side uses arrReg, right side uses valReg
            std::string outerIterLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outerIterLoadReg + " = load i32, ptr " + outerIterReg + "\n";
            std::string leftStringAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += leftStringAccessReg + " = getelementptr [" + std::to_string(arrLen) + " x i8*], ptr " + arrReg;
            RelationP_assembly += ", i32 0, i32 " + outerIterLoadReg + "\n";
            std::string leftStringReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += leftStringReg + " = load i8*, ptr " + leftStringAccessReg + "\n";

            std::string rightStringReg = valReg;

            { // perform relation
                std::string iterReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += iterReg + " = alloca i32\n";
                RelationP_assembly += "store i32 0, ptr " + iterReg + "\n";

                std::string resultReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += resultReg + " = alloca i1\n";
                RelationP_assembly += "store i1 true, ptr " + resultReg + "\n";

                std::string ifHeadLabel = currentFunctionLlvm.GetNextBlockLabel();
                std::string ifLoopLabel = currentFunctionLlvm.GetNextBlockLabel();
                std::string ifFalseLabel = currentFunctionLlvm.GetNextBlockLabel();
                std::string afterIfLabel = currentFunctionLlvm.GetNextBlockLabel();

                RelationP_assembly += "br label %" + ifHeadLabel + "\n\n";

                RelationP_assembly += ifHeadLabel + ":\n";
                std::string iterLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                std::string leftAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                std::string leftLoadReg = currentFunctionLlvm.GetNextUnnamedReg();
                std::string rightAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
                std::string rightLoadReg = currentFunctionLlvm.GetNextUnnamedReg();

                RelationP_assembly += iterLoadReg + " = load i32, ptr " + iterReg + "\n";
                // use array of size 0 ([0 x i8]) to represent the pointer
                RelationP_assembly += leftAccessReg + " = getelementptr [0 x i8], ptr " + leftStringReg;
                RelationP_assembly += ", i32 0, i32 " + iterLoadReg + "\n";
                RelationP_assembly += leftLoadReg + " = load i8, ptr " + leftAccessReg + "\n";

                RelationP_assembly += rightAccessReg + " = getelementptr [0 x i8], ptr " + rightStringReg;
                RelationP_assembly += ", i32 0, i32 " + iterLoadReg + "\n";
                RelationP_assembly += rightLoadReg + " = load i8, ptr " + rightAccessReg + "\n";

                std::string compReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += compReg + " = icmp ne i8 " + leftLoadReg + ", " + rightLoadReg + "\n";
                RelationP_assembly += "br i1 " + compReg + ", label %" + ifFalseLabel + ", label %" + ifLoopLabel + "\n\n";

                RelationP_assembly += ifLoopLabel + ":\n";
                std::string contReg = currentFunctionLlvm.GetNextUnnamedReg();
    
                RelationP_assembly += contReg + " = icmp ne i8 " + leftLoadReg + ", 0\n";
                std::string iterPlusReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += iterPlusReg + " = add i32 1, " + iterLoadReg + "\n";
                RelationP_assembly += "store i32 " + iterPlusReg + ", ptr " + iterReg + "\n";
                RelationP_assembly += "br i1 " + contReg + ", label %" + ifHeadLabel + ", label %" + afterIfLabel + "\n\n";

                RelationP_assembly += ifFalseLabel + ":\n";
                RelationP_assembly += "store i1 false, ptr " + resultReg + "\n";
                RelationP_assembly += "br label %" + afterIfLabel + "\n\n";

                RelationP_assembly += afterIfLabel + ":\n";

                // store string relation result
                std::string loadOutReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += loadOutReg + " = getelementptr [" + std::to_string(arrLen) + " x i1], ptr " + outReg;
                RelationP_assembly += ", i32 0, i32 " + outerIterLoadReg + "\n";
                
                std::string resultValueReg = currentFunctionLlvm.GetNextUnnamedReg();
                RelationP_assembly += resultValueReg + " = load i1, ptr " + resultReg + "\n";
                RelationP_assembly += "store i1 " + resultValueReg + ", ptr " + loadOutReg + "\n"; 
            }

            // iterate for next string pair
            std::string iterLabel = currentFunctionLlvm.GetNextBlockLabel();
            RelationP_assembly += "br label %" + iterLabel + "\n\n";
            RelationP_assembly += iterLabel + ":\n";
            std::string outerIterLoadReg2 = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += outerIterLoadReg2 + " = load i32, ptr " + outerIterReg + "\n";
            std::string nextIterReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += nextIterReg + " = add i32 " + outerIterLoadReg2 + ", 1\n";
            std::string compReg = currentFunctionLlvm.GetNextUnnamedReg();
            RelationP_assembly += compReg + " = icmp slt i32 " + nextIterReg + ", " + std::to_string(arrLen) + "\n";

            // continue or end iterating
            std::string endLabel = currentFunctionLlvm.GetNextBlockLabel();
            RelationP_assembly += "br i1 " + compReg + ", label %" + bodyLabel + ", label %" + endLabel + "\n\n";
            RelationP_assembly += endLabel + ":\n";

            output.outputRegister = outReg;


        }
    }

    currentFunctionLlvm.llvm += RelationP_assembly;
}



void codegen_arithOpPrime(const Resolution& leftSideArith, const Resolution& rightSideArith, Resolution& output, ArithmaticOperator::ArithmaticOperator op) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string arithP_assembly = "";

    std::string leftSideReg = leftSideArith.outputRegister;
    std::string rightSideReg = rightSideArith.outputRegister;

    // cast if type mismatch

    if (leftSideArith.resolvedType == Typemark::INTEGER
    && output.resolvedType == Typemark::FLOAT) {

        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
        arithP_assembly += castReg + " = sitofp ";

        if (leftSideArith.resolvedShape == TypeShape::ARRAY) {
            arithP_assembly += "[" + std::to_string(leftSideArith.resolvedArraySize) + " x i32] ";
        } else {
            arithP_assembly += "i32 ";
        }

        arithP_assembly += leftSideArith.outputRegister + " to ";

        if (leftSideArith.resolvedShape == TypeShape::ARRAY) {
            arithP_assembly += "[" + std::to_string(leftSideArith.resolvedArraySize) + " x float] ";
        } else {
            arithP_assembly += "float ";
        }

        leftSideReg = castReg;
    }
    else if (rightSideArith.resolvedType == Typemark::INTEGER
    && output.resolvedType == Typemark::FLOAT) {

        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
        arithP_assembly += castReg + " = sitofp ";

        if (rightSideArith.resolvedShape == TypeShape::ARRAY) {
            arithP_assembly += "[" + std::to_string(leftSideArith.resolvedArraySize) + " x i32] ";
        } else {
            arithP_assembly += "i32 ";
        }

        arithP_assembly += rightSideArith.outputRegister + " to ";

        if (rightSideArith.resolvedShape == TypeShape::ARRAY) {
            arithP_assembly += "[" + std::to_string(rightSideArith.resolvedArraySize) + " x float] ";
        } else {
            arithP_assembly += "float ";
        }

        rightSideReg = castReg;
    }

    // perform operation

    if (leftSideArith.resolvedShape == rightSideArith.resolvedShape) {
        
        std::string arithReg = currentFunctionLlvm.GetNextUnnamedReg();

        arithP_assembly += arithReg + " = ";

        if (output.resolvedType == Typemark::FLOAT) {
            arithP_assembly += "f";
        }

        if (op == ArithmaticOperator::PLUS) arithP_assembly += "add ";
        else arithP_assembly += "sub ";

        if (leftSideArith.resolvedShape == TypeShape::ARRAY) {
            arithP_assembly += "[" + std::to_string(leftSideArith.resolvedArraySize) + " x ";
            if (output.resolvedType == Typemark::FLOAT) {
                arithP_assembly += "float] ";
            } else arithP_assembly += "i32] ";
        } else {
            if (output.resolvedType == Typemark::FLOAT) {
                arithP_assembly += "float ";
            } else arithP_assembly += "i32 ";
        }

        arithP_assembly += leftSideReg + ", " + rightSideReg + "\n";

        output.outputRegister = arithReg;
    } else {

        std::string arrayReg;
        std::string valReg;
        unsigned arrayLen = output.resolvedArraySize;

        if (leftSideArith.resolvedShape == TypeShape::ARRAY) {
            arrayReg = leftSideReg;
            valReg = rightSideReg;
        } else {
            arrayReg = rightSideReg;
            valReg = leftSideReg;
        }

        std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
        arithP_assembly += outReg + " = alloca ";

        if (output.resolvedType == Typemark::INTEGER) {
            arithP_assembly += "[" + std::to_string(arrayLen) + " x i32]\n";
        } else {
            arithP_assembly += "[" + std::to_string(arrayLen) + " x float]\n";
        }

        for (size_t i=0; i<arrayLen; ++i) {

            std::string elemAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
            arithP_assembly += elemAccessReg + " = getelementptr ";
            if (output.resolvedType == Typemark::INTEGER) {
                arithP_assembly += "[" + std::to_string(arrayLen) + " x i32]";
            } else {
                arithP_assembly += "[" + std::to_string(arrayLen) + " x float]";
            }
            arithP_assembly += ", ptr " + arrayReg + " i32 0, i32 " + std::to_string(i) + "\n";

            std::string loadReg = currentFunctionLlvm.GetNextUnnamedReg();
            arithP_assembly += loadReg + " = load ";
            if (output.resolvedType == Typemark::INTEGER) {
                arithP_assembly += "i32, ";
            } else {
                arithP_assembly += "float, ";
            }
            arithP_assembly += elemAccessReg + "\n";

            std::string resultReg = currentFunctionLlvm.GetNextUnnamedReg();
            arithP_assembly += resultReg + " = ";
            if (output.resolvedType == Typemark::FLOAT) {
                arithP_assembly += "f";
            } 
            
            if (op == ArithmaticOperator::PLUS) arithP_assembly += "add ";
            else arithP_assembly += "sub ";

            if (output.resolvedType == Typemark::INTEGER) {
                arithP_assembly += "i32 ";
            } else arithP_assembly += "float ";

            arithP_assembly += loadReg + ", " + valReg + "\n";

            std::string elemTargetReg = currentFunctionLlvm.GetNextUnnamedReg();
            arithP_assembly += elemTargetReg + " = getelementptr ";
            if (output.resolvedType == Typemark::INTEGER) {
                arithP_assembly += "[" + std::to_string(arrayLen) + " x i32]";
            } else {
                arithP_assembly += "[" + std::to_string(arrayLen) + " x float]";
            }
            arithP_assembly += ", ptr " + outReg + " i32 0, i32 " + std::to_string(i) + "\n";

            arithP_assembly += "store ";
            if (output.resolvedType == Typemark::INTEGER) {
                arithP_assembly += "i32, ";
            } else {
                arithP_assembly += "float, ";
            }
            arithP_assembly += resultReg + ", ptr " + elemTargetReg + "\n";
        }

        output.outputRegister = outReg;
    }

    currentFunctionLlvm.llvm += arithP_assembly;

}


void codegen_expressionBitwiseNot(Resolution& expr) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string expr_assembly = "";
    std::string exprReg = expr.outputRegister;
    
    if (expr.resolvedShape == TypeShape::VALUE) {
        std::string xor_register = currentFunctionLlvm.GetNextUnnamedReg();

        if (expr.resolvedType == Typemark::INTEGER) {
            expr_assembly += xor_register + " = xor i32 -1, " + exprReg + "\n";
        } else {
            expr_assembly += xor_register + " = xor i1 1, " + exprReg + "\n";
        }

        expr.outputRegister = xor_register;
    } else {
        unsigned len = expr.resolvedArraySize;

        std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
        if (expr.resolvedType == Typemark::INTEGER) {
            expr_assembly += outReg + " = alloca [" + std::to_string(len) + " x i32]\n";
        } else {
            expr_assembly += outReg + " = alloca [" + std::to_string(len) + " x i1]\n";
        }

        for (size_t i=0; i < expr.resolvedArraySize; ++i) {
            // xor each value in the array with -1

            // get pointer to index in source array
            std::string elemAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
            if (expr.resolvedType == Typemark::INTEGER) {
                expr_assembly += elemAccessReg + " = getelementptr [" + std::to_string(len) + " x i32]";
            } else {
                expr_assembly += elemAccessReg + " = getelementptr [" + std::to_string(len) + " x i1]";
            }
            expr_assembly += ", ptr " + exprReg;
            expr_assembly += ", i32 0, i32 " + std::to_string(i) + "\n";

            // load value from index
            std::string loadReg = currentFunctionLlvm.GetNextUnnamedReg();
            if (expr.resolvedType == Typemark::INTEGER) {
                expr_assembly += loadReg + " = load i32, ptr " + elemAccessReg + "\n";
            } else {
                expr_assembly += loadReg + " = load float, ptr " + elemAccessReg + "\n";
            }

            // xor value with -1 if integer and 1 if boolean
            std::string xorReg = currentFunctionLlvm.GetNextUnnamedReg();
            if (expr.resolvedType == Typemark::INTEGER) {
                expr_assembly += xorReg + " = xor i32 -1, " + loadReg + "\n";
            } else {
                expr_assembly += xorReg + " = xor i1 1, " + loadReg + "\n";
            }

            // get pointer to index in target array
            std::string elemTargetReg = currentFunctionLlvm.GetNextUnnamedReg();
            if (expr.resolvedType == Typemark::INTEGER) {
                expr_assembly += elemTargetReg + " = getelementptr [" + std::to_string(len) + " x i32]";
            } else {
                expr_assembly += elemTargetReg + " = getelementptr [" + std::to_string(len) + " x i1]";
            }
            expr_assembly += ", ptr " + outReg + ", i32 0, i32 " + std::to_string(i) + "\n";

            // store value at index
            if (expr.resolvedType == Typemark::INTEGER) {
                expr_assembly += "store i32 " + xorReg + ", ptr " + elemTargetReg + "\n";
            } else {
                expr_assembly += "store i1 " + xorReg + ", ptr " + elemTargetReg + "\n";
            }
        }
        expr.outputRegister = outReg;
    }

    currentFunctionLlvm.llvm += expr_assembly;
}


void codegen_expressionPrime(const Resolution& leftSideExpr, const Resolution& rightSideExpr, Resolution& output, BinaryBitwise::BinaryBitwise op) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string exprP_assembly = "";

    std::string leftSideReg = leftSideExpr.outputRegister;
    std::string rightSideReg = rightSideExpr.outputRegister;

    // cast if type mismatch

    if (leftSideExpr.resolvedType == Typemark::BOOL
    && output.resolvedType == Typemark::INTEGER) {
        
        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
        exprP_assembly += castReg + " = zext ";

        if (leftSideExpr.resolvedShape == TypeShape::ARRAY) {
            exprP_assembly += "[" + std::to_string(leftSideExpr.resolvedArraySize) + " x i1] ";
        } else exprP_assembly += "i1 ";
        
        exprP_assembly += leftSideExpr.outputRegister + " to ";

        if (leftSideExpr.resolvedShape == TypeShape::ARRAY) {
            exprP_assembly += "[" + std::to_string(leftSideExpr.resolvedArraySize) + " x i32] ";
        } else exprP_assembly += "i32 ";
        
        leftSideReg = castReg;
    }
    else if (rightSideExpr.resolvedType == Typemark::BOOL
    && output.resolvedType == Typemark::INTEGER) {

        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();
        exprP_assembly += castReg + " = zext ";

        if (rightSideExpr.resolvedShape == TypeShape::ARRAY) {
            exprP_assembly += "[" + std::to_string(rightSideExpr.resolvedArraySize) + " x i1] ";
        } else exprP_assembly += "i1 ";
        
        exprP_assembly += rightSideExpr.outputRegister + " to ";

        if (rightSideExpr.resolvedShape == TypeShape::ARRAY) {
            exprP_assembly += "[" + std::to_string(rightSideExpr.resolvedArraySize) + " x i32] ";
        } else exprP_assembly += "i32 ";
        
        rightSideReg = castReg;
    }

    // perform operation

    if (leftSideExpr.resolvedShape == rightSideExpr.resolvedShape) {
        
        std::string arithReg = currentFunctionLlvm.GetNextUnnamedReg();

        exprP_assembly += arithReg + " = ";

        if (op == BinaryBitwise::AND) exprP_assembly += "and ";
        else exprP_assembly += "or ";

        if (leftSideExpr.resolvedShape == TypeShape::ARRAY) {
            exprP_assembly += "[" + std::to_string(leftSideExpr.resolvedArraySize) + " x ";
            if (output.resolvedType == Typemark::INTEGER) {
                exprP_assembly += "i32] ";
            } else exprP_assembly += "i1] ";
        } else {
            if (output.resolvedType == Typemark::INTEGER) {
                exprP_assembly += "i32 ";
            } else exprP_assembly += "i1 ";
        }

        exprP_assembly += leftSideReg + ", " + rightSideReg + "\n";

        output.outputRegister = arithReg;
    } else {

        std::string arrayReg;
        std::string valReg;
        unsigned arrayLen = output.resolvedArraySize;

        if (leftSideExpr.resolvedShape == TypeShape::ARRAY) {
            arrayReg = leftSideReg;
            valReg = rightSideReg;
        } else {
            arrayReg = rightSideReg;
            valReg = leftSideReg;
        }

        std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
        exprP_assembly += outReg + " = alloca ";

        if (output.resolvedType == Typemark::INTEGER) {
            exprP_assembly += "[" + std::to_string(arrayLen) + " x i32]\n";
        } else {
            exprP_assembly += "[" + std::to_string(arrayLen) + " x i1]\n";
        }

        for (size_t i=0; i<arrayLen; ++i) {

            std::string elemAccessReg = currentFunctionLlvm.GetNextUnnamedReg();
            exprP_assembly += elemAccessReg + " = getelementptr ";
            if (output.resolvedType == Typemark::INTEGER) {
                exprP_assembly += "[" + std::to_string(arrayLen) + " x i32]";
            } else {
                exprP_assembly += "[" + std::to_string(arrayLen) + " x i1]";
            }
            exprP_assembly += ", ptr " + arrayReg + " i32 0, i32 " + std::to_string(i) + "\n";

            std::string loadReg = currentFunctionLlvm.GetNextUnnamedReg();
            exprP_assembly += loadReg + " = load ";
            if (output.resolvedType == Typemark::INTEGER) {
                exprP_assembly += "i32, ";
            } else {
                exprP_assembly += "i1, ";
            }
            exprP_assembly += elemAccessReg + "\n";

            std::string resultReg = currentFunctionLlvm.GetNextUnnamedReg();
            exprP_assembly += resultReg + " = ";
            
            if (op == BinaryBitwise::AND) exprP_assembly += "and ";
            else exprP_assembly += "or ";

            if (output.resolvedType == Typemark::INTEGER) {
                exprP_assembly += "i32 ";
            } else exprP_assembly += "i1 ";

            exprP_assembly += loadReg + ", " + valReg + "\n";

            std::string elemTargetReg = currentFunctionLlvm.GetNextUnnamedReg();
            exprP_assembly += elemTargetReg + " = getelementptr ";
            if (output.resolvedType == Typemark::INTEGER) {
                exprP_assembly += "[" + std::to_string(arrayLen) + " x i32]";
            } else {
                exprP_assembly += "[" + std::to_string(arrayLen) + " x i1]";
            }
            exprP_assembly += ", ptr " + outReg + " i32 0, i32 " + std::to_string(i) + "\n";

            exprP_assembly += "store ";
            if (output.resolvedType == Typemark::INTEGER) {
                exprP_assembly += "i32, ";
            } else {
                exprP_assembly += "i1, ";
            }
            exprP_assembly += resultReg + ", ptr " + elemTargetReg + "\n";
        }

        output.outputRegister = outReg;
    }

    currentFunctionLlvm.llvm += exprP_assembly;
}



// always sets the output register as a pointer type, so that it can be stored to
void codegen_variableAccess(Variable_Access& variableAcc) { 

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    Symbol* var_symbol = symbol_table.lookup(variableAcc.identifier);
    std::string varAcc_assembly = "";

    if (var_symbol->shape == TypeShape::VALUE) { // no transform required, just pass the pointer
        variableAcc.resolution.outputRegister = var_symbol->llvmName;
    }
    else if (variableAcc.resolution.resolvedShape == TypeShape::ARRAY) { // no transform required, just pass the pointer
        variableAcc.resolution.outputRegister = var_symbol->llvmName;
    } else {
        // array index access, get pointer to index

        Expression& indexExpr = *variableAcc.array_index->get();

        // check if index is in bounds

        std::string zeroCheckReg = currentFunctionLlvm.GetNextUnnamedReg();
        std::string highCheckReg = currentFunctionLlvm.GetNextUnnamedReg();
        varAcc_assembly += zeroCheckReg + " = icmp sge i32 " + indexExpr.resolution.outputRegister + ", 0 \n";
        varAcc_assembly += highCheckReg + " = icmp slt i32 " + indexExpr.resolution.outputRegister +  ", " + std::to_string(var_symbol->array_size) + "\n";

        std::string inBoundsReg = currentFunctionLlvm.GetNextUnnamedReg();
        varAcc_assembly += inBoundsReg + " = and i1 " + zeroCheckReg + ", " + highCheckReg + "\n";

        // Out of bounds handing; set-jump long-jump :: TODO

        std::string outofboundsBlock = currentFunctionLlvm.GetNextBlockLabel();
        std::string continueBlock = currentFunctionLlvm.GetNextBlockLabel();

        varAcc_assembly += "br i1 " + inBoundsReg + ", label %" + continueBlock + ", label %" + outofboundsBlock + "\n\n";

        varAcc_assembly += outofboundsBlock + ":\n";
        varAcc_assembly += "call void @throw_arrayoutofbounds()\n";
        varAcc_assembly += "unreachable\n\n";

        varAcc_assembly += continueBlock + ":\n";

        std::string outReg = currentFunctionLlvm.GetNextUnnamedReg();
        varAcc_assembly += outReg + " = getelementptr [" + std::to_string(var_symbol->array_size) + " x ";
        switch (var_symbol->type) {
            case Typemark::INTEGER:
            varAcc_assembly += "i32]";
                break;
            case Typemark::FLOAT:
            varAcc_assembly += "float]";
                break;
            case Typemark::BOOL:
            varAcc_assembly += "i1]";
                break;
            case Typemark::STRING:
            varAcc_assembly += "i8*]";
                break;
        }

        varAcc_assembly += ", ptr " + var_symbol->llvmName + ", i32 0, i32 " + indexExpr.resolution.outputRegister + "\n";

        variableAcc.resolution.outputRegister = outReg;
    }

    currentFunctionLlvm.llvm += varAcc_assembly;
}




void codegen_beginMain() {

    std::string scopeName = symbol_table.getCurrentScopeFullName(); // "GLOBAL"
    std::string functionLlvmName = "@main";
    LlvmAssemblyStrings::FunctionString functionDefinition {};

    std::string main_assembly = "define i32 " + functionLlvmName + "() {\n";
    main_assembly += functionDefinition.GetNextBlockLabel() + ":\n";

    main_assembly += "%_status = call i8 @exception_block()\n";
    main_assembly += "%_jmp = icmp ne i8 %_status, 0\n";

    std::string exitBlock = functionDefinition.GetNextBlockLabel();
    std::string mainBlock = functionDefinition.GetNextBlockLabel();

    main_assembly += "br i1 %_jmp, label %" + exitBlock + ", label %" + mainBlock + "\n\n";

    main_assembly += exitBlock + ":\n";
    main_assembly += "ret i32 1\n\n";

    main_assembly += mainBlock + ":\n";

    functionDefinition.llvm = main_assembly;
    llvmAssemblyStrings.functionDefinitions.insert({scopeName, functionDefinition});

}
void codegen_endMain() {
    std::string scope_name = symbol_table.getCurrentScopeFullName(); 
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);
    currentFunctionLlvm.llvm += "ret i32 0\n";
    currentFunctionLlvm.llvm += "}\n\n";
}




void codegen_returnStatement(Expression& expr, Typemark::Typemark returnType) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string return_assembly = "";

    // type cast

    std::string castReg = "";

    if (expr.resolution.resolvedType == returnType) {
        castReg = expr.resolution.outputRegister;
    }
    else if (expr.resolution.resolvedType == Typemark::INTEGER
    && returnType == Typemark::FLOAT) {
        castReg = currentFunctionLlvm.GetNextUnnamedReg();
        return_assembly += castReg + " = sitofp i32 " + expr.resolution.outputRegister + " to float\n";
    }
    else if (expr.resolution.resolvedType == Typemark::FLOAT
    && returnType == Typemark::INTEGER) {
        castReg = currentFunctionLlvm.GetNextUnnamedReg();
        return_assembly += castReg + " = fptosi float " + expr.resolution.outputRegister + " to i32\n";
    }
    else if (expr.resolution.resolvedType == Typemark::INTEGER
    && returnType == Typemark::BOOL) {
        castReg = currentFunctionLlvm.GetNextUnnamedReg();
        return_assembly += castReg + " = icmp ne i32 0, i32 " + expr.resolution.outputRegister + "\n";
    }
    else if (expr.resolution.resolvedType == Typemark::BOOL
    && returnType == Typemark::INTEGER) {
        castReg = currentFunctionLlvm.GetNextUnnamedReg();
        return_assembly += castReg + " = zext i1 " + expr.resolution.outputRegister + " to i32\n";
    }

    // return assembly

    return_assembly += "ret " + typemarkToLLVMType(returnType) + " " + castReg + "\n\n";

    // after ret, if there is additional code llvm will create an
    // implicitly numbered basic block using the unnamed register counter
    currentFunctionLlvm.GetNextUnnamedReg();

    currentFunctionLlvm.llvm += return_assembly;
}



std::pair<std::string, std::string> codegen_ifStatementCondition(const Resolution& condition) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string if_assembly = "";

    std::string condReg = condition.outputRegister;

    if (condition.resolvedType == Typemark::INTEGER) { // cast to bool
        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();

        if_assembly += castReg + " = icmp ne i32 0, i32 " + condition.outputRegister + "\n";
        condReg = castReg;
    }

    std::string thenLabel = currentFunctionLlvm.GetNextBlockLabel();
    std::string elseLabel = currentFunctionLlvm.GetNextBlockLabel();
    std::string endLabel = currentFunctionLlvm.GetNextBlockLabel();

    if_assembly += "br i1 " + condReg + ", label %" + thenLabel + ", label %" + elseLabel + "\n\n";
    if_assembly += thenLabel + ":\n";

    currentFunctionLlvm.llvm += if_assembly;
    return {elseLabel, endLabel};
}

void codegen_ifStatementElse(std::string elseLabel, std::string endLabel) {
    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    currentFunctionLlvm.llvm += "br label %" + endLabel + "\n\n";
    currentFunctionLlvm.llvm += elseLabel + ":\n";
}

void codegen_ifStatementEnd(std::string endLabel) {
    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    currentFunctionLlvm.llvm += "br label %" + endLabel + "\n\n";
    currentFunctionLlvm.llvm += endLabel + ":\n";
}


std::tuple<std::string/*cond_block*/, std::string/*body_block*/, std::string/*end_block*/> codegen_forStatementConditionStart() {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string condLabel = currentFunctionLlvm.GetNextBlockLabel();
    std::string bodyLabel = currentFunctionLlvm.GetNextBlockLabel();
    std::string endLabel = currentFunctionLlvm.GetNextBlockLabel();

    currentFunctionLlvm.llvm += "br label %" + condLabel + "\n\n";
    currentFunctionLlvm.llvm += condLabel + ":\n";

    return {condLabel, bodyLabel, endLabel};
}


void codegen_forStatementBodyStart(std::string bodyLabel, std::string endLabel, const Resolution& condition) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    std::string condReg = condition.outputRegister;

    if (condition.resolvedType == Typemark::INTEGER) { // cast to bool
        std::string castReg = currentFunctionLlvm.GetNextUnnamedReg();

        currentFunctionLlvm.llvm += castReg + " = icmp ne i32 0, i32 " + condition.outputRegister + "\n";
        condReg = castReg;
    }

    currentFunctionLlvm.llvm += "br i1 " + condReg + ", label %" + bodyLabel + ", label %" + endLabel + "\n\n";
    currentFunctionLlvm.llvm += bodyLabel + ":\n";

}


void codegen_forStatementEnd(std::string condLabel, std::string endLabel) {

    std::string scope_name = symbol_table.getCurrentScopeFullName();
    LlvmAssemblyStrings::FunctionString& currentFunctionLlvm = llvmAssemblyStrings.functionDefinitions.at(scope_name);

    currentFunctionLlvm.llvm += "br label %" + condLabel + "\n\n";
    currentFunctionLlvm.llvm += endLabel + ":\n";

}