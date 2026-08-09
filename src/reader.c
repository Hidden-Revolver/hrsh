#include "reader.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOK_DELIM " \t\r\n"

char *trimwhitespace(char *str) {
    char *end;

    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str == 0) {
        return str;
    }

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }

    end[1] = '\0';

    return str;
}

void check_err(void *buffer) {
    if (!buffer) {
        fprintf(stderr, "hdsh: Error en la asignación de memoria del buffer\n");
        exit(EXIT_FAILURE);
    }
}

void increment_buffer_size_if_cualified(unsigned char flag, int position,
                                        int *buff_size, char **buffer) {
    if (position >= *buff_size) {
        *buff_size += BUFFSIZE;
        if (flag == 1) {
            buffer = realloc(buffer, *buff_size);
        } else {
            buffer = realloc(*buffer, *buff_size);
        }
        check_err(*buffer);
    }
}

char *read_line() {
    int buff_size = BUFFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * buff_size);
    char c;

    check_err(buffer);
    while (1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        ++position;
        increment_buffer_size_if_cualified(0, position, &buff_size, &buffer);
    }
}

char **split_lines(char *line) {
    int buff_size = BUFFSIZE, position = 0;
    char **tokens = malloc(sizeof(char *) * buff_size);
    char *token;

    if (!tokens) {
        check_err(tokens);
    }

    token = strtok(line, TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        ++position;
        increment_buffer_size_if_cualified(1, position, &buff_size, tokens);
        token = strtok(NULL, TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}
