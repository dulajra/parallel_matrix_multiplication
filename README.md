# Matrix Multiplication

This repository contains multiple implemetations for serial and parallel matrix multiplication. 

* Serial version
* Parallel version with openmp
* Optimized parallel version

## Prerequisite
* GCC
* OpenMP

## Instructions
Here we do our calculations such that performance results are within an accuracy of ±5% and 95% confidence level. Therefore, we have to run the `sample_size_calculator.c` with a higher sample size and calculate required sample sizes for each case. At the beginning of the `sample_size_calculator.c` we have hard coded `sample_size` to 100. Then we can decide the required sample size by looking at the results. 

Now we can tun `matrix_multiplication.c` to calculate average execution times. 

### Compile
    gcc -o bin/sample_size_calculator.out sample_size_calculator.c utils.c -fopenmp -lm
    gcc -o bin/matrix_multiplication.out matrix_multiplication.c utils.c -fopenmp

### Run
Serial version

    ./bin/sample_size_calculator.out s
    ./bin/matrix_multiplication.out s 10

Parallel version

    ./bin/sample_size_calculator.out p
    ./bin/matrix_multiplication.out p 10
    
Optimzie parallel version

    ./bin/sample_size_calculator.out op
    ./bin/matrix_multiplication.out op 10
