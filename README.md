# Matrix Multiplication

This repository contains implemetations for serial and parallel matrix multiplication. 

## Compile
    gcc -o bin/sample_size_calculator.out sample_size_calculator.c utils.c -fopenmp -lm

## Run
For serial
    `./bin/sample_size_calculator.out s`

For parallel 
    `./bin/sample_size_calculator.out p`
