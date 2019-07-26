/* Andrew Wilson (wilsoan6) CS 372 Project 1
 * Simple chat client with socket programming
 * Last modified; 7/26/2019
 */



//reference https://medium.com/@yashitmaheshwary/simple-chat-server-using-sockets-in-c-f72fc8b5b24e



#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)

void clear_stdin(void){
    int c = getchar();
    while (c != EOF && c != '\n')
        c = getchar();
}

int main(int argc, char *argv[]){

    struct sockaddr_in chatServer; //main socket to store server details
    int fd; //file descriptor that is used to idenfiy the socket
    int connection; //connection file descriptor
    char message[501]; 
    char messageWithHandle[511]; 
    char clientHandle[11]; 
    memset(message, '\0', sizeof(message));
    memset(messageWithHandle, '\0', sizeof(messageWithHandle));
    memset(clientHandle, '\0', sizeof(message));
    
	if(argc != 3) {
		fprintf(stderr, "Usage: $ /client <server name> <port>");
		exit(1);
	}

    printf("Please enter a handle of up to 10 characters: ");
    scanf("%s", clientHandle);
    clear_stdin(); //remove newline in stdin

    //create an create an AF_INET (IPv4), STREAM socket (TCP)
    fd = socket(AF_INET, SOCK_STREAM, 0); 
    if(fd < 0)
    {
        perror("Client Error: Socket not created succesfully");
        exit(1);
    }

    chatServer.sin_family = AF_INET;
    chatServer.sin_port = htons(atoi(argv[2])); //user specified port

    inet_pton(AF_INET, argv[1], &chatServer.sin_addr); //change from localhost later
    connect(fd, (struct sockaddr *)&chatServer, sizeof(chatServer));

    while(1) {
    //send message
        printf("%s> ", clientHandle);
        fgets(message, 501, stdin);

        //prepend handle to message
        strcpy(messageWithHandle, clientHandle);
        strcat(messageWithHandle, "> ");
        strcat(messageWithHandle, message);

        //send message, then quit if \quit was entered
        send(fd, messageWithHandle, strlen(messageWithHandle), 0);
        if(strcmp(message, "\\quit\n") == 0){
            break;
        }        
        memset(message, '\0', sizeof(message));
        memset(messageWithHandle, '\0', sizeof(messageWithHandle));

    //recieve message
        recv(fd,message,501,0);
        if(strcmp(message, "Serverer> \\quit\n") == 0){
            break;
        }        
        printf("%s",message);
        memset(message, '\0', sizeof(message));
    }
    
    close(fd);
	return 0;
}