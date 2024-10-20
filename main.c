#include"src/fileTypeOrg.c"

    int main() {
    char dir[] = "C:/Users/all/Downloads";

    // Move executable files
    moveFilesByExtension(dir, ".exe", "Executables");

    // Move image files Currently Not Working for images
    // moveFilesByExtension(dir, ".png", "Images");
    // moveFilesByExtension(dir, ".jpg", "Images");
    // moveFilesByExtension(dir, ".jpeg", "Images");
    // moveFilesByExtension(dir, ".jfif", "Images");
    return 0;
}
