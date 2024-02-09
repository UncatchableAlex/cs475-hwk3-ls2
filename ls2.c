#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"
#include "stack.h"

char *runLs2(char *path, char *match, int depth) {
    printf("discovered: %s\n", path);
    struct stat buf;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return "";
    }
    struct dirent *entry;
    char *treeStr = (char*) calloc(1,1);
    int childMatch = FALSE;
    while ((entry = readdir(dir)) != NULL) {
        // don't recurse on this directory of the last directory:
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }
        // get the path for the next entry:
        char *newPath = (char*)malloc(strlen(path) + strlen(entry->d_name) + 2);
        sprintf(newPath, "%s/%s", path, entry->d_name);
        // get the entry's stats:
        lstat(newPath, &buf);
        int isDir =  S_ISDIR(buf.st_mode);
        //printf("entry name: %s\nmode: %d\nsize: %ld\n", entry->d_name, isDir, buf.st_size);
        if (isDir) {
            char *childStr = runLs2(newPath, match, 1);
            if (*childStr != '\0') {
                childMatch = TRUE;
              //  treeStr = catWithIndents(treeStr, childStr, depth);
            }
        }
        else if (!strcmp(entry->d_name, match)) {
          //  treeStr = catWithIndents(treeStr, entry->d_name, depth + 1);
        }
    }
    char *token = strtok(path, "/");
    char *last;
    while (token) {
        last = token;
        token = strtok(NULL, "/");
    }
    childMatch |= strcmp(last, match);
   // printf("last: %s\n", last);
    if (childMatch) {
        treeStr = catWithIndents(last, treeStr, depth);
    }
    printf("%s\n", treeStr);
   return treeStr;
}


char *catWithIndents(char *str1, char *str2, int indents) {
    char *newStr = (char*) calloc(strlen(INDENT) * indents + strlen(str1) + strlen(str2) + 2, sizeof(char));
    for (int i = 0; i < indents; i++) {
        strcat(newStr, INDENT);
    }
    sprintf(newStr + (indents * strlen(INDENT)),"%s%s\n", str1, str2);
    return newStr;
}



/**

- Test irregular files
- test weird nesting
- test weird file names (starting with .)
- really long directory names
*/

