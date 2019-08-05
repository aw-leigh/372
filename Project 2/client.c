/* Andrew Wilson (wilsoan6) CS 372 Project 1
 * Simple chat client with socket programming
 * Last modified: 7/26/2019
 */

//reference https://medium.com/@yashitmaheshwary/simple-chat-server-using-sockets-in-c-f72fc8b5b24e

#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //For Sockets
#include <stdlib.h>
#include <netinet/in.h> //For the AF_INET (Address Family)

// clears stdin to remove trailing newlines
// pre:
// post: stdin is cleared
void clear_stdin(void){
    int c = getchar();
    while (c != EOF && c != '\n')
        c = getchar();
}

// prompts users for 10-character handle
// pre:
// post: client handle is saved to string
void getHandle(char * handle){
    printf("Please enter a handle of up to 10 characters: ");
    scanf("%s", handle);
    clear_stdin(); //remove newline in stdin
}

// prompts for a message from stdin, and sends to the server
// pre: need to be connected with the server
// post: message is sent to server
int sendMessage(char * clientHandle, char * message, int fd){
    char messageWithHandle[511]; 
    memset(messageWithHandle, '\0', sizeof(messageWithHandle));    

    //prompt for message
    printf("%s> ", clientHandle);
    fgets(message, 501, stdin);

    //prepend handle to message
    strcpy(messageWithHandle, clientHandle);
    strcat(messageWithHandle, "> ");
    strcat(messageWithHandle, message);

    //send it to server
    send(fd, messageWithHandle, strlen(messageWithHandle), 0);

    //if the message was "\quit", return 1 so we can exit the loop
    if(strcmp(message, "\\quit\n") == 0){
        memset(message, '\0', sizeof(message));
        return 1;
    }
    memset(message, '\0', sizeof(message));
    return 0;
}

// prints recieved message to stdout. Returns 1 if mesage is "\quit"
// pre: need to be connected with the server
// post: message from server is printed to stdout
int recieveMessage(char * message, int fd){
    
    //get message from server
    recv(fd,message,501,0);

    //if it's "\quit", return 1 to exit the loop
    if(strcmp(message, "Serverer> \\quit\n") == 0){
            return 1;
        }        
    
    //otherwise, print to stdout and clear the string
    printf("%s",message);
    memset(message, '\0', sizeof(message));
    return 0;
}
// creates TCP connection to sesrver
// pre: argv[1] must be host name, argv[2] must be port
// post: connection established with server
void setupConnection(int argc, char *argv[], int * fd){
    struct sockaddr_in chatServer; //main socket to store server details

    //create an create an AF_INET (IPv4), STREAM socket (TCP)
    *fd = socket(AF_INET, SOCK_STREAM, 0); 

    chatServer.sin_family = AF_INET;
    chatServer.sin_port = htons(atoi(argv[2])); //user specified port

    inet_pton(AF_INET, argv[1], &chatServer.sin_addr); //user specified host name
    connect(*fd, (struct sockaddr *)&chatServer, sizeof(chatServer));
}

int main(int argc, char *argv[]){

    int fd; //file descriptor that is used to idenfiy the socket
    char message[501]; 
    char clientHandle[11]; 
    memset(message, '\0', sizeof(message));
    memset(clientHandle, '\0', sizeof(message));
    
	if(argc != 3) {
		fprintf(stderr, "Usage: $ /client <server name> <port>");
		exit(1);
	}

    getHandle(clientHandle);
    setupConnection(argc, argv, &fd);

    while(1) {
        //send message, exiting loop if message was \quit (sendMessage returns 1)
        if(sendMessage(clientHandle, message, fd) == 1){
            break;
        }              
        //recieve message, exiting loop if message was \quit (recieveMessage returns 1)
        if(recieveMessage(message, fd) == 1){
            break;
        }
    }
    
    close(fd);
	return 0;
}