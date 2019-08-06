#!/usr/bin/env python

# Andrew Wilson (wilsoan6) CS 372 Project 2
# Simple FTP utility using socket programming
# Last modified: 8/7/2019

# references: https://www.binarytides.com/python-socket-programming-tutorial/
# references: https://shakeelosmani.wordpress.com/2015/04/13/python-3-socket-programming-example/

import socket   # sockets
import sys      # exit, write

# Sets up TCP socket for recieving data from server
# Pre: argv1 must be a port number
# Post: socket is set up
def setupDataPort():
    HOST = ''                       # all available interfaces

    if len(sys.argv) == 5:          # user-specified port
        PORT = int(sys.argv[4])
    else:
        PORT = int(sys.argv[5])

    # create an AF_INET (IPv4), STREAM socket (TCP)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    s.bind((HOST, PORT))
    return s


# Sets up TCP socket to connect to remote server
# Pre: argv1 must be a port number
# Post: socket is set up
def connectToServer():

    # create an AF_INET (IPv4), STREAM socket (TCP)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    host = sys.argv[1]
    port = int(sys.argv[2])

    try: #get IP address of host
	remote_ip = socket.gethostbyname(host) 

    except socket.gaierror:
        #could not get IP address
        print 'Could not get hostname IP address ;('
        sys.exit()

    #Connect to remote server
    s.connect((remote_ip , port))

    return s
    
####################
### MAIN PROGRAM ###
####################

if (len(sys.argv) < 5 or len(sys.argv) > 6):
    print(len(sys.argv))
    print("Use: ftclient.py <SERVER_HOST> <SERVER_PORT> <COMMAND> (<FILENAME>) <DATA_PORT>")
    exit(1)

dataSocket = setupDataPort()
serverSocket = connectToServer()

# send request to server
if(len(sys.argv) == 5):
    message = sys.argv[3] + " " + sys.argv[4]
else:
    message = sys.argv[3] + " " + sys.argv[4] + " " + sys.argv[5]

serverSocket.sendall(message)

#setup data port to revieve reply
while True:
    dataSocket.listen(1)  # listen for 1 processes at a time only
    connection, address = dataSocket.accept()

    while True:
        data = recieveMessage()
    
    print(data)
    connection.close()

serverSocket.close()
dataSocket.close()
exit(0)