Compile and run chatserve using a port number in a linux or ubuntu terminal. In this example, the server address flip1.engr.oregonstate.edu was used.

	python ./chatserve.py [port number]
	Example: python ./chatserve.py 29000

Next compile chatclient in a different terminal window and on a different server address.  In this example flip2.engr.oregonstate.edu was used.

	g++ -o chatclient chatclient.cpp

Run it by adding the server address, and the port number used by chatserve, after the program name.  
The hostname and port can be changed depending on the user's input. In this example, the server is on Oregon State's server flip2. 

	./chatclient [server name] [port number] 
	Example: ./chatclient flip2.engr.oregonstate.edu 29000
	
The client will be prompted for the user's username, which can be up to 10 characters long.  Then the user is prompted for a message. Then the user on chatserve will receive the messages, and then be able to message the client.  Then chatserve and chatclient will take turns messaging. A prompt will show up when it's the user's turn to chat.  The chat can be ended by either the server or the client by typing in /quit when it's their turn to message or by sending a sigint (Ctrl + c). 
