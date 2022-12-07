#include <stdio.h> // for printf
#include <stdlib.h> // for rand
#include <mpi.h>

// create random number between min and max (inclusive)
int rand_nr(int min, int max)
{
    int nr = (rand()/(RAND_MAX + 1.0))*(max+1-min) + min;
    return nr;
}

int main()
{
    int size, rank;

    /* Starts MPI processes ... */

    MPI_Init(NULL, NULL);                 /* starts MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   /* get current process id */
    MPI_Comm_size(MPI_COMM_WORLD, &size);     /* get number of processes */

    srand(42 + rank); // initialise rng

    int sailorpos = 25,i,j;
    int pos_arr[51*size];
    pos_arr[0] = sailorpos + 1;

    for (i = 0; i < 50; i++) {
        int units_to_move = rand_nr(1,10);
        int left_or_right = rand_nr(0,1);
        if (left_or_right) {
            sailorpos += units_to_move;
        }
        else {
            sailorpos -= units_to_move;
        }
        // -1 % 51 == -1 so we just always add 51...
        sailorpos = (sailorpos + 51) % 51;

        pos_arr[i + 1] = sailorpos + 1;
    }

    MPI_Gather(pos_arr,51,MPI_INT,pos_arr,51,MPI_INT,0,MPI_COMM_WORLD);
    if(rank == 0) {
        printf("# Time Sailor Position\n");
        for(i = 0; i < size; i++)
            for(j = 0; j < 51; j++)
                printf("%d\t%d\t%d\n", j, i, pos_arr[i*51+j]);
    }

    MPI_Finalize();
    return 0;
}
