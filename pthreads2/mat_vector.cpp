// g++ mat_vector.cpp -pthread -o mat_vector
// ./mat_vector 4

#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

using namespace std;

long thread_count = 0;
long m = 0, n = 0;
long long A[512][512];
long long y[512];
long long x[512];

void *Pth_mat_vect(void *rank)
{
  long my_rank = (long)rank;
  int i, j;
  int local_m = m / thread_count;
  int my_first_row = my_rank * local_m;
  int my_last_row = (my_rank + 1) * local_m - 1;
  for (i = my_first_row; i <= my_last_row; i++)
  {
    y[i] = 0.0;
    for (j = 0; j < n; j++)
      y[i] += A[i][j] * x[j];
  }
}

int main(int argc, char *argv[])
{
  long thread;
  thread_count = strtol(argv[1], NULL, 10);
  pthread_t *thread_handles = (pthread_t *)(malloc(thread_count * sizeof(pthread_t)));
  clock_t tStart = clock();
  for (thread = 0; thread < thread_count; thread++)
  {
    pthread_create(&thread_handles[thread], NULL, Pth_mat_vect, (void *)thread);
  }
  for (thread = 0; thread < thread_count; thread++)
  {
    pthread_join(thread_handles[thread], NULL);
  }
  clock_t tEnd = clock();
  cout << tEnd - tStart << endl;
  free(thread_handles);
  return 0;
}
