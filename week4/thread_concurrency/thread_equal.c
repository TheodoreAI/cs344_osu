#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
 The function that each thread will call
*/
void *perform_work(void *argument)
{
    pthread_t pw_tid = pthread_self();

    printf("It's me, thread with id %ld!\n", pw_tid);
    return NULL;
}

/*
* The goal is that threads t1 & t2 should run concurrently.
* Question: does the program achieve the goal?
*/
int main(void)
{
    pthread_t t1;
    pthread_t t2;

    // first create and then
    pthread_create(&t1, NULL, perform_work, NULL);
    pthread_create(&t2, NULL, perform_work, NULL);
    // join them now they are concurrent
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("In main: All threads completed successfully\n");
    exit(EXIT_SUCCESS);
}