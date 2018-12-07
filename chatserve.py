'''
Socket server: using code from http://www.binarytides.com/python-socket-server-code-example/. This program creates the server
socket using the startup function and then waits for the client to connect to it on the corresponding port.  The port number
is entered at compile/run time on the command line. An infinite while loop occurs so that the server stays open until it 
receives a sigint from the server user.  A different infinite while loop occurs so that messages can be sent and received on
an alternating basis from the connecting client. 
'''
 
import socket
import sys
from thread import *
 
HOST = ''   # Symbolic name meaning all available interfaces
PORT = int(sys.argv[1])
'''
This function starts up the server's socket and then waits for the client to connect to it before messages are send and
received from the server. It creates the socket using the socket function and places it into the variable s.  Next it
binds the socket to the local host and port using the bind function on s.  An error code will print if it can't bind, 
otherwise it'll print a completion message.  Then it uses the listen function on s to wait for a client to connect to it.
Finally, it returns the s variable.  
'''
def startup(): #initiate request?
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	print 'Socket created'
 
	#Bind socket to local host and port
	try:
		s.bind((HOST, PORT))
	except socket.error as msg:
		print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
		sys.exit()
     
	print 'Socket bind complete'
 
	#Start listening on socket
	s.listen(10)
	print 'Socket now listening'
	return s
'''
This function's parameter is a variable that contains the socket connection. The function then uses the recv function 
to place the received message into the data variable. Then it returns the data variable.
'''
def receivemessage (conn):
	data = conn.recv(1024)
	return data
'''
This function's parameter is a variable that contains the socket connection, and the inputted message. The function then 
uses the send function to send the inputted message.  
'''	
def sendmessage (conn, fullmessage):
	conn.send(fullmessage);
	
'''
Function for handling connections. This function prompts the client to type in their username and then the first message.
It also uses an infinite while loop to continue receiving incoming messages from the client after the first two messages.  
The while loop also allows the user on the server to send messages to the client.  This loop also prepends the username 
data to the client's and server's messages. 
'''
def clientthread(conn):
	conn.send('Hello, what is your name?')
	name = conn.recv(10)
	
	#Sending message to connected client
	conn.send('Welcome to the server. Type something and hit enter') #send only takes string
	
	#infinite loop so that function do not terminate and thread do not end.
	while(1):
         
		#Receiving from client
		data = receivemessage(conn)
		reply = name + '> ' + data
		print reply
		#If there's no data, the while loop will stop iterating
		if not data: 
			break
		#The server sends back the client's messages prepended with the client's username
		conn.sendall(reply)
		#If the server receives a "/quit" message, then it stops the while loop and prints that the socket has been closed
		#by the client
		if data == "/quit":
			print "The socket has been closed by the client"
			break
		quittext = "/quit"
		#The raw_input function is called here and prompts the user to enter a message. Then server user's input is placed 
		#into textdata
		textdata = raw_input("Please enter a message\n")
		#If the server user inputted "/quit" then the while loop stops and the connection is broken with the client
		if textdata == "/quit":
			break
		#The server user's message is prepended with the server's username, which has been prechosen, and sent to the client
		#using the sendmessage function
		fullmessage = 'serverPy> ' + textdata
		print fullmessage
		sendmessage(conn, fullmessage)
		
     
    #came out of loop
	conn.close()
#Start the server	
s = startup() 
#Start talking with the client
while 1:
    #wait to accept a connection - blocking call
	conn, addr = s.accept()
	print 'Connected with ' + addr[0] + ':' + str(addr[1])
     
    #start new thread takes 1st argument as a function name to be run, second is the tuple of arguments to the function.
	start_new_thread(clientthread ,(conn,))

s.close()