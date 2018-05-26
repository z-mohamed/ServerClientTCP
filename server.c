/*
Cpts 455 Project 1
Fall 2016
Programmers: Conner Mattingly
             Zakariya Mohamed

*/
#include "UtilityFunctions.c"
#include "ServerFunctions.c"

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int main(int argc, char *argv[])
{
    if (argc != 2) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Port>");

    in_port_t servPort = atoi(argv[1]); // First arg:  local port

    // open log file for logging

    // Create socket for incoming connections
    int servSock; // Socket descriptor for server
    if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithSystemMessage("socket() failed");

    // Construct local address structure
    struct sockaddr_in servAddr;                  // Local address
    memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
    servAddr.sin_family = AF_INET;                // IPv4 address family
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface
    servAddr.sin_port = htons(servPort);          // Local port

    // Bind to the local address
    if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
        DieWithSystemMessage("bind() failed");

    // Mark the socket so it will listen for incoming connections
    if (listen(servSock, MAXPENDING) < 0)
        DieWithSystemMessage("listen() failed");

    for (;;)
    {                                // Run forever
      struct sockaddr_in clntAddr; // Client address
        // Set length of client address structure (in-out parameter)
        socklen_t clntAddrLen = sizeof(clntAddr);

        // Wait for a client to connect
        int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntAddrLen);
        if (clntSock < 0)
            DieWithSystemMessage("accept() failed");

        // clntSock is connected to a client!

        char clntName[INET_ADDRSTRLEN]; // String to contain client address
        if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
                      sizeof(clntName)) != NULL)
            printf("\n\nHandling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
        else
            puts("Unable to get client address");

       //Set Up Switch Statement
        char buffer[1];
        memset(buffer,0,1);

        serverLog = open("log.txt", O_RDWR | O_APPEND | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
        printf("serverLog: %d\n", serverLog);

        for(int i = 1; i < 8; i++)
        {
          //printf("\n\nLoop %d\n", i);
          //Recieve some data from client
          //char *readBuffer = readAll(clntSock);

          ssize_t numBytesRcvd = recv(clntSock, buffer, 1, 0);
          write(serverLog, buffer, numBytesRcvd );

          if (numBytesRcvd == 0)
          {
            printf("For this connection %d bytes were read\n", serverBytesRead);
            close(serverLog);
            //printf("Closing client connection\n");
          }
          char cmd = buffer[0];
          //memcpy(&cmd,&buffer[0],sizeof(uint16_t));
          //cmd = ntohs(cmd);
          //printf("We read : %d bytes\n", numBytesRcvd);
          //printf("CMD bytes is: %c\n", cmd);
          char c = (char)buffer[0];

          //printf("switch on %c\n",c);
          switch (c)
          {
            case '1': // nullTerminatedCmd
                nullTerminatedServer(clntSock);
              break;
            case '2': // givenLengthCmd
                givenLengthServer (clntSock);
              break;
            case '3': // badIntCmd
                badIntServer(clntSock);
                //int
              break;
            case '4': // goodIntCmd
                goodIntServer(clntSock);
              break;
            case '5': // byteAtATimeCmd
                byteAtATimeServer(clntSock);
              break;
            case '6': // kByteAtATimeCmd
                //printf("HERE!!!!\n");
                kByteAtATimeServer(clntSock);
              break;
            default:
              break;
          }
          //free(readBuffer);
        }


    }
    // NOT REACHED
}
