// 2210872   Felipe Antelo Machado de Oliveira
// 2112171   Pedro Henrique de Oliveira Valentim Gomes

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

#define TAM_VETOR 10000
#define NUM_PROCESSOS 10

int main() {
    int i;
    int tam_processo = TAM_VETOR / NUM_PROCESSOS;

    int shm_id_vetor = shmget(IPC_PRIVATE, TAM_VETOR * sizeof(int), IPC_CREAT | 0666);
    if (shm_id_vetor < 0) {
        perror("Erro ao criar memória compartilhada para vetor");
        exit(EXIT_FAILURE);
    }

    int shm_id_somas = shmget(IPC_PRIVATE, NUM_PROCESSOS * sizeof(int), IPC_CREAT | 0666);
    if (shm_id_somas < 0) {
        perror("Erro ao criar memória compartilhada para somas");
        exit(EXIT_FAILURE);
    }

    int *a = (int *) shmat(shm_id_vetor, NULL, 0);
    int *somas_parciais = (int *) shmat(shm_id_somas, NULL, 0);

    for (i = 0; i < TAM_VETOR; i++) {
        a[i] = 10;
    }

    for (i = 0; i < NUM_PROCESSOS; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro no fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            int *a_child = (int *) shmat(shm_id_vetor, NULL, 0);
            int *somas_child = (int *) shmat(shm_id_somas, NULL, 0);

            int inicio = i * tam_processo;
            int fim = inicio + tam_processo;
            int soma = 0;

            for (int j = inicio; j < fim; j++) {
                a_child[j] *= 2;
                soma += a_child[j];
            }

            somas_child[i] = soma;

            shmdt(a_child);
            shmdt(somas_child);
            exit(0);
        }
    }

    for (i = 0; i < NUM_PROCESSOS; i++) {
        wait(NULL);
    }

    int soma_total = 0;
    for (i = 0; i < NUM_PROCESSOS; i++) {
        soma_total += somas_parciais[i];
    }

    printf("Soma total: %d\n", soma_total);

    shmdt(a);
    shmdt(somas_parciais);
    shmctl(shm_id_vetor, IPC_RMID, NULL);
    shmctl(shm_id_somas, IPC_RMID, NULL);

    return 0;
}