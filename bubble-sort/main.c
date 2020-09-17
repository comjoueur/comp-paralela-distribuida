/*
	mpicc main.c -o main // compilation
	mpirun -np 4 ./main // running
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define TOTAL_SIZE 4096
#define ROOT 0

int num_process = 0;
int rank = 0;
int block_size = 0;

void sort_bubble(int arr[], int size) {
  int i,j,tmp;
  for(i=0;i<size;++i){
    for(j=0;j<size-1;++j){
      if(arr[j]>arr[j+1]) {
        tmp = arr[j];
        arr[j] = arr[j+1];
        arr[j+1] = tmp;
      }
    }
  }
}

void merge(int arr[], int size, int arr2[], int size2) {
  int i=0 ,j=0;
  int total_size = size + size2;
  int new_arr[total_size];
  int k = 0;
  while(i < size || j < size2) {
    if (i == size) {
      new_arr[k++] = arr2[j++];
    }
    else if (j == size2) {
      new_arr[k++] = arr[i++];
    }
    else if (arr[i] > arr2[j]) {
      new_arr[k++] = arr2[j++];
    }
    else {
      new_arr[k++] = arr[i++];
    }
  }
  for(i=0;i<size;i++){
    arr[i] = new_arr[i];
  }
  for(i=size;i<total_size;++i) {
    j = i - size;
    arr2[j] = new_arr[i];
  }
}

void assign(int arr[], int block[], int begin, int end) {
  for(int i=begin;i<end;i++) {
    block[i-begin] = arr[i];
  }
}

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &num_process);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int arr[TOTAL_SIZE];
  block_size = TOTAL_SIZE / num_process;

  if (rank == ROOT) {
    for(int i=0;i<TOTAL_SIZE;i++) {
      arr[i] = TOTAL_SIZE - i;
    }
  }

  int local_arr[block_size];

  double start = MPI_Wtime();

  MPI_Scatter(arr, block_size, MPI_INT, local_arr, block_size, MPI_INT, ROOT, MPI_COMM_WORLD);
  sort_bubble(local_arr, block_size);
  MPI_Allgather(local_arr, block_size, MPI_INT, arr, block_size, MPI_INT, MPI_COMM_WORLD);

  int new_local_arr[block_size];
  int near_local_arr[block_size];

  for(int phase = 0; phase < num_process; phase++) {
    if (phase % 2 == 0) {
      if (rank % 2 == 0) {
        assign(arr, new_local_arr, rank * block_size, (rank + 1) * block_size);
        if (rank + 1 < num_process) {
          assign(arr, near_local_arr, (rank + 1) * block_size, (rank + 2) * block_size);
          merge(new_local_arr, block_size,near_local_arr, block_size);
        }
      }
      else {
        assign(arr, new_local_arr, rank * block_size, (rank + 1) * (block_size));
        if (rank - 1 >= 0) {
          assign(arr, near_local_arr, (rank - 1) * block_size, rank * block_size);
          merge(near_local_arr, block_size,new_local_arr, block_size);
        }
      }
    }
    else {
      if (rank % 2 == 0) {
        assign(arr, new_local_arr, rank * block_size, (rank + 1) * (block_size));
        if (rank - 1 >= 0) {
          assign(arr, near_local_arr, (rank - 1) * block_size, rank * block_size);
          merge(near_local_arr, block_size,new_local_arr, block_size);
        }
      }
      else {
        assign(arr, new_local_arr, rank * block_size, (rank + 1) * block_size);
        if (rank + 1 < num_process) {
          assign(arr, near_local_arr, (rank + 1) * block_size, (rank + 2) * block_size);
          merge(new_local_arr, block_size,near_local_arr, block_size);
        }
      }
    }
    MPI_Allgather(new_local_arr, block_size, MPI_INT, arr, block_size, MPI_INT, MPI_COMM_WORLD);
  }

  double end = MPI_Wtime();

  printf("%f seconds in process %d\n", end-start, rank);

	MPI_Finalize();
}