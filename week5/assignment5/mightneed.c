
    // Loop through the entire message:
    // for(int i = 0; i < tokenLength - 1; i++){
    //     // check to see if both characters are letters
    //     if(message[i] != ' ' && key[i] != ' '){
    //         tokenValue = message[i] - 65;
    //         keyValue = key[i] - 65;
    //         tempSpaceEncryptedMessage[i] = ("%c", alph[(tokenValue + keyValue)] % 27);
    //     // check to see if the key letter is the space 
    //     }else if(message[i] != ' ' && key[i] == ' '){
    //         tokenValue = message[i] - 65;
    //         keyValue = key[i] - 6;
    //         tempSpaceEncryptedMessage[i] = ("%c", alph[(tokenValue + keyValue)] % 27);
    //     // check to see if the message letter is the space
    //     }else if(message[i] == ' ' && key[i] != ' '){
    //         tokenValue = message[i] - 6;
    //         keyValue = key[i] - 65;
    //         tempSpaceEncryptedMessage[i] = ("%c", alph[(tokenValue + keyValue)] % 27);
            
    //     }else{
    //         // if both are the space
    //         tokenValue = message[i] - 6;
    //         keyValue = key[i] - 6;
    //         tempSpaceEncryptedMessage[i] = ("%c", alph[(tokenValue + keyValue)] % 27);
            
    //     }
    //     printf("%c", tempSpaceEncryptedMessage[i]);

    // }