
#include <stdlib.h>
#include <pthread.h>
#include "async.h"
#include "utlist.h"
#include <string.h>
#include <stdio.h>
void* work_routine(void* args){
    while(1){
        pthread_mutex_lock(&threadPool->mutex);
        while(!threadPool->head && !threadPool->shutdown){
            pthread_cond_wait(&threadPool->cond,&threadPool->mutex);
        }
        if(threadPool->shutdown){
            pthread_mutex_unlock(&threadPool->mutex);
            pthread_exit(NULL);
        }
        my_item_t* work=threadPool->head;
        threadPool->head=threadPool->head->next;
        pthread_mutex_unlock(&threadPool->mutex);
        work->work_routine(work->args);

        free(work);
    }
    pthread_exit(NULL);

}

void async_init(int num_threads) {
    /** TODO: create num_threads threads and initialize the thread pool **/
    // *threadPool=NULL;
    if(num_threads<=0){
        return;
    }
    threadPool=(my_queue_t*)malloc(sizeof(my_queue_t));
    if(NULL==threadPool){
        return ;
    }

    // memset(threadPool,0,sizeof(threadPool));
    (threadPool)->shutdown=0;
    (threadPool)->size=num_threads;
    (threadPool)->head=NULL;

    pthread_mutex_init(&threadPool->mutex,NULL);
    pthread_cond_init(&threadPool->cond,NULL);
    (threadPool)->thread_id=(pthread_t*)malloc(sizeof(pthread_t)*num_threads);

    for(int i=0;i<num_threads;i++){
        if(pthread_create(&threadPool->thread_id[i],NULL,work_routine,(void*)(threadPool))!=0){
            return;
        }
    }
    
}


void async_run(void (*hanlder)(int), int args) {
    hanlder(args);
    /** TODO: rewrite it to support thread pool **/
    my_item_t *new_item=(my_item_t*)malloc(sizeof(my_item_t));
    new_item->work_routine=hanlder;
    new_item->args=args;
    new_item->next=NULL;

    pthread_mutex_lock(&threadPool->mutex);

    DL_APPEND(threadPool->head,new_item);


    pthread_cond_signal(&threadPool->cond);

    pthread_mutex_unlock(&threadPool->mutex);
}