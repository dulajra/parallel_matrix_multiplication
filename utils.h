#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "omp.h"

#define MAX_MATRIX_ITEM 1000

#define MATRIX_SIZE_INITIAL 200
#define MATRIX_SIZE_MAX 2000
#define MATRIX_SIZE_STEP 200
#define NO_OF_EXPERIMENTS (MATRIX_SIZE_MAX - MATRIX_SIZE_INITIAL)/MATRIX_SIZE_STEP + 1

double ** init_matrix(int n);

void populate_matrix_randomly(double ** matrix, int n);

void clean_matrix_memory(double ** metrix, int n);

void print_matrix(char * name, double** matrix, int size);

double ** matrix_transpose(double ** matrix, int n);

void save_output_to_file(char * file_name, double ** data, char ** column_names, int rows, int columns);

int min(int a, int b);

double ** multiply_serial(double ** matrix_a, double ** matrix_b, int n);

double ** multiply_parallel(double ** matrix_a, double ** matrix_b, int n);

double ** multiply_parallel_optimized(double ** matrix_a, double ** matrix_b, int n);

double ** run(char type, int sample_size);