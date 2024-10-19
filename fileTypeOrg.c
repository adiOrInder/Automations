#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Function to move files
void moveFile(const char *source, const char *destination) {
    FILE *srcFile, *destFile;
    char ch;

    // Open source file for reading
    srcFile = fopen(source, "r");
    if (srcFile == NULL) {
        printf("Cannot open source file: %s\n", source);
        exit(EXIT_FAILURE);
    }

    // Open destination file for writing
    destFile = fopen(destination, "w");
    if (destFile == NULL) {
        fclose(srcFile);
        printf("Cannot open destination file: %s\n", destination);
        exit(EXIT_FAILURE);
    }

    // Copy contents from source to destination
    while ((ch = fgetc(srcFile)) != EOF) {
        fputc(ch, destFile);
    }

    fclose(srcFile);
    fclose(destFile);

    // Remove the source file
    if (remove(source) != 0) {
        printf("Error deleting source file: %s\n", source);
    } else {
        printf("File moved successfully: %s\n", destination);
    }
}

// Function to create a directory if it doesn't exist
void createDir(const char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        mkdir(dir);
    }
}

// Function to move files based on their extension
void moveFilesByExtension(const char *dir, const char *ext, const char *subDir) {
    DIR *dr = opendir(dir);
    if (dr == NULL) {
        printf("Could not open directory: %s\n", dir);
        exit(EXIT_FAILURE);
    }

    struct dirent *de;
    char execDir[128];
    snprintf(execDir, sizeof(execDir), "%s/%s", dir, subDir);
    createDir(execDir);

    while ((de = readdir(dr)) != NULL) {
        if (strstr(de->d_name, ext)) {
            char source[256];
            snprintf(source, sizeof(source), "%s/%s", dir, de->d_name);
            char destination[256];
            snprintf(destination, sizeof(destination), "%s/%s", execDir, de->d_name);
            moveFile(source, destination);
            printf("Moved %s to %s\n", de->d_name, destination);
        }
    }

    closedir(dr);
}

int main() {
    char dir[] = "C:/Users/all/Downloads";

    // Move executable files
    moveFilesByExtension(dir, ".exe", "Executables");

    // Move image files
    moveFilesByExtension(dir, ".png", "Images");
    moveFilesByExtension(dir, ".jpg", "Images");
    moveFilesByExtension(dir, ".jpeg", "Images");

    return 0;
}
