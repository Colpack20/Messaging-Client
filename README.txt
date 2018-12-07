Compile chatclient with
	g++ -o chatclient chatclient.cpp
Run it by adding the hostname (flip server) and the port number onto the program name.  
The hostname and port can be changed depending on the user's input. 
	./chatclient flip2.engr.oregonstate.edu 8881

Compile and run chatserve by adding the port number as the third argument. The
port can be changed depending on the user's input.
	python ./chatserve.py 8881
	
The client will be prompted for the user's username before the client sends the first message. Then the 
chatserve and chatclient will take turns messaging. Prompts will be provided to make it clear what's needed 
from the user to continue, or if the user or client has decided to quit. Sending a sigint (Ctrl + c) will
end the chatserve program. 
