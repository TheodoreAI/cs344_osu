#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h> // myfifo
#include <unistd.h>

int main(){
  char stringToWrite[21] = "WRITER: Hi reader!@@";
  char* FIFOfilename = "myNewFifo";

  // Sleep for a short while to allow the other process to create the FIFO
  sleep(10);

  // Open the FIFO for writing
  int fd = open(FIFOfilename, O_WRONLY); 
  if (fd == -1) { 
    perror("Writer: open()"); 
    exit(1); 
  }
  // Write the entire string into the FIFO
  write(fd, stringToWrite, strlen(stringToWrite));
  // Terminate the prcoess
  exit(0);
}