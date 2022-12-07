from mpi4py import MPI
import math
import time
import numpy as np
import random

# MPI initialization
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()


# number of samples
N = 50000000

start_time = time.time()

x_min, x_max = -1.0, 1.0
y_min, y_max = -1.0, 1.0
z_min, z_max = 0.0, 1.0

if rank == 0:

    x = (x_max - x_min) * np.random.rand(N) + x_min
    y = (y_max - y_min) * np.random.rand(N) + y_min
    z = (z_max - z_min) * np.random.rand(N) + z_min
    
    for i in range(1, size):
        comm.Send(x, dest=i, tag=0)
        comm.Send(y, dest=i, tag=1)
        comm.Send(z, dest=i, tag=2)

    part_value = []
    for i in range(1, size):
        part_value.append(comm.recv(source=i, tag=11))
    res = np.array(np.mean(part_value))

    end_time = time.time()

    print("estimate the integral: %f\nupper case Send/Recv---elasped time: %.5f seconds " %(res, end_time-start_time))

else:
    # arrays initialization
    print("This is rank %d" %rank)
    x = np.empty(N, dtype=np.float)
    y = np.empty(N, dtype=np.float)
    z = np.empty(N, dtype=np.float)
    
    comm.Recv(x, source=0, tag=0)
    comm.Recv(y, source=0, tag=1)
    comm.Recv(z, source=0, tag=2)
    
    x_arr_spl = np.random.choice(x, int(N/size-1), replace=False)
    y_arr_spl = np.random.choice(y, int(N/size-1), replace=False)
    z_arr_spl = np.random.choice(z, int(N/size-1), replace=False)
    
    res = np.exp(-0.5 * (x_arr_spl*x_arr_spl + y_arr_spl*y_arr_spl))
    count = np.where(z_arr_spl < res)
    count = count[0].shape[0]
    
    ratio = count / (N/size-1)
    volume = (x_max-x_min)*(y_max-y_min)*(z_max-z_min) 
    value = ratio * volume
    
    comm.send(value, dest=0, tag=11)

