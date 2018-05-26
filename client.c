/*
Cpts 455 Project 1
Fall 2016
Programmers: Conner Mattingly
             Zakariya Mohamed

*/
#include "project.h"
#include "ClientFunctions.c"


int main(int argc, char *argv[])
{
    if (argc != 3) // Test for correct number of arguments
    {   DieWithUserMessage("Parameter(s)",
                               "<Server Address> <[<Server Port>]");
    }

    char *servIP = argv[1];     // First arg: server IP address (dotted quad)
    //char *echoString = argv[2]; // Second arg: string to echo

    // Third arg (optional): server port (numeric). 7 is well-known echo port
    in_port_t servPort = (argc == 3) ? atoi(argv[2]) : 7;

    int sock = connect2Server(servIP, servPort);

    //We are conncected
    // Show Client Command Menu

    command current;
    bool flag = true;
    int i = 0;
    char *cmdARG = malloc(24);
    char buffer[256];
    memset(buffer,0,256);

    while (flag)
    {
      current = commands[i];

      //printf("\n\nInitial Commad Value: %s\n\n", current.arg);
      //printf("Initial Commad Number: %d\n\n", current.cmd);

      switch (current.cmd)
      {
        case 0:
          //printf("Break Son\n");
          flag = false;
          break;

        case 1:
          nullTerminatedClient(sock);
          break;

        case 2:
          givenLengthClient(sock);
          break;

        case 3:
          badIntClient(sock);
          break;

        case 4:
          goodIntClient(sock);
          break;

        case 5:
            byteAtATimeClient(sock);
          break;

        case 6:
            kByteAtATimeClient(sock);

        default:
          //printf("Standard\n");
          break;

      }
      //goodIntClient(int sock)
      i++;
    }

    close(sock);

    exit(0);
}
