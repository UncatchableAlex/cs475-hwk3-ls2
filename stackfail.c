#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"
#include "stack.h"

void runLs2(char *match) {
    struct dirent *entry;
    struct stat buf;
    stack_t *traversalStack = initstack();
    stack_t *printStack = initstack();
    char *path = (char*) malloc(2);
    strcpy(path, ".");
    push(traversalStack, path);
    printf("stack: %s", (char*)traversalStack->top->data);
    int printMode = FALSE;
    while (traversalStack->size) {
        path = traversalStack->top->data;
        DIR *dir = opendir(path);
        if (dir == NULL) {
           // printf("COULDNT OPEN: %s\n", path);
            free(path);
            continue;
        }
       // printf("OPENING: %s\n", path);
        int foundTarget = FALSE;
        while ((entry = readdir(dir)) != NULL) {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
                continue;
            }

            lstat(entry->d_name, &buf);
            int isDir =  S_ISDIR(buf.st_mode);
            int isMatch = !strcmp(entry->d_name, match);
            printf("entry name: %s\nmode: %d\nsize: %ld\n", entry->d_name, isDir, buf.st_size);
            if (isMatch) {
                printf("entry name: %s\nmode: %d\nsize: %ld\n path: %s", entry->d_name, isDir, buf.st_size, path);
            }
            
            if (isDir) {
                char *newPath = (char*)malloc(strlen(path) + strlen(entry->d_name) + 1);
                sprintf(newPath, "%s/%s", path, entry->d_name);
                //printf("DIRECTORY: %s\n", entry->d_name);
               // printf("Pushing: %s\n", newPath);
                push(traversalStack, newPath);
                //printstack(traversalStack);
            }
            if (isMatch && !foundTarget) {
                foundTarget = TRUE;
            }
            if (isMatch || isDir) {
                push(printStack, entry->d_name);
            }
            printf("\n");
        }
        free(path);
        closedir(dir);
    }
    free(traversalStack);
}

/**

- Test irregular files
- test weird nesting
- test weird file names (starting with .)
- really long directory names
*/

