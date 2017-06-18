#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ARG_COUNT 3
#define MAX_MATRIX_ITEM 1000

#define MATRIX_SIZE_INITIAL 200
#define MATRIX_SIZE_MAX 2000
#define MATRIX_SIZE_STEP 200

int no_of_threads;

double ** init_matrix(int n);

void populate_matrix_randomly(double ** matrix, int n);

void clean_matrix_memory(double ** metrix, int n);

void print_matrix(char * name, double** matrix, int size);

double ** multiply(double ** matrix_a, double ** matrix_b, int n);

void save_output_to_file(char * file_name, double * data, int no_of_items_in_data);

int main(int argc, char const *argv[])
{
	int sample_size, n, i, j, no_of_experiments;
	double ** matrix_a, ** matrix_b, ** matrix_c;
	double elapsed_time, * all_elapsed_times;
	clock_t start, end;

	if(argc < ARG_COUNT){
		printf("Invalid arguments. Correct form: ./<executable name> <sample_size> <no_of_threads>\n");
		exit(0);
	}

	// Read the matrix size from args
	no_of_experiments = (MATRIX_SIZE_MAX - MATRIX_SIZE_INITIAL)/MATRIX_SIZE_STEP + 1;
	j = 0;
	sscanf(argv[1], "%d", &sample_size);
	sscanf(argv[2], "%d", &no_of_threads);
	all_elapsed_times = (double *)malloc(sizeof(double) * no_of_experiments);

	for(n = MATRIX_SIZE_INITIAL; n <= MATRIX_SIZE_MAX; n += MATRIX_SIZE_STEP){
		printf("Running experiment for matrix size %d\n", n);
		// Intializations
		srand(time(NULL));
		elapsed_time = 0;

		matrix_a =  init_matrix(n);
		matrix_b =  init_matrix(n);
	
		populate_matrix_randomly(matrix_a, n);
		populate_matrix_randomly(matrix_b, n);

		for(i=0; i < sample_size; i++){
			start = clock();
			matrix_c = multiply(matrix_a, matrix_b, n);
			end = clock();

			elapsed_time += (end - start)/(double)CLOCKS_PER_SEC;
		}

		elapsed_time /= sample_size;
		all_elapsed_times[j++] = elapsed_time;

		clean_matrix_memory(matrix_a, n);
		clean_matrix_memory(matrix_b, n);
		clean_matrix_memory(matrix_c, n);

		printf("Experiment for matrix size %d completed\n", n);
		printf("Average time: %.3f\n\n", elapsed_time);

	}

	save_output_to_file("paralllel_output.csv", all_elapsed_times, no_of_experiments);

	return 0;
}

double ** init_matrix(int n){
	int i;
	double ** matrix;

	matrix = (double **) malloc(sizeof(double *) * n);

	for (i = 0; i < n; i++) {
	    matrix[i] = (double *) malloc(sizeof(double) * n);
	}

	return matrix;
}

void clean_matrix_memory(double ** matrix, int n){
	int i;

	for(i=0; i < n; i++){
		free(matrix[i]);
	}

	free(matrix);

}

void populate_matrix_randomly(double ** matrix, int n){
	int i, j;

	for (i=0; i < n; i++){
		for (j=0; j < n; j++){
			matrix[i][j] = (double)rand() / RAND_MAX * MAX_MATRIX_ITEM;
		}
	}
}

void print_matrix(char * name, double** matrix, int size){
	int i, j;

	printf("%s\n", name);

	for (i=0; i < size; i++){
		for (j=0; j < size; j++){
			printf("%.3f\t", matrix[i][j]);
		}
		printf("\n");
	}
}

double ** multiply(double ** matrix_a, double ** matrix_b, int n){
	int i, j, k, sum;
	double ** matrix_c;

	matrix_c = init_matrix(n);

	#pragma omp parallel num_threads(no_of_threads) shared(matrix_a,  matrix_b, matrix_c) private(i, j, k, sum)
	{
		for (i=0; i < n; i++){
			for (j=0; j < n; j++){
				sum = 0;
				for(k=0; k < n;k++){
					sum += matrix_a[i][k] * matrix_b[k][j];
				}
				matrix_c[i][j] = sum;
			}
		}
	}

	return matrix_c;
}

void save_output_to_file(char * file_name, double * data, int no_of_items_in_data){
	FILE *f = fopen(file_name, "w");
	int i;

	if(f == NULL) { 
		perror("Cannot write results to output file"); 
		exit(1); 
	}

	for(i = 0; i < no_of_items_in_data; i++){
		fprintf(f, "%d, %.4f\n", (i+1) * MATRIX_SIZE_STEP, data[i]);  
	}
}