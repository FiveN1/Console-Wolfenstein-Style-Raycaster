#ifndef ASSERT_H
#define ASSERT_H

// abort the program if condition id false and log debug info
#define assert(condition, message) _assert(condition, __FILE__, __LINE__, message);
// assert function
void _assert(bool condition, const char* file, int line, const char* message);

#endif ASSERT_H