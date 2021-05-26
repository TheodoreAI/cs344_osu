#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h> // sleep

// Number of threads to spawn
#define NUM_THREADS     4
// Number of resources in the pool
#define POOL_SIZE       2

#define ITERATIONS      5
#define SLEEP_DURATION  10

// Declare a semaphone
sem_t counting_sem;

// This is the critical section where the shared resource is manipulated. We just sleep for SLEEP_DURATION seconds
void use_resource(){
  sleep(SLEEP_DURATION);
}

void* perform_work(void* argument){
  // The argument to the thread is a pointer to a character 
  char thread_name = *((char*) argument);
  int resources_available;
  for(int i = 0; i < ITERATIONS; i++){
    printf("%c wants resource, iteration: %d\n", thread_name, i);
    
    // sem_getvalue returns the current value of the semaphore in resourcesAvailable
    sem_getvalue(&counting_sem, &resources_available);
    printf("Resources available: %d\n", resources_available);

    sem_wait(&counting_sem);
    printf("%c got resource\n", thread_name);
    use_resource();

    printf("%c giving up resource\n", thread_name);
    sem_post(&counting_sem);
  }
  return NULL;
}

int main(void){
  // Initialize the semaphore to POOL_SIZE
  int res = sem_init(&counting_sem, 0, POOL_SIZE);

  pthread_t threads[NUM_THREADS];
  char thread_name[NUM_THREADS];

  for(int i = 0; i < NUM_THREADS; i++){
    // Give threads a name starting with 'A' which is ASCII 65
    thread_name[i] = i + 65;
    pthread_create(&threads[i], 
                  NULL, 
                  perform_work, 
                  (void*) &thread_name[i]);
  }
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }
  // Destroy the semphore
  sem_destroy(&counting_sem);
  exit(EXIT_SUCCESS);
}