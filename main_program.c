#include <stdio.h>
#include <math.h>

#include "utils.h"

#define ARG_COUNT 2

double ** multiply(double ** matrix_a, double ** matrix_b, int n);

int sample_size;

const double confidence = 1.96; // 95%
const int accuracy = 5; // 5%
char * column_names[] = {"Matrix size", "Average Time", "SD", "No of samples"};

int main(int argc, char const *argv[])
{	
	double avg_time, variance, standard_deviation, required_sample_size, ** results, ** data;	
	char type;

	// Validate the inputs
	if(argc < ARG_COUNT){
		printf("Invalid arguments. Correct form: ./<executable name> <algorithm_type (s or p or m)> <no_of_samples>\n");
		exit(1);
	}

	sscanf(argv[1], "%c", &type);
	sscanf(argv[2], "%d", &sample_size);

	if(type != 's' && type != 'p' && type != 'm'){
		printf("Invalid type paramater. Valid types are s, p and m. s for serial, p for parallel and m for optimized\n");
		exit(1);
	}

	if(sample_size < 1){
		printf("Invalid sample_size. Sample size should be an integer greater than 0");
		exit(1);
	}

	// Start the experiment with given algorithm type and number of samples. 
	results = run(type, sample_size);

	// Allcate memeory to store calculation results. 
	data = (double **)malloc(sizeof(double *) * NO_OF_EXPERIMENTS);
	for (int i = 0; i < NO_OF_EXPERIMENTS; i++){
		data[i] = (double *)malloc(sizeof(double) * 4);
	}

	// Calcuate mean, standard deviation and required minimum sample size for each matriz size. 
	for (int n = 0;n < NO_OF_EXPERIMENTS; n++){
		avg_time = 0;

		for (int i = 0; i < sample_size; i++){
			avg_time += results[n][i];	
		}

		avg_time /= sample_size;	

		for (int i = 0; i < sample_size; i++){
			variance += pow(results[n][i] - avg_time, 2);
		}

		standard_deviation = sqrt(variance/(sample_size - 1)); // Here we divide by (sample_size - 1) as we find the standard deviation of the sample. Not of the population. 
		required_sample_size = pow(100 * confidence * standard_deviation / accuracy /avg_time, 2);

		data[n][0] = (n+1) * MATRIX_SIZE_STEP;
		data[n][1] = avg_time;
		data[n][2] = standard_deviation;
		data[n][3] = required_sample_size;

		printf("Matrix size %.0f\n", data[n][0]);
		printf("Average time: %.4f\n", data[n][1]);
		printf("Standard deviation: %.4f\n", data[n][2]);
		printf("Sample size: %.4f\n\n", data[n][3]);
	}

	// Write final results to a file in CSV format. 
	switch(type){
		case 's':
			save_output_to_file("out/sample_sizes_serial.csv", data, column_names, NO_OF_EXPERIMENTS, 4);
			break;
		case 'p':
			save_output_to_file("out/sample_sizes_parallel.csv", data, column_names, NO_OF_EXPERIMENTS, 4);	
			break;
		case 'm':
			save_output_to_file("out/sample_sizes_parallel_optimized.csv", data, column_names, NO_OF_EXPERIMENTS, 4);	
			break;

	}

	return 0;
}