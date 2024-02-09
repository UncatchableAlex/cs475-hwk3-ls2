#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 1
#define FALSE 0
#define INDENT "    "

// TODO: Any global variables go below

char *runLs2(char*, char*, int);
char *catWithIndents(char*, char*, int);
void suffixToTree(char**, char*, int);
void prefixToTree(char**, char*, int);
#endif
