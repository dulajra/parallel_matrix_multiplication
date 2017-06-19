#include <stdio.h>
#include <time.h>
#include <math.h>

#include "utils.h"

#define ARG_COUNT 1

double ** multiply(double ** matrix_a, double ** matrix_b, int n);

int no_of_threads;

const int sample_size = 50;
const double confidence = 1.96; // 95%
const int accuracy = 5; // 5%

int main(int argc, char const *argv[])
{
	time_t start_wct = time(NULL);
	int n, i, j;
	double ** matrix_a, ** matrix_b, ** matrix_c;
	double avg_time, variance, standard_deviation, * times_for_each_case, * required_sample_sizes;
	clock_t start, end;

	if(argc < ARG_COUNT){
		printf("Invalid arguments. Correct form: ./<executable name>\n");
		exit(0);
	}

	// Read the matrix size from args
	j = 0;
	required_sample_sizes = (double *)malloc(sizeof(double) * NO_OF_EXPERIMENTS);
	times_for_each_case = (double *)malloc(sizeof(double) * sample_size);

	for(n = MATRIX_SIZE_INITIAL; n <= MATRIX_SIZE_MAX; n += MATRIX_SIZE_STEP){
		printf("Running experiment for matrix size %d\n", n);
		// Intializations
		srand(time(NULL));
		avg_time = 0;
		variance = 0;

		matrix_a =  init_matrix(n);
		matrix_b =  init_matrix(n);
	
		populate_matrix_randomly(matrix_a, n);
		populate_matrix_randomly(matrix_b, n);

		for(i=0; i < sample_size; i++){
			start = clock();
			matrix_c = multiply(matrix_a, matrix_b, n);
			end = clock();

			times_for_each_case[i] = (end - start)/(double)CLOCKS_PER_SEC;
			avg_time += times_for_each_case[i];
		}

		avg_time /= sample_size;

		for (i = 0; i < sample_size; i++){
			variance += pow(times_for_each_case[i] - avg_time, 2);
		}

		standard_deviation = sqrt(variance/(sample_size - 1));

		required_sample_sizes[j++] = pow(100 * confidence * standard_deviation / accuracy /avg_time, 2);
	
		printf("Experiment for matrix size %d completed\n", n);
		printf("Average time: %.4f\n", avg_time);
		printf("Standard deviation time: %.4f\n", standard_deviation);
		printf("Sample size: %.4f\n\n", required_sample_sizes[j-1]);

		clean_matrix_memory(matrix_a, n);
		clean_matrix_memory(matrix_b, n);
		clean_matrix_memory(matrix_c, n);

	}

	save_output_to_file("out/sample_sizes_serial.csv", required_sample_sizes, NO_OF_EXPERIMENTS);

	printf("Total execution time: %.4f\n", (double)(time(NULL) - start_wct));

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