#!/bin/sh
#SBATCH -t 10:00
#SBATCH -N 1
#SBATCH -n 5

module load openmpi
module load python

#mpirun -n 4 python3 -m mpi4py e2_a.py
#mpirun -n 4 python3 -m mpi4py e2_b.py
mpirun -n 1 python3 -m mpi4py e2_c_master.py



