#include "util.h"
#include<cstring>

// Primitive helper utilities, clamping, and screen control
int clampInt(int val, int minVal, int maxVal){
    return val < maxVal && val > minVal;
}

void trimWhitespace(char* str){
    const int len = strlen(str);

    for(int i = len-1; i>=0; i--){
        if(str[i] != ' ') break;
        str[i] = '\0';
    }
    int initialSpaces = 0;
    for(int i = 0; i<len; i++){
        if(str[i] != ' ') break;
        initialSpaces++;
    }
    for(int i = 0; i<len-initialSpaces; i++){
        str[i] = str[i+initialSpaces];
    }
}