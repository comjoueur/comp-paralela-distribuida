// g++ linked_list.cpp -pthread -o linked_list
// ./linked_list 4

#include <bits/stdc++.h>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#define LIST_SIZE 16384

using namespace std;

class list_node_s
{
public:
  int data;
  list_node_s *next = NULL;
  pthread_mutex_t mutex;

  list_node_s()
  {
    pthread_mutex_init(&mutex, NULL);
  }
};

class linked_list
{
public:
  list_node_s *head = NULL;
  list_node_s **head_p = &head;
  pthread_mutex_t head_p_mutex;

  linked_list()
  {
    pthread_mutex_init(&head_p_mutex, NULL);
    for (int i = 0; i < LIST_SIZE; i++)
    {
      Insert(i);
    }
  }

  int Member(int value)
  {
    list_node_s *temp_p;
    pthread_mutex_lock(&head_p_mutex);
    temp_p = *head_p;
    while (temp_p != NULL && temp_p->data < value)
    {
      if (temp_p->next != NULL)
        pthread_mutex_lock(&(temp_p->next->mutex));
      if (temp_p == *head_p)
        pthread_mutex_unlock(&head_p_mutex);
      pthread_mutex_unlock(&(temp_p->mutex));
      temp_p = temp_p->next;
    }
    if (temp_p == NULL || temp_p->data > value)
    {
      if (temp_p == *head_p)
        pthread_mutex_unlock(&head_p_mutex);
      if (temp_p != NULL)
        pthread_mutex_unlock(&(temp_p->mutex));
      return 0;
    }
    else
    {
      if (temp_p == *head_p)
        pthread_mutex_unlock(&head_p_mutex);
      pthread_mutex_unlock(&(temp_p->mutex));
      return 1;
    }
  }

  int SimpleMember(int value)
  {
    list_node_s *curr_p = *head_p;
    while (curr_p != NULL && curr_p->data < value)
      curr_p = curr_p->next;
    if (curr_p == NULL || curr_p->data > value)
    {
      return 0;
    }
    else
    {
      return 1;
    }
  }

  int Insert(int value)
  {
    list_node_s *curr_p = *head_p;
    list_node_s *pred_p = NULL;
    list_node_s *temp_p;
    while (curr_p != NULL && curr_p->data < value)
    {
      pred_p = curr_p;
      curr_p = curr_p->next;
    }
    if (curr_p == NULL || curr_p->data > value)
    {
      temp_p = new list_node_s;
      temp_p->data = value;
      temp_p->next = curr_p;
      if (pred_p == NULL)
        *head_p = temp_p;
      else
        pred_p->next = temp_p;
      return 1;
    }
    else
    {
      return 0;
    }
  }

  int Delete(int value)
  {
    struct list_node_s *curr_p = *head_p;
    struct list_node_s *pred_p = NULL;
    while (curr_p != NULL && curr_p->data < value)
    {
      pred_p = curr_p;
      curr_p = curr_p->next;
    }
    if (curr_p != NULL && curr_p->data == value)
    {
      if (pred_p == NULL)
      {
        *head_p = curr_p->next;
        free(curr_p);
      }
      else
      {
        pred_p->next = curr_p->next;
        free(curr_p);
      }
      return 1;
    }
    else
    {
      return 0;
    }
  }
};

linked_list L;
long thread_count = 0;
pthread_mutex_t general_mutex;
pthread_rwlock_t rwlock;

void* ReadWrite(void *rank)
{
  long my_rank = (long)rank;
  for (int j = my_rank; j < LIST_SIZE; j += thread_count)
  {
    pthread_rwlock_rdlock(&rwlock);
    L.SimpleMember(j);
    pthread_rwlock_unlock(&rwlock);
  }
}
void* OneMutex(void *rank)
{
  long my_rank = (long)rank;
  for (int j = my_rank; j < LIST_SIZE; j += thread_count)
  {
    pthread_mutex_lock(&general_mutex);
    L.SimpleMember(j);
    pthread_mutex_unlock(&general_mutex);
  }
}
void* OneMutexPerNode(void *rank)
{
  long my_rank = (long)rank;
  for (int j = my_rank; j < LIST_SIZE; j += thread_count)
  {
    L.Member(j);
  }
}

int main(int argc, char *argv[])
{
  pthread_mutex_init(&general_mutex, NULL);
  pthread_rwlock_init(&rwlock, NULL);
  long thread;
  thread_count = strtol(argv[1], NULL, 10);
  pthread_t *thread_handles = (pthread_t *)(malloc(thread_count * sizeof(pthread_t)));
  clock_t tStart = clock();
  for (thread = 0; thread < thread_count; thread++)
  {
    // pthread_create(&thread_handles[thread], NULL, ReadWrite, (void *)thread);
    // pthread_create(&thread_handles[thread], NULL, OneMutexPerNode, (void *)thread);
    pthread_create(&thread_handles[thread], NULL, OneMutex, (void *)thread);
  }
  for (thread = 0; thread < thread_count; thread++)
  {
    pthread_join(thread_handles[thread], NULL);
  }
  clock_t tEnd = clock();
  cout << tEnd - tStart << endl;
  free(thread_handles);
  pthread_mutex_destroy(&general_mutex);
  return 0;
}