#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>  // ssize_t
#include <sys/socket.h> // send(),recv()
#include <netdb.h>      // gethostbyname()

// Global variables
#define BUFFERSIZE (int)100000

// Function prototypes
void setupAddressStruct(struct sockaddr_in *address, int portNumber);
int sendMessageLength(int socketFD, char *message, char *buffer, int plaintextLength);
int sendMessage(int socketFD, char *message, char* buffer);
int sendKeyLength(int socketFD, char *key, char*buffer, int keySize);
int sendKey(int socketFD, char*key, char*buffer);
int getEncodedMsgServer(int socketFD, char*buffer, int plaintextLength);

// CAPS Alphabet and space to use to review the plaintext file and the key for invalid character.
static const char alph[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

/**
* enc_client code
* 1. Create a socket and connect to the server specified in the command arugments.
* 2. Prompt the user for input and send that input as a message to the server.
* 3. Print the message received from the server and exit the program.
*/


// Error function used for reporting issues.
void error(const char *msg){
  fprintf(stderr, msg);
  exit(0);
}

void setupAddressStruct(struct sockaddr_in *address, int portNumber){
  // Setting up the address struct
  memset((char*) address, '\0', sizeof(*address));

  // The address should be network capable
  address->sin_family = AF_INET;

  // Store the port number
  address->sin_port = htons(portNumber);

  // Get the DNS entry for this hostname
    
  struct hostent *hostInfo = gethostbyname("localhost");
  if (hostInfo == NULL){
    fprintf(stderr, "enc_client: ERROR, no such host\n");
    exit(0);
  }

  // Copy the first IP address from the DNS entry to sin_addr.s_addr
  memcpy((char*) &address->sin_addr.s_addr, 
                  hostInfo->h_addr_list[0], 
                  hostInfo->h_length);
}


int sendMessageLength(int socketFD, char *message, char *buffer, int plaintextLength){
   /*
                      SEND MESSAGE LENGTH:
    STEP 4:  I am going to send the message length to use it to make sure how much to iterate.
    And also recv() message from the server.
  */

  int charsWritten, charsRead;
  char strLength[BUFFERSIZE];
  sprintf(strLength, "%d", plaintextLength);
  charsWritten = send(socketFD, strLength, strlen(strLength), 0);

  if (charsWritten < strlen(strLength)){
    fprintf(stderr, "enc_client: Error writing to socket\n");
  }

  // if (charsWritten < strlen(message)){
  //   printf("Warning: not all plaintext file content passed to server. \n");
  // }

  // Get return message from server
  // clear array again for resure
  memset(buffer, '\0', BUFFERSIZE);
   /*
    The client recieves data from the server socket through the recv() function.
    Data may arrive in odd size bundles, we may need to call the recv() function multiple times
    to read the complete message sent on the socket.
    If the size of the message is unknown or varies, special characters can be designated to mark termination of the message.
  */
  charsRead = recv(socketFD, buffer, BUFFERSIZE, 0);
  if(charsRead < 0 ){
    fprintf(stderr, "enc_client: ERROR reading from socket length\n");
  }
  // printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
  return 0;

}


int sendMessage(int socketFD, char *message, char *buffer){
  /*
                      SEND MESSAGE
    STEP 4:  I am going to send the message with the \n character to use it as a stoppage point on the server.
    And also recv() message from the server.
  */
  int charsWritten, charsRead;
  charsWritten = send(socketFD, message, strlen(message), 0);

  if (charsWritten < 0){
    fprintf(stderr, "enc_client: Error writing to socket\n");
  }

  // if (charsWritten < strlen(message)){
  //   printf("Warning: not all plaintext file content passed to server. \n");
  // }

  // Get return message from server
  // clear array again for resure
  memset(buffer, '\0', BUFFERSIZE);
   /*
    The client recieves data from the server socket through the recv() function.
    Data may arrive in odd size bundles, we may need to call the recv() function multiple times
    to read the complete message sent on the socket.
    If the size of the message is unknown or varies, special characters can be designated to mark termination of the message.
  */
  charsRead = recv(socketFD, buffer, BUFFERSIZE, 0);
  if(charsRead < 0 ){
    fprintf(stderr, "enc_client: ERROR reading from socket msg\n");
  }
  // printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
  return 0;
}



int sendKeyLength(int socketFD, char*key, char*buffer, int keySize){
   /*
                      SEND KEY LENGTH
    STEP 4:  I am going to send the message with the \n character to use it as a stoppage point on the server.
    And also recv() message from the server.
  */

  int charsWritten, charsRead;
  char keyLength[BUFFERSIZE]; // turn an int into a char to send
  sprintf(keyLength, "%d", keySize);
  charsWritten = send(socketFD, keyLength, strlen(keyLength), 0); // Send keylength
  if (charsWritten < 0){
    fprintf(stderr, "enc_client: Error writing to socket\n");
  }
  // if (charsWritten < strlen(key)){
  //   printf("Warning: not all plaintext file content passed to server. \n");
  // }
    // clear array again for resure
  memset(buffer, '\0', BUFFERSIZE);
    charsRead = recv(socketFD, buffer, BUFFERSIZE, 0);

  if(charsRead < 0 ){
    fprintf(stderr, "enc_client: ERROR reading from socket\n");
  }
}


int sendKey(int socketFD, char*key, char*buffer){
   /*
                      SEND KEY
    STEP 4:  I am going to send the key.
  */
  int charsWritten, charsRead;
  charsWritten = send(socketFD, key, strlen(key), 0);

  if (charsWritten < 0){
    fprintf(stderr, "enc_client: Error writing to socket\n");
  }

  // if (charsWritten < strlen(key)){
  //   printf("Warning: not all plaintext file content passed to server. \n");
  // }

  // Get return message from server
  // clear array again for resure
  memset(buffer, '\0', BUFFERSIZE);
   /*
    The client recieves data from the server socket through the recv() function.
    Data may arrive in odd size bundles, we may need to call the recv() function multiple times
    to read the complete message sent on the socket.
    If the size of the message is unknown or varies, special characters can be designated to mark termination of the message.
  */
  charsRead = recv(socketFD, buffer, BUFFERSIZE, 0);
  if(charsRead < 0 ){
    fprintf(stderr, "enc_client: ERROR reading from socket msg\n");
  }
  // printf("CLIENT: I received this from the server: \"%s\"\n", buffer);
  return 0;
}

int getEncodedMsgServer(int socketFD, char*buffer, int messageLength){
    /*
      This function is used to retrive the encrypted message from the enc_server.
      Should the message should be able to be outputted to a file?
      messageLength: the length of the message originally sent, should stay the same as the 
      length of the encrypted message.
    */
  int charsRead, charsSend;
  char encMessage[BUFFERSIZE]="";

  memset(buffer, '\0', BUFFERSIZE); // clear buffer
  charsRead = recv(socketFD, buffer, BUFFERSIZE, 0); // place message in buffer
  
  if(charsRead < 0){
    fprintf(stderr, "Error reading from the socket");
  }
  strcat(encMessage, buffer); // save the message in encMessage
  messageLength -= strlen(buffer); // decrement messageLength until 0
  while(messageLength != 0){
    if(strlen(buffer) == 0){
      break;
    }else{
            memset(buffer, '\0', BUFFERSIZE); // clear buffer again
            charsRead = recv(socketFD, buffer, BUFFERSIZE, 0); // save in buffer again
            if(charsRead < 0){
                fprintf(stderr, "Error reading from the socket");
            }
            messageLength -= strlen(buffer); // decrement
            strcat(encMessage, buffer); // save and repeat
        }
    }
    // printf("encryption: [%s]\n", encMessage);

    
    // Once all the message has been read:
    // char recievedAllMessage[31] = "I got all the encrypted message";
    // charsRead = send(socketFD, recievedAllMessage, strlen(recievedAllMessage), 0);
    // if(charsRead < 0){
    //     fprintf(stderr, "enc_server: Error sending to socket.\n");
    // }
    // Attach a \n at the end
    encMessage[strcspn(encMessage, "\0")] = '\n';
    fprintf(stdout, encMessage);
    
  return 0;
}




int main(int argc, char*argv[]){
  int socketFD, portNumber, charsWritten, charsRead;
  FILE* filename;
  FILE* keyfile;
  struct sockaddr_in serverAddress;
  char buffer[BUFFERSIZE] = "";
  char message[BUFFERSIZE] = "";
  char key[BUFFERSIZE] = "";



  // Check the usage & arguments

  if(argc < 3){
    fprintf(stderr, "USAGE: %s filename key port", argv[0]);
    exit(0);
  }

  /*
    Create a socket: STEP 1: creates the socket endpoint for the client by calling this function.
  */

  socketFD = socket(AF_INET, SOCK_STREAM, 0);
 
  if(socketFD < 0){
    fprintf(stderr, "enc_client: ERROR opening socket!\n");
  }

  /*
    STEP 2: sets up a socket address structure sockaddr_in with
    arg        -> address
    argv[1] -> filename:
    argv[2] -> keyfile:
    argv[3] -> portNumber;
    optional -> filename output (stdout)
  */
  
  setupAddressStruct(&serverAddress, atoi(argv[3]));

/*
   Connect to server: STEP 3: Calls the function connect to connect the client socket to the server socket. After this
    It should be ready for reading and writing.
    socketFD:
    serverAddress:
    sizeof(serverAddress):
*/

  if(connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
    fprintf(stderr, "enc_client: Error connecting\n");
  }


  // Open the file passed as an argument
  filename = fopen(argv[1], "r");

  if (filename == NULL){
    fprintf(stderr, "Error opening filename\n");
  }
  // Get the content from the file and place it on messge to be send()' to the enc_server.
  fgets(message, BUFFERSIZE, filename);
  fclose(filename);
  message[strcspn(message, "\n")] = '\0'; // remove the newline char

  // Perform the same for the key to get it and send it to the server
  keyfile = fopen(argv[2], "r");

  if (keyfile == NULL){
    fprintf(stderr, "Error with keyfile\n");

  }

    // Get the content from the file and place it on a key to be send()
  fgets(key, BUFFERSIZE, keyfile);
  fclose(keyfile);
  key[strcspn(key, "\n")] = '\0'; // remove the newline char
  // Checking that the key is not shorter than the plaintext file.

  int plaintextLength = strlen(message);
  int keySize = strlen(key);

  // Key and message review 1: making sure that the key is as long or longer than message.
  if(keySize < plaintextLength){
    fprintf(stderr, "ERROR: key is shorter than the plaintext.\n key: [%d] \n plaintext: [%d]\n", keySize, plaintextLength);
    exit(1);
  } 

  /*
     Making sure that key and message only have capital letters from alphabet and the space.
     Valid characters only.
  */

  for (int i = 0;  i< plaintextLength; i++){
    for(int j = 0; j < 28; j++){
      if (j == 27){
        fprintf(stderr, "Error: plaintext file has an invalid character.\n");
        exit(1);
      }
      if(message[i] == alph[j]){
        break;
      }
    }
  }
 
  // Same thing for key
  for (int i = 0;  i< keySize; i++){
    for(int j = 0; j < 28; j++){
      if (j == 27){
        fprintf(stderr, "Error: key file has an invalid character.\n");
        exit(1);
      }
      if(key[i] == alph[j]){
        break;
      }
    }
  }

  sendMessageLength(socketFD, message, buffer, plaintextLength);
  sendMessage(socketFD, message, buffer);
  sendKeyLength(socketFD, key, buffer, keySize);
  sendKey(socketFD, key, buffer);
  // Remember that the size of the message and the size of the encrypted message will be the same so you can use this.
  getEncodedMsgServer(socketFD, buffer, plaintextLength); 
  close(socketFD);

 return 0;
}



