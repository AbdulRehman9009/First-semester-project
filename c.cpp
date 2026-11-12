#include <iostream>
#include <cstring>
using namespace std;

void trimWhitespace(char *str)
{
    int len = strlen(str);
    int initialSpaces = 0;
    for (int i = 0; i < len - 1; i++)
    {
        if (str[i] != ' ')
            break;
        initialSpaces++;
    }
    for(int i = 0; i< len-initialSpaces; i++){
        str[i] = str[i+initialSpaces];
    }
    for (int i = len - 1; i > 0; i--)
    {
        if (str[i] != ' ')
            break;
        str[i] = '\0';
    }
}

int main()
{
    char str[] = "abdul   ";
    trimWhitespace(str);
    cout << str << "a" << endl;
    return 0;
}