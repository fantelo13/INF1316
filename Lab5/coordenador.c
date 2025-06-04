#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(void) {
    pid_t filho1, filho2;
    int tempoTotal = 15; // Tempo de execução
    int tempoPorFilho = 1; // Alternância de 1 segundo

    // Cria primeiro filho
    if ((filho1 = fork()) == 0) {
        execlp("./infinito", "infinito", (char *)NULL);
        perror("execvp filho1");
        exit(1);
    }

    // Cria segundo filho
    if ((filho2 = fork()) == 0) {
        execlp("./infinito", "infinito", (char *)NULL);
        perror("execvp filho2");
        exit(1);
    }

    sleep(1); // Dá tempo para os filhos inicializarem

    for (int i = 0; i < tempoTotal; i += 2) {
        kill(filho1, SIGCONT);
        kill(filho2, SIGSTOP);
        sleep(tempoPorFilho);

        kill(filho1, SIGSTOP);
        kill(filho2, SIGCONT);
        sleep(tempoPorFilho);
    }

    printf("Tempo acabou! Matando os filhos...\n");
    kill(filho1, SIGKILL);
    kill(filho2, SIGKILL);

    wait(NULL);
    wait(NULL);

    printf("Filhos terminados.\n");
    return 0;
}
