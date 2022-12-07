#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void factorial(int x[], int n);

int main (int argc, const char * argv[])
{
    int arr[8];
    int thread_id;
    factorial(arr, 8);
    
    return 0;
}

void factorial(int x[], int n)
{
    int i,j,thread_id=42;
    
    #pragma omp parallel for num_threads(2) \
        default(none) private(i,j) shared (n,x) schedule(static,1)
    for(i=0; i<n; i++)
    {
       if(i<2)
         x[i] = 1;
       else
       {
	 x[i] = 1;
         for(j=i;j>1;j--)
	    x[i] *=j;
       }
    
       int thread_id = omp_get_thread_num();
       
       // FIXME: thread_id=...
        printf("Thread id # %d computed factorial(%d) = %d \n",thread_id, i, x[i]);
    }
    
    for(j=0; j<n; j++)
        printf("%d\t",x[j]);
    printf("\n");
}


