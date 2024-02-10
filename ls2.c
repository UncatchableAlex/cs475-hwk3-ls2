#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

/**
 * Take a path, a string to match to (NULL if matching is disabled), and an internal
 * depth parameter. Return a string representation of the file tree rooted at "path" including 
 * all subdirectories recursively containing any file that matches. 
*/
char *runLs2(char *path, char *match, int depth) {
    struct stat buf;
    // open the directory on this path:
    DIR *dir = opendir(path);
    struct dirent *entry;
    // if we can't open the directory for whatever reason, we return NULL
    // Maybe the user gave the path to a file?
    if (dir == NULL) {
        return NULL;
    }
    // this is the stringified tree that we will use as our data structure.
    char *treeStr = (char*) calloc(1,1);
    // read the next subentry within this directory until there are no more entries left to read
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
        // is the entry a directory? is it a normal file?
        int isDir =  S_ISDIR(buf.st_mode);
        int isNormalFile = S_ISREG(buf.st_mode);
        if (isDir) {
            // the current entry is a directory. Recurse! 
            char *childStr = runLs2(newPath, match, depth + 1);
            // if the value from our recursion isn't empty then there was a match that we found
            // (if we were looking for matches):
            if (!match || *childStr) {
                // prefix the subtree associated with the current entry to our tree
                prefixToTree(&treeStr, childStr, 0);
            }
            free(childStr);
        }
        // if it's a file and the name matches, add it to our tree string:
        else if (isNormalFile && (!match || !strcmp(entry->d_name, match))) {
            // allocate enough space to fit the number for how many bytes the file has (maybe a lot?)
            char prefix[strlen(entry->d_name) + 40];
            sprintf(prefix, "%s (%d bytes)", entry->d_name, (int)buf.st_size);
            prefixToTree(&treeStr, prefix, depth);
        }
        free(newPath);
    }
    // if we aren't at the top-level directory, and there was a match in one of the
    // subdirectories (or we weren't even looking for matches)...
    if (depth && (!match || *treeStr)) {
        // get this directory's name from the path
        char *token = strtok(path, "/");
        char *last;
        while (token) {
            last = token;
            token = strtok(NULL, "/");
        }
        // add this directory to the tree:
        char prefix[strlen(last) + 13];
        sprintf(prefix, "%s/ (directory)", last);
        prefixToTree(&treeStr, prefix, depth - 1);
    }
    closedir(dir);
    return treeStr;
}

/**
 * Concatenate a prefix with the file tree string input parameter. Add appropriate indentation, 
 * and add a newline to delimit prefix and tree (if neither are empty).
 */
void prefixToTree(char **tree, char *prefix, int indents) {
    char *newStr = (char*) malloc(strlen(INDENT)*indents + strlen(prefix) + strlen(*tree) + 2);
    newStr[0] = '\0';
    for (int i = 0; i < indents; i++) {
        strcat(newStr, INDENT);
    }
    char *format = **tree && *prefix ? "%s\n%s" : "%s%s";
    sprintf(newStr + (indents * strlen(INDENT)),format, prefix, *tree);
    free(*tree);
    *tree = newStr;
}
