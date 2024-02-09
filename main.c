#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "ls2.h"
#include <string.h>

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 **/
int main(int argc, char* argv[]) {
    char* tree = runLs2("./a", "main", 0);
    //printf("%s", tree);
   char *str1 = "hello";
   char *str2 = "world";
   char *str3 = catWithIndents(str1, str2, 3);
   //printf("%s", str3);
    return 0;
}
