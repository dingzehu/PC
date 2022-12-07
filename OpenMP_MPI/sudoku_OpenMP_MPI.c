//compile: mpicc -g -Wall -o s sudoku.c -lm -fopenmp

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

// Sudoku-Solver
//
//  ------------------------------------------------------
// print the sudoku on screen
//  ------------------------------------------------------
/*void printsudoku(int n, int m, int field[n][m]) {
  printf("\n");
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      printf("%d ",field[i][j]);
    }
    printf("\n");
  }
}*/
//  ------------------------------------------------------
//  search for fitting number in cell i,j
//  ------------------------------------------------------
void testnumber(int n, int m, int field[n][m], int i, int j) {
  // I'm at the row i, column j
  // Test each number k
  // If only one number fits -> success
  // 
  //int success=1;
  int k_success;
  int k_tobeset;
  int n_numbers=0;
  int mb,nb;
  int ldim = (int)sqrt((double)n);
  //
  for (int k=1; k <n+1; k++) {
    //
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
  if (n_numbers == 1) {
    field[i][j]=k_tobeset;
    // printf("Success, new number!\n");
    //    printsudoku(n,m,field);
  }
}
//  ------------------------------------------------------
//  Main Function 
//  ------------------------------------------------------
int main(int argc, const char * argv[]){
  //
  int p_num,num_p;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &num_p);
  MPI_Comm_rank(MPI_COMM_WORLD, &p_num);
  //printf("%d\t%d\n",num_p,p_num);

  int n,m;
  FILE *file;
  int minval;
  // 
  // Read dimension and sudoku from file
  //      input.file
  // Rename one of the input files from the folder
  //
  file = fopen("input.file","r");
  //
  fscanf(file, "%d,", &n );
  fscanf(file, "%d,", &m );
  //
  int field[n][m];
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      fscanf(file, "%d,", &field[i][j] );
    }
  }
  //printsudoku(n,m,field);
  fclose(file);
  //
  // Start of the main loop, each field is tested if not 0
  //  
  minval=0;
  int checksum=0;
  int testsum=0;
  #pragma omp parallel for default(shared) reduction(+:checksum) collapse(2)
  for (int i=0; i<n; i++) {
    for (int j=0; j<m; j++) {
      checksum+=field[i][j];
    }
  }
  
  int work[num_p], pos[num_p], start = 0, end;
  pos[0] = 0;
  for (int i = 0; i < num_p; i++) {
    end = ((i + 1) * n) / num_p;
    work[i] = (end - start) * m;
    if (i > 0) {
      pos[i] = pos[i - 1] + work[i - 1];
    }
    start = end;
  }
  start = (p_num * n) / num_p;
  end = ((p_num + 1) * n) / num_p;

  while (minval == 0) {    
    #pragma omp parallel for default(shared) collapse(2)
    for (int i=start; i<end; i++) {
      for (int j=0; j<m; j++) {
        if (field[i][j]==0) {
          testnumber(n,m,field,i,j);
        }
      }
    }
    MPI_Allgatherv(field[start],(end - start) * m,MPI_INT,field,work,pos,MPI_INT,
	MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    //
    // The Sudoku is succesfully filled if
    //  the array contains no zeros
    // Another option would be to compute the sum
    // of all fields, which is fixed for given dimension.
    minval=1;
    #pragma omp parallel for default(shared) reduction(+:testsum) collapse(2)
    for (int i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
	testsum+=field[i][j];
        if (field[i][j] == 0) {minval=0; } 
      }
    }
    if (testsum == checksum) {
      printf("Sudoku does not have a unique solution. STOP");
      minval=111;
      
    } else {
      printf("%d\tsome fields set, continue\n",testsum-checksum);
      checksum = testsum;
      testsum = 0;
    }
  }
  if (minval == 111) {
    printf("\n");
  }/* else {
    printsudoku(n,m,field);
  }*/
  MPI_Finalize();
  return 0;
}
