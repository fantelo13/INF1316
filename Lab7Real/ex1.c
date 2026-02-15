#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM 10000
#define NTHREADS 10

int vetor[TAM];

void* tarefa(void* arg) {
    int id = *((int*)arg);
    int operacao = (id % 2 == 0) ? id : -id;  // Soma se par, subtrai se ímpar

    for (int i = 0; i < TAM; i++) {
        vetor[i] += operacao;
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NTHREADS];
    int ids[NTHREADS];

    // Inicializa vetor com 10
    for (int i = 0; i < TAM; i++) vetor[i] = 10;

    for (int i = 0; i < NTHREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, tarefa, (void*)&ids[i]);
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Valor esperado: 10 + (-1 + 2 - 3 + 4 - 5 + 6 - 7 + 8 - 9 + 10) = 15
    int esperado = 15;
    for (int i = 0; i < TAM; i++) {
        if (vetor[i] != esperado) {
            printf("Concorrência detectada na posição %d: %d vs %d\n", i, vetor[i], esperado);
            break;
        }
    }

    printf("Valor final de vetor[0]: %d\n", vetor[0]);

    return 0;
}
