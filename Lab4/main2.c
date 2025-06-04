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

    int *a = (int *) shmat(shm_id_vetor, NULL, 0);
    
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
            
            for (int j = 0; j < TAM_VETOR; j++) {
                a_child[j] *= 2;
                a_child[j] += 2;
            }

            shmdt(a_child);
            exit(0);
        }
    }

    for (i = 0; i < NUM_PROCESSOS; i++) {
        wait(NULL);
    }

    int soma_total = 0;
    for (i = 0; i < TAM_VETOR; i++) {
        printf("%d\n", a[i]);
    }

    shmdt(a);
    shmctl(shm_id_vetor, IPC_RMID, NULL);

    return 0;
}