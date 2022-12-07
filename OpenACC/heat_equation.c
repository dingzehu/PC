#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <openacc.h>

double fmax(double a, double b) {
    if (a > b)
       return a;
    else
       return b;
}

int main(void)
{
    int N = 512; 
    double A[N][N];
    double Anew[N][N];
    double pi = 3.1415;
    double err = 2.0, thre = 0.01;
    int iter = 0, iter_max = 10000000;

    #pragma acc parallel loop collapse(2)
    for (int j = 0; j < N; j++)
    {
        for (int i = 0; i < N; i ++)
        {
            A[j][i] = cos((4*pi*i)/(N-1))*cos((4*pi*j)/(N-1));
        }
    }

    #pragma acc data copy(A), create(Anew)
    while ( err > thre && iter < iter_max)
    {
        err=0.0;  

        #pragma acc parallel loop collapse(2)
        for( int j = 1; j < N-1; j++)
        {
            for(int i = 1; i < N-1; i++)
            {
                Anew[j][i] = 0.25 * (A[j][i+1] + A[j][i-1] + A[j-1][i] + A[j+1][i]);
            }
        }

        #pragma acc parallel loop reduction(max:err)
        for( int j = 1; j < N-1; j++)
        {
            for( int i= 1; i < N-1; i++ ) 
            {
		err = fmax(err, abs(Anew[j][i] - A[j][i]));
                A[j][i] = Anew[j][i];
            }
        } 
        iter++;
    }

    #pragma acc exit data copyout(A)
    for( int j = 1; j < N-1; j++)
        {
            for( int i= 1; i < N-1; i++ )
            {
                printf("%f\t", A[j][i]);
            }
	printf("\n");
        }
    
    FILE *fp;
    fp = fopen("output.txt", "w");
    
    for( int j = 1; j < N-1; j++)
        {
            for( int i= 1; i < N-1; i++ )
            {
                fprintf(fp, "%d\t%d\t%f\n", j, i, A[j][i]);
            }
        }

    fclose(fp);
    
return 0;
}
