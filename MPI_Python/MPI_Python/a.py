from mpi4py import MPI
comm = MPI.COMM_WORLD
rank = comm.Get_rank()

class Msg:
    def __init__(self):
        self.i = 0
    def set(self, i):
        self.i = i
    def print(self):
        return self.i

other = (rank + 1) % 2
rounds = 100000

tStart = MPI.Wtime()
if rank == 0:
    comm.send(Msg(),dest=other,tag=11)
for i in range(rounds - 1):
    x = comm.recv(source=other,tag=11)
    #print(rank, x.print())
    x.set(rank)
    comm.send(x,dest=other,tag=11)
if rank == 1:
    x = comm.recv(source=other,tag=11)
    #print(x.print())
print(MPI.Wtime() - tStart)

