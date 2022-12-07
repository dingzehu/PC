from mpi4py import MPI
import numpy as np
import math
import random

comm = MPI.Comm.Get_parent()
size = comm.Get_size()
rank = comm.Get_rank()

N = 50000000

x = np.empty(N, dtype=np.float)
y = np.empty(N, dtype=np.float)
z = np.empty(N, dtype=np.float)

# receive arrays from master process
comm.Bcast([x, MPI.FLOAT], root=0)
comm.Bcast([y, MPI.FLOAT], root=0)
comm.Bcast([z, MPI.FLOAT], root=0)

x_arr_spl = np.random.choice(x, int(N/size-1), replace=False)
y_arr_spl = np.random.choice(y, int(N/size-1), replace=False)
z_arr_spl = np.random.choice(z, int(N/size-1), replace=False)

x_min, x_max = -1.0, 1.0
y_min, y_max = -1.0, 1.0
z_min, z_max = 0.0, 1.0

res = np.exp(-0.5 * (x_arr_spl*x_arr_spl + y_arr_spl*y_arr_spl))
count = np.where(z_arr_spl < res)
count = count[0].shape[0]

ratio = count / (N/size-1)
volume = (x_max-x_min)*(y_max-y_min)*(z_max-z_min)
value = ratio * volume
print("slave process %d estimates: %f" % (rank, value))
# reduce calculated value to master process
res = np.array(value, dtype='d')
comm.Reduce([res, MPI.DOUBLE], None, op=MPI.SUM, root=0)

comm.Disconnect()

