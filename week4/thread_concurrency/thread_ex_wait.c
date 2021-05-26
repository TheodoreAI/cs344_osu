#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define NUM_THREADS 5


// Example of threads waiting for each other using for loops and indecies

/*
 The function that each thread will call
*/
void *perform_work(void *argument)
{
    pthread_t pw_tid = pthread_self();
    int passed_in_value;
    // We cast the void* to int*, and then dereference the int* to get the int value the pointer is pointing to
    passed_in_value = *((int *)argument);
    printf("It's me, thread with argument %d!\n", passed_in_value);
    return NULL;
}

int main(void)
{
    pthread_t threads[NUM_THREADS]; // an array of process threads id's
    int thread_args[NUM_THREADS]; // an array of thread arguments
    int result_code, index; // setting the type of the result and the index for the loops
    for (index = 0; index < NUM_THREADS; ++index) // for loop to make the threads
    {
        // Create all threads one by one
        thread_args[index] = index; 
        printf("In main: creating thread %d\n", index);

        // type is int and it creates the threads using the thread[index] and the start_routine() aka perform_work
        result_code = pthread_create(&threads[index],
                                     NULL,
                                     perform_work,
                                     (void *)&thread_args[index]);
        assert(0 == result_code);
    }

    // Wait for each thread to complete
    for (index = 0; index < NUM_THREADS; ++index) // another loop to wait for all the threads to finish
    {
        result_code = pthread_join(threads[index], NULL); // wait on threads one by one with threads[index]
        printf("In main: thread %d has completed\n", index);
        assert(0 == result_code);
    }
    printf("In main: All threads completed successfully\n");
    exit(EXIT_SUCCESS);
}
