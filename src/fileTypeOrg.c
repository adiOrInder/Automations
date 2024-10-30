#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
void moveFile(const char *source, const char *destination) {
    FILE *srcFile, *destFile;
    char ch;

    srcFile = fopen(source, "rb");
    if (srcFile == NULL) {
        perror("Cannot open source file");
        exit(EXIT_FAILURE);
    }

    destFile = fopen(destination, "wb");
    if (destFile == NULL) {
        fclose(srcFile);
        perror("Cannot open destination file");
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(srcFile)) != EOF) {
        if (fputc(ch, destFile) == EOF) {
            perror("Error writing to destination file");
            fclose(srcFile);
            fclose(destFile);
            exit(EXIT_FAILURE);
        }
    }

    if (fclose(srcFile) != 0 || fclose(destFile) != 0) {
        perror("Error closing file");
        exit(EXIT_FAILURE);
    }

    srcFile = fopen(source, "rb");
    destFile = fopen(destination, "rb");

    if (srcFile == NULL || destFile == NULL) {
        perror("Cannot reopen files for verification");
        exit(EXIT_FAILURE);
    }

    while (!feof(srcFile) && !feof(destFile)) {
        if (fgetc(srcFile) != fgetc(destFile)) {
            perror("File copy verification failed");
            fclose(srcFile);
            fclose(destFile);
            exit(EXIT_FAILURE);
        }
    }

    if (!feof(srcFile) || !feof(destFile)) {
        perror("File sizes do not match");
        fclose(srcFile);
        fclose(destFile);
        exit(EXIT_FAILURE);
    }

    fclose(srcFile);
    fclose(destFile);

    if (remove(source) != 0) {
        perror("Error deleting source file");
        exit(EXIT_FAILURE);
    } else {
        printf("File moved successfully: %s\n", destination);
    }
}


void createDir(const char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
#ifdef _WIN32
        if (_mkdir(dir) != 0) {
            perror("Error creating directory");
        }
#else
        if (mkdir(dir, 0700) != 0) {
            perror("Error creating directory");
        }
#endif
    }
}

void moveFilesByExtension(const char *dir, const char *ext, const char *subDir) {
    DIR *dr = opendir(dir);
    if (dr == NULL) {
        perror("Could not open directory");
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

    if (closedir(dr) != 0) {
        perror("Error closing directory");
    }
}


