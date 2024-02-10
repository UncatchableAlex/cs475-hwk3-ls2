#include <stdlib.h>
#include <stdio.h>
#include "ls2.h"

/**
 * Main function
 * Usage: ls2 <path> [exact-match-pattern]
 **/
int main(int argc, char* argv[]) {
    if (argc == 1 || argc > 3) {
        printf("Usage: ./ls2 <path> [exact-match-pattern]\n");
        return 0;
    }
    char *match = argc <= 2 ? NULL : argv[2];
    char *tree = runLs2(argv[1], match, 0);
    if (tree) {
        printf("%s\n", tree);
        free(tree);
    }
    return 0;
}
