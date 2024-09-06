#include"pch.h"
#include"log.h"

#include"../../game/console/console.h"

// formats a string with defined args
char* stringf(const char* format, va_list args) {
    // credit: https://stackoverflow.com/questions/66094905/how-to-pass-a-formatted-string-as-a-single-argument-in-c
    int len = vsnprintf(NULL, 0, format, args);
    if(len < 0) 
        return NULL;
    // format message
    char* string = (char*)malloc(sizeof(char) * (len + 1)); //[len + 1]; // or use heap allocation if implementation doesn't support VLAs
    vsnprintf(string, len + 1, format, args);
    return string;
}
// deletes formated string
void stringfclear(char* string) {
    free(string);
}

void logf(const char* message, ...) {
    // formating the string
    va_list args;
    va_start(args, message);
    char* msg = stringf(message, args);
    va_end(args);
    // logging of the message
    printf("%s\n", message);
    // delete formated string
    stringfclear(msg);
}

void logfSuccess(const char* message, ...) {
    // formating the string
    va_list args;
    va_start(args, message);
    char* msg = stringf(message, args);
    va_end(args);
    // logging of the message
    Console::Get().SetColor(CONCOL_GREEN);
    printf("%s\n", message);
    Console::Get().SetColorDefault();
    // delete formated string
    stringfclear(msg);
}

void logfWarn(const char* message, ...) {
    // formating the string
    va_list args;
    va_start(args, message);
    char* msg = stringf(message, args);
    va_end(args);
    // logging of the message
    Console::Get().SetColor(CONCOL_YELLOW);
    printf("%s\n", message);
    Console::Get().SetColorDefault();
    // delete formated string
    stringfclear(msg);
}

void logfError(const char* message, ...) {
    // formating the string
    va_list args;
    va_start(args, message);
    char* msg = stringf(message, args);
    va_end(args);
    // logging of the message
    Console::Get().SetColor(CONCOL_RED);
    printf("%s\n", message);
    Console::Get().SetColorDefault();
    // delete formated string
    stringfclear(msg);
}