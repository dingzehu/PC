#include "stdio.h"
#include "stdlib.h"
#include "omp.h"

//Execute as: ./q2 6 (command line param assigns size of two square matrices to be multiplied)
int main(int argc, char *argv[])
{
    long ** m1, ** m2, ** prod;
    int i,j,k,r1,c1,r2,c2, result=0, thread_id, number_of_threads;
    r1 = c1 = r2 = c2 = atoi(argv[1]); //Input from command line argument. Restricting to square matrices.
    double start,end,thread_start,thread_end;
    int n,z,threads[4096],a;
    double elapsed, time[4096];
    /*
    printf("Enter number of rows and columns of first matrix\n");
    scanf("%d%d",&r1,&c1);
    printf("Enter number of rows and columns of second matrix\n");
    scanf("%d%d",&r2,&c2);
    */
    omp_set_nested(1);

    if(r2==c1)
    {
        //Allocating memory
        m1 = (long **) malloc(r1 * sizeof(long *));
        for(i = 0; i < r1; i++)
        {
            m1[i] = (long *) malloc(c1 * sizeof(long));
        }

        m2 = (long **) malloc(r2 * sizeof(long *));
        for(i = 0; i < r2; i++)
        {
            m2[i] = (long *) malloc(c2 * sizeof(long));
        }

        prod = (long **) malloc(r1 * sizeof(long *));
        for(i = 0; i < r1; i++)
        {
            prod[i] = (long *) malloc(c2 * sizeof(long));
        }

        //Populate the matrices & print out to see it is correctly allocated memory.
        printf("Populating the matrices ...\n");
        printf("Matrix 1:\n");
        for(i=0; i<r1; i++)
        {
            for(j=0; j<c1; j++){
                m1[i][j] = j+1;
                printf("%lu\t",m1[i][j]);
            }
            printf("\n");
        }
        printf("Matrix 2:\n");
        for(i=0; i<r2; i++)
        {
            for(j=0; j<c2; j++){
                m2[i][j] = j+2;
                printf("%lu\t",m2[i][j]);
            }
            printf("\n");
        }
        printf("Matrices populated, proceeding to multiply ...\n");

        z=0;
        for (number_of_threads=1;number_of_threads<17;number_of_threads++)
        {
            start=omp_get_wtime();
            thread_start=omp_get_wtime();
            #pragma omp parallel for private(i,j,k) shared(r1,c2,c1,m1,m2,prod,thread_id) num_threads(number_of_threads)
            for(i=0;i<r1;i++)
            {

                number_of_threads=omp_get_num_threads();
                thread_id=omp_get_thread_num();
                //#pragma omp parallel for num_threads(number_of_threads)
                for(j=0;j<c2;j++)
                {
                    prod[i][j] = 0;
                    for(k=0;k<c1;k++)
                        prod[i][j] += m1[i][k]*m2[k][j];

                // FIXME: How to get the right thread-ID?
                // thread_id=...
                // number_of_threads=...
                    printf("Thread#%d of %d computed prod[%d][%d]=%lu\t\n",thread_id,number_of_threads,i,j,prod[i][j]);
                }
                printf("\n");
                thread_end=omp_get_wtime();
                printf("Thread time taken = %f seconds\n\n",thread_end-thread_start);
                threads[z]=number_of_threads;
            }
            end=omp_get_wtime();
            elapsed=(end-start);
            time[number_of_threads]=elapsed;
            printf("Total time taken = %f seconds\n",end-start);
            printf("\n%d\n", z);
            z++;
        }

        FILE *fpt;

        fpt=fopen("data.csv", "w+");

        fprintf(fpt,"time, number_of_threads\n");
        for (a=0;a<z;a++)
        {
            fprintf(fpt,"%f, %d\n", time[a+1], threads[a]);
        }
        fclose(fpt);


/* FIXME: add a timing routing that might look like:
        end = 
        elapsed = (end - start) ;
        printf("Time Taken =%f sec\n",elapsed);
*/

        //Print the product
        printf("\nMultiplied Matrix:\n");
        for(i=0; i<r1; i++)
        {
            for(j=0; j<c2; j++){
                printf("%lu\t",prod[i][j]);
            }
            printf("\n");
        }

        //Free memory
        for(i=0;i<r1;i++) {
            free(m1[i]);
        }
        free(m1);
        for(i=0;i<r2; i++) {
            free(m2[i]);
        }
        free(m2);
        for(i=0;i<r1; i++) {
            free(prod[i]);
        }
        free(prod);

    }
    else
    {
        printf("Matrix multiplication cannot be done");
    }

    return 0;
}

