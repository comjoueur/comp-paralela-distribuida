#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv) {
	srand(time(NULL));
	double hit_ratio[2] = {0.95, 0.93};
	int rank = 0;
	int MAX_PLAYES = 100;
	int START_PLAYER_RANK = 0;
	int MSG_TAG = 0;

	MPI_Init(&argc , &argv); // MPI Initialization
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get process id
	int count = 0;

	do {
		if (count != 0 || rank != START_PLAYER_RANK) {
			MPI_Recv(&count, 1, MPI_INT, 1 - rank, MSG_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (count == -1) {
				MPI_Finalize();
				return 0;
			}
		}
		if (count % 2 == rank) {
			double attempt = ((double)(rand() % 1000)) / 1000;
			if (attempt <= hit_ratio[rank]) {
				count += 1;
				MPI_Send(&count, 1, MPI_INT, 1 - rank, MSG_TAG, MPI_COMM_WORLD);
				printf("Well played, player %d hit ping pong ball\n", (rank + 1));
				sleep(1);
			}
			else {
				printf("Bad played, player %d failed\n", (rank + 1));
				printf("Player %d won\n", (2 - rank));
				count = -1;
				MPI_Send(&count, 1, MPI_INT, 1 - rank, MSG_TAG, MPI_COMM_WORLD);
				MPI_Finalize();
				return 0;
			}
		}
	}
	while(count <= MAX_PLAYES);

	MPI_Finalize();
	printf("No one won");
	return 0;
}
