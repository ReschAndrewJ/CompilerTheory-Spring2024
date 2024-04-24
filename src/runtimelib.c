

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <setjmp.h>


int8_t getbool_impl() {
    char buffer[100]; 
    fgets(buffer, 100, stdin);
    for (unsigned i=0; i<4; ++i) buffer[i] = tolower(buffer[i]);
    if (
        buffer[0] == '1' || !strncmp(buffer, "true", 4)
    ) return 1;
    return 0;
}

int32_t getinteger_impl() {
    char buffer[100];
    fgets(buffer, 100, stdin);
    return atoi(buffer);
}

float getfloat_impl() {
    char buffer[100];
    fgets(buffer, 100, stdin);
    return atof(buffer);
}

int8_t* getstring_impl() {
    char* buffer = (char*)malloc(1024);
    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}


int8_t putbool_impl(int8_t val) {
    char* outstring = NULL;
    if (val) {
        printf("true\n");
    } else {
        printf("false\n");
    }
    printf("%s", outstring);
    return 1;
}

int8_t putinteger_impl(int32_t val) {
    printf("%d\n", val);
    return 1;
}

int8_t putfloat_impl(float val) {
    printf("%f\n", val);
    return 1;
}

int8_t putstring_impl(int8_t* val) {
    printf("%s\n", (char*)val);
    return 1;
}


float sqrt_impl(float val) {
    return sqrtf(val);
}



jmp_buf jump;

int8_t exception_block() {
    if (setjmp(jump)) {
        printf("%s\n", "error: array access out of bounds");
        return 1;
    }
    return 0;
}

void throw_arrayoutofbounds() {
    longjmp(jump, 1);
}
