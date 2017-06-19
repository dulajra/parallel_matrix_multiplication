#include <stdio.h>

#include "utils.h"

#define ARG_COUNT 3

double ** multiply(double ** matrix_a, double ** matrix_b, int n);

char * column_names[] = {"Matrix size", "Average Time"};

int main(int argc, char const *argv[])
{	
	int sample_size;
	double avg_time, variance, standard_deviation, required_sample_size, ** results, ** data;	
	char type;

	if(argc < ARG_COUNT){
		printf("Invalid arguments. Correct form: ./<executable name> <algorithm_type (s or p)> <no_of_samples>\n");
		exit(1);
	}

	sscanf(argv[1], "%c", &type);
	sscanf(argv[2], "%d", &sample_size);

	if(type != 's' && type != 'p'){
		printf("Invalid type paramater. Valid types are s and p. s for serial and p for parallel");
		exit(1);
	}

	results = run(type, sample_size);

	data = (double **)malloc(sizeof(double *) * NO_OF_EXPERIMENTS);
	for (int i = 0; i < 2; i++){
		data[i] = (double *)malloc(sizeof(double) * sample_size);
	}

	for (int n = 0;n < NO_OF_EXPERIMENTS; n++){
		avg_time = 0;

		for (int i = 0; i < sample_size; i++){
			avg_time += results[n][i];	
		}

		avg_time /= sample_size;	

		data[n][0] = (n+1) * MATRIX_SIZE_STEP;
		data[n][1] = avg_time;

		printf("Matrix size %.0f\n", data[n][0]);
		printf("Average time: %.4f\n\n", data[n][1]);
	}

	switch(type){
		case 's':
			save_output_to_file("out/matrix_multiplication_serial.csv", data, column_names, NO_OF_EXPERIMENTS, 2);
			break;
		case 'p':
			save_output_to_file("out/matrix_multiplication_parallel.csv", data, column_names, NO_OF_EXPERIMENTS, 2);	
			break;

	}

	return 0;
}