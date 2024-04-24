#include "header.h"

#include <stdio.h>


int main(int argc, char** argv) {

    if (argc != 2) { 
        printf("incorrect number of arguments. must include a filename\n\n");
        return 0;
    }

    std::string programFilename = argv[1];

    if (!inFile.attachFile(programFilename.c_str())) {
        printf("file not found. filename: %s\n\n", argv[1]);
        return 0;
    }

    printf("filename: %s\n\n", inFile.getFilename().c_str());
    
    parse();

    std::string error_string = "ErrorStatus: %s\n";
    std::string error_status = "";
    switch (error_reporting.GetErrorStatus()) {
        case ErrorStatus::NO_ERROR:
            error_status = "NO_ERROR";
            break;
        case ErrorStatus::WARNING:
            error_status = "WARNING";
            break;
        case ErrorStatus::ERROR:
            error_status = "ERROR";
            break;
        
        default:
            error_status = "INVALID_ERROR_STATUS";
            break;
    }
    printf(error_string.c_str(), error_status.c_str());


    if (error_reporting.GetErrorStatus() < ErrorStatus::ERROR) {
        std::string assembly = "";
        assembly += llvmAssemblyStrings.builtinFunctionDeclarations + "\n";
        assembly += llvmAssemblyStrings.globalVariableDeclarations + "\n";
        for (auto& function : llvmAssemblyStrings.functionDefinitions) {
            assembly += function.second.llvm + "\n";
        }
    
        size_t startA = programFilename.find_last_of('/') + 1;
        size_t startB = programFilename.find_last_of('\\') + 1;
        size_t start = startA > startB ? startA : startB;
        size_t afterEnd = programFilename.find_last_of(".");
        std::string fName = programFilename.substr(start, afterEnd-start) + ".ll";

        FILE* outFile = fopen((fName).c_str(), "w");
        for (char c : assembly) fputc(c, outFile);
        fputc('\0', outFile);
        fclose(outFile);

        printf("Saved Program: %s\n", (fName).c_str());
    }

}