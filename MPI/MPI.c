/*
A simple MPI example program using standard mode send and receive

The program consists of two processes. Process 0 sends a large message
to the receiver. This receives the message and sends it back.

Run it on two processes.
*/

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
  const int K = 1024;
  const int msgsize = 256*K;  /* Messages will be of size 1 MB */
  //char hostname[MPI_MAX_PROCESSOR_NAME];
  int np, me, i;
  int *X, *Y;
  int *Z;
  int tag = 42;
  MPI_Status  status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &np);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);

  /* Check that we run on exactly two processes */
  if (np != 2) {
    if (me == 0) {
      printf("You have to use exactly 2 processes to run this program\n");
    }
    MPI_Finalize();	       /* Quit if there is only one process */
    exit(0);
  }
  
  /* Allocate memory for large message buffers */
  X = (int *) malloc(msgsize*sizeof(int));
  Y = (int *) malloc(msgsize*sizeof(int));

  /* Initialize X and Y */
  //for (i=0; i<msgsize; i++) {
    //X[i] = 12345;
    //Y[i] = me;
  //}
  
  if (me == 0) {

    printf("Message size is %d bytes\n", msgsize*sizeof(int));
    
    for (i=0; i<msgsize; i++) {
        X[i] = 12345;
        Y[i] = me;
    }

    printf("Process %d sending to process 1\n", me);
    MPI_Send(X, msgsize, MPI_INT, 1, 10, MPI_COMM_WORLD);
    
    MPI_Recv(X, msgsize, MPI_INT, 1, 20, MPI_COMM_WORLD, &status);
    printf("Send back: Process %d receiving %d from process 1\n", me, X);
  } else { /* me == 1 */

    printf("Process %d receiving from process 0\n", me);
    MPI_Recv(X, msgsize, MPI_INT, 0, 10, MPI_COMM_WORLD, &status);
    printf("Process %d now has the value %d\n", me, X);
    
    //Z = Y[0];
    printf("Send back\n");
    MPI_Send(X, msgsize, MPI_INT, 0, 20, MPI_COMM_WORLD);
    printf("Send back: Process %d sending %d to process 0\n",me, X);
  }
  
  MPI_Finalize();
  exit(0);
}
