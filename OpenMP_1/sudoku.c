#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
// Sudoku-Solver
//
//  ------------------------------------------------------
// print the sudoku on screen
//  ------------------------------------------------------
void printsudoku(int n, int m, int field[n][m]) {
  printf("\n");
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      printf("%2d ",field[i][j]);
    }
    printf("\n");
  }
}
//  ------------------------------------------------------
//  search for fitting number in cell i,j
//  ------------------------------------------------------
void testnumber(int n, int m, int field[n][m], int i, int j) {
  // I'm at the row i, column j
  // Test each number k
  // If only one number fits -> success
  // 
  //printf("\n%d\n",omp_get_thread_num());
  int success=1;
  int k_success;
  int k_tobeset;
  int n_numbers=0;
  int mb,nb;
  int ldim = (int)sqrt((double)n);
  //
  //#pragma omp parallel for schedule(dynamic) num_threads(9)
  for (int k=1; k <n+1; k++) {
    //
    printf("\n%d\n",omp_get_thread_num());
    k_success=1;
    for (int l=0; l< n; l++) {
      if (field[i][l] == k) { k_success=0;}
    }
    for (int l=0; l< m; l++) {
      if (field[l][j] == k) { k_success=0;}
    }
    // Check if the number in the region alread exists
    nb =i-i%ldim;
    mb =j-j%ldim;
    for (int l1=nb; l1<nb+ldim;l1++) {
      for (int l2=mb; l2<mb+ldim;l2++) {
	if (field[l1][l2] == k) { k_success=0;}
      }
    }
    if (k_success == 1) {k_tobeset=k;}
    n_numbers = n_numbers + k_success;
  }
  // Success, new number will be added
  #pragma omp critical
  {
  if (n_numbers == 1) {
    field[i][j]=k_tobeset;
    printsudoku(n,m,field);
    }
  }
}
//  ------------------------------------------------------
//  Main Function 
//  ------------------------------------------------------
int main(int argc, const char * argv[]){
  //
  int c,n,m;
  FILE *file;
  int minval;
  // 
  // Read dimension and sudoku from file
  //      input.file
  // Rename one of the input files from the folder
  //
  file = fopen("input.file","r");
  //
  fscanf(file, "%2d,", &n );
  fscanf(file, "%2d,", &m );
  //
  int field[n][m];
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      fscanf(file, "%2d,", &field[i][j] );
    }
  }
  printsudoku(n,m,field);
  fclose(file);
  //
  // Start of the main loop, each field is tested if not 0
  // 
  double start=omp_get_wtime(); 
  minval=0;
  while (minval == 0) {
    //double start=omp_get_wtime();
    #pragma omp parallel for ordered schedule(static,3) num_threads(9)
    //printf("\n%d\n",omp_get_thread_num());
    for (int i=0; i<n; i++) {
    //#pragma omp parallel for schedule(static,1) num_threads(9)
      for (int j=0; j<m; j++) {
	if (field[i][j]==0) {
	  testnumber(n,m,field,i,j);
	}
      }
    }
    //double end=omp_get_wtime();
    //printf("\n%f\n",end-start);
    //
    // The Sudoku is succesfully filled if
    //  the array contains no zeros
    // Another option would be to compute the sum
    // of all fields, which is fixed for given dimension.
    minval=1;
    for (int i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
	if (field[i][j] == 0) {minval=0; } 
      }
    }
  }
  double end=omp_get_wtime();
  printf("\n%f\n",end-start);
  return 0;
}
