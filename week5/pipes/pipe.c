#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h> // pipe

/*
* Example of using a pipe to send data from a child process to the parent process.
* The child process writes a message to the pipe. This message is terminated by @@.
* The parent process reads the message from the pipe until it encounters the terminating characters @@.
*/
int main(){
  // Buffer that will be used to read data from the pipe.
  char readBuffer[10];
  // Buffer that will store the complete message read from the pipe.
  char completeMessage[512] = {0};

  int r, pipeFDs[2];
  // Create the pipe with error check
  if (pipe(pipeFDs) == -1) {
    perror("Call to pipe() failed\n"); 
    exit(1); 
  } 
  
  // Fork the child
  pid_t spawnpid = fork();
  switch (spawnpid){ 
    case 0:
      // Child process will write into the pipe, so close the input file descriptor
      close(pipeFDs[0]); 
      // Write the entire string and terminate it by @@
      write(pipeFDs[1], "CHILD: Hi parent!@@", 19); 
      // Terminate the child
      exit(0);
      break; 
    default:
      // Parent process will read from the pipe, so close the output file desriptor.
      close(pipeFDs[1]);
      // As long as we haven't found the terminal indicators @@
      while (strstr(completeMessage, "@@") == NULL){

        // Use memset to clear the read buffer before reading from the pipe
        // https://www.man7.org/linux/man-pages/man3/memset.3.html
        memset(readBuffer, '\0', sizeof(readBuffer));

        // Read the next chunk of data from the pipe
        // We read one byte less than the size of readBuffer because the readBuffer is initalized to '\0' and this way the chunk we read becomes a null terminated string.
        r = read(pipeFDs[0], readBuffer, sizeof(readBuffer) - 1);

        // Add the chunk we read to what we have read up to now
        strcat(completeMessage, readBuffer); 
        printf("PARENT: Chunk received from child: \"%s\" \n", 
                readBuffer);
        printf("PARENT: Total received till now: \"%s\"\n", completeMessage);
        // Check for errors
        if (r == -1) { 
          // -1 indicates an error
          printf("r == -1\n"); 
          return EXIT_FAILURE; 
        } 
        if (r == 0) {
          // 0 indicates an error or end-of-file
          printf("r == 0\n"); 
          return EXIT_FAILURE; 
        }
      } // while

      // Find the location of the terminal indicator @@ and remove it from the message by replacing by the null character
      char* terminalLoc = strstr(completeMessage, "@@");
      *terminalLoc = '\0';
      printf("PARENT: Complete string: \"%s\"\n", completeMessage);
      break;
    } // switch
    return EXIT_SUCCESS;
}