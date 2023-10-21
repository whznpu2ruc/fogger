#ifndef __ASYNC__
#define __ASYNC__

#include <pthread.h>


typedef struct my_item {
  /* TODO: More stuff here, maybe? */
  struct my_item *next;
  struct my_item *prev;
  // function to be called
  void* (*work_routine)(void*);
  void* args;
} my_item_t;

typedef struct my_queue {
  int size;
  my_item_t *head; // works head;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  size_t shutdown; // is threadPool shutdown or not? 1 yes 0 not
  pthread_t *thread_id;// a array of threads
  /* TODO: More stuff here, maybe? */

} my_queue_t;

void async_init(int);
void async_run(void (*fx)(int), int args);

my_queue_t *threadPool;

#endif
