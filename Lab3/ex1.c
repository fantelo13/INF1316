//2210872   Felipe Antelo Machado de Oliveira
//2112171   Pedro Henrique de Oliveira Valentim Gomes

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAM_VETOR 10000
#define NUM_PROCESSOS 10


int main() {
    int a[TAM_VETOR];
    int i;
    int tam_processo = TAM_VETOR / NUM_PROCESSOS;
    // Inicializa o vetor com 10
    for (i = 0; i < TAM_VETOR; i++) {
        a[i] = 10;
    }

    int pipes[NUM_PROCESSOS][2];

    // Criar os pipes
    for (i = 0; i < NUM_PROCESSOS; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < NUM_PROCESSOS; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Processo filho (trabalhador)

            int inicio = i * tam_processo;
            int fim = inicio + tam_processo;
            int soma = 0;

            for (int j = inicio; j < fim; j++) {
                a[j] *= 2;
                soma += a[j];
            }

            // Envia a soma para o pai
            write(pipes[i][1], &soma, sizeof(int));
            close(pipes[i][1]);
            exit(0);
        }
    }

    // Processo pai (coordenador)
    int soma_total = 0;
    for (i = 0; i < NUM_PROCESSOS; i++) {
        int soma_parcial;
        read(pipes[i][0], &soma_parcial, sizeof(int));
        close(pipes[i][0]);
        soma_total += soma_parcial;
    }

    // Espera todos os filhos terminarem
    for (i = 0; i < NUM_PROCESSOS; i++) {
        wait(NULL);
    }

    printf("Soma total: %d\n", soma_total);
    return 0;
}