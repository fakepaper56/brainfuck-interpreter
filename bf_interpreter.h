#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum {
HALT  // halt the interpreter.
,INCP // ++ptr increase pointer
,DECP // --ptr decrease pointer
,INCV // ++*ptr increase value
,DECV // --*ptr decrease value
,PUT  // putchar()  
,GET  // getchar()
,LW   // left curly bracket for while
,RW   // right curly bracket for while
} Token;

int token();
void interpreter();
void syntaxErr(char *err);
void leftBracket(char *);
void rightBracket(char *);

char *src; // pointer to data in source file.
int top = -1;
char *bracketStack[16];
