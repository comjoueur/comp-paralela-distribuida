//g++ main.cpp -pthread -o main
//./main 4 

#include <bits/stdc++.h>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define NUM_THREADS 4
#define COMPUTE_SIZE 20

using namespace std;

double sum = 0;
int flag = 0;
int n = 4096;
int thread_count;

void* thread_sum(void *rank)
{
  long my_rank = (long)rank;
  double factor;
  long long i;
  long long my_n = n / thread_count;
  long long my_first_i = my_n * my_rank;
  long long my_last_i = my_first_i + my_n;
  double my_sum = 0;
  if (my_first_i % 2 == 0)
    factor = 1.0;
  else
    factor = -1.0;
  for (i = my_first_i; i < my_last_i; i++, factor = -factor)
  {
    my_sum += factor / (2 * i + 1);
  }
  while(flag != my_rank);
  sum += my_sum;
  flag += 1;
  return NULL;
}

int main(int argc, char* argv[])
{
  long thread;
  thread_count = strtol(argv[1], NULL, 10);
  pthread_t* thread_handles = (pthread_t*)(malloc(thread_count*sizeof(pthread_t)));
  clock_t tStart = clock();
  for(thread = 0; thread < thread_count; thread++) {
    pthread_create(&thread_handles[thread], NULL, thread_sum, (void*) thread);
  }
  for(thread = 0; thread < thread_count; thread++) {
    pthread_join(thread_handles[thread], NULL);
  }
  clock_t tEnd = clock();
  cout << tEnd - tStart << endl;
  free(thread_handles);
  return 0;
}