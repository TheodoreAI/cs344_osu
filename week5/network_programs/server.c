#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/*
    Requires one argument: port

    To run:
    ./server $port

    Ex:
    ./server 4040
*/


// Error function used for reporting issues
void error(const char *msg) {
  perror(msg);
  exit(1);
} 

// Set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, 
                        int portNumber){
 
  // Clear out the address struct
  memset((char*) address, '\0', sizeof(*address)); 

  // The address should be network capable
  address->sin_family = AF_INET;
  // Store the port number
  address->sin_port = htons(portNumber);
  // Allow a client at any address to connect to this server
  address->sin_addr.s_addr = INADDR_ANY;
}

int main(int argc, char *argv[]){
  int connectionSocket, charsRead;
  char buffer[256];
  struct sockaddr_in serverAddress, clientAddress;
  socklen_t sizeOfClientInfo = sizeof(clientAddress);

  // Check usage & args
  if (argc < 2) { 
    fprintf(stderr,"USAGE: %s port\n", argv[0]); 
    exit(1);
  } 
  
  /*
    Create the socket that will listen for connections.
    STEP 1: create the socket endpoint for the server
  */

  int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    error("ERROR opening socket");
  }

  /*
  
   Set up the address struct for the server socket
   STEP 2: Calls this function to set up a socket address structure sockaddr_in with the (PORT NUMBER) at 
   which the server socket will listen for connections. 

    serverAddress:
    port number: 
  */
  

  setupAddressStruct(&serverAddress, atoi(argv[1]));

  /*
     Associate the socket to the port (binding the socket to the port).
     STEP 3: call bind() to associate the socket with the socket address.
  */
  if (bind(listenSocket, 
          (struct sockaddr *)&serverAddress, 
          sizeof(serverAddress)) < 0){
    error("ERROR on binding");
  }

  /*
    STEP 4: call listen() on the socket to start listneing for client connections, 
    this exists until the server process ends.
    Start listening for connetions. Allow up to 5 connections to queue up
  */
  listen(listenSocket, 5); 
  


  /*
    STEP 5: Loop and accept connections on the listneing socket by calling accept().
    Create a new connected socket as the endpoint fo the connection between client and server.
    Accept a connection, blocking if one is not available until one connects
  */
    
  while(1){
    // Accept the connection request which creates a connection socket
    connectionSocket = accept(listenSocket, 
                (struct sockaddr *)&clientAddress, 
                &sizeOfClientInfo); 
    if (connectionSocket < 0){
      error("ERROR on accept");
    }

    printf("SERVER: Connected to client running at host %d port %d\n", 
                          htons(clientAddress.sin_addr.s_addr),
                          htons(clientAddress.sin_port));

    // Get the message from the client and display it
    memset(buffer, '\0', 256);


    /*
        STEP 6: read(), write(), send(), recv() to recieve and 
        send data to the client using the connected socket.
    */

    // Read the client's message from the socket
    charsRead = recv(connectionSocket, buffer, 255, 0); 
    if (charsRead < 0){
      error("ERROR reading from socket");
    }
    printf("SERVER: I received this from the client: \"%s\"\n", buffer);

    // Send a Success message back to the client
    charsRead = send(connectionSocket, 
                    "I am the server, and I got your message", 39, 0); 
    if (charsRead < 0){
      error("ERROR writing to socket");
    }

    /*
    // STEP 7: Close the connection socket for this client
    */
    
    close(connectionSocket); 
  }
  // Close the listening socket
  close(listenSocket); 
  return 0;
}
