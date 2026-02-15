#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define TAM 10000
#define NTHREADS 10

int vetor[TAM];

void* tarefa(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < TAM; i++) {
        if (id % 2 == 1) {
            vetor[i] += id;        // Soma
        } else {
            vetor[i] *= id;        // Multiplica
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NTHREADS];
    int ids[NTHREADS];

    // Inicializa vetor com 0
    for (int i = 0; i < TAM; i++) vetor[i] = 0;

    for (int i = 0; i < NTHREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, tarefa, (void*)&ids[i]);
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Verifica inconsistências
    for (int i = 0; i < TAM; i++) {
        if (vetor[i] != vetor[0]) {
            printf("Concorrência detectada na posição %d: %d vs %d\n", i, vetor[i], vetor[0]);
            break;
        }
    }

    printf("Valor final de vetor[0]: %d\n", vetor[0]);

    return 0;
}
