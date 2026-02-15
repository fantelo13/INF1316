//Exercício 1 que trata de troca assíncrona com buffer

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_BUFFER 64    // tamanho máximo da fila de mensagens
#define TOTAL_MSGS 256   // total de mensagens para produzir/consumir

// Estrutura da mensagem
struct msgbuf {
    long mtype;      // tipo da mensagem (vamos usar 1 para todos)
    int valor;       // valor inteiro produzido
};

// Função Produtor
void produtor(int msqid) {
    for (int i = 0; i < TOTAL_MSGS; i++) {
        struct msgbuf msg;
        msg.mtype = 1;
        msg.valor = i+1;  // produz um inteiro (1 a 256)
        // Envia mensagem para a fila, bloqueia se fila cheia
        if (msgsnd(msqid, &msg, sizeof(msg.valor), 0) == -1) {
            perror("Erro ao enviar mensagem");
            exit(1);
        }
        printf("[PRODUTOR] Enviou: %d (Mensagem %d)\n", msg.valor, i+1);
        sleep(1); // espera 1 segundo
    }
}

// Função Consumidor
void consumidor(int msqid, int consumidor_id, int intervalo, int msgs_consumir) {
    for (int i = 0; i < msgs_consumir; i++) {
        struct msgbuf msg;
        // Recebe mensagem (qualquer tipo)
        if (msgrcv(msqid, &msg, sizeof(msg.valor), 0, 0) == -1) {
            perror("Erro ao receber mensagem");
            exit(1);
        }
        printf("[CONSUMIDOR %d] Recebeu: %d (Mensagem %d)\n", consumidor_id, msg.valor, i+1);
        sleep(intervalo); // intervalo entre consumos
    }
}

int main() {
    key_t key = ftok("lab8file", 65);  // Gera chave única baseada em arquivo
    int msqid = msgget(key, IPC_CREAT | 0666); // Cria fila de mensagens
    if (msqid == -1) {
        perror("Erro ao criar fila de mensagens");
        exit(1);
    }

    pid_t pid1 = fork();  // cria primeiro consumidor
    if (pid1 == 0) {
        consumidor(msqid, 1, 1, TOTAL_MSGS/2);  // 1 segundo, 128 msgs
        exit(0);
    }

    pid_t pid2 = fork();  // cria segundo consumidor
    if (pid2 == 0) {
        consumidor(msqid, 2, 2, TOTAL_MSGS/2);  // 2 segundos, 128 msgs
        exit(0);
    }

    // Processo pai vira o produtor
    produtor(msqid);

    // Espera os consumidores terminarem
    wait(NULL);
    wait(NULL);

    // Remove fila de mensagens
    msgctl(msqid, IPC_RMID, NULL);

    printf("\n[DEBUGGANDO] Todos os processos terminaram. Fila removida.\n");
    return 0;
}