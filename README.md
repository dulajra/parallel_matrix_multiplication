# Matrix Multiplication

This repository contains multiple implemetations for serial and parallel matrix multiplication. 

* Serial version
* Parallel version with openmp
* Optimized parallel version

## Prerequisite
* GCC
* OpenMP

## Instructions
Here we do our calculations such that performance results are within an accuracy of ±5% and 95% confidence level. Therefore, we have to run the `main_program.c` with a higher sample size (say 100) and calculate required sample sizes for each case. Then if the sample sizes we got for each case in the results are lower than the sample size we used for execution (say less than 100) we can guarantee that our performance results are within an accuracy of ±5% and 95% confidence level. If not, we have to run the program again with a higher sample size (say greater than 100).

### Compile
    gcc -o bin/main_program.out main_program.c utils.c -fopenmp -lm

### Run
For all follwing commands,

    n = sample_size
    
Serial version

    ./bin/main_program.out s n

Parallel version

    ./bin/main_program.out p n
    
Optimzie parallel version

    ./bin/main_program.out m n
