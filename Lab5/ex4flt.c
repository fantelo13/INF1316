#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sigfpe_handler(int signum) {
    printf("\nErro: Divisão por zero detectada! (sinal %d)\n", signum);
    exit(EXIT_FAILURE);
}

int main() {
    double num1, num2, resultado;

    signal(SIGFPE, sigfpe_handler);

    printf("Digite o primeiro número real: ");
    scanf("%lf", &num1);

    printf("Digite o segundo número real: ");
    scanf("%lf", &num2);

    printf("\nResultados das operações:\n");
    printf("Soma: %.2lf + %.2lf = %.2lf\n", num1, num2, num1 + num2);
    printf("Subtração: %.2lf - %.2lf = %.2lf\n", num1, num2, num1 - num2);
    printf("Multiplicação: %.2lf * %.2lf = %.2lf\n", num1, num2, num1 * num2);
    printf("Divisão: %.2lf / %.2lf = %.2lf\n", num1, num2, num1 / num2);
    
    return 0;
}

//divisão por zero da inf sempre sem ativar o SIGFPE