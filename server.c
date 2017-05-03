/*
 * Author: 		Dler Hasan
 * Filename: 	server.c
 * Description: 	Sockets Programming
 * University: 	Stevens Institute of Technology
 */

// Includes necessary library for sockets programming
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

// Define port number.  
#define PORT_NUMBER "3354"
#define BACKLOG 5 // Number of feasable clients
#define size_limit 5 // Size limit of array 


// Main code 
int main(){

	struct sockaddr_storage their_addr;
	int socket_fd, socket_is_connected;
	struct addrinfo hints; 
	struct addrinfo *servinfo; // Points to the results
	socklen_t addr_size; 
	int sum[size_limit];
	int receive_value;   

	// Make sure the struct is empty 
	memset(&hints, 0, sizeof(hints)); 

    /* 
     * Does not matter if it is IPv4 or IPv6
     * Protocol is set to TCP 
     * Set local IP address automatically
     */
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;  
	hints.ai_flags = AI_PASSIVE; 

	/* Gets important information
	 * Param1 (Node): Host name, or an IP address.
	 * Param2 (Service): Port number, or the name of a particular service http, ftp, smtp.
	 * Param2 (Hints): Points to struct you've already filled up 
	 */
	if(getaddrinfo(NULL, PORT_NUMBER, &hints, &servinfo) != 0){
		printf("There is an error in function: getaddrinfo().\n");
		return 1; 
	}

	/* Choose what socket you want 
	 * Param1: IPv4 or IPv6
	 * Param2: Stream or Datagram
	 * Param3: TCP or UDP 
	 */
	socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if(socket_fd == -1){
		printf("There is an error in function: socket().\n");
		return 1; 
	}

	// Bind socket to the port we passed into getaddrinfo():
	if(bind(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1){
		printf("There is an error in function: bind().\n");
		close(socket_fd); 
		return 1;
	}

	// Listen for incoming connections
	if(listen(socket_fd, BACKLOG) == -1){
		printf("There is an error in function: listen().\n");
		close(socket_fd);
		return 1; 
	}

	// Print waiting message
	printf("Server is waiting for connection on port %s...\n", PORT_NUMBER);

	// Wait for incoming connection
	addr_size = sizeof(their_addr);
	socket_is_connected = accept(socket_fd, (struct sockaddr *)&their_addr, &addr_size);

	// Ready to communicate with new socket 
	if(socket_is_connected == -1){
		printf("There is an error in function: accept().\n");
		return 1; 
	}

	// Print if connection is established 
	printf("Connection between server and client established.\n");
	
	int run = 1; 
	int counter = 0; 
	int return_status;
	int send_status;

	// Critical states for server-side
	enum states{
		wait,
		calculateSum, 
		send,
		exit  
	} state;

	// Receive/send Data until socket is closed
	while(run == 1){

		// Receive value from client
		return_status = recv(socket_is_connected, &receive_value, sizeof(receive_value), 0); 

		if(return_status > 0){

			/*
			 * Different computers use different byte orderings internally, one way to solve this:  	
			 * htonl = Host to Network Long (Client-side)
			 * ntohl = Network to Host Long (Server-side)
			 */
			receive_value = ntohl(receive_value); 
			printf("Value received %d\n", receive_value);
			// printf("Server received %d from client \n",sum[counter]);
			// counter++;
		}
		





		// Exit if array limit is reached 
		if(counter == (size_limit - 1)){
			printf("Exit while loop because array size limit %d is reached.\n", counter);
			run = 0; 
		}

	}


	printf("STOP\n");
	

	// Free the linked list 
	freeaddrinfo(servinfo);
	close(socket_fd); 
	close(socket_is_connected); 

	return 0; 

}