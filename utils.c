#include "utils.h"

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

// void save_output_to_file(char * file_name, double * data, int size){
// 	FILE *f = fopen(file_name, "w");
// 	int i;

// 	if(f == NULL) { 
// 		perror("Cannot write results to output file"); 
// 		exit(1); 
// 	}

// 	fprintf(f, "Matrix size, Average Time\n");  

// 	for(i = 0; i < size; i++){
// 		fprintf(f, "%d, %.4f\n", (i+1) * MATRIX_SIZE_STEP, data[i]);  
// 	}

// 	fclose(f);
// }

void save_output_to_file(char * file_name, double ** data, int size){
	FILE *f = fopen(file_name, "w");
	int i, j;

	if(f == NULL) { 
		perror("Cannot write results to output file"); 
		exit(1); 
	}

	fprintf(f, "Matrix size, Average Time, SD, No of samples\n");  

	for(i = 0; i < size; i++){
		fprintf(f, "%.0f, %.4f, %.4f, %.4f\n", data[i][0], data[i][1], data[i][2], data[i][3]);  
	}

	fclose(f);
}

double ** multiply_serial(double ** matrix_a, double ** matrix_b, int n){
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

double ** multiply_parallel(double ** matrix_a, double ** matrix_b, int n){
	int i, j, k;
	double ** matrix_c;

	matrix_c = init_matrix(n);

	#pragma omp parallel for shared(matrix_a,  matrix_b, matrix_c) private(i, j, k)
	for (i=0; i < n; i++){
		for (j=0; j < n; j++){
			for(k=0; k < n;k++){
				matrix_c[i][j] += matrix_a[i][k] * matrix_b[k][j];
			}
		}
	}

	return matrix_c;
}

double ** run(char type, int sample_size){
	double ** matrix_a, ** matrix_b, ** matrix_c, ** results;
	clock_t start, end;

	results = (double **)malloc(sizeof(double *) * NO_OF_EXPERIMENTS);
	for (int i = 0; i < NO_OF_EXPERIMENTS; i++){
		results[i] = (double *)malloc(sizeof(double) * sample_size);
	}

	for(int n = MATRIX_SIZE_INITIAL; n <= MATRIX_SIZE_MAX; n += MATRIX_SIZE_STEP){
		printf("Running experiment for matrix size %d\n", n);
		// Intializations
		srand(time(NULL));

		switch(type){
			case 's':
				for(int i=0; i < sample_size; i++){
					matrix_a =  init_matrix(n);
					matrix_b =  init_matrix(n);
				
					populate_matrix_randomly(matrix_a, n);
					populate_matrix_randomly(matrix_b, n);

					start = clock();
					matrix_c = multiply_serial(matrix_a, matrix_b, n);
					end = clock();

					results[n/200 -1][i] = (end - start)/(double)CLOCKS_PER_SEC;

					clean_matrix_memory(matrix_a, n);
					clean_matrix_memory(matrix_b, n);
					clean_matrix_memory(matrix_c, n);
				}

				break;

			case 'p':
				for(int i=0; i < sample_size; i++){
					matrix_a =  init_matrix(n);
					matrix_b =  init_matrix(n);
				
					populate_matrix_randomly(matrix_a, n);
					populate_matrix_randomly(matrix_b, n);

					start = clock();
					matrix_c = multiply_parallel(matrix_a, matrix_b, n);
					end = clock();

					results[n/200 -1][i] = (end - start)/(double)CLOCKS_PER_SEC;

					clean_matrix_memory(matrix_a, n);
					clean_matrix_memory(matrix_b, n);
					clean_matrix_memory(matrix_c, n);
				}

				break;

		}

		printf("Completed\n\n");
	}

	return results;
}