#include"pch.h"
#include"assert.h"

#include"../../game/console/console.h"

void _assert(bool condition, const char* file, int line,  const char* message) {
    if (condition) return;
    Console::Get().SetColor(CONCOL_RED);
    printf("ERROR: %s\n", message);
    printf("at: %s, line: %d\n", file, line);
    Console::Get().SetColorDefault();
    abort();
}