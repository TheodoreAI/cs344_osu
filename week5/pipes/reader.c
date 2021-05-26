#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h> // myfifo
#include <unistd.h>

int main(){
  // Buffer that will be used to read data from the pipe.
  char readBuffer[10];
  // Buffer that will store the complete message read from the pipe.
  char completeMessage[512] = {0};

  int bytesRead;

  char* FIFOfilename = "myNewFifo";
  // Create the FIFO
  int newfifo = mkfifo(FIFOfilename, 0640);

  // Open the FIFO for reading
  int fd = open(FIFOfilename, O_RDONLY); 
  if (fd == -1) { 
    perror("Reader: open()"); 
    exit(1); 
  }
  while (strstr(completeMessage, "@@") == NULL){
    // Clear the read buffer before reading from the pipe
    memset(readBuffer, '\0', sizeof(readBuffer));
    // Read the next chunk of data from the pipe
    // We read one byte less than the size of readBuffer because the readBuffer is initalized to '\0' and this way the chunk we read becomes a null terminated string.
    bytesRead = read(fd, readBuffer, sizeof(readBuffer) - 1);

    // Add the chunk we read to what we have read up to now
    strcat(completeMessage, readBuffer); 
    printf("Reader: Chunk received: \"%s\" \n", 
            readBuffer);
    printf("Reader: Total received till now: \"%s\"\n", completeMessage);
    // Check for errors
    if (bytesRead == -1) { 
      // -1 indicates an error
      printf("bytesRead == -1\n"); 
      break; 
    } 
    if (bytesRead == 0) {
      // 0 indicates an error or end-of-file
      printf("bytesRead == 0\n"); 
      break; 
    }
  }
  // Find the location of the terminal indicator @@ and remove it from the message by replacing by the null character
  char* terminalLoc = strstr(completeMessage, "@@");
  *terminalLoc = '\0';
  printf("Reader: Complete string: \"%s\"\n", completeMessage);

  remove(FIFOfilename); // Delete the FIFO
}