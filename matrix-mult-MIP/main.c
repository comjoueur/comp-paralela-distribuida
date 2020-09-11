/*
	mpicc main.c -o main // compilation
	mpirun -np 4 ./main // running
*/

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define M_SIZE 512
#define N_SIZE 512
#define ROOT 0

int local_M_SIZE = 0;
int TOTAL_SIZE = 0;
int local_TOTAL_SIZE = 0;
int num_process = 0;
int rank = 0;

void MatVectmult(
  double local_M[],
  double x[],
  double local_y[],
  MPI_Comm comm
) {
	int i, j;
	for (i = 0; i < local_M_SIZE; i++) {
		local_y[i] = 0.0;
		for (j = 0; j < N_SIZE; j++)
			local_y[i] += local_M[i * N_SIZE + j] * x[j];
	}
}

int main(int argc, char **argv) {
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_process);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	local_M_SIZE = M_SIZE / num_process;
	TOTAL_SIZE = M_SIZE * N_SIZE;
	local_TOTAL_SIZE = TOTAL_SIZE / num_process;

	double local_M[local_TOTAL_SIZE];

	double M[TOTAL_SIZE];

	if(rank == ROOT) for(int i = 0; i < M_SIZE; i++) for(int j = 0;j < N_SIZE; j++) M[i * N_SIZE + j] = i + j;

	double x[local_TOTAL_SIZE];

	for(int i = 0; i < N_SIZE; i++) x[i] = i;

	double local_y[local_M_SIZE];

	double start = MPI_Wtime();

	MPI_Scatter(M, local_TOTAL_SIZE, MPI_DOUBLE, local_M, local_TOTAL_SIZE, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

	MatVectmult(local_M, x, local_y, MPI_COMM_WORLD);

	double answer[M_SIZE];

	MPI_Gather(local_y, local_M_SIZE, MPI_DOUBLE, answer, local_M_SIZE, MPI_DOUBLE, ROOT, MPI_COMM_WORLD);

	double end = MPI_Wtime();
	printf("%f seconds in process %d\n", end-start, rank);

	MPI_Finalize();
}
