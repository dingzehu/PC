#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, next, prev, number;
    int tag = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;
    
    if (0 == rank) {
	number = 0;

	printf("Process 0 sends value %d to %d\n", number, next);
	MPI_Send(&number, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
    }

    while (1) {

	MPI_Recv(&number, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
	number = number + 1;

	MPI_Send(&number, 1, MPI_INT, next, tag, MPI_COMM_WORLD);
	printf("Process %d sends incremented value %d to %d\n", rank, number, next);
	if (number == 10) {
	    printf("Process %d exiting\n", rank);
	}
	break;
    }

    if (0 == rank) {
	MPI_Recv(&number, 1, MPI_INT, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}

