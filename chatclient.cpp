/*
* Using code from OSU's operating systems class: client.c  
*This program sets up and creates the client.  It uses the host name and port number that was entered in on the command line 
at run time to connect to the corresponding server. It uses the struct sockaddr_in serveraddress to set up the socket.  It 
also uses a do/while loop to prompt the user for input and place it into a buffer.  The client user can type "/quit" to close
the client side of the socket connection.  The sendmessage and receivemessage functions are used to receive and send messages.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <iostream>
#include <string>
#include <limits>
using std::cout;
using std::cin;
using std::endl;

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues
/*This function takes as parameters the socket int, the buffer array, the length of the buffer array, and an int variable for
the number of chars sent to the server.  The function calls the send function in order to read the data from the server. 
It will print an error if the number of chars sent is less than zero or less than the length of the message. */
void sendmessage(int socket, char b[], int len, int chars)
{
	chars = send(socket, b, len, 0); // Write to the server
	if (chars < 0) error("CLIENT: ERROR writing to socket");//If chars is less then 0 an error is displayed
	if (chars < len) printf("CLIENT: WARNING: Not all data written to socket!\n");
}
/*This function takes as parameters the socket int, the buffer array, the length of the buffer array, and an int variable for
the number of chars received from the server.  The function calls the recv function in order to read the data from the server. 
It will print an error if the number of chars received is zero or less.  It then prints out the received message. */
void receivemessage(int socket, char b[], int len, int chars)
{
	chars = recv(socket, b, len, 0); // Read data from the socket, leaving \0 at end
	if (chars < 0) error("CLIENT: ERROR reading from socket");//If chars is less then 0 an error is displayed
	if(chars == 0) error("The socket has been closed by the server");
	//The received message is printed out to the screen using printf
	printf("%s\n", b);
}
/*This function has the parameter of struct sockaddr_in and uses it to set up the socket and connect to the server. It
then returns the value of the socket in variable socketF. It will also print out errors if the client can't open or connect
to the socket.*/
int initiateContact(struct sockaddr_in serverAddress)
{
	// Set up the socket
	int socketF = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketF < 0) error("CLIENT: ERROR opening socket");
	
	// Connect to server
	if (connect(socketF, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");
	return socketF;
}
int main(int argc, char *argv[])
{
	FILE * f, * g;
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	//The buffers all have a size of 500 to allow for messages no larger than 500 characters.
	char buffer[500];
	char buffer3[500];
	char buffer4[500];

	if (argc < 3) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	//serverHostInfo = gethostbyname("flip2.engr.oregonstate.edu"); // Convert the machine name into a special form of address
	serverHostInfo = gethostbyname(argv[1]);
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	socketFD = initiateContact(serverAddress);

	size_t size = 0;
	memset(buffer, '\0', sizeof(buffer));//Clear out the buffer array
	
	int counter2 = 0;		
	int z = 1;
	//The following do/while loop runs while the client is connected to the server.  
	do{	
		//The following if statements prompts the client user's for a message and inputs the message into the buffer 
		if(counter2 > 0)
		{
			if(counter2 > 2)
				printf("Please enter a message\n");
			cin.getline(buffer, 500);
		}
		//The following statements end the while loop, which closes the client's connection, if the client user types "/quit"
		if(buffer[0] == '/' && buffer[1] == 'q' && buffer[2] == 'u' && buffer[3] == 'i' && buffer[4] == 't')
		{
			z = 0;
			counter2 = 1;
		}
		
	//The following section uses the send function to send a message to the server
	if(counter2 > 0)
	{	
		sendmessage(socketFD, buffer, strlen(buffer), charsWritten);
	}
	
	//The following section gets the received message from the server	
	memset(buffer3, '\0', sizeof(buffer3)); // Clear out the buffer again for reuse
	receivemessage(socketFD, buffer3, sizeof(buffer3) - 1, charsWritten);
	
	//The following section gets the received message from the server	
	if(counter2 > 1)
	{
		memset(buffer4, '\0', sizeof(buffer4)); // Clear out the buffer again for reuse
		receivemessage(socketFD, buffer4, sizeof(buffer3) - 1, charsWritten);
	}
	counter2++;
	}while(z);
	
	//The following statements flush out stderr and stdout
	fflush(stderr);
	fflush(stdout);
	//The socket is closed
	close(socketFD); 
	return 0;
}

