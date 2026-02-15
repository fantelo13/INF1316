#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TAM 10000
#define NTHREADS 10

int vetor[TAM];
sem_t sem;

void* tarefa(void* arg) {
    int id = *((int*)arg);
    int operacao = (id % 2 == 0) ? id : -id;  // Soma se par, subtrai se ímpar

    // Região crítica protegida por semáforo
    sem_wait(&sem);
    for (int i = 0; i < TAM; i++) {
        vetor[i] += operacao;
    }
    sem_post(&sem);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NTHREADS];
    int ids[NTHREADS];

    // Inicializa o vetor com 10
    for (int i = 0; i < TAM; i++) vetor[i] = 10;

    // Inicializa o semáforo binário (valor inicial 1)
    sem_init(&sem, 0, 1);

    for (int i = 0; i < NTHREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, tarefa, (void*)&ids[i]);
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroi o semáforo
    sem_destroy(&sem);

    // Verifica inconsistência
    int esperado = 10 + (-1 + 2 - 3 + 4 - 5 + 6 - 7 + 8 - 9 + 10); // Esperado = 15
    int erros = 0;

    for (int i = 0; i < TAM; i++) {
        if (vetor[i] != esperado) {
            printf("Erro na posição %d: %d (esperado: %d)\n", i, vetor[i], esperado);
            erros++;
        }
    }

    printf("Total de erros: %d\n", erros);
    printf("Valor final de vetor[0]: %d\n", vetor[0]);

    return 0;
}
