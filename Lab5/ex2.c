#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void handler(int sig) {
    printf("Recebido sinal %d\n", sig);
}

int main() {
    signal(SIGKILL, handler);
    while(1) {
        printf("Rodando...\n");
        sleep(1);
    }
    return 0;
}

//SIGKILL não pode ser interceptado, o programa somente termina