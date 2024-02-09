#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"
#include "stack.h"

char *runLs2(char *path, char *match, int depth) {
    //printf("discovered: %s\n", path);
    struct stat buf;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return "";
    }
    struct dirent *entry;
    char *treeStr = (char*) calloc(1,1);
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
            char *childStr = runLs2(newPath, match, depth + 1);
            // if our child string isn't empty (there was a match that our child string found):
            if (!match || *childStr) {
                suffixToTree(&treeStr, childStr, depth);
            }
            free(childStr);
        }
        // if it's a file and the name matches, add it to the string:
        else if (!match || !strcmp(entry->d_name, match)) {
            char prefix[strlen(entry->d_name) + 40];
            sprintf(prefix, "%s (%d bytes)", entry->d_name, (int)buf.st_size);
            prefixToTree(&treeStr, prefix, depth + 1);
        }
        free(newPath);
    }
   // printf("line 45 with: %s\n", path);
    char *token = strtok(path, "/");
    char *last;
    while (token) {
        last = token;
        token = strtok(NULL, "/");
    }
   // printf("last: %s\n", last);
    if (!match || *treeStr) {
        //printf("added %s to tree which is weird because my treeStr is %d\n", last, *treeStr);
        char prefix[strlen(last) + 13];
        sprintf(prefix, "%s (directory)", last);
      //  prefixToTree(&treeStr, prefix, depth);
        char *temp = catWithIndents(prefix, treeStr, depth);
        free(treeStr);
        treeStr = temp;
    }
  //  printf("tree: %s\n", treeStr);
    closedir(dir);
    return treeStr;
}

void prefixToTree(char **tree, char *prefix, int depth) {
    char *temp = catWithIndents(prefix, *tree, depth);
    free(*tree);
    *tree = temp;
}
void suffixToTree(char **tree, char *prefix, int depth) {
    char *temp = catWithIndents(*tree, prefix, depth);
    free(*tree);
    *tree = temp;
}


char *catWithIndents(char *str1, char *str2, int indents) {
    char *newStr = (char*) malloc(strlen(INDENT)*indents + strlen(str1) + strlen(str2) + 2);
    newStr[0] = '\0';
    for (int i = 0; i < indents; i++) {
        strcat(newStr, INDENT);
    }
    //char *format = *str1 && *str2 ? "%s\n%s" : "%s%s";
    char *format = "%s\n%s";
    sprintf(newStr + (indents * strlen(INDENT)),format, str1, str2);
    return newStr;
}



/**

- Test irregular files
- test weird nesting
- test weird file names (starting with .)
- really long directory names
*/

