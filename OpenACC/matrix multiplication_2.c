#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <openacc.h>

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
	    B[i][j] = A[i][j];
        }
    }
    int chunks = 8;
    for(int x = 0; x < chunks; x++){
        int sx = x * N / chunks, ex = (x+1) * N / chunks;
        for(int y = 0; y < chunks; y++){
            int sy = y * N / chunks, ey = (y+1) * N / chunks;
            #pragma acc data copyin(A[sy:ey][:N], B[:N][sx:ex]) copyout(C[sy:ey][sx:ex])
            #pragma acc parallel loop async(x*chunks+y)
            for (int i = sy; i < ey; i++)
            {
	        #pragma acc loop
                for (int j = sx; j < ex; j++)
	        {
	            for (int k = 0; k < N; k++)
	            {
		        C[i][j] += A[i][k] * B[k][j];
	            }
	            //printf("%.3lf ",C[i][j]);
	        }
	        //printf("\n");
            }
        }
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%.0lf ",C[i][j]);
        }
        printf("\n");
    }
    return 0;
}
