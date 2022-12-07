#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()
{
    int N = 512;
    double n = 512;
    double A[N][N];
    double B[N][N];
    double C[N][N];
   
    for (int i = 0; i < N; i++)
    {
	for (int j = 0; j < N; j++)
	{
	    A[i][j] = cos(12*((i+j)/n))*sin(24*((i-j)/n));
	    B[i][j] = cos(12*((i+j)/n))*sin(24*((i-j)/n));
	    C[i][j] = 0.0;
	}
    }
 
    #pragma acc parallel
    {
    	#pragma acc loop
        for (int i = 0; i < N; i++)
        {
	    #pragma acc loop
	    for (int j = 0; j < N; j++)
	    {
	        #pragma acc loop
	        for (int k = 0; k < N; k++)
	        {
		    C[i][j] += A[i][k] * B[k][j];
	        }
	        printf("%.3lf ",C[i][j]);
	    }
	    printf("\n");
        }
    } 
    return 0;
} 
