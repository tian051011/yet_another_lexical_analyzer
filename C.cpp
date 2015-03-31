#include <stdio.h>  
#include <iostream>  
#include <cstring>  
  
#define NUMERROR -1  
  
#define _RESERVE_NUM 32  
#define _DELIMILTER_NUM 8  
#define _DELIMILTER_SIZE 100  
#define _CHARLIST_SIZE 100  
#define _NUMLIST_SIZE 100  
#define _TOKEN_SIZE 100  
#define COL 1000  
  
#define LT 1  
#define LE 2  
#define EQ 3  
  
using namespace std;  
  
FILE* fp;  
int lastRetval = 0;  
  
char charList[_CHARLIST_SIZE][15] = {0};  
char charList_nu[_CHARLIST_SIZE] = {0};  
char charList_index = 0;  
  
char numList[_NUMLIST_SIZE][15] = {0};  
char numList_nu[_CHARLIST_SIZE] = {0};  
char numList_index = 0;  
  
char delimilterList[_DELIMILTER_SIZE][15] = {0};  
char delimilterList_nu[_DELIMILTER_SIZE] = {0};  
char delimilterList_index = 0;  
  
char reserveList[_RESERVE_NUM][15] = {  
    "void", "int", "char", "float", "double",  
    "while", "auto", "break", "case", "const",  
    "continue", "default", "do", "else", "enum",  
    "extern", "for", "goto", "if", "long",  
    "return", "short", "signed", "sizeof", "static",  
    "struct", "switch", "typedef", "union", "unsigned",  
    "volatile",  "redister"  
};  
  
char delimilter[_DELIMILTER_NUM][5] = {  
    "+", "-", "*", "/", "<", ";", "<=", "=="      // six plus two  
};  
  
void nu_print(int nu) {  
    int i, cindex, nindex, flag, hasPrint;  
    cindex = nindex = 0;  
    printf("\n======================== each line to see =========================\n");  
    for(i = 0; i < nu; i++) {  
        hasPrint = 0;  
        for( ; cindex <= charList_index || nindex <= numList_index; ) {  
            flag = 0;  
            if(charList_nu[cindex] == i+1) {  
                if(0 == hasPrint) {  
                    printf("\nline %d\n", i+1);  
                    hasPrint = 1;  
                }  
                printf("    %s ", charList[cindex]);  
                ++cindex;  
                flag = 1;  
            }  
            if(numList_nu[nindex] == i+1) {  
                if(0 == hasPrint) {  
                    printf("\nline %d\n", i+1);  
                    hasPrint = 1;  
                }  
                printf("    %s ", numList[nindex]);  
                ++nindex;  
                flag = 1;  
            }  
            if(0 == flag) break;  
        }  
    }  
}  
  
void _print(int nu) {  
    int i, j;  
    printf("\n============== char of list ================\n");  
    for(i = 0; i < charList_index; i++) {  
        printf("%s  nu: %d\n", charList[i], charList_nu[i]);  
        fprintf(fp,"%s  nu: %d\n", charList[i], charList_nu[i]);  
    }  
    printf("\n============== const number of list ================\n");  
    for(i = 0; i < numList_index; i++) {  
        printf("%s  nu: %d\n", numList[i], numList_nu[i]);  
        fprintf(fp, "%s  nu:%d\n", numList[i], numList_nu[i]);  
    }  
    printf("\ntotal nu: %d\n", nu);  
}  
  
bool isLetter(char a){  
    if((a <= 'Z' && a >= 'A') || (a <= 'z' && a >= 'a') || '_' == a) {  
        return true;  
    }  
    else return false;  
}  
  
bool isDigit(char a) {  
    if(a <= '9' && a >= '1') {  
        return true;  
    }  
    else return false;  
}  
  
void concatenation(char token[_TOKEN_SIZE], char str) {  
    int len = strlen(token);  
    token[len] = str;  
}  
  
int reserve(char token[_TOKEN_SIZE]) {  
    int i, j;  
  
    for(i = 0; i < _RESERVE_NUM; i++) {  
        if(!strcmp(&reserveList[i][0], token)) {  
            return 1;  
        }  
    }  
    for(i = 0; i < _DELIMILTER_NUM; i++) {  
        if(!strcmp(&delimilter[i][0], token)) {  
            return 2;  
        }  
    }  
    return 0;  
}  
  
int buildCharList(char token[_TOKEN_SIZE]) {  
    strcpy(&charList[charList_index][0], token);  
    ++charList_index;  
  
    return charList_index-1;  
}  
  
int buildNumList(char token[_TOKEN_SIZE]) {  
    strcpy(&numList[numList_index][0], token);  
    ++numList_index;  
  
    return numList_index-1;  
}  
  
int analysisCode(char str, int& nu) {  
    int num;  
    char token[_TOKEN_SIZE];  
    memset(token, 0, sizeof(token));  
      
    if('\n' == str) {  
        ++nu;  
        return '\n';  
    }  
    else if(isLetter(str)) {  
        while(isLetter(str) || isDigit(str)) {  
            concatenation(token, str);  
            str = getchar();  
        }  
        ungetc(str, stdin);  
        int type = reserve(token);  
        if(0 == type) {  
            num = buildCharList(token);  
            charList_nu[num] = nu;  
        }  
        memset(token, 0, sizeof(token));  
        return num;  
    }  
    else if(isDigit(str) || '+' == str || '-' == str) {  
        if(NUMERROR == lastRetval) {  
            return NUMERROR;  
        }  
        int dotFlag, eFlag, numFlag, fFlag;  
        int eNum, dotNum, fNum;  
  
        dotFlag = eFlag = 0;  
        numFlag = fFlag = 1;  
        eNum = dotNum = fNum = 0;  
        while(isDigit(str) || 'e' == str || '.' == str || '+' == str || '-' == str) {  
            if('e' == str) {  
                if(0 == eFlag || 1 == eNum) {  
                    ungetc(str, stdin);  
                    return NUMERROR;  
                }  
                dotFlag = 0;  
                eFlag = 0;  
                numFlag = 1;  
                fFlag = 1;  
                ++eNum;  
                dotNum = 0;  
                fNum = 0;  
            }  
            else if('+' == str || '-' == str) {  
                if(0 == fFlag || 1 == fNum) {  
                    ungetc(str, stdin);  
                    return NUMERROR;  
                }  
                dotFlag = 0;  
                eFlag = 0;  
                numFlag = 1;  
                fFlag = 0;  
                ++fNum;  
            }  
            else if('.' == str) {  
                if(0 == dotFlag || 1 == dotNum) {  
                    ungetc(str, stdin);  
                    return NUMERROR;  
                }  
                dotFlag = 0;  
                eFlag = 0;  
                numFlag = 1;  
                fFlag = 0;  
                ++dotNum;  
            }  
            else if(isDigit(str)) {  
                dotFlag = 1;  
                eFlag = 1;  
                numFlag = 1;  
                fFlag = 0;  
            }  
            concatenation(token, str);  
            str = getchar();  
        }  
        ungetc(str, stdin);  
        num = buildNumList(token);  
        numList_nu[num] = nu;  
        memset(token, 0, sizeof(token));  
        return num;  
    }  
    else if('"' == str) {  
        int flag = 0;  
        while(0 == flag) {  
            concatenation(token, str);  
            str = getchar();  
            if('"' == str) {  
                flag = 1;  
            }  
  
        }  
        concatenation(token, str);  
    }  
    else {  
        for(int i = 0; i < 6; i++) {  
            if(delimilter[i][0] == str) {  
                return str;  
            }  
        }  
        if('<' == str) {  
            str = getchar();  
            if('=' == str) {  
                return LE;  
            }  
            ungetc(str, stdin);  
            return LT;  
        }  
        if('=' == str) {  
            str = getchar();  
            if('=' == str) {  
                return EQ;  
            }  
            ungetc(str, stdin);  
            return '=';  
        }  
    }  
//  return NUMERROR;  
}  
  
int main() {  
    freopen("t1.txt", "rw", stdin);  
    fp = fopen("D://file.txt", "w");  
  
    char str;  
    char token[_TOKEN_SIZE];  
    int nu = 1;  
    memset(token, 0, sizeof(token));  
    while(scanf("%c", &str) != EOF) {  
        lastRetval = analysisCode(str, nu);  
    }  
      
    _print(nu-1);  
  
    nu_print(nu-1);  
  
    fclose(stdin);  
    return 0;  
}
