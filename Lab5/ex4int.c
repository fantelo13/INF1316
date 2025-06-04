#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sigfpe_handler(int signum) {
    printf("\nErro: Divisão por zero detectada! (sinal %d)\n", signum);
    exit(EXIT_FAILURE);
}

int main() {
    int num1, num2;

    signal(SIGFPE, sigfpe_handler);

    printf("Digite o primeiro número inteiro: ");
    scanf("%d", &num1);

    printf("Digite o segundo número inteiro: ");
    scanf("%d", &num2);

    printf("\nResultados das operações:\n");
    printf("Soma: %d + %d = %d\n", num1, num2, num1 + num2);
    printf("Subtração: %d - %d = %d\n", num1, num2, num1 - num2);
    printf("Multiplicação: %d * %d = %d\n", num1, num2, num1 * num2);
    printf("Divisão: %d / %d = %d\n", num1, num2, num1 / num2);

    return 0;
}

//divisão por zero ativa o SIGFPE