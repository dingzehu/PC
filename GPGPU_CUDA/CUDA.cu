#include <stdio.h>

__global__ void add_d( int N, float *a, float *b, float *c ) {
  int i = threadIdx.x + blockIdx.x*blockDim.x;
  if (i < N) c[i] = a[i] + b[i];

}

int main(void) {
   int N = 128, i;
   float *a, *b, *c;
   float *a_d, *b_d, *c_d;

   a = (float *)malloc( sizeof(float)*N );
   b = (float *)malloc( sizeof(float)*N );
   c = (float *)malloc( sizeof(float)*N );

   cudaMalloc( &a_d, sizeof(float)*N);
   cudaMalloc( &b_d, sizeof(float)*N);
   cudaMalloc( &c_d, sizeof(float)*N);

   for (i=0; i<N; i++) {
      a[i] = i;
      b[i] = 2*i;
   }

   cudaMemcpy(a_d, a, sizeof(float)*N, cudaMemcpyHostToDevice);
   cudaMemcpy(b_d, b, sizeof(float)*N, cudaMemcpyHostToDevice);
   cudaEvent_t start, stop;
   cudaEventCreate(&start);
   cudaEventCreate(&stop);
   cudaEventRecord(start, 0);
   add_d<<<1,N>>>(N,a_d, b_d, c_d);
   cudaEventRecord(stop, 0);
   cudaEventSynchronize(stop);
   float elapsedTime;
   cudaEventElapsedTime(&elapsedTime, start, stop);

   cudaMemcpy(c, c_d, sizeof(float)*N, cudaMemcpyDeviceToHost);

   for(i=0; i<N; i++) {
        printf( "%f + %f = %f\n", a[i], b[i], c[i] );
  }
   printf("%f\n",elapsedTime);
   cudaFree(a_d); cudaFree(b_d); cudaFree(c_d);
}

