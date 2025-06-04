#include <stdio.h>
#include <unistd.h>

int main(void) {
    while (1) {
        printf("Processo %d em execução...\n", getpid());
        sleep(1);
    }
    return 0;
}
