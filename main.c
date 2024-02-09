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
    char path[14];
    strcpy(path, "./a");
    char* tree = runLs2(path, NULL, 0);
    printf("%s\n", tree);
    free(tree);
    return 0;
}
