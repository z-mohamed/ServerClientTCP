

#include "project.h"
//prepends prependPiece into actualPiece
void prepend(char* actualPiece, const char* prependPiece )
{
    size_t len = strlen(prependPiece);
    size_t i;
    memmove(actualPiece + len, actualPiece, strlen(actualPiece) + 1);

    for (i = 0; i < len; ++i)
    {
        actualPiece[i] = prependPiece[i];
    }
}

char * receiveGivenLength(int sock)
{

  char * value = (char *) malloc(sizeof(char) * 256);

  //set up buffer
  char buffer[256];
  memset(buffer,0,256);

  // will store first two bytes of incoming buffer here
  char len[2];

  //Recieve two bytes
  ssize_t  numBytesRcvd = recv(sock, buffer, 2, 0);
  write(serverLog, buffer, numBytesRcvd);
  serverBytesRead += numBytesRcvd;

  //copy first byte into len buffer
  memcpy(&len[0], &buffer[0], sizeof(char));

  //we are only guarantee one byte
  //so we must call recieve again if that is the case
  //in both senarios we set the second byte of our len buffer
  if(numBytesRcvd<2)
  {
    numBytesRcvd = recv(sock, &buffer[1], 1, 0);
    write(serverLog, buffer, numBytesRcvd);
    serverBytesRead += numBytesRcvd;
    memcpy(&len[1], &buffer[1], sizeof(char));
  }
  else
  {
    memcpy(&len[1], &buffer[1], sizeof(char));
  }


  //now we have everything to construct
  //the incomming string length
  uint16_t lengthString = 0;

  //copy first two bytes to stringlength
  memcpy(&lengthString,&len[0],2);


  //change stringlength to host byte order
  lengthString = ntohs(lengthString);

  memset(buffer,0,256);

  //set up offset
  int offset = 0;

  //recieve 10 bytes
  numBytesRcvd = recv(sock, &buffer[offset], 10, 0);
  write(serverLog, buffer, numBytesRcvd);
  serverBytesRead += numBytesRcvd;

  int accumulator = numBytesRcvd;

  while (accumulator < lengthString)
  {
    offset += numBytesRcvd;

    numBytesRcvd = recv(sock, &buffer[offset], 10, 0);
    write(serverLog, buffer, numBytesRcvd);
    serverBytesRead += numBytesRcvd;

    accumulator+=numBytesRcvd;
  }

  memcpy(&value[0], &buffer[0], lengthString);


  return value;

}
