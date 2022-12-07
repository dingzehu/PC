from mpi4py import MPI
import numpy as np
import sys
import random
import math
import time

num_slave=4
comm = MPI.COMM_SELF.Spawn(sys.executable, args=['e2_c_slave.py'], maxprocs=num_slave)

start_time = time.time()

# number of samples
N = 50000000

x_min, x_max = -1.0, 1.0
y_min, y_max = -1.0, 1.0
z_min, z_max = 0.0, 1.0

x = (x_max - x_min) * np.random.rand(N) + x_min
y = (y_max - y_min) * np.random.rand(N) + y_min
z = (z_max - z_min) * np.random.rand(N) + z_min

# broadcast arrays to slave processes
comm.Bcast([x, MPI.FLOAT], root=MPI.ROOT)
comm.Bcast([y, MPI.FLOAT], root=MPI.ROOT)
comm.Bcast([z, MPI.FLOAT], root=MPI.ROOT)

# reduce calculated value from slave processes
res = np.array(0.0, 'd')
comm.Reduce(None, [res, MPI.DOUBLE],
            op=MPI.SUM, root=MPI.ROOT)
print("Master process received summed result: ", res)
# divide the value by number of slave processes
res /= num_slave

end_time = time.time()

print("estimated integral: %f\nelasped time: %.5f seconds" %(res, end_time-start_time))

comm.Disconnect()
