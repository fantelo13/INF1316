#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALG_NAME 16
#define MAX_TRACE 10000000

typedef struct {
    unsigned int page_number;
    int valid;
    int referenced;
    int modified;
    unsigned long last_access_time;
} Frame;

Frame *frames = NULL;
unsigned int num_frames = 0;
unsigned int page_size = 0;
unsigned int page_shift = 0;
unsigned long time_counter = 0;
unsigned int page_faults = 0;
unsigned int pages_written = 0;
char algorithm[MAX_ALG_NAME];
unsigned int clock_pointer = 0;

unsigned int *trace = NULL;
char *trace_rw = NULL;
unsigned int trace_len = 0;

// Cálculo do deslocamento com base no tamanho da página
unsigned int calc_shift(unsigned int page_size_kb) {
    unsigned int shift = 0;
    unsigned int bytes = page_size_kb * 1024;
    while (bytes > 1) {
        bytes >>= 1;
        shift++;
    }
    return shift;
}

// Procura se a página está em algum quadro
int find_page(unsigned int page_number) {
    for (unsigned int i = 0; i < num_frames; i++) {
        if (frames[i].valid && frames[i].page_number == page_number)
            return i;
    }
    return -1;
}

// LRU: menor tempo de uso
int select_frame_LRU() {
    unsigned long min_time = ~0UL;
    int victim = -1;
    for (unsigned int i = 0; i < num_frames; i++) {
        if (!frames[i].valid)
            return i;
        if (frames[i].last_access_time < min_time) {
            min_time = frames[i].last_access_time;
            victim = i;
        }
    }
    return victim;
}

// Segunda Chance (Clock)
int select_frame_SecondChance() {
    while (1) {
        if (!frames[clock_pointer].valid)
            return clock_pointer;
        if (frames[clock_pointer].referenced) {
            frames[clock_pointer].referenced = 0;
            clock_pointer = (clock_pointer + 1) % num_frames;
        } else {
            int victim = clock_pointer;
            clock_pointer = (clock_pointer + 1) % num_frames;
            return victim;
        }
    }
}

int select_frame_Clock() {
    return select_frame_SecondChance(); // mesmo funcionamento
}

// Algoritmo Ótimo: escolhe a página usada mais tarde no futuro
int select_frame_Optimal(unsigned int current_index) {
    int victim = -1;
    int farthest_use = -1;

    for (unsigned int i = 0; i < num_frames; i++) {
        if (!frames[i].valid)
            return i;

        int found = 0;
        for (unsigned int j = current_index + 1; j < trace_len; j++) {
            if ((trace[j] >> page_shift) == frames[i].page_number) {
                if ((int)j > farthest_use) {
                    farthest_use = j;
                    victim = i;
                }
                found = 1;
                break;
            }
        }

        if (!found)
            return i; // não será mais usada
    }

    return (victim != -1) ? victim : 0;
}

// Simulação principal
void simulate() {
    for (unsigned int i = 0; i < trace_len; i++) {
        unsigned int addr = trace[i];
        char rw = trace_rw[i];
        unsigned int page_number = addr >> page_shift;
        time_counter++;

        int frame_index = find_page(page_number);

        if (frame_index != -1) {
            frames[frame_index].referenced = 1;
            if (rw == 'W') frames[frame_index].modified = 1;
            frames[frame_index].last_access_time = time_counter;
        } else {
            page_faults++;
            int victim = -1;

            if (strcmp(algorithm, "LRU") == 0)
                victim = select_frame_LRU();
            else if (strcmp(algorithm, "2nd") == 0)
                victim = select_frame_SecondChance();
            else if (strcmp(algorithm, "clock") == 0)
                victim = select_frame_Clock();
            else if (strcmp(algorithm, "otimo") == 0)
                victim = select_frame_Optimal(i);
            else {
                fprintf(stderr, "Algoritmo inválido: %s\n", algorithm);
                exit(1);
            }

            if (frames[victim].valid && frames[victim].modified)
                pages_written++;

            frames[victim].page_number = page_number;
            frames[victim].valid = 1;
            frames[victim].referenced = 1;
            frames[victim].modified = (rw == 'W') ? 1 : 0;
            frames[victim].last_access_time = time_counter;
        }
    }
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s [LRU/2nd/clock/otimo] arquivo.log tamanho_pagina_KB tamanho_memoria_MB\n", argv[0]);
        return 1;
    }

    strncpy(algorithm, argv[1], MAX_ALG_NAME);
    char *filename = argv[2];
    page_size = atoi(argv[3]) * 1024;
    unsigned int mem_size = atoi(argv[4]) * 1024 * 1024;

    page_shift = calc_shift(atoi(argv[3]));
    num_frames = mem_size / page_size;

    frames = (Frame *) calloc(num_frames, sizeof(Frame));
    if (!frames) {
        perror("Erro ao alocar memória para frames");
        return 1;
    }

    trace = (unsigned int *) malloc(sizeof(unsigned int) * MAX_TRACE);
    trace_rw = (char *) malloc(sizeof(char) * MAX_TRACE);
    if (!trace || !trace_rw) {
        perror("Erro ao alocar memória para trace");
        return 1;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    unsigned int addr;
    char rw;
    while (fscanf(fp, "%x %c", &addr, &rw) == 2) {
        if (trace_len >= MAX_TRACE) {
            fprintf(stderr, "Trace muito grande\n");
            exit(1);
        }
        trace[trace_len] = addr;
        trace_rw[trace_len] = rw;
        trace_len++;
    }
    fclose(fp);

    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", filename);
    printf("Tamanho da memoria fisica: %d MB\n", atoi(argv[4]));
    printf("Tamanho das paginas: %d KB\n", atoi(argv[3]));
    printf("Algoritmo de substituicao: %s\n", algorithm);

    simulate();

    printf("Numero de Faltas de Paginas: %u\n", page_faults);
    printf("Numero de Paginas Escritas: %u\n", pages_written);

    free(frames);
    free(trace);
    free(trace_rw);
    return 0;
}
