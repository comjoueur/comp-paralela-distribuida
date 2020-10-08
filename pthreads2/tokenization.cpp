// g++ tokenization.cpp -pthread -o tokenization
// ./tokenization 4

#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>
#define MAX 1024

using namespace std;

long thread_count = 0;
sem_t *semaphores;
FILE *fp;

void *Tokenize(void *rank)
{
  long my_rank = (long)rank;
  int count;
  int next = (my_rank + 1) % thread_count;
  char *fg_rv;
  char my_line[MAX];
  char *my_string;
  sem_wait(&semaphores[my_rank]);
  fg_rv = fgets(my_line, MAX, fp);
  sem_post(&semaphores[next]);
  while (fg_rv != NULL)
  {
    printf("Thread %ld > my line = %s", my_rank, my_line);
    count = 0;
    my_string = strtok(my_line, " \t\n");
    while (my_string != NULL)
    {
      count++;
      printf("Thread %ld > string %d = %s\n", my_rank, count,
             my_string);
      my_string = strtok(NULL, " \t\n");
    }
    sem_wait(&semaphores[my_rank]);
    fg_rv = fgets(my_line, MAX, fp);
    sem_post(&semaphores[next]);
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  long thread;
  thread_count = strtol(argv[1], NULL, 10);
  fp = fopen("file.txt", "r");
  if (fp == NULL)
  {
    perror("Error opening file");
    return (-1);
  }
  semaphores = (sem_t *)(malloc(thread_count * sizeof(sem_t)));
  for (thread = 0; thread < thread_count; thread++)
  {
    sem_init(&semaphores[thread], 0, 0);
  }

  pthread_t *thread_handles = (pthread_t *)(malloc(thread_count * sizeof(pthread_t)));
  for (thread = 0; thread < thread_count; thread++)
  {
    pthread_create(&thread_handles[thread], NULL, Tokenize, (void *)thread);
  }
  for (thread = 0; thread < thread_count; thread++)
  {
    pthread_join(thread_handles[thread], NULL);
  }
  free(thread_handles);
  return 0;
}
