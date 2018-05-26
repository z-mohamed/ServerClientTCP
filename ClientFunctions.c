/*
Cpts 455 Project 1
Fall 2016
Programmers: Conner Mattingly
             Zakariya Mohamed

*/

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

  //copy first byte into len buffer
  memcpy(&len[0], &buffer[0], sizeof(char));

  //we are only guarantee one byte
  //so we must call recieve again if that is the case
  //in both senarios we set the second byte of our len buffer
  if(numBytesRcvd<2)
  {
    numBytesRcvd = recv(sock, &buffer[1], 1, 0);
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

  int accumulator = numBytesRcvd;

  while (accumulator < lengthString)
  {
    offset += numBytesRcvd;

    numBytesRcvd = recv(sock, &buffer[offset], 10, 0);

    accumulator+=numBytesRcvd;
  }

  memcpy(&value[0], &buffer[0], lengthString);


  return value;

}

int connect2Server(char *servIP ,  in_port_t servPort)
{
  // Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  //sock return -1 if it fails to create a socket
  if (sock < 0)
  {
    DieWithSystemMessage("socket() failed");
  }

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family

  // Convert address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
  {
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  }
  else if (rtnVal < 0)
  {
    DieWithSystemMessage("inet_pton() failed");
  }

  servAddr.sin_port = htons(servPort); // Server port

  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
  {
    DieWithSystemMessage("connect() failed");
  }

  return sock;

}

void nullTerminatedClient (int sock)
{
  //printf("\nSending null terminated string to the server\n");

  command current;

  current = commands[0];

  //set up buffer
  char buffer[256];
  memset(buffer,0,256);

  char cmdNum = '1';

  //copy cmd to buffer
  memcpy(&buffer[0], &cmdNum, sizeof(char));

  //get length of argument
  int length = strlen(current.arg);

  //copy argument to buffer
  memcpy(&buffer[1], current.arg,length);

  //set up null terminator charactor
  char null = '\0';

  //copy null terminator to buffer
  memcpy(&buffer[length+1], &null,sizeof(char));

  //send buffer to server
  ssize_t numBytes = send(sock, buffer, length + 2, 0);

  //printf("Recieving repy from server\n");

  char * value = receiveGivenLength(sock);

  printf("%s\n", value);

}

void givenLengthClient (int sock)
{
  //printf("Sending unterminated string to server \n");
  command current;

  current = commands[1];

  //set up buffer
  char buffer[256];
  memset(buffer,0,256);
  char cmdNum = '2';

  //copy cmd to buffer
  memcpy(&buffer[0], &cmdNum, sizeof(char));
  int stringLength = strlen(current.arg);

  uint16_t len = htons((uint16_t)stringLength);
  memcpy(&buffer[1], &len, sizeof(uint16_t));

  //copy argument to buffer
  memcpy(&buffer[3], current.arg,stringLength);

  ssize_t numBytes = send(sock, buffer, stringLength + 3, 0);


  //printf("Recieving repy from server\n");

  char * value = receiveGivenLength(sock);

  printf("%s\n", value);
}

void badIntClient(int sock)
{

  command current;

  //set up buffer
  char buffer[256];
  memset(buffer,0,256);

  current = commands[2];

  int number = atoi(current.arg);
  //printf("Sending bad Int(%d) to server \n", number);
  char cmdNum = '3';

  //copy cmd to buffer
  memcpy(&buffer[0], &cmdNum, sizeof(char));
  memcpy(&buffer[1], &number, sizeof(int));

  ssize_t numBytes = send(sock, buffer, 5, 0);

  //printf("Recieving repy from server\n");

  char * value = receiveGivenLength(sock);

  char buffer2[256];
  memset(buffer2,0,256);

  memcpy(&buffer2[0], &value[0],9);

  memcpy(&number, &value[9], sizeof(int));


  printf("%s", buffer2);
  printf("%d\n", number);

}

void goodIntClient(int sock)
{

  command current;

  //set up buffer
  char buffer[256];
  memset(buffer,0,256);

  current = commands[3];

  int number = atoi(current.arg);
  int numberSend = htonl(number);
  //printf("Sending good Int(%d) to server \n", number);
  char cmdNum = '4';

  //copy cmd to buffer
  memcpy(&buffer[0], &cmdNum, sizeof(char));
  memcpy(&buffer[1], &numberSend, sizeof(int));

  ssize_t numBytes = send(sock, buffer, 5, 0);

  //printf("Recieving repy from server\n");

  char * value = receiveGivenLength(sock);

  char buffer2[256];
  memset(buffer2,0,256);

  memcpy(&buffer2[0], &value[0],10);

  memcpy(&number, &value[10], 4);

  number = ntohl(number);

  printf("%s", buffer2);
  printf("%d\n", number);

}

void byteAtATimeClient(int sock)
{
  command current;
  //printf("byteAtATimeClient:\n" );

  //set up buffer
  char initialBuffer[256];
  char buffer[500000];
  memset(buffer,0,256);
  memset(buffer,1,500000);

  current = commands[4];

  int numberOfBytesToSend = atoi(current.arg);
  //printf("(%d)host byte \n", numberOfBytesToSend);
  int numberSend = htonl(numberOfBytesToSend); // convert to network byte order
  //printf("(%d)network byte \n", numberSend);
  char cmdNum = '5';

  //copy cmd to buffer
  memcpy(&initialBuffer[0], &cmdNum, sizeof(char));
  memcpy(&initialBuffer[1], &numberSend, sizeof(int));

//  printf("sending initialBuffer!\n");
  ssize_t numBytes = send(sock, initialBuffer, sizeof(int) + sizeof(char), 0);
  //printf("bytes sent: %d\n", numBytes);

  char byte[1];
  memset(byte,0,1);

  int bytesSent = 0;
  numBytes = 0;

  uint8_t b = 0;
  for(int i =0; i<numberOfBytesToSend; i++)
  {
    b = i%2;
    memcpy(&byte[0], &b, 1);
    //printf("byte: %d\n", byte[0]);
    //printf("i: %d\n", i);
    numBytes = send(sock, byte, 1, 0);
    //printf("numBytes: %d\n", numBytes);
    bytesSent += numBytes;
  }

  //printf("bytes Sent: %d\n", bytesSent);

  //printf("Recieving repy from server\n");


  char * value = receiveGivenLength(sock);

  char buffer2[256];
  int number =0;
  memset(buffer2,0,256);
  memcpy(&buffer2[0], &value[0],16);
  memcpy(&number, &value[16], sizeof(int));

  number = ntohl(number);

  printf("%s", buffer2);
  printf("%d\n", number);

}



void kByteAtATimeClient(int sock)
{
  command current;
  //printf("kByteAtATimeClient:\n" );

  //set up buffer
  char initialBuffer[256];
  char buffer[500000];
  memset(buffer,0,256);
  memset(buffer,1,500000);

  current = commands[5];

  int numberOfBytesToSend = atoi(current.arg);
  //printf("(%d)host byte \n", numberOfBytesToSend);
  int numberSend = htonl(numberOfBytesToSend); // convert to network byte order
  //printf("(%d)network byte \n", numberSend);
  char cmdNum = '6';

  //copy cmd to buffer
  memcpy(&initialBuffer[0], &cmdNum, sizeof(char));
  memcpy(&initialBuffer[1], &numberSend, sizeof(int));

  //printf("sending initialBuffer!\n");
  ssize_t numBytes = send(sock, initialBuffer, sizeof(int) + sizeof(char), 0);
  //printf("bytes sent: %d\n", numBytes);


  char bytes[1000];
  uint8_t b = 0;
  for (int i = 0; i < 1000; i++)
  {
    b = i%2;
    memcpy(&bytes[i], &b, 1);
    //printf("bytes[%d]: %d\n",i, bytes[i]);
  }

  int bytesSent = 0;
  numBytes = 0;
  for(int i =0; i<numberOfBytesToSend/1000; i++)
  {
    //printf("byte: %d\n", byte[0]);
    //printf("i: %d\n", i);
    numBytes = send(sock, bytes, 1000, 0);
    //printf("numBytes: %d\n", numBytes);
    bytesSent += numBytes;
  }
  //printf("bytes Sent: %d\n", bytesSent);

  //printf("Recieving repy from sdd XXXXXXserver\n");
  char * value = receiveGivenLength(sock);

  char buffer2[256];
  int number = 0 ;
  memset(buffer2,0,256);
  memcpy(&buffer2[0], &value[0],17);
  memcpy(&number, &value[17], sizeof(int));

  number = ntohl(number);

  printf("%s", buffer2);
  printf("%d\n", number);

}
