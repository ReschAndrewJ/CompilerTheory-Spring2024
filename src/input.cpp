#include "header.h"



bool InputFile::attachFile(const char* filename) {
    filePtr = fopen(filename, "r");
    if (filePtr != nullptr) this->filename = std::string(filename);
    return (filePtr != nullptr);
}

char InputFile::getChar() {
    return fgetc(filePtr);
}

char InputFile::peekChar() const {
    char peek = fgetc(filePtr);
    ungetc(peek, filePtr);
    return peek;
}

void InputFile::incrementLineCount() { ++lineCount; }

unsigned InputFile::getLineCount() const { return lineCount; }

std::string InputFile::getFilename() const { return filename; }


