# references: https://www.binarytides.com/python-socket-programming-tutorial/
# references: https://shakeelosmani.wordpress.com/2015/04/13/python-3-socket-programming-example/

import socket   # sockets
import sys      # exit

if len(sys.argv) != 2:
    print("Please enter a port number")
    print("Use: server.py PORTNUMBER")
    exit(1)

HOST = ''                   # all available interfaces
PORT = int(sys.argv[1])     # user-specified port
serverHandle = 'Serverer> ' # hardcoded handle

# create an AF_INET (IPv4), STREAM socket (TCP)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.bind((HOST, PORT))
s.listen(1)  # listen for 1 processes at a time only

connection, address = s.accept()
print('Connected with ' + address[0] + ':' + str(address[1]))

while True:
    data = connection.recv(1024).decode()

    # READ
    if ('\\quit' in data):  # unless client says quit, print the recieved message
        break
    else:
        print(data, end="") # remove trailing newline

    # REPLY
    reply = input("> ")

    while (len(reply) > 500 or len(reply) < 1):
        print("Your message must be 1-500 characters in length")
        reply = input("> ")

    if ('\\quit' in reply):  # unless server says quit, send message
        break

    reply = serverHandle + reply + '\n'

    connection.sendall(reply.encode())

connection.close()
s.close()
exit(0)
