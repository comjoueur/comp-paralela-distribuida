//g++ main.cpp -pthread -o main
//./main 4 

#include <bits/stdc++.h>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

using namespace std;

int thread_count;
sem_t my_sem;
int *msgs;
sem_t* semaphores;

void* send_msg(void *rank)
{
  long my_rank = (long)rank;
  long dest = (my_rank + 1)%thread_count;
  int message = my_rank;
  msgs[dest] = message;
  sem_post(&semaphores[dest]);
  sem_wait(&semaphores[my_rank]);
  return NULL;
}

int main(int argc, char* argv[])
{
  thread_count = strtol(argv[1], NULL, 10);
  long thread;
  semaphores = (sem_t*)(malloc(thread_count*sizeof(sem_t)));
  for(thread = 0; thread < thread_count; thread++) {
    sem_init(&semaphores[thread], 0, 0);
  }

  msgs = (int*)(malloc(thread_count * sizeof(int)));
  for(thread = 0; thread < thread_count; thread++) {
    msgs[thread]=-1;
  }
  pthread_t* thread_handles = (pthread_t*)(malloc(thread_count*sizeof(pthread_t)));
  clock_t tStart = clock();
  for(thread = 0; thread < thread_count; thread++) {
    pthread_create(&thread_handles[thread], NULL, send_msg, (void*) thread);
  }
  for(thread = 0; thread < thread_count; thread++) {
    pthread_join(thread_handles[thread], NULL);
  }
  clock_t tEnd = clock();
  cout << tEnd - tStart << endl;
  free(thread_handles);
  free(msgs);
  free(semaphores);
  return 0;
}