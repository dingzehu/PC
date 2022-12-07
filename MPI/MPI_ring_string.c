#include <mpi.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[])
{
    int rank, size, next, prev, number;
    int tag = 1;
    
    char foobar[100];
    char bar[] = "bar";

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    next = (rank + 1) % size;
    prev = (rank + size - 1) % size;
    
    if (0 == rank) {
	number = 0;
	char foobar[] = "foo";	

	printf("Process 0 has %s\n", foobar);
	printf("Process 0 sends string %s to %d\n", foobar, next);
	MPI_Send(&foobar, strlen(foobar), MPI_CHAR, next, tag, MPI_COMM_WORLD);
    }

    while (1) {
	MPI_Recv(&foobar, 100, MPI_CHAR, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);	
 	printf("Process %d has %s\n", rank, foobar);
        		
	number = number + 1;
	
	if ((rank % 2) == 0) {
	char ch[] = "foo";
	strcat(foobar, ch);
	MPI_Send(&foobar, strlen(foobar), MPI_CHAR, next, tag, MPI_COMM_WORLD);
	printf("Process %d sends incremented string %s to %d\n", rank, foobar, next);
	}
	if ((rank % 2) == 1) {
	char ch[] = "bar";
	strcat(foobar, ch);
	MPI_Send(&foobar, strlen(foobar), MPI_CHAR, next, tag, MPI_COMM_WORLD);
	printf("Process %d sends incremented string %s to %d\n", rank, foobar, next);
	}
	if (number == 10) {
	    printf("Process %d exiting\n", rank);
	}
	break;
    }

    if (0 == rank) {
	MPI_Recv(&foobar, 100, MPI_CHAR, prev, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Finalize();
    return 0;
}

