#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void moveFile(const char *source, const char *destination) {
    FILE *srcFile, *destFile;
    char ch;

    srcFile = fopen(source, "r");
    if (srcFile == NULL) {
        printf("Cannot open source file.\n");
        exit(EXIT_FAILURE);
    }

    destFile = fopen(destination, "w");
    if (destFile == NULL) {
        fclose(srcFile);
        printf("Cannot open destination file.\n");
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(srcFile)) != EOF) {
        fputc(ch, destFile);
    }

    fclose(srcFile);
    fclose(destFile);

    if (remove(source) != 0) {
        printf("Error deleting source file.\n");
    } else {
        printf("File moved successfully.\n");
    }
}

int main() {
    struct dirent *de;
    char dir[40] = "C:/Users/all/Downloads/";
    DIR *dr = opendir(dir);

    if (dr == NULL) {
        printf("Could not open directory\n");
        return 0;
    }
    char execDir[128];
    snprintf(execDir, sizeof(execDir), "%sExecutables", dir);
    mkdir(execDir);
    
    while ((de = readdir(dr)) != NULL) {
        if (strstr(de->d_name, ".exe")) {
            char source[128];
            snprintf(source, sizeof(source), "%s%s", dir, de->d_name);
            
            char destination[128];
            snprintf(destination, sizeof(destination), "%sExecutables/%s", dir, de->d_name);
            
            moveFile(source, destination);
            printf("Moved %s to %s\n", de->d_name, destination);
        }
    }

    closedir(dr);
    return 0;
}
