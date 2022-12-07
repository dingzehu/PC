#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <openacc.h>
#include <cufft.h>

int main()
{
    int N = 1024;
    cufftReal A[N];
    cufftComplex B[N/2+1];
   
    FILE *fp;
    fp = fopen("output_fft.txt", "w");
    
    #pragma acc data copyout(A[:N]) create(B[:N/2+1])
    {

    #pragma acc parallel loop
    for (int i = 0; i < N; i++)
    {
	A[i] = cos(2*M_PI * 3*i/N) + .2 * sin(2*M_PI * 100*i/N);
        printf("%f",A[i]);
    }
    
    #pragma acc update self(A[:N])
    for(int i = 0; i < N; i++) fprintf(fp,"%f\t", A[i]);
        
    cufftHandle plan, plan2;
    cufftPlan1d(&plan, N, CUFFT_R2C, 1);
    cufftPlan1d(&plan2, N/2+1, CUFFT_C2R, 1);

    cudaStream_t accStream = (cudaStream_t) acc_get_cuda_stream(acc_async_sync);
    cufftSetStream(plan, accStream);
    cufftSetStream(plan2, accStream);
    
    #pragma acc host_data use_device(A,B)
    cufftExecR2C(plan, A, B);
    for(int i = 10; i < N/2+1; i++){
        B[i].x = 0;
        B[i].y = 0;
    }
    #pragma acc host_data use_device(A,B)
    cufftExecC2R(plan2, B, A);

    cufftDestroy(plan);
    cufftDestroy(plan2);
    
    printf("\n\n");
    #pragma acc parallel
    for(int i = 0; i < N; i++) printf("%f", A[i]);
    
    }

    fprintf(fp, "\n");
    for(int i = 0; i < N; i++) fprintf(fp,"%f\t", A[i]);
    fclose(fp);

    return 0;
} 
