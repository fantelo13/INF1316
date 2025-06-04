//2210872   Felipe Antelo Machado de Oliveira
//2112171   Pedro Henrique de Oliveira Valentim Gomes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

void inputPass(char *input, char **args) {
    int i = 0, j = 0, start = 0;
    while (input[i] != '\0' && input[i] != '\n') {
        while (input[i] == ' ' || input[i] == '\t') {
            i++; 
        }
        if (input[i] == '\0' || input[i] == '\n') {
            break;
        }
        start = i;
        while (input[i] != ' ' && input[i] != '\t' && input[i] != '\0' && input[i] != '\n') {
            i++;
        }
        args[j] = strndup(&input[start], i - start);
        j++;
    }
    args[j] = NULL;
}

void executeCommand(char **args) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Erro ao criar processo");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Erro ao executar comando");
            exit(EXIT_FAILURE);
        }
    } else {
        wait(NULL);
    }
}

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("meushell> ");
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }

        if (strcmp(input, "exit\n") == 0) {
            break;
        }

        inputPass(input, args);
        if (args[0] != NULL) {
            executeCommand(args);
        }

        for (int i = 0; args[i] != NULL; i++) {
            free(args[i]);
        }
    }

    return 0;
}
