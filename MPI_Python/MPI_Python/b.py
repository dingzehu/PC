from mpi4py import MPI
import numpy as np
comm = MPI.COMM_WORLD
rank = comm.Get_rank()

rounds = 100
for i in range(1,8):
    data = np.empty(10**i)
    tStart = MPI.Wtime()
    for x in range(rounds):
        if rank == 0:
            comm.send(data,dest=1,tag=11)
        elif rank == 1:
            comm.recv(source=0,tag=11)
    if rank == 0:
        print(i,(MPI.Wtime()-tStart)/rounds)
