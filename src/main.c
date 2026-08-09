#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char last_directory[BUFFSIZE];

int cd_builtin(char **args) {
    char curdir[BUFFSIZE];
    char path[BUFFSIZE];

    getcwd(curdir, sizeof(curdir));

    if (args[1] == NULL) {
        args[1] = getenv("HOME");
    }
    if (!strcmp(args[1], "-")) {
        if (*last_directory == '\0') {
            fprintf(stderr, "Error: no hay directorio anterior.\n");
            return 1;
        }
        args[1] = last_directory;
    } else {
        if (*args[1] == '~') {
            if (args[1][1] == '/' || args[1][1] == '\0') {
                snprintf(path, sizeof path, "%s%s", getenv("HOME"),
                         args[1] + 1);
                args[1] = path;
            } else {
                fprintf(stderr, "Error: syntaxis no aceptada: %s\n", args[1]);
                return 1;
            }
        }
    }
    if (chdir(args[1])) {
        fprintf(stderr, "Error: Error al cambiar el directorio: %s\n", path);
        return 1;
    }

    strcpy(last_directory, curdir);
    return 0;
}

int dash_execute(char **args, unsigned char *return_status) {
    pid_t cpid;
    int status;
    int pipe_dscr[2];
    if (!strcmp(args[0], "cd")) {
        *return_status = cd_builtin(args);
        return 1;
    }

    if (pipe(pipe_dscr) == -1) {
        fprintf(
            stderr,
            "Error, no puedo crear el pipe de comunicación entre procesos.\n");
        exit(1);
    }

    if (strcmp(args[0], "exit") == 0) {
        return 0;
    }

    cpid = fork();

    if (cpid == 0) {
        if (execvp(args[0], args) < 0) {
            fprintf(stderr, "hrsh: comando no encontrado: %s\n", args[0]);
            *return_status = 1;
            write(pipe_dscr[1], return_status, sizeof(unsigned char));
        }
        exit(0);
    } else if (cpid < 0) {
        fprintf(stderr, "Error: no puedo invocar al error\n");
        *return_status = 2;
    } else {
        close(pipe_dscr[1]);
        read(pipe_dscr[0], return_status, sizeof(unsigned char));
        waitpid(cpid, &status, WUNTRACED);
    }

    return 1;
}

int loop() {
    char *line;
    char **args;
    int status = 1;
    unsigned char return_status = 0;

    do {
        printf("[%i]> ", return_status);
        return_status = 0;
        line = read_line();
        if (strcmp(trimwhitespace(line), "") != 0) {
            args = split_lines(line);
            status = dash_execute(args, &return_status);
            free(line);
            free(args);
        }
    } while (status);
    return 0;
}

int main(int argc, char *argv[]) {
    loop();
    return 0;
}
