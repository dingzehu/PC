#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    const int K = 1024;
    const int msgsize = 256*K;  /* Messages will be of size 1 MB */
    int *X, *Y;   
    int me, i;
    MPI_Init(&argc, &argv);

    int size;
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(size != 2) {
       printf("You have to use 2 exactly processors for this program. \n", size);
       MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    int buffer_send = 1024;
    int buffer_recv;
    int send_tag = 0;
    int recv_tag = 0;
    int partner = (my_rank == 0) ? 1 : 0;
    
    X = (int *) malloc(msgsize*sizeof(int));
    Y = (int *) malloc(msgsize*sizeof(int));


    /* Initialize X and Y */
    for (i=0; i<msgsize; i++) {
        X[i] = 12345;
        Y[i] = me;
    }
        
    printf("Process %d is sending value %d to %d process\n", my_rank, X, partner);
    MPI_Sendrecv(X, msgsize, MPI_INT, partner, send_tag, X, msgsize, MPI_INT, partner, recv_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Process %d is receiving value %d from %d process\n", my_rank, X, partner);

    MPI_Finalize();

    exit(0);
}     
