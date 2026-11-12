#ifndef UTIL_H
#define UTIL_H

// Helper utilities for clamping, string comparisons, and screen control
int clampInt(int val, int minVal, int maxVal);
void trimWhitespace(char *str);
bool stringEquals(const char *a, const char *b);
void printHeader(const char *title);
void clearScreen();

#endif // UTIL_H
