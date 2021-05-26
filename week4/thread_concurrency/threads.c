#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// The following examples are from my OS class at OSU.
// May 11, 2021



// Examples of a pointer function:


// 1.


// Delcarando la funcion que imprimira el texto.
// void * foo(void * val){
//     printf("Buenas tardes, hoy es Mayo 11 del %d\n", val);
// }

// typedef void *(*RoutinePtr)(void *);

// void routineInvoker(RoutinePtr routine){
//     (*routine)(2021); //invoke the routine 
// }


// int main(void){

//     // declarando un puntador hacia una funcion que tomara un argumento del tipo void *
//     // y regresara un puntador del typo void *

//     // void *(*routine)(void *);
//     // routine = foo;
//     // (*routine)(2021);
//     // return 0;


//     // otra forma de hacer esto es:
//     RoutinePtr routine = foo;
//     routineInvoker(routine);
//     return 0;

// }




// 2.

struct Movie {
    int year;
    char *title;
    float rating;
};



/*
  A function that prints Hello World
*/
void * perform_work(void * arg){
    struct Movie* curr_movie = (struct Movie*) arg;
    printf("title = %s, year = %d, rating = %.1f\n", curr_movie->title, curr_movie->year, curr_movie->rating);
    return NULL;
}

int main(void) {
//  set up data in the struct
struct Movie* a_movie = calloc(1, sizeof(struct Movie));
a_movie->title = strdup("Ant-man");
a_movie->year = 2017;
a_movie->rating = 8.01;


// create the thread and join it

pthread_t t;
int result_code = pthread_create(&t, NULL, perform_work, (void *) a_movie);
result_code = pthread_join(t, NULL);


// free the allocated memory on the heap of calloc()
free(a_movie->title);

exit(EXIT_SUCCESS);

}