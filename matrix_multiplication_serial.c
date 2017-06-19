#include <stdio.h>
#include <time.h>

#include "utils.h"

#define ARG_COUNT 2

double ** multiply(double ** matrix_a, double ** matrix_b, int n);

int main(int argc, char const *argv[])
{
	int sample_size,n, i, j;
	double elapsed_time, * all_elapsed_times;
	clock_t start, end;
	double ** matrix_a, ** matrix_b, ** matrix_c;

	if(argc < ARG_COUNT){
		printf("Invalid arguments. Correct form: ./<executable name> <sample_size>\n");
		exit(0);
	}

	// Read the matrix size from args
	j = 0;
	sscanf(argv[1], "%d", &sample_size);
	all_elapsed_times = (double *)malloc(sizeof(double) * NO_OF_EXPERIMENTS);

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


		printf("Experiment for matrix size %d completed\n", n);
		printf("Average time: %.4f\n\n", elapsed_time);

		clean_matrix_memory(matrix_a, n);
		clean_matrix_memory(matrix_b, n);
		clean_matrix_memory(matrix_c, n);
	}

	printf("All experiments completed\n");
	save_output_to_file("out/serial_output.csv", all_elapsed_times, NO_OF_EXPERIMENTS);
	printf("Time statistics were saved to serial_output.csv\n");

	return 0;
}

double ** multiply(double ** matrix_a, double ** matrix_b, int n){
	int i, j, k, sum;
	double ** matrix_c;

	matrix_c = init_matrix(n);

	for (i=0; i < n; i++){
		for (j=0; j < n; j++){
			sum = 0;
			for(k=0; k < n;k++){
				sum += matrix_a[i][k] * matrix_b[k][j];
			}
			matrix_c[i][j] = sum;
		}
	}

	return matrix_c;
}