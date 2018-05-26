

void nullTerminatedServer(int clntSock)
{
  //printf("Commad NullTerminated\n");
  char buffer[256];
  memset(buffer,0,256);
  bool nullFound = false;
  int offset = 0;
  int x = 0;
  int length = 0;
  char * point;
  char buffOUT[256];
  memset(buffOUT,0,256);
  //printf("Recieving until null-terminator is recieved\n");
  while (!nullFound)
  {
    memset(buffer,0,256);
    ssize_t  numBytesRcvd = recv(clntSock, &buffer[offset], 10, 0);
    write(serverLog, buffer, numBytesRcvd );
    serverBytesRead += numBytesRcvd;
  //  printf("Number of bytes Recieve: %d\n", numBytesRcvd );
    for(int i = 0; i < numBytesRcvd; i++)
    {
      int numO = offset + i;
    //  printf("buffer[%d] = %c\n", numO, buffer[offset+i] );
      if(buffer[offset + i] == '\0')
      {
        nullFound = true;
        //p/rintf("Null-terminator recieved\n");
        length = offset + i;
        //point = buffer[0];

      //  printf("buffer %c \n", buffer[offset+i - 1] );
        //printf("buffout %c\n", buffOUT[0] );
      }
      if(!nullFound)
      {
        memcpy(&buffOUT[offset + i], &buffer[offset + i],sizeof(char));
        //printf("")
      }
      //nullFound = true;
      //offset += numBytesRcvd;
    }
    //x++;
    offset += numBytesRcvd;
  }
  //char buffOUT[256];
  //memset(buffOUT,0,256);
  //memcpy(&buffOUT[0], &buffer[0],length - 2);

  //get command name
  //printf("Preparing response\n");
  //char *command = commandNames[1];

  char *value = buffOUT;
  prepareSendResponse(1, value, clntSock);

}

void givenLengthServer (int clntSock)
{
  //printf("Command GivenLength!\n");
  char * value = receiveGivenLength(clntSock);
  ///printf("Recieved: %s\n", value);
  prepareSendResponse(2, value, clntSock);
}

void prepareSendResponse(int cmd, char *value, int clntSock)
{
  ///printf("Preparing response\n");
  char *command = commandNames[cmd];
  //printf("value = %s\n", value);
  //printf("commandName = %s\n", command);
  prepend(value, ": " );
  //printf("new value = %s\n", value);
  prepend(value, command );

  int newLEN = strlen(value);
  uint16_t networkLength = htons((uint16_t)newLEN);

  char sendBuffer[256];
  memset(sendBuffer,0,256);

  memcpy(&sendBuffer[0], &networkLength, sizeof(uint16_t));

//  printf("size of uint16_t: %d\n", sizeof(uint16_t));

  //get length of argument
  //int lengthValue = strlen(value);
  //printf("lengthValue222: %d\n", lengthValue);
  //copy argument to buffer
  //printf("Response ready:  %s\n", value);
  memcpy(&sendBuffer[2], value,strlen(value));
  //length = length + 2;
 // printf("Sending response\n");
  ssize_t numBytes = send(clntSock, sendBuffer, strlen(value)+2, 0);
  //printf("last char %c\n", buffOUT[50]);
  ///printf("Bytes sent: %d\n", numBytes);
}




void badIntServer(int clntSock)
{
  char *command = commandNames[3];
//  printf("Command BadInt!\n");

  char buffer[256];
  memset(buffer,0,256);
  //readBuffer = recv(clntSock, buffer2, 2, 0);
  //set up offset
  int offset = 0;

  //recieve 10 bytes
  ssize_t numBytesRcvd = recv(clntSock, &buffer[offset], 2, 0);
  write(serverLog, buffer, numBytesRcvd );
  serverBytesRead += numBytesRcvd;

  int accumulator = numBytesRcvd;

  while (accumulator < 4)
  {
    offset += numBytesRcvd;

    numBytesRcvd = recv(clntSock, &buffer[offset], 2, 0);
    write(serverLog, buffer, numBytesRcvd );
    serverBytesRead += numBytesRcvd;

    accumulator+=numBytesRcvd;
  }

  int number = 0;
  memcpy(&number, &buffer[0], sizeof(int));

  number = ntohl(number);
  //printf("Recieved: %d\n", number);

  //pr//intf("Preparing response\n");

  char *commadString =  malloc(2);
  memcpy(&commadString[0], ": ", 2);

  prepend(commadString, command );

  int commadStringLen = strlen(commadString);

  int totalLen = commadStringLen+ sizeof number;

  uint16_t networkLength = htons((uint16_t)totalLen);

  char sendBuffer[256];
  memset(sendBuffer,0,256);

  // copy length to sendBuffer
  memcpy(&sendBuffer[0], &networkLength, sizeof(uint16_t));
  // copy command string to sendBuffer
  memcpy(&sendBuffer[2], &commadString[0] ,commadStringLen);

  //copy number to sendBuffer
  memcpy(&sendBuffer[2 + commadStringLen], &number, sizeof(int));

  //printf("Response ready\n");

  //p//rintf("Sending response\n");

  ssize_t numBytes = send(clntSock, sendBuffer, totalLen + 2, 0);


  //printf("Bytes sent: %d\n", numBytes);

}

void goodIntServer(int clntSock)
{
  char *command = commandNames[4];
//  printf("Command GoodInt!\n");

  char buffer[256];
  memset(buffer,0,256);
  //readBuffer = recv(clntSock, buffer2, 2, 0);
  //set up offset
  int offset = 0;

  //recieve 10 bytes
  ssize_t numBytesRcvd = recv(clntSock, &buffer[offset], 2, 0);
  write(serverLog, buffer, numBytesRcvd );
  serverBytesRead += numBytesRcvd;

  int accumulator = numBytesRcvd;

  while (accumulator < 4)
  {
    offset += numBytesRcvd;

    numBytesRcvd = recv(clntSock, &buffer[offset], 2, 0);
    write(serverLog, buffer, numBytesRcvd );
    serverBytesRead += numBytesRcvd;

    accumulator+=numBytesRcvd;
  }

  int number = 0;
  memcpy(&number, &buffer[0], sizeof(int));


//  printf("Number is %d\n", number);

  number = ntohl(number);

  // printf("Number Now  is %d\n", number);
  // printf("Size of number: %d\n", sizeof number);
  // printf("Command: %s\n", command);
  // printf("size of Command: %d\n", sizeof command);

  char *commadString =  malloc(2);
  memcpy(&commadString[0], ": ", 2);

  prepend(commadString, command );


//rintf("commadString: _%s_\n", commadString);
  int commadStringLen = strlen(commadString);
//rintf("size of commadString: %d\n", commadStringLen);

  int totalLen = commadStringLen+ sizeof number;

  //printf("totalLen: % d\n", totalLen);
  uint16_t networkLength = htons((uint16_t)totalLen);

  number = htonl(number);
  //printf("Number Now Now Now Now  is %d\n", number);

  char sendBuffer[256];
  memset(sendBuffer,0,256);

  // copy length to sendBuffer
  memcpy(&sendBuffer[0], &networkLength, sizeof(uint16_t));
  // copy command string to sendBuffer
  memcpy(&sendBuffer[2], &commadString[0] ,commadStringLen);

  //copy number to sendBuffer
  memcpy(&sendBuffer[2 + commadStringLen], &number, sizeof number);

  ssize_t numBytes = send(clntSock, sendBuffer, totalLen + 2, 0);

  ///printf("Bytes sent: %d\n", numBytes);

}

void byteAtATimeServer(int clntSock)
{
  char *command = commandNames[5];
  //printf("Command byteAtATimeServer!\n");


  char initialBuffer[256];
  //char buffer[500000];
  memset(initialBuffer,0,256);
  int numberOfBytesToReceive = 0;

  ssize_t bytesRead = recv(clntSock, initialBuffer, 4, 0);
  write(serverLog, initialBuffer, bytesRead );
  serverBytesRead += bytesRead;
  //printf("bytesRead: %d\n", bytesRead);

  memcpy(&numberOfBytesToReceive, &initialBuffer[0], sizeof(int));
  numberOfBytesToReceive = ntohl(numberOfBytesToReceive);
  //printf("numberOfBytesToReceive: %d\n", numberOfBytesToReceive);

  char buffer[1];
  int bytesRecv = 0;
  ssize_t numBytes = 0;

  for(int i = 0; i<numberOfBytesToReceive; i++)
  {
    numBytes = recv(clntSock, buffer, 1, 0);
    write(serverLog, buffer, numBytes );
    serverBytesRead += numBytes;
    //printf("bytes read: %d\n", numBytes);
    //printf("buffer[0]: %d\n", buffer[0]);
    bytesRecv += numBytes;
  }
  //printf("bytesRecv %d\n", bytesRecv);

  char sendBuffer[256];
  memset(sendBuffer,0,256);


  // command prep
  char *commadString =  malloc(2);
  memcpy(&commadString[0], ": ", 2);
  prepend(commadString, command );
  //printf("commadString: _%s_\n", commadString);
  int commadStringLen = strlen(commadString);
  //printf("size of commadString: %d\n", commadStringLen);
  //printf("size of bytesRecv %d\n", sizeof bytesRecv);

  int totalLen = commadStringLen + sizeof bytesRecv;
  //printf("totalLen %d\n", totalLen );

  uint16_t networkLength = htons((uint16_t)totalLen);

  // // copy length to sendBuffer
  memcpy(&sendBuffer[0], &networkLength, sizeof(uint16_t));

  // // copy command string to sendBuffer
   memcpy(&sendBuffer[2], &commadString[0] ,commadStringLen);

  // //copy number to sendBuffer

  bytesRecv = htonl(bytesRecv);
  memcpy(&sendBuffer[2 + commadStringLen], &bytesRecv, sizeof bytesRecv);

  numBytes = send(clntSock, sendBuffer, totalLen + 2, 0);

  //printf("Bytes sent: %d\n", numBytes);

}


void kByteAtATimeServer(int clntSock)
{
  char *command = commandNames[6];
  ///printf("Command byteAtATimeServer!\n");

  char initialBuffer[256];
  //char buffer[500000];
  memset(initialBuffer,0,256);
  int numberOfBytesToReceive = 0;

  ssize_t bytesRead = recv(clntSock, initialBuffer, 4, 0);
  write(serverLog, initialBuffer, bytesRead );
  serverBytesRead += bytesRead;
  //printf("bytesRead: %d\n", bytesRead);

  memcpy(&numberOfBytesToReceive, &initialBuffer[0], sizeof(int));
  numberOfBytesToReceive = ntohl(numberOfBytesToReceive);
  //printf("numberOfBytesToReceive: %d\n", numberOfBytesToReceive);

  char buffer[1000];
  int bytesRecv = 0;
  ssize_t numBytes = 0;
  int readCount = 0;
  while(numberOfBytesToReceive > 0)
  {
    numBytes = recv(clntSock, buffer, 1000, 0);
    write(serverLog, buffer, numBytes );
    serverBytesRead += numBytes;
    readCount++;
    //printf("bytes read: %d\n", numBytes);
    //printf("buffer[0]: %d\n", buffer[0]);
    bytesRecv += numBytes;
    numberOfBytesToReceive = numberOfBytesToReceive - numBytes;
  }
  //printf("bytesRecv %d\n", bytesRecv);

  char sendBuffer[256];
  memset(sendBuffer,0,256);

  bytesRecv = readCount;
  // command prep
  char *commadString =  malloc(2);
  memcpy(&commadString[0], ": ", 2);
  prepend(commadString, command );
  //printf("commadString: _%s_\n", commadString);
  int commadStringLen = strlen(commadString);
  //printf("size of commadString: %d\n", commadStringLen);
  //printf("size of bytesRecv %d\n", sizeof bytesRecv);

  int totalLen = commadStringLen + sizeof bytesRecv;
  //printf("totalLen %d\n", totalLen );

  uint16_t networkLength = htons((uint16_t)totalLen);

  // // copy length to sendBuffer
  memcpy(&sendBuffer[0], &networkLength, sizeof(uint16_t));

  // // copy command string to sendBuffer
   memcpy(&sendBuffer[2], &commadString[0] ,commadStringLen);

  // //copy number to sendBuffer

  bytesRecv = htonl(bytesRecv);
  memcpy(&sendBuffer[2 + commadStringLen], &bytesRecv, sizeof bytesRecv);

  numBytes = send(clntSock, sendBuffer, totalLen + 2, 0);

  ///printf("Bytes sent: %d\n", numBytes);


}









































//
