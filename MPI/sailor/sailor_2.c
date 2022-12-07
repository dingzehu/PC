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

    MPI_Barrier(MPI_COMM_WORLD);

    srand(42 + rank); // initialise rng

    int sailorpos = 25,i;
    int pos[size];

    MPI_Gather(&sailorpos,1,MPI_INT,pos,1,MPI_INT,0,MPI_COMM_WORLD);
    if (rank == 0) {
        printf("# Time  Position\n");
        printf("0");
        for (int i = 0; i < size; i++)
            printf("\t%d", pos[i] + 1);
        printf("\n");
    }

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

        MPI_Gather(&sailorpos,1,MPI_INT,pos,1,MPI_INT,0,MPI_COMM_WORLD);
        if (rank == 0) {
            printf("%d", i + 1);
            for (int j = 0; j < size; j++)
                printf("\t%d", pos[j]);
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}
