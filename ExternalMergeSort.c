#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define CHUNK_SIZE 5000

// Function to merge two sorted files into one
void mergeTwoFiles(FILE *file1, FILE *file2, FILE *outputFile) {
    int val1, val2;
    int read1 = fscanf(file1, "%d", &val1);
    int read2 = fscanf(file2, "%d", &val2);

    while (read1 != EOF && read2 != EOF) {
        if (val1 < val2) {
            fprintf(outputFile, "%d\n", val1);
            read1 = fscanf(file1, "%d", &val1);
        } else {
            fprintf(outputFile, "%d\n", val2);
            read2 = fscanf(file2, "%d", &val2);
        }
    }

    while (read1 != EOF) {
        fprintf(outputFile, "%d\n", val1);
        read1 = fscanf(file1, "%d", &val1);
    }

    while (read2 != EOF) {
        fprintf(outputFile, "%d\n", val2);
        read2 = fscanf(file2, "%d", &val2);
    }
}

void writeSortedChunk(int *buffer, int count, int *tempFileCount);
void mergeSortedFiles(char **tempFileNames, int numFiles, char *outputFileName);
int compare(const void *a, const void *b);

int main() {
    FILE *inputFile = fopen("../inputs/input4.txt", "r");

    if (inputFile == NULL) {
        printf("Error! opening file");
        exit(1);
    }

    int *buffer = (int *)malloc(CHUNK_SIZE * sizeof(int));
    int count = 0, tempFileCount = 0;

    // Start timer of process
    clock_t start, end;

    start = clock();

    // Step 1: Read the file in chunks of CHUNK_SIZE
    // Step 2: Sort the chunk and write it to temporary files
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

    char **tempFileNames = (char **)malloc(tempFileCount * sizeof(char *));
    for (int i = 0; i < tempFileCount; i++) {
        tempFileNames[i] = (char *)malloc(20 * sizeof(char));
        snprintf(tempFileNames[i], 20, "temp%d.txt", i);
    }

    char outputFileName[] = "sorted_output.txt";

    // Step 4: For each pair of sorted files, merge them into a single file until only one file is left
    mergeSortedFiles(tempFileNames, tempFileCount, outputFileName);

    for (int i = 0; i < tempFileCount; i++) {
        free(tempFileNames[i]);
    }
    free(tempFileNames);

    end = clock();

    // Calculate the time taken by merge sort
    double time_taken = ((double)end - start) / CLOCKS_PER_SEC;
    printf("Ms taken by merge sort: %f\n", time_taken);

    return 0;
}

// Step 4: For each pair of sorted files, merge them into a single file until only one file is left
// Function to merge sorted files pair by pair until only one file is left
void mergeSortedFiles(char **tempFileNames, int numFiles, char *outputFileName) {
    if (numFiles == 1) {
        rename(tempFileNames[0], outputFileName);
        return;
    }

    int newNumFiles = (numFiles + 1) / 2;
    char **newTempFileNames = (char **)malloc(newNumFiles * sizeof(char *));
    for (int i = 0; i < newNumFiles; i++) {
        newTempFileNames[i] = (char *)malloc(20 * sizeof(char));
        snprintf(newTempFileNames[i], 20, "temp%d.txt", numFiles + i);
    }

    for (int i = 0; i < numFiles / 2; i++) {
        FILE *file1 = fopen(tempFileNames[2 * i], "r");
        FILE *file2 = fopen(tempFileNames[2 * i + 1], "r");
        FILE *outputFile = fopen(newTempFileNames[i], "w");
        mergeTwoFiles(file1, file2, outputFile);
        fclose(file1);
        fclose(file2);
        fclose(outputFile);
        remove(tempFileNames[2 * i]);
        remove(tempFileNames[2 * i + 1]);
    }

    if (numFiles % 2 == 1) {
        rename(tempFileNames[numFiles - 1], newTempFileNames[newNumFiles - 1]);
    }

    mergeSortedFiles(newTempFileNames, newNumFiles, outputFileName);

    for (int i = 0; i < newNumFiles; i++) {
        free(newTempFileNames[i]);
    }
    free(newTempFileNames);
}

// Compare function for qsort (can receive any data type)
int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

// Step 2: Write the sorted chunk to a temporary file
void writeSortedChunk(int *buffer, int count, int *tempFileCount) {
    char tempFileName[20];

    qsort(buffer, count, sizeof(int), compare);
    snprintf(tempFileName, sizeof(tempFileName), "temp%d.txt", (*tempFileCount)++);
    FILE *tempFile = fopen(tempFileName, "w");
    for (int i = 0; i < count; i++) {
        fprintf(tempFile, "%d\n", buffer[i]);
    }
    fclose(tempFile);
}