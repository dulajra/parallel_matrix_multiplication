#include <stdio.h>
#include <time.h>
#include <math.h>

#include "utils.h"

#define ARG_COUNT 2

double ** multiply(double ** matrix_a, double ** matrix_b, int n);

int no_of_threads;

const int sample_size = 50;
const double confidence = 1.96; // 95%
const int accuracy = 5; // 5%

int main(int argc, char const *argv[])
{	
	double avg_time, variance, standard_deviation, required_sample_size, ** results, ** data;	
	char type;

	if(argc < ARG_COUNT){
		printf("Invalid arguments. Correct form: ./<executable name> <algorithm_type (s or p)>\n");
		exit(1);
	}

	sscanf(argv[1], "%c", &type);

	if(type != 's' && type != 'p'){
		printf("Invalid type paramater. Valid types are s and p. s for serial and p for parallel");
		exit(1);
	}

	results = run(type, sample_size);

	data = (double **)malloc(sizeof(double *) * NO_OF_EXPERIMENTS);
	for (int i = 0; i < 4; i++){
		data[i] = (double *)malloc(sizeof(double) * sample_size);
	}

	for (int n = 0;n < NO_OF_EXPERIMENTS; n++){
		avg_time = 0;

		for (int i = 0; i < sample_size; i++){
			avg_time += results[n][i];	
		}

		avg_time /= sample_size;	

		for (int i = 0; i < sample_size; i++){
			variance += pow(results[n][i] - avg_time, 2);
		}

		standard_deviation = sqrt(variance/(sample_size - 1));
		required_sample_size = pow(100 * confidence * standard_deviation / accuracy /avg_time, 2);

		data[n][0] = n;
		data[n][1] = avg_time;
		data[n][2] = standard_deviation;
		data[n][3] = required_sample_size;

		printf("Matrix size %.0f\n", data[n][0]);
		printf("Average time: %.4f\n", data[n][1]);
		printf("Standard deviation: %.4f\n", data[n][2]);
		printf("Sample size: %.4f\n\n", data[n][3]);
	}

	switch(type){
		case 's':
			save_output_to_file("out/sample_sizes_serial.csv", data, NO_OF_EXPERIMENTS);
			break;
		case 'p':
			save_output_to_file("out/sample_sizes_parallel.csv", data, NO_OF_EXPERIMENTS);	
			break;

	}

	return 0;
}