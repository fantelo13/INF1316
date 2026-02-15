#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define TAM 10000
#define NTHREADS 10

int vetor[TAM];
sem_t mutex;

void* tarefa(void* arg) {
    int id = *((int*)arg);

    sem_wait(&mutex); // Down

    for (int i = 0; i < TAM; i++) {
        if (id % 2 == 1)
            vetor[i] += id;
        else
            vetor[i] *= id;
    }

    sem_post(&mutex); // Up
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NTHREADS];
    int ids[NTHREADS];

    for (int i = 0; i < TAM; i++) vetor[i] = 0;

    sem_init(&mutex, 0, 1);

    for (int i = 0; i < NTHREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, tarefa, (void*)&ids[i]);
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&mutex);

    printf("Valor final de vetor[0]: %d\n", vetor[0]);

    return 0;
}
