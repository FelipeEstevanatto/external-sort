/**
 * External sort
 * 
 * input1.txt has 10.000 integers
 * input2.txt has 100.000 integers
 * input3.txt has 1.000.000 integers
 * input4.txt has 10.000.000 integers
 * 
 * Considere o seguinte método de ordenação por intercalação externa (external merge sort) para
ordenar N registros de um arquivo utilizando memória principal com capacidade para armazenar
e ordenar no máximo M (M<N) registros simultaneamente:
1) Ler até M registros do arquivo de entrada e ordená-los utilizando um algoritmo de
ordenação convencional (Quicksort, Mergesort, Heapsort, etc);
2) Escrever os registros ordenados em um arquivo intermediário;
3) Repetir os passos 1 e 2 até que os N registros sejam lidos e gravados em arquivos
intermediários e ordenados;
4) Para cada par de arquivos intermediários ordenados, realizar intercalação dos registros e
escrever os registros ordenados em um novo arquivo intermediário;
5) Executar o passo 4 enquanto esse passo gerar mais de um arquivo intermediário.
Execute a sua implementação para o arquivo de entrada fornecido ou crie outro alternativo,
calculando os tempos de execução do programa para cada entrada. Além disso, para cada
arquivo de entrada, calcule a quantidade de vezes que cada registro é lido e escrito no disco.
Faça um relatório com gráficos que comparam o desempenho do método implementado para
cada entrada. Descreva os detalhes dos experimentos realizados e discuta sobre os resultados
observados. Para este trabalho, considere M = 5.000.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define CHUNK_SIZE 5000

void mergeSortedFiles(FILE **tempFiles, int numFiles, FILE *outputFile);
void writeSortedChunk(int *buffer, int count, int *tempFileCount);
int compare(const void *a, const void *b);

int main() {
    FILE *inputFile = fopen("./inputs/input4.txt", "r");

    if (inputFile == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    int *buffer = (int *)malloc(CHUNK_SIZE * sizeof(int));
    int count = 0, tempFileCount = 0;
    char tempFileName[20];

    // Start timer of process
    clock_t start, end;

    start = clock();

    // Step 1: Read the file in chunks of CHUNK_SIZE
    // Step 2: Sort the chunk and write it to a temporary file
    while (fscanf(inputFile, "%d", &buffer[count]) == 1) {
        count++;
        if (count == CHUNK_SIZE) {
            writeSortedChunk(buffer, count, &tempFileCount);
            count = 0;
        }
    }

    if (count > 0) {
        writeSortedChunk(buffer, count, &tempFileCount);
    }

    free(buffer);
    fclose(inputFile);

    FILE **tempFiles = (FILE **)malloc(tempFileCount * sizeof(FILE *));
    for (int i = 0; i < tempFileCount; i++) {
        snprintf(tempFileName, sizeof(tempFileName), "temp%d.txt", i);
        tempFiles[i] = fopen(tempFileName, "r");
    }

    FILE *outputFile = fopen("sorted_output.txt", "w");
    mergeSortedFiles(tempFiles, tempFileCount, outputFile);

    // Close and remove the temporary files
    for (int i = 0; i < tempFileCount; i++) {
        fclose(tempFiles[i]);
        snprintf(tempFileName, sizeof(tempFileName), "temp%d.txt", i);
        remove(tempFileName);
    }

    free(tempFiles);
    fclose(outputFile);

    end = clock();

    // Calculate the time taken by merge sort
    double time_taken = ((double)end - start) / CLOCKS_PER_SEC;
    printf("Seconds taken by merge sort: %f\n", time_taken);

    return 0;
}

// Step 4: For each pair of sorted files, merge them into a single file until only one file is left
void mergeSortedFiles(FILE **tempFiles, int numFiles, FILE *outputFile) {
    int *buffer = malloc(numFiles * sizeof(int));
    for (int i = 0; i < numFiles; i++) {
        if (fscanf(tempFiles[i], "%d", &buffer[i]) == EOF) {
            buffer[i] = INT_MAX;
        }
    }

    while (1) {
        int min = INT_MAX, minIndex = -1;
        for (int i = 0; i < numFiles; i++) {
            if (buffer[i] < min) {
                min = buffer[i];
                minIndex = i;
            }
        }
        if (min == INT_MAX) break;

        fprintf(outputFile, "%d\n", min);
        if (fscanf(tempFiles[minIndex], "%d", &buffer[minIndex]) == EOF) {
            buffer[minIndex] = INT_MAX;
        }
    }

    free(buffer);
}

// Compare function for qsort (can receive any data type)
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Step 2: Write the sorted chunk to a temporary file
void writeSortedChunk(int *buffer, int count, int *tempFileCount) {
    char tempFileName[20];

    // Sort the buffer
    qsort(buffer, count, sizeof(int), compare);
    snprintf(tempFileName, sizeof(tempFileName), "temp%d.txt", (*tempFileCount)++);
    FILE *tempFile = fopen(tempFileName, "w");
    for (int i = 0; i < count; i++) {
        fprintf(tempFile, "%d\n", buffer[i]);
    }
    fclose(tempFile);
}