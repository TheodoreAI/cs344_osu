// Mateo Estrada
// May 26, 2021
// Builds a key to use with one-time-pads encryption methods.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// The entire alphabet and the space
static const char alph[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
// I have to let the randomizer be set every time it runs.

static const char newline[4] = "\n";



int main(int argc, char *argv[]){
    // I have to seed it: https://stackoverflow.com/questions/1108780/why-do-i-always-get-the-same-sequence-of-random-numbers-with-rand
    srand(time(NULL));
    // int i = 0;
    if (argc < 2){
        fprintf(stderr, "Error: insufficient input. \n Usage: keygen keylength", argv[0]);
        exit(1);
    }

    // size of the array that I'm going to use to store the key
    int size = atoi(argv[1]);

    // array where the key will be is size + 1 for the newline character
    char key[size+1];

    if (!key){
        fputs("Error: There was an error with my key array", stderr);
        exit(1);
    }


    // Loop to the size of the array + 1 to fill in the key array with a random set of capital letters
    for (int i = 0; i < size; i++){
        int idx_letter = rand()%27;
        key[i] = alph[idx_letter];
        
    }
    key[size] = '\n';
    key[size+1] = '\0';

    printf("%s",key);


    return 0;

}