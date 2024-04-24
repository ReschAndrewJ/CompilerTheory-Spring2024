#include "header.h"



void Reporting::ReportError(std::string message) {
    if (awaitingResync) return;  
    else awaitingResync = true;
    
    std::string message_full = inFile.getFilename() + ":" + 
    std::to_string(inFile.getLineCount()) + ":0: " +
    message + "\n";
    fprintf(stderr, "%s", message_full.c_str());
    status = ErrorStatus::ERROR > status ? ErrorStatus::ERROR : status;
}


void Reporting::ReportWarning(std::string message) {
    std::string message_full = inFile.getFilename() + ":" + 
    std::to_string(inFile.getLineCount()) + ":0: " +
    message + "\n";
    fprintf(stderr, "%s", message_full.c_str());
    status = ErrorStatus::WARNING > status ? ErrorStatus::WARNING : status;
}

void Reporting::Resync() {awaitingResync = false;}
bool Reporting::AwaitingResync() {return awaitingResync;}

ErrorStatus::ErrorStatus Reporting::GetErrorStatus() const { return status; }

