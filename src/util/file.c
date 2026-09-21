#include "file.h"

#include <stdint.h>
#include <malloc.h>
#include <string.h>

char* file_read_string(FILE* file) {
    uint8_t len;
    fread(&len, 1, 1, file);
    char* result = malloc(len + 1);
    fread(result, len, 1, file);
    result[len] = '\0';
    return result;
}

char* filename_find_ext(char* filename) {
    char* end = filename + strlen(filename);
    char* curr = end;

    while (curr >= filename) {
        --curr;

        if (*curr == '.') {
            return curr;
        }

        if (*curr == '/') {
            return end;
        }
    }

    return end;
}