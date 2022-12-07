#!/bin/bash

#SBATCH -o sudoku.out
#SBATCH -p medium
#SBATCH -N 1
#SBATCH -n 1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=20
#SBATCH -t 20:00
#SBATCH -C scratch

module load openmpi

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

mpicc -fopenmp -o sudoku sudoku.c -lm

mpirun -np 1 sudoku                    
