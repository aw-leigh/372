/* Andrew Wilson (wilsoan6) CS 372 Project 2
 * Simple FTP utility using socket programming
 * Last modified: 8/7/2019
 */

#define _POSIX_C_SOURCE 200112L //https://stackoverflow.com/a/37545256

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> //For Sockets
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)

void setupConnection(int * fd, char * port){

    //the following function is taken mostly directly from Beej's guide:
    struct addrinfo hints;
    struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    
    getaddrinfo(NULL, port, &hints, &res);

    *fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(*fd < 0){
        fprintf(stderr, "could not create socket");
        exit(1);
    }
    if(bind(*fd, res->ai_addr, res->ai_addrlen) < 0){
        fprintf(stderr, "could not bind");
        exit(1);
    }
    if(listen(*fd, 1) < 0){
        fprintf(stderr, "could not listen");
		exit(1);
    }
}

void handleRequest(int controlConnectionFD){
    int dataConnectionFD;
    int numCommands = 0;
    struct sockaddr_storage dataAddress;
    socklen_t dataAdrSize;
    char * commands[5]; //used to store commands from client. There should be a max of 3, but added extra just to be safe
    char * token;
    char buffer[1028];
    memset(buffer, "\0", sizeof(buffer));

    recv(controlConnectionFD, buffer, sizeof(buffer), 0);

    //token is null when string is empty, so this will loop over entire string
    //saving each space-delimited argument into the array
    token = strtok(buffer, " ");
    while (token != NULL)
    {
        commands[numCommands] = token;
        numCommands++;
        token = strtok(NULL, " ");
    }
}

int main(int argc, char *argv[]){

    int serverFD, controlConnectionFD; //file descriptor that is used to idenfiy the socket
    struct sockaddr_storage controlAddress; //for storing connection info
    socklen_t ctrlAdrSize;


	if(argc != 2) {
		fprintf(stderr, "Usage: $ /ftserver <port>");
		exit(1);
	}

    setupConnection(&serverFD, argv[1]);
    printf("Server open on port %s\n", argv[1]);

    //main server loop
    while(1){
        ctrlAdrSize = sizeof(controlAddress);
        controlConnectionFD = accept(serverFD, (struct sockaddr *)&controlAddress, &ctrlAdrSize);
        printf("connected to something");

        handleRequest(controlConnectionFD);
    }
}