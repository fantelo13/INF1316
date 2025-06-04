//2210872   Felipe Antelo Machado de Oliveira
//2112171   Pedro Henrique de Oliveira Valentim Gomes

#include <stdio.h>
#include <stdlib.h>

void exibirArquivo(const char *nomearq) {
    FILE *arq = fopen(nomearq, "r");
    if (arq == NULL) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char caractere;
    while ((caractere = fgetc(arq)) != EOF) {
        putchar(caractere);
    }

    fclose(arq);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Erro nos arquivos\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        exibirArquivo(argv[i]);
    }

    return 0;
}
