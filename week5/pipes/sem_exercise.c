#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define COUNT_TO 10000000
#define NUM_THREADS 3

// Declare a semaphone
sem_t binary_sem;

int counter = 0;
void* perform_work(void* argument){
  for(int i = 0; i < COUNT_TO; i++){

    // Critical section starts
    // decrement the semaphore 
    sem_wait(&binary_sem);
    counter += 1;
    // Critical section ends
    // increment the semaphore
    sem_post(&binary_sem);

  }
  return NULL;
}

int main(void){
  // Initialize the semaphore
  // The 2nd argument is 0 because the semaphore will be shared among threads
  // The 3rd argument is 1, i.e., the intial value of the semaphore will be 1
  int res = sem_init(&binary_sem, 0, 1);

  pthread_t threads[NUM_THREADS];

  for(int i = 0; i < NUM_THREADS; i++){
    pthread_create(&threads[i], NULL, perform_work, NULL);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }
  // Destroy the semphore
  sem_destroy(&binary_sem);

  printf("Expected value of counter = %d\n", COUNT_TO * NUM_THREADS);
  printf("Actual value of counter = %d\n", counter);
  exit(EXIT_SUCCESS);
}