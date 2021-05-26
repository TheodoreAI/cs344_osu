// 1.
/*
    BAD for large values of COUNT_TO use the second part of this instead where
    Where it used Mutex (mutual exclusion).
*/ 

// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>

// #define COUNT_TO 100
// #define NUM_THREADS 3

// // counter is a variable shared by all the threads
// int counter = 0;

// /*
//  Function that the threads will run
// */
// void* perform_work(void* argument){
//     pthread_t pw_tid = pthread_self();
//     printf("It's me, thread with id %ld!\n", pw_tid);
//   for(int i = 0; i < COUNT_TO; i++){
    

//     // THE CRITICAL SECTION STARTS
//     counter += 1;
//     // THE CRITICAL SECTION ENDS

//   }
//   return NULL;
// }

// int main(void){
//   pthread_t threads[NUM_THREADS];

//   // Create NUM_THREADS threads
//   for(int i = 0; i < NUM_THREADS; i++){
//     pthread_create(&threads[i], NULL, perform_work, NULL);
//   }

//   // Wait for the threads to end
//   for(int i = 0; i < NUM_THREADS; i++){
//     pthread_join(threads[i], NULL);
//   }

//   printf("Expected value of counter = %d\n", COUNT_TO * NUM_THREADS);
//   printf("Actual value of counter = %d\n", counter);
//   exit(EXIT_SUCCESS);
// }


// 2.


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNT_TO 10000000
#define NUM_THREADS 3

// Declare a mutex
pthread_mutex_t counterMutex; //mutual exclusion declaration

// counter is a variable shared by all the threads
int counter = 0;

void* perform_work(void* argument){
  
  for(int i = 0; i < COUNT_TO; i++){
    // Try to acquire the lock before entering the critical section
    pthread_mutex_lock(&counterMutex); // locking the counterMutex

    // THE CRITICAL SECTION STARTS
    counter += 1;
    // THE CRITICAL SECTION ENDS

    // Unlock the mutex
    pthread_mutex_unlock(&counterMutex); // unlock it after the critical section ends
  }

  return NULL;
}

int main(void){
  // Initialize the mutex
  pthread_mutex_init(&counterMutex, NULL);  // initialize this function

  pthread_t threads[NUM_THREADS];

  // Start NUM_THREADS threads
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_create(&threads[i], NULL, perform_work, NULL);
  }

  // Wait for the threads to terminate
  for(int i = 0; i < NUM_THREADS; i++){
    pthread_join(threads[i], NULL);
  }
  
  // Destroy the mutex
  pthread_mutex_destroy(&counterMutex);

  printf("Expected value of counter = %d\n", COUNT_TO * NUM_THREADS);
  printf("Actual value of counter = %d\n", counter);
  exit(EXIT_SUCCESS);
}