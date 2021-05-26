// 1. 
/*
    This method does not use condition variables to let the consumer when the buffer has items in it and to let the producer when the buffer is full.

*/ 

// #include <stdlib.h>
// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>

// // Size of the buffer
// #define SIZE 5

// // Special marker used to indicate end of the producer data
// #define END_MARKER -1

// // Buffer, shared resource
// int buffer[SIZE];
// // Number of items in the buffer, shared resource
// int count = 0;
// // Index where the producer will put the next item
// int prod_idx = 0;
// // Index where the consumer will pick up the next item
// int con_idx = 0;
// // How many items will be produced before the END_MARKER
// int num_items = 0;

// // Initialize the mutex
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// /*
// Produces a random integer between [0, 1000] unless it is the last item to be produced in which case the value -1 is returned.
// */
// int produce_item(int i){
//   int value;
//   if (i == num_items)
//       value = END_MARKER;
//   else
//       value = rand() % 1000;
//   return value;
// }

// /*
//  Put an item in the shared buffer
// */
// int put_item(int value)
// {
//     buffer[prod_idx] = value;
//     // Increment the index where the next item will be put. Roll over to the start of the buffer if the item was placed in the last slot in the buffer
//     prod_idx = (prod_idx + 1) % SIZE;
//     count++;
//     return value;
// }

// /*
//  Function that the producer thread will run. Produce an item. Put in the buffer only when there is space in the buffer. If the buffer is full, then wait until there is space in the buffer.
// */
// void *producer(void *args)
// {
//     for (int i = 0; i < num_items + 1; i++)
//     {
//       // Produce the item outside the critical section
//       int value = produce_item(i);
//       // Lock the mutex before checking where there is space in the buffer
//       pthread_mutex_lock(&mutex);
//       while (count == SIZE)
//       {
//           // Buffer if full. Give up the lock
//           pthread_mutex_unlock(&mutex);
//           // Sleep for 1 seconds before checking the buffer again.
//           sleep(1);
//           // Lock the mutex before checking the buffer for space
//           pthread_mutex_lock(&mutex);
//       }
//       put_item(value);
//       // Give up the lock
//       pthread_mutex_unlock(&mutex);
//       // Print message outside the critical section
//       printf("PROD %d\n", value);
//     }
//     return NULL;
// }

// /*
//  Get the next item from the shared buffer
// */
// int get_item()
// {
//     int value = buffer[con_idx];
//     // Increment the index from which the item will be picked up, rolling over to the start of the buffer if currently at the end of the buffer
//     con_idx = (con_idx + 1) % SIZE;
//     count--;
//     return value;
// }

// /*
//  Function that the consumer thread will run. Get  an item from the buffer if the buffer is not empty. If the buffer is empty then wait until there is data in the buffer.
// */
// void *consumer(void *args)
// {
//     int value = 0;
//     // Continue consuming until the END_MARKER is seen
//     while (value != END_MARKER)
//     {
//       // Lock the mutex before checking if the buffer has data
//       pthread_mutex_lock(&mutex);
//       while (count == 0)
//       {
//         // Buffer is empty. Give up the mutex
//         pthread_mutex_unlock(&mutex);
//         // Sleep for 1 seconds before checking the buffer again
//         sleep(1);
//         // Lock the mutex before checking if the buffer has data
//         pthread_mutex_lock(&mutex);
//       }
//       value = get_item();
//       // Give up the mutex
//       pthread_mutex_unlock(&mutex);
//       // Print the message outside the critical section
//       printf("CONS %d\n", value);
//     }
//     return NULL;
// }

// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//       printf("Usage: ./main num_items\n");
//       printf("\tProvide number of items for the program to produce as an integer greater than 0\n");
//       exit(1);
//     }
//     srand(time(0));
//     num_items = atoi(argv[1]);
//     pthread_t p, c;
//     // Create the producer thread
//     pthread_create(&p, NULL, producer, NULL);
//     // Sleep for a few seconds to allow the producer to fill up the buffer. This has been put in to demonstrate the the producer blocks when the buffer is full. Real-world systems won't have this sleep
//     sleep(5);
//     // Now create the consumer thread
//     pthread_create(&c, NULL, consumer, NULL);
//     // Wait for the threads to finish
//     pthread_join(p, NULL);
//     pthread_join(c, NULL);
//     return 0;
// }


// 2. Using condition variables: this one runs way way faster than the part 1 version

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// Size of the buffer
#define SIZE 5

// Special marker used to indicate end of the producer data
#define END_MARKER -1

// Buffer, shared resource
int buffer[SIZE];
// Number of items in the buffer, shared resource
int count = 0;
// Index where the producer will put the next item
int prod_idx = 0;
// Index where the consumer will pick up the next item
int con_idx = 0;
// How many items will be produced before the END_MARKER
int num_items = 0;

// Initialize the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Initialize the condition variables
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

/*
Produces a random integer between [0, 1000] unless it is the last item to be produced in which case the value -1 is returned.
*/
int produce_item(int i){
  int value;
  if (i == num_items)
      value = END_MARKER;
  else
      value = rand() % 1000;
  return value;
}

/*
 Put an item in the shared buffer
*/
int put_item(int value)
{
    buffer[prod_idx] = value;
    // Increment the index where the next item will be put. Roll over to the start of the buffer if the item was placed in the last slot in the buffer
    prod_idx = (prod_idx + 1) % SIZE;
    count++;
    return value;
}

/*
 Function that the producer thread will run. Produce an item and put in the buffer only if there is space in the buffer. If the buffer is full, then wait until there is space in the buffer.
*/
void *producer(void *args)
{
    for (int i = 0; i < num_items + 1; i++)
    {
      // Produce the item outside the critical section
      int value = produce_item(i);
      // Lock the mutex before checking where there is space in the buffer
      pthread_mutex_lock(&mutex);
      while (count == SIZE)
        // Buffer is full. Wait for the consumer to signal that the buffer has space
        pthread_cond_wait(&empty, &mutex);
      put_item(value);
      // Signal to the consumer that the buffer is no longer empty
      pthread_cond_signal(&full);
      // Unlock the mutex
      pthread_mutex_unlock(&mutex);
      // Print message outside the critical section
      printf("PROD %d\n", value);
    }
    return NULL;
}

/*
 Get the next item from the buffer
*/
int get_item()
{
    int value = buffer[con_idx];
    // Increment the index from which the item will be picked up, rolling over to the start of the buffer if currently at the end of the buffer
    con_idx = (con_idx + 1) % SIZE;
    count--;
    return value;
}

/*
 Function that the consumer thread will run. Get  an item from the buffer if the buffer is not empty. If the buffer is empty then wait until there is data in the buffer.
*/
void *consumer(void *args)
{
    int value = 0;
    // Continue consuming until the END_MARKER is seen    
    while (value != END_MARKER)
    {
      // Lock the mutex before checking if the buffer has data      
      pthread_mutex_lock(&mutex);
      while (count == 0)
        // Buffer is empty. Wait for the producer to signal that the buffer has data
        pthread_cond_wait(&full, &mutex);
      value = get_item();
      // Signal to the producer that the buffer has space
      pthread_cond_signal(&empty);
      // Unlock the mutex
      pthread_mutex_unlock(&mutex);
      // Print the message outside the critical section
      printf("CONS %d\n", value);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
      printf("Usage: ./main num_items\n");
      printf("\tProvide number of items for the program to produce as an integer greater than 0\n");
      exit(1);
    }
    srand(time(0));
    num_items = atoi(argv[1]);
    pthread_t p, c;
    // Create the producer thread
    pthread_create(&p, NULL, producer, NULL);
    // Sleep for a few seconds to allow the producer to fill up the buffer. This has been put in to demonstrate that the producer blocks when the buffer is full. Real-world systems won't have this sleep    
    sleep(5);
    // Now create the consumer thread
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}