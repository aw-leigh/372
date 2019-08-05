#!/usr/bin/env python

# Andrew Wilson (wilsoan6) CS 372 Project 1
# Simple chat server with socket programming
# Last modified: 7/26/2019

# references: https://www.binarytides.com/python-socket-programming-tutorial/
# references: https://shakeelosmani.wordpress.com/2015/04/13/python-3-socket-programming-example/

import socket   # sockets
import sys      # exit, write

# Sets up TCP connection
# Pre: argv1 must be a port number
# Post: socket is set up
def setupConection():
    HOST = ''                   # all available interfaces
    PORT = int(sys.argv[1])     # user-specified port

    # create an AF_INET (IPv4), STREAM socket (TCP)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST, PORT))
    return s;

# Recieves message from remote host
# Pre: Must have connection with remote host
# Post: Message from remote host is printed to stdout
def recieveMessage():
    data = connection.recv(1024).decode()

    # READ
    if ('\\quit' not in data):  # unless client says quit, print the recieved message
        sys.stdout.write(data) # remove trailing newline

    return data;

# Sends message to remote host
# Pre: Must be connected to remote host
# Post: String message is sent to remote host
def sendMessage():
    serverHandle = 'Serverer> ' # hardcoded handle    
    reply = raw_input(serverHandle)

    while (len(reply) > 500 or len(reply) < 1):
        sys.stdout.write("Your message must be 1-500 characters in length")
        reply = raw_input("> ")

    reply = serverHandle + reply + '\n'
    connection.sendall(reply.encode())

    return reply;

####################
### MAIN PROGRAM ###
####################

if len(sys.argv) != 2:
    print("Please enter a port number")
    print("Use: server.py PORTNUMBER")
    exit(1)

s = setupConection()

while True:
    s.listen(1)  # listen for 1 processes at a time only
    connection, address = s.accept()

    while True:
        # READ
        data = recieveMessage()
        if ('\\quit' in data):  # unless client says quit, print the recieved message
            break

        # REPLY
        reply = sendMessage()       
        if ('\\quit' in reply):  # unless server says quit, send message
            break

    connection.close()
s.close()
exit(0)
