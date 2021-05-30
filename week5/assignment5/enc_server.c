// Mateo Estrada
// CS344 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


/*
    This file will be used to build the encryption server. 
    It will use the following steps to build a server with the following call systems:
    bind()
    listen()
    accept()
    recv()
    send()
    close()

    Running: 
    ./enc_server listening_port
*/

// Make a global variable
// static const int BUFFERSIZE = 10000;
#define BUFFERSIZE (int)100000
// Function Prototypes:

int getMessageLength(int connectionSocket, char *buffer);
int getKeyLength(int connectionSocket, char *buffer);
void getMessageClient(int connectionSocket, char*message, char*buffer, int messageLength, char **arr, size_t *arr_len); // get message back
void getKeyClient(int connectionSocket, char*key, char*buffer, int keyLengthRecieved, char **arrKey, size_t *arr_len_key);
void encryptMessageFunction( char*message, char*key, char*encryptedMessage);
void sendEncodeMessage(int connectionSocket, char *arr, char *arrKey);
void printValues(int value);
void printMessage(char *arr, size_t arr_len);

// Error function used for reporting issues
void error(const char* msg){
    perror(msg);
    exit(1);
}

// CAPS Alphabet and space to use to review the plaintext file and the key for invalid character.
static const char alph[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";

// To allow to reuse my port with: http://beej.us/guide/bgnet/html/
int yes=1;
//char yes='1'; // Solaris people use this

// lose the pesky "Address already in use" error message
// if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) {
//     perror("setsockopt");
//     exit(1);
// } 

void printValues(int value){
    printf("[%d]", value);
}


void printMessage(char *arr, size_t arr_len){

    for(int i = 0; i< arr_len; i++){
        printf("%c", arr[i]);
    }
    
}
// This function will set up the address struct for the server socket
void setupAddressStruct(struct sockaddr_in* address, int portNumber){

    // Clear out the address struct.
    memset((char*) address, '\0', sizeof(*address));

    // The address should be netowkr capable.
    address->sin_family = AF_INET; // IPv4
    // Store the port number
    address->sin_port = htons(portNumber);
    // Allow enc_client address to connect to this server. 
    address->sin_addr.s_addr = INADDR_ANY;

}


int getMessageLength(int connectionSocket, char *buffer){
    // Get the length of the message from the client
    // 

    int charsRead, charsSend;
    int messageLength;

    // Recover the length of the incoming message
    memset(buffer, '\0', BUFFERSIZE);
    charsRead = recv(connectionSocket, buffer, sizeof(buffer) - 1, 0);
    if (charsRead < 0){
        fprintf(stderr, "Error reading from the socket.");
    }
    messageLength = atoi(buffer);
    // say that you got the length of the message
    charsSend = send(connectionSocket, "Connected to enc_server!", 24, 0);
    if(charsSend< 0){
        fprintf(stderr, "Error writing to the socket\n");
    }
   
    return messageLength;

}


void getMessageClient(int connectionSocket, char*message, char*buffer, int messageLength, char **arr, size_t *arr_len){
    // Get the message from the client.
    /* 
        To get the message back to the main() I used a method like so to be able to return a pointer to the
        char array: https://stackoverflow.com/questions/31060404/how-can-i-return-a-character-array-from-a-function-in-c
    */
    int charsRead, charsSend;

    memset(buffer, '\0', BUFFERSIZE);
    charsRead = recv(connectionSocket, buffer, BUFFERSIZE, 0);

    if(charsRead < 0){
        fprintf(stderr, "Error reading from the socket");
    }

    // Saving the message to the message array.
    strcat(message, buffer);
    messageLength -= strlen(buffer);
    while(messageLength != 0){
        if(strlen(buffer) == 0){
            
            break;
        }else{

            memset(buffer, '\0', BUFFERSIZE);
            charsRead = recv(connectionSocket, buffer, BUFFERSIZE, 0);

            if(charsRead < 0){
                fprintf(stderr, "Error reading from the socket");
            }
            messageLength -= strlen(buffer);
            strcat(message, buffer);
        
        }
    }
    // Once all the message has been read:
    char recievedAllMessage[22] = "I got all the message";
    charsRead = send(connectionSocket, recievedAllMessage, strlen(recievedAllMessage), 0);


    if(charsRead < 0){
        fprintf(stderr, "enc_server: Error sending to socket.\n");
    }
    char *messagePtr = malloc(sizeof(char)*BUFFERSIZE);
    // printf("message: [%d]\n", strlen(message));
    strncpy(messagePtr, message, strlen(message));
    // Making a pointer to return it
    
    *arr = messagePtr;
    *arr_len = strlen(message);
    
}

// continue here!!!


int getKeyLength(int connectionSocket, char *buffer){
    // Get the length of the key
    int charsRead, charsSend, lengthKey;
    memset(buffer, '\0', BUFFERSIZE);
    charsRead = recv(connectionSocket, buffer, BUFFERSIZE, 0);

    if(charsRead < 0){
        fprintf(stderr, "Error reading from socket.");

    }

    lengthKey = atoi(buffer);
    charsSend = send(connectionSocket, "Im enc_server and I got your key length", 39, 0 );

    if(charsSend< 0){
        fprintf(stderr, "Error writing to socket");
    }
    return lengthKey;

}

void getKeyClient(int connectionSocket, char*key, char*buffer, int keyLengthRecieved, char **arrKey, size_t *arr_len_key){
     // Get the message from the client.
    int charsRead, charsSend;

    memset(buffer, '\0', BUFFERSIZE);
    charsRead = recv(connectionSocket, buffer, BUFFERSIZE, 0);

    if(charsRead < 0){
        fprintf(stderr, "Error reading from the socket");
    }

    // Saving the message to the message array.
    strcat(key, buffer);
    keyLengthRecieved -= strlen(buffer);

    while(keyLengthRecieved != 0){
        if(strlen(buffer) == 0){
            break;
        }
        memset(buffer, '\0', BUFFERSIZE);
        charsRead = recv(connectionSocket, buffer, BUFFERSIZE, 0);

        if(charsRead < 0){
            fprintf(stderr, "Error reading from the socket");
            }

        keyLengthRecieved -= strlen(buffer);
        strcat(key, buffer);
    }
    // Once all the message has been read:
    char recievedAllKey[22] = "I got all the key";
    charsRead = send(connectionSocket, recievedAllKey, strlen(recievedAllKey), 0);
    if(charsRead < 0){
        fprintf(stderr, "enc_server: Error sending to socket.\n");
    }

    char *keyPtr = malloc(sizeof(char)*BUFFERSIZE);
    strncpy(keyPtr, key, strlen(key));

    *arrKey = keyPtr;
    *arr_len_key = strlen(key);
}

void encryptMessageFunction(char*message, char*key, char*encryptedMessage){
    /*
        I have to use the message and the key to encrypt it using the One-Time-Pads method:

    */
    int tokenLength;
    int tokenValue;
    int keyLength;
    int keyValue;

    char tempSpaceEncryptedMessage[BUFFERSIZE];
    char *messageEncrypted;

    // length of key and message
    tokenLength =strlen(message);
    keyLength = strlen(key);

    // Allocate memory to return the encrypted message
    // messageEncrypted = malloc(sizeof(char)*tokenLength);
  

    // saving the index:

    int alphabetIndexed[tokenLength];
    int keyIndexed[tokenLength];
    int encryptedMessageInt[tokenLength];

    // another method where I assign a value to each index of the letters

    // Letter to its corresponding index for message
    for (int i = 0; i < tokenLength; i++){
        for(int j = 0; j < 27; j++){
            if (message[i] == alph[j]){
                alphabetIndexed[i] = j;
                
            }
        }
    }

    // Letter to its corresponding index for key
    for (int i = 0; i < tokenLength; i++){
        for(int j = 0; j < 27; j++){
            if (key[i] == alph[j]){
                keyIndexed[i] = j;
                
            }
        }
    }

    // Run the actual encryption algorithm:
    for (int x = 0; x < tokenLength; x++){
        encryptedMessageInt[x] = (alphabetIndexed[x] + keyIndexed[x]) % 27;

    }

    // Change back to alphabet letters
    for (int ltr = 0; ltr<tokenLength; ltr++){
        encryptedMessage[ltr] = alph[encryptedMessageInt[ltr]];
        printf("%c", encryptedMessage[ltr]);
    }

    // Decrypting!
    // char decryptedMessage[tokenLength];
    // for(int k = 0; k<tokenLength; k++){

    //     decryptedMessage[k] = alphabetIndexed[k] - keyIndexed[k];
    //     if (decryptedMessage[k] < 0) {
    //         decryptedMessage[k] +=27;
    //     }

    // }
    // printf("Size: %d\n", sizeof(alphabetIndexed)/sizeof(alphabetIndexed[0]));
    // strcpy(messageEncrypted, tempSpaceEncryptedMessage);
    // printf("[decrypted: %s]\n", decryptedMessage);
    // printf("[encrypted: %s]", encryptedMessage);
}

void sendEncodeMessage(int connectionSocket, char *arr, char *arrKey){
    /*
     Send the encrypted message back to the client.
     this function will return an encrypted message in the form of an array.
     Uses encryptMessageFunction()
     arguments:
     connectionSocket
     arr: message from client
     arrKey: key from the client
    */ 
    int charsRead;
    // Pointers for char arrays for encrypted message:
    char *arrEncrypt;
    size_t arr_len_encrypt;
    char encryptedMessage[strlen(arr)];
    encryptMessageFunction(arr, arrKey, encryptedMessage);
    charsRead = send(connectionSocket, encryptedMessage, strlen(arr), 0);
    if (charsRead < 0){
        fprintf(stderr, "enc_server: Error writing to socket with encrypted message.\n");
    }
    if (charsRead < 20){
        printf("WARNING: not all data written to socket!\n");
    }

}


int main(int argc, char *argv[]){
    int ch = 0;
    char buffer[BUFFERSIZE] = "";
    char message[BUFFERSIZE] = "";
    char key[BUFFERSIZE] = "";
    int connectionSocket, charsRead;

    // I'm going to have to figure out how this will change size depending on the size 
    // of the chars in the key and plaintext file.
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t sizeOfClientInfo = sizeof(clientAddress);

    // check usage & args

    if (argc < 2){
        fprintf(stderr, "USAGE: %s ./enc_server PORT\n", argv[1]);
        exit(1);
    }

    /*
    Create the socket that will listen for connections.
    STEP 1: create the socket endpoint for enc_server.
    */

    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0){
        fprintf(stderr, "Error opening socket");
        exit(1);
    }

    /*
    Set up the address struct for the server socket:
    STEP 2: calls this function to set up a socket address structure
    sockaddr_in with the port number at which the server socket will listen for connections.
    serverAddress
    portNumber
    */

    setupAddressStruct(&serverAddress, atoi(argv[1]));

    /*
    Associate the socket to the port (binding the socket to the port)
    STEP 3: call bind() to associate the socket with the socket address.
    */
    if (bind(listenSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0){
        fprintf(stderr, "Error on binding the port to the socket!");
        exit(1);
    }

    /*
    STEP 4: call listen() on the socket to start listening for client connections, 
    this exists until the server process ends.
    Allow up to 5 connections to queue up. (different than the concurrent socket connections running processes).
    */
    listen(listenSocket, 5);
     /*
    STEP 5: Loop and accept connections on the listneing socket by calling accept().
    Create a new connected socket as the endpoint fo the connection between enc_client and enc_server.
    Accept a connection, blocking if one is not available until one connects
  */

  while(1){
    // Accept the connection request which creates a connection socket specifically with
      // enc_client
    connectionSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);
    if (connectionSocket < 0){
          fprintf(stderr, "Error on accepting the connection between this client and server.");
          exit(1);
    }
    /* 
        In order to achieve the multiple processes to make several encryptions at the same time.
        I will use the  One Process Per Client Connection method where the server fork()'s  a new process to handle each connection.
        Pros: simple
        cons: slow
        Resource: http://retis.sssup.it/~lipari/courses/OS_CP/sockets.pdf
    */ 
    
    ch = fork();
    if (ch < 0){
        fprintf(stderr, "Error: fork()\n");
        exit(1);
    }

    // If there are no errors with the fork() then proceed to make it.
    if (ch == 0){
        // Pointers for char arrays for the message & key unecrypted
        char *arr;
        size_t arr_len;
        char *arrKey;
        size_t arr_len_key;

        int messageLength = getMessageLength(connectionSocket, buffer);
        getMessageClient(connectionSocket, message, buffer, messageLength, &arr, &arr_len);
        int keyLength = getKeyLength(connectionSocket, buffer);
        getKeyClient(connectionSocket, key, buffer, keyLength, &arrKey, &arr_len_key);
        sendEncodeMessage(connectionSocket, arr, arrKey);
        // printf("MessageLength: [%d]\n", messageLength);
        // printf("KeyLength: [%d]\n", keyLength);
        // // printMessage(arr, arr_len);
        // printf("[%s]\n", arrKey);
        // printf("[%d]\n", arr_len);


        close(connectionSocket);
        free(arr);
        free(arrKey);
        exit(0);
    }
    close(connectionSocket);
    }
    close(listenSocket);

    return 0;


}