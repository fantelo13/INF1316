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

            for (int j = 0; j < TAM_VETOR; j++) {
                a[j] *= 2;
                a[j] += 2;
            }

            // Envia a soma para o pai
            write(pipes[i][1], a, sizeof(a));
            close(pipes[i][1]);
            exit(0);
        }
    }

    // Processo pai (coordenador)
    int vetores[NUM_PROCESSOS][TAM_VETOR];

    for (int i = 0; i < NUM_PROCESSOS; i++) {
        read(pipes[i][0], vetores[i], sizeof(vetores[i]));
        close(pipes[i][0]);
        wait(NULL); // espera um filho por vez
    }
    for(int j = 0; j < NUM_PROCESSOS; j++){
    for (int i = 0; i < TAM_VETOR; i++){
        printf("%d\n", vetores[j][i]);
    }
    }

    // Espera todos os filhos terminarem
    for (i = 0; i < NUM_PROCESSOS; i++) {
        wait(NULL);
    }

    
    return 0;
}