//Exercício 2 que trata de troca síncronaS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define TOTAL_MSGS 32

// Estrutura da mensagem
struct msgbuf {
    long mtype;    // tipo da mensagem (vamos usar 1 para todas)
    int valor;     // valor inteiro produzido
};

// Função produtor: envia 32 mensagens, esperando confirmação do consumidor (síncrono)
void produtor(int msqid) {
    struct msgbuf msg;
    struct msgbuf conf;
    for (int i = 0; i < TOTAL_MSGS; i++) {
        msg.mtype = 1;
        msg.valor = i + 1;
        // Envia mensagem ao consumidor
        if (msgsnd(msqid, &msg, sizeof(msg.valor), 0) == -1) {
            perror("Erro ao enviar mensagem");
            exit(1);
        }
        printf("[PRODUTOR] Enviou: %d (Mensagem %d)\n", msg.valor, i+1);

        // Aguarda confirmação (mtype=2)
        if (msgrcv(msqid, &conf, sizeof(conf.valor), 2, 0) == -1) {
            perror("Erro ao receber confirmação");
            exit(1);
        }
        printf("[PRODUTOR] Recebeu confirmação do consumidor.\n");
    }
}

// Função consumidor: recebe 32 mensagens e envia confirmação para cada uma
void consumidor(int msqid) {
    struct msgbuf msg;
    struct msgbuf conf;
    for (int i = 0; i < TOTAL_MSGS; i++) {
        // Recebe mensagem do produtor (mtype=1)
        if (msgrcv(msqid, &msg, sizeof(msg.valor), 1, 0) == -1) {
            perror("Erro ao receber mensagem");
            exit(1);
        }
        printf("[CONSUMIDOR] Recebeu: %d (Mensagem %d)\n", msg.valor, i+1);

        // Envia confirmação para o produtor (mtype=2)
        conf.mtype = 2;
        conf.valor = msg.valor; // só para referência, pode ser ignorado
        if (msgsnd(msqid, &conf, sizeof(conf.valor), 0) == -1) {
            perror("Erro ao enviar confirmação");
            exit(1);
        }
        printf("[CONSUMIDOR] Enviou confirmação ao produtor.\n");
    }
}

int main() {
    key_t key = ftok("lab8file2", 66);  // Gera chave única baseada em arquivo diferente
    int msqid = msgget(key, IPC_CREAT | 0666); // Cria fila de mensagens
    if (msqid == -1) {
        perror("Erro ao criar fila de mensagens");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Erro ao criar processo");
        exit(1);
    }
    else if (pid == 0) {
        // Processo filho: consumidor
        consumidor(msqid);
        exit(0);
    }
    else {
        // Processo pai: produtor
        produtor(msqid);
        wait(NULL);
        // Remove fila de mensagens
        msgctl(msqid, IPC_RMID, NULL);
        printf("\n[DEBUGGANDO] Troca síncrona finalizada. Fila removida.\n");
    }
    return 0;
}
