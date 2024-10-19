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
        fputc(ch, destFile);
    }

    if (fclose(srcFile) != 0 || fclose(destFile) != 0) {
        perror("Error closing file");
    }

    if (remove(source) != 0) {
        perror("Error deleting source file");
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
