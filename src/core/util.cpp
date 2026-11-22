#include<util.h>
#include<cstring>
#include<iostream>
#include<iomanip>

using namespace std;

// Primitive helper utilities, clamping, and screen control
int clampInt(int val, int minVal, int maxVal){
    if(val < minVal) return minVal;
    if(val > maxVal) return maxVal;
    return val;
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

    bool stringEquals(const char *a, const char *b){
        if(strlen(a) != strlen(b)) return 0;
        for(int i = 0; i< strlen(a); i++){
            if(a[i] != b[i]) return 0;
        }
        return 1;
    }
    void printHeader(const char *title){
        cout << setw(30) << setfill('-') << "-" << setfill(' ') <<"\n";
        cout << setw(15) << title  << "\n";
        cout << setw(30) << setfill('-') << "-" << setfill(' ') <<"\n";
    }
    void clearScreen(){
        for(int i = 0; i< 15; i++) cout <<"\n";
    }
