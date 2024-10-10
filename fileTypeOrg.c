#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
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
    DIR *dr = opendir("../CS50");

    if (dr == NULL) {
        printf("Could not open directory");
        return 0;
    }
    char destination[128];  
    char res[128];  

    strcpy(res, "../Automations/testFolder");

    while ((de = readdir(dr)) != NULL) {
        if (strstr(de->d_name, ".cpp")) {
            snprintf(destination, sizeof(destination), "%s%s", res, de->d_name);
            moveFile(de->d_name, destination);
            printf("Moved %s to %s\n", de->d_name, destination);
        }
    }

    closedir(dr);
    return 0;
}
