//g++ barriers.cpp -pthread -o barriers
//./barriers 4

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

double sum = 0;
int n = 4096;
int thread_count;
pthread_mutex_t my_mutex;
pthread_mutex_t BW_mutex;
pthread_mutex_t CV_mutex;
sem_t barrier_sem;
sem_t count_sem;
pthread_cond_t cond_var;

int counter = 0;

void *thread_sum_BW(void *rank)
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
  pthread_mutex_lock(&my_mutex);
  sum += my_sum;
  pthread_mutex_unlock(&my_mutex);

  pthread_mutex_lock(&BW_mutex);
  counter++;
  pthread_mutex_unlock(&BW_mutex);
  while (counter < thread_count)
    ;
  cout<<"All in the same moment"<<endl;
  return NULL;
}

void *thread_sum_Sem(void *rank)
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
  pthread_mutex_lock(&my_mutex);
  sum += my_sum;
  pthread_mutex_unlock(&my_mutex);

  sem_wait(&count_sem);
  if (counter == thread_count - 1)
  {
    counter = 0;
    sem_post(&count_sem);
    for (int j = 0; j < thread_count - 1; j++)
      sem_post(&barrier_sem);
  }
  else
  {
    counter++;
    sem_post(&count_sem);
    sem_wait(&barrier_sem);
  }
  cout<<"All in the same moment"<<endl;
  return NULL;
}

void *thread_sum_CV(void *rank)
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
  pthread_mutex_lock(&my_mutex);
  sum += my_sum;
  pthread_mutex_unlock(&my_mutex);

  pthread_mutex_lock(&CV_mutex);
  counter++;
  if (counter == thread_count)
  {
    counter = 0;
    pthread_cond_broadcast(&cond_var);
  }
  else
  {
    while (pthread_cond_wait(&cond_var, &CV_mutex) != 0);
  }
  pthread_mutex_unlock(&CV_mutex);
  cout<<"All in the same moment"<<endl;
  return NULL;
}

int main(int argc, char *argv[])
{
  long thread;
  pthread_mutex_init(&my_mutex, NULL);
  pthread_mutex_init(&BW_mutex, NULL);
  sem_init(&barrier_sem, 0, 0);
  sem_init(&count_sem, 0, 0);
  pthread_cond_init(&cond_var, NULL);
  thread_count = strtol(argv[1], NULL, 10);
  pthread_t *thread_handles = (pthread_t *)(malloc(thread_count * sizeof(pthread_t)));
  clock_t tStart = clock();
  for (thread = 0; thread < thread_count; thread++)
  {
    pthread_create(&thread_handles[thread], NULL, thread_sum_BW, (void *)thread);
    //pthread_create(&thread_handles[thread], NULL, thread_sum_Sem, (void *)thread);
    //pthread_create(&thread_handles[thread], NULL, thread_sum_CV, (void *)thread);
  }
  for (thread = 0; thread < thread_count; thread++)
  {
    pthread_join(thread_handles[thread], NULL);
  }
  clock_t tEnd = clock();
  cout << tEnd - tStart << endl;
  free(thread_handles);
  pthread_mutex_destroy(&my_mutex);
  return 0;
}