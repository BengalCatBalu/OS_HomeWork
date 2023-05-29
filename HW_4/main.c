#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./program <input_file> <output_file>\n");
        return 1;
    }

    char *inputFile = argv[1];
    char *outputFile = argv[2];

    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");

    if (input == NULL || output == NULL) {
        perror("Error opening files");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
        fwrite(buffer, 1, bytesRead, output);
    }

    fclose(input);
    fclose(output);

    printf("File copied successfully.\n");

    return 0;
}
