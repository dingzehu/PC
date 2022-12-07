#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double myFunc();
double myFuncj();
//Execute as: ./e1 6   (here 6 is command line param for the size of the vectors to be multiplied)
int main (int argc, const char * argv[])
{
  double tmp;

//Snippet#1: What is wrong with the following code? Please fix
//  
  printf("Snippet#1\n");
#pragma omp parallel for ordered num_threads(5)
  for (int i=0; i<20; i++) 
  {
	#pragma omp ordered
	  printf("Thread %d\n",omp_get_thread_num());
	printf("%d\n",i);
  }

//
//Snippet#2: What is wrong with the following code? Please fix
//
  printf("Snippet#2\n");
#pragma omp parallel for ordered num_threads(5)
    // N code lines
    //#pragma omp parallel for ordered
    for (int i = 0; i < 10; i++)
      {
        #pragma omp ordered
	printf("Thread %d\n",omp_get_thread_num());
        tmp=myFunc(i);
	printf("%1f\n",tmp);
      }
  
//
//Snippet#3: What is wrong with the following code? Please fix
//
  printf("Snippet#3\n");
  omp_set_num_threads(3);
#pragma omp parallel
    //omp_set_num_threads(3);
    #pragma omp single
    for (int i = 0; i < 10; i++)
        {	
	printf("Thread %d\n",omp_get_thread_num());
	tmp=myFunc(i);
	printf("%1f\n",tmp);
	}
 
//
//Snippet#4: What is wrong with the following code? Please fix
//
  printf("Snippet#4\n");
  int tmp_reduce;
  int i;
  i=1;
#pragma omp parallel reduction(+:tmp_reduce) num_threads(4)
  {
  //printf("Threads %d\n",omp_get_num_threads());
	  for (int n = 0; n < 4; n++)
		{
    		tmp_reduce = i;
    		//printf("Threads %d\n",omp_get_thread_num());
    		//printf("%d\n",tmp_reduce);
  		}
	  printf("Threads %d\n",omp_get_num_threads());
  }
  printf("%d\n",tmp_reduce);
  //return 0;
}


double myFunc(int i) {
  //int i;
  i++;
  return i;
}

double myFuncj(int j) {
  //int j;
  j++;
  return j;
}
