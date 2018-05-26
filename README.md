# TCP Client and Server

A client and server that interacts over a TCP connection. The purpose of this project is to gain familiarity with the programming techniques involved in TCP. 

## Getting Started

1. The client program takes two command-line arguments that specify the IP address of the server and the port on which the server is listening. The server program takes a command-line argument that specifies the port on which it is to listen.

2. Start the server first. Once started, it will continue running exited using a
“Ctrl-C” signal (from the keyboard). When the client starts it will connect to the server.

3. Once the client has connected, it can send several different commands to the server, as described in the file project1.h. The client’s specific behavior is governed by the table commands[] in project1.h which lists the specific commands and arguments

### Prerequisites

```
GCC Complier
libc6-dev
```

### Running

A step by step series of examples that tell you how to get a development env running

Say what the step will be

```
1. Clone or download this repository

2. Modify file s on line 15 to listen to a port of your choosing currently it is set to listen on port 5004 
        ex: ./server <port> 

3. modify file c on line 17 to connect to server by changing the ip address to your local host and the port to the one you choose in step 2
        ex: ./client <localhost> <port>

4. Execute the s file in the command line 
        ex: ./s

5. In a separate terminal execute the c file in the command line
        ex: ./c 

```

## Authors

* **Zakariya Mohamed** - [zmohamed](https://github.com/z-mohamed)


## Acknowledgments

* **Billie Thompson** - *README Template* - [PurpleBooth](https://github.com/PurpleBooth)

