/* Andrew Wilson (wilsoan6) CS 372 Project 2
 * Simple FTP utility using socket programming
 * Last modified: 8/7/2019
 */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)

void setupConnection(int * fd, int port){

    //the following function is taken mostly directly from Beej's guide:
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    
    getaddrinfo(NULL, htons(port)), &hints, &res);

    *fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
}


int main(int argc, char *argv[]){

    int serverFD, controlConnectionFD, dataConnectionFD; //file descriptor that is used to idenfiy the socket
    struct sockaddr_storage controlAddress, dataAddress; //for storing connection info
    socklen_t ctrlAdrSize, dataAdrSize;

	if(argc != 2) {
		fprintf(stderr, "Usage: $ /ftserver <port>");
		exit(1);
	}

    setupConnection(&serverFD, atoi(argv[1]);

    //main server loop
    while(1){
        listen(serverFD, 1); //listen for 1 control connection max
        ctrlAdrSize = sizeof(controlAddress);
        controlConnectionFD = accept(serverFD, (struct sockaddr *)&controlAddress, &ctrlAdrSize);
    
    }
}