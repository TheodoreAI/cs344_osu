### Assignment5: One-Time-Pads

### Description: This program is designed to encrypt and decrypt with the use of a key messages using the OTP method.



### Usage for Encryption::
    1. To use the encryption make sure the enc_server.c file is compiled and is listening:

        Compile:
        gcc --std=gnu99 -pthread -o enc_server enc_server.c // TO run enc_server

        listening:
        ./enc_server PORTNUMBER


    2. To speak to the server, make sure to compile the enc_client.c file:

        Compile:
        gcc --std=gnu99 -pthread -o enc_client enc_client.c

        Connect to server:  
        ./enc_client plaintextfile key PORTNUMBER (same as above)




### Usage for Decryption::
    1. To use the encryption make sure the dec_server.c file is compiled and is listening:

        Compile:
        gcc --std=gnu99 -pthread -o dec_server dec_server.c // TO run dec_server

        listening:
        ./dec_server PORTNUMBER


    2. To speak to the server, make sure to compile the dec_client.c file:

        Compile:
        gcc --std=gnu99 -pthread -o dec_client dec_client.c

        Connect to server:  
        .dec_client ciphertext key PORTNUMBER (same as above)

