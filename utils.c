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

void save_output_to_file(char * file_name, double ** data, char ** column_names, int rows, int columns){
	FILE *f = fopen(file_name, "w");

	if(f == NULL) { 
		perror("Cannot write results to output file"); 
		exit(1); 
	}

	for (int i = 0; i < columns ; ++i){
		fprintf(f, "%s, ", column_names[i]);  
	}

	fprintf(f, "\n");  

	for(int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			fprintf(f, "%.4f, ", data[i][j]);
		}
		fprintf(f, "\n");  
	}

	fclose(f);
}

double ** multiply_serial(double ** matrix_a, double ** matrix_b, int n){
	int i, j, k;
	double ** matrix_c;
	double sum;

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
	double sum;

	matrix_c = init_matrix(n);

	#pragma omp parallel for shared(matrix_a,  matrix_b, matrix_c) private(i, j, k, sum)
	for (i=0; i < n; i++){
		for (j=0; j < n; j++){
			sum = 0;
			for(k=0; k < n; k++){
				sum += matrix_a[i][k] * matrix_b[k][j];
			}
			matrix_c[i][j] = sum;
		}
	}

	return matrix_c;
}

int min(int a, int b) {
	return a < b ? a: b;
}

double ** multiply_parallel_optimized(double ** matrix_a, double ** matrix_b, int n){
	int i0, j0, k0, i, j, k, thread_count, step;
	double ** matrix_c;
	double sum, temp;

	matrix_c = init_matrix(n);

	thread_count = 20;
	step = n/thread_count;

	/*
	reference: http://www.netlib.org/utk/papers/autoblock/node2.html
	outermost for loop has always 8 iterations, 1 per thread. Therefore i,j combination for any
	thead will be unique making multiple writes possible 
	*/
	#pragma omp parallel for shared(matrix_a,  matrix_b, matrix_c) private(i, j, k, sum) schedule(static) num_threads(thread_count)
	for (i0 = 0; i0 < n; i0 = i0 + step) {
		for (j0 = 0; j0 < n; j0 = j0 + step) {
			for (k0 = 0; k0 < n; k0 = k0 + step) {
				for (i=i0; i < min(i0+step,n); i++){
					for (j=j0; j < min(j0+step,n); j++){
						sum = 0;
						for(k=k0; k < min(k0+step,n); k++){
							sum += matrix_a[i][k] * matrix_b[k][j];
						}
						matrix_c[i][j] += sum;
					}
				}
			}	
		}	
	}
	
	return matrix_c;
}

double ** matrix_transpose(double ** matrix, int n) {
	int i, j;
	double temp;
	for (int i = 0; i < n; i++){
        for (int j = 0; j < i + 1; j++){
            temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

double ** run(char type, int sample_size){
	int thread_count;
	double ** matrix_a, ** matrix_b, ** matrix_c, ** results;
	// clock_t start, end;
	struct timeval start, end;

	results = (double **)malloc(sizeof(double *) * NO_OF_EXPERIMENTS);
	for (int i = 0; i < NO_OF_EXPERIMENTS; i++){
		results[i] = (double *)malloc(sizeof(double) * sample_size);
	}

	for(int n = MATRIX_SIZE_INITIAL; n <= MATRIX_SIZE_MAX; n += MATRIX_SIZE_STEP){
		// Intializations
		srand(time(NULL));
		switch(type){
			case 's':
				for(int i=0; i < sample_size; i++){
					matrix_a =  init_matrix(n);
					matrix_b =  init_matrix(n);
				
					populate_matrix_randomly(matrix_a, n);
					populate_matrix_randomly(matrix_b, n);

					// start = clock();
					gettimeofday(&start, NULL);
					matrix_c = multiply_serial(matrix_a, matrix_b, n);
					// end = clock();
					gettimeofday(&end, NULL);

					// results[n/200 -1][i] = (end - start)/(double)CLOCKS_PER_SEC;
					results[n/200 -1][i] = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)1000000; // seconds with ms accuracy

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

					// start = clock();
					gettimeofday(&start, NULL);
					matrix_c = multiply_parallel(matrix_a, matrix_b, n);
					// end = clock();
					gettimeofday(&end, NULL);

					// results[n/200 -1][i] = (end - start)/(double)CLOCKS_PER_SEC;
					results[n/200 -1][i] = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)1000000; // seconds with ms accuracy

					clean_matrix_memory(matrix_a, n);
					clean_matrix_memory(matrix_b, n);
					clean_matrix_memory(matrix_c, n);
				}

				break;

			case 'm':
				// Get optimal thread count
				#pragma omp parallel
				thread_count = omp_get_num_threads();

				for(int i=0; i < sample_size; i++){
					matrix_a =  init_matrix(n);
					matrix_b =  init_matrix(n);
				
					populate_matrix_randomly(matrix_a, n);
					populate_matrix_randomly(matrix_b, n);

					// start = clock();
					gettimeofday(&start, NULL);
					matrix_c = multiply_parallel_optimized(matrix_a, matrix_b, n);
					// end = clock();
					gettimeofday(&end, NULL);

					// results[n/200 -1][i] = (end - start)/(double)CLOCKS_PER_SEC;
					results[n/200 -1][i] = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)1000000; // seconds with ms accuracy

					clean_matrix_memory(matrix_a, n);
					clean_matrix_memory(matrix_b, n);
					clean_matrix_memory(matrix_c, n);
				}

				break;

		}

		printf("Experiment for matrix size %d completed\n\n", n);
	}

	return results;
}

