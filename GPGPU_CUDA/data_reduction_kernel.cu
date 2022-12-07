#include <stdio.h>

static const int blockSize = 1024;

__global__ void sum_d( int N, float *arr ) {
  int idx = threadIdx.x;
  int sum = 0;
  for (int i = idx; i < N; i += blockSize)
    sum += arr[i];
  arr[idx] = sum;
  __syncthreads();
  for (int size = blockSize/2; size>0; size/=2) {
    if (idx<size) arr[idx] += arr[idx+size];
    __syncthreads();
  }
}

int main(void) {
  int N = 1024*1024, i;
  float arr[N], sum = 0;
  float *arr_d;

   cudaMalloc( &arr_d, sizeof(float)*N);

   for (i=0; i<N; i++) {
      arr[i] = i;
      sum += i;
   }
   
   cudaMemcpy(arr_d, arr, sizeof(float)*N, cudaMemcpyHostToDevice);

   sum_d<<<1,blockSize>>>(N,arr_d);

   cudaMemcpy(arr, arr_d , sizeof(float), cudaMemcpyDeviceToHost);

   printf( "sum = %f\n", sum );
   printf( "sum from d = %f\n", arr[0] );

   cudaFree(arr_d);
}

