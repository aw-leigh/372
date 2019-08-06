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
#include <dirent.h> //for directory listing

void setupServer(int * fd, char * port){

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

void setupClientConnection (int argc, char *argv[], int * fd){
    struct sockaddr_in chatServer; //main socket to store server details

    //create an create an AF_INET (IPv4), STREAM socket (TCP)
    *fd = socket(AF_INET, SOCK_STREAM, 0); 

    chatServer.sin_family = AF_INET;
    chatServer.sin_port = htons(atoi(argv[2])); //user specified port

    inet_pton(AF_INET, argv[1], &chatServer.sin_addr); //user specified host name
    connect(*fd, (struct sockaddr *)&chatServer, sizeof(chatServer));
}

// adapted from: https://www.geeksforgeeks.org/c-program-list-files-sub-directories-directory/
void printDirectoryContentsToString(char contents[]){
    struct dirent *de; // Pointer for directory entry
    char name[64];

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(".");

    if (dr == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory");
        exit(1);
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html for readdir()
    while ((de = readdir(dr)) != NULL)
    {
        if(!(strcmp(de->d_name, "..") == 0 || strcmp(de->d_name, ".") == 0))
        {
            sprintf(name, "%s\n", de->d_name);
            strcat(contents, name);
        }
    }
    //printf("%s", contents);

    closedir(dr);
}

void sendDirectoryContents(){
    char contents[1000];
    memset(contents, '\0', sizeof(contents));
    int bytesSent, messageLength;

    printDirectoryContentsToString(contents);
    messageLength = strlen(contents);

    //bytesSent = send(, contents, messageLength, 0);

}

void handleRequest(int controlConnectionFD, char * clientHost){
    int dataConnectionFD;
    struct sockaddr_storage dataAddress;
    socklen_t dataAdrSize;
    char buffer[1028];
    char * commands[5]; //used to store commands from client. There should be a max of 3, but added extra just to be safe
    int numCommands = 0;
    char * token;

    memset(buffer, '\0', sizeof(buffer));

    //recieve commands from client into buffer
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
    commands[4] = clientHost; //add hostname to last place in commands array

    if(strcmp(commands[0], "-l") == 0){
        // printf("List directory requested on port %d", clientPort);
        // printf("Sending directory contents to %s:%d", clientHost, clientPort);
        //commands should be "-l <port>"
        printf("Sending directory contents to %s:%s\n", commands[4], commands[1]);
        sendDirectoryContents();
    }
    else if(strcmp(commands[0], "-g")==0){
        strcpy(clientPort, commands[2]); //commands should be "-g <filename> <port>"
        //send file
    }
    else{
        //error
    }
    close(dataConnectionFD);
    close(controlConnectionFD);
}

int main(int argc, char *argv[]){

    int serverFD, controlConnectionFD; //file descriptor that is used to idenfiy the socket
    struct sockaddr_storage controlAddress; //for storing connection info
    socklen_t ctrlAdrSize;
    char clientHost[128];
    char clientPort[16];
    char *token;


	if(argc != 2) {
		fprintf(stderr, "Usage: $ /ftserver <port>\n");
		exit(1);
	}

    setupServer(&serverFD, argv[1]);
    printf("Server open on port %s\n", argv[1]);

    //main server loop
    while(1){
        //accept connection
        ctrlAdrSize = sizeof(controlAddress);
        controlConnectionFD = accept(serverFD, (struct sockaddr *)&controlAddress, &ctrlAdrSize);
        
        //extract & print the hostname
        getnameinfo((struct sockaddr *)&controlAddress, ctrlAdrSize, clientHost, 128, clientPort, 16, 0);
        token = strtok(clientHost, ".");
        strcpy(clientHost, token);
        printf("Connection from %s\n", clientHost);

        handleRequest(controlConnectionFD, clientHost);
    }
}