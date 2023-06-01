#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_DEPTH 100

int main() {
    const char* base_file = "a";
    const char* link_format = "%c%c";
    char link_name[MAX_DEPTH + 1];
    char next_link_name[MAX_DEPTH + 1];
    int depth = 0;
    FILE* file;

    // Создание директории для файлов
    if (mkdir("files", 0777) == -1) {
        perror("mkdir");
        exit(1);
    }

    // Создание базового файла
    file = fopen("files/a", "w");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    fclose(file);

    // Генерация символьных связей и проверка открытия файлов
    while (depth < MAX_DEPTH) {
        snprintf(link_name, sizeof(link_name), link_format, 'a' + depth, 'a' + depth);
        snprintf(next_link_name, sizeof(next_link_name), link_format, 'a' + depth, 'a' + depth + 1);

        if (symlink(base_file, link_name) == -1) {
            perror("symlink");
            break;
        }

        file = fopen(next_link_name, "r");
        if (file == NULL) {
            printf("Reached recursion depth: %d\n", depth);
            break;
        }
        fclose(file);

        depth++;
    }

    // Удаление созданных файлов и символьных связей
    for (int i = 0; i < depth; i++) {
        snprintf(link_name, sizeof(link_name), link_format, 'a' + i, 'a' + i);
        unlink(link_name);
    }

    // Удаление директории
    if (rmdir("files") == -1) {
        perror("rmdir");
        exit(1);
    }

    return 0;
}