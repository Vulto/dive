#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

void list_dirs(const char* path, char*** dirs, int* count) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Failed to open directory: %s\n", path);
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        struct stat st;
        if (stat(full_path, &st) == 0) {
            // Check if it's a directory
            if (S_ISDIR(st.st_mode)) {
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                    // Dynamically allocate memory for the new directory
                    *dirs = (char**)realloc(*dirs, (*count + 1) * sizeof(char*));
                    (*dirs)[*count] = strdup(full_path);
                    (*count)++;
                    list_dirs(full_path, dirs, count);
                }
            }
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    char* path;
    if (argc > 1) {
        path = argv[1];
    } else {
        path = getenv("HOME");
    }

    // Initialize variables
    char** dirs = NULL;
    int count = 0;

    // List directories in the directory
    list_dirs(path, &dirs, &count);

    // Print the directories
    for (int i = 0; i < count; i++) {
        printf("%s\n", dirs[i]);
    }
    // Free memory
    for (int i = 0; i < count; i++)
        free(dirs[i]);
    free(dirs);

    return 0;
}
