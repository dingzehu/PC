#!/bin/bash

#SBATCH -o sudoku_icc.out
#SBATCH -p medium
#SBATCH -N 2
#SBATCH -n 2
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=20
#SBATCH -t 20:00
#SBATCH -C scratch

module load intel
module load intel-mpi

export LC_CTYPE=en_US.UTF-8
export LC_ALL=en_US.UTF-8

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK


mpiicc -fopenmp -g -Wall -o sudoku_mpiicc sudoku.c -lm -std=c99

mpirun -n 2 ./sudoku_mpiicc                           
