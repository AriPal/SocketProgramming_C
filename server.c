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
#define PORT_NUMBER "700" // My name is James, James Bond! 
#define BACKLOG 5 // Number of connections

// Main code 
int main(){

	struct sockaddr_storage their_addr;
	int socket_fd, socket_is_connected;
	int status; 
	struct addrinfo hints; 
	struct addrinfo *servinfo; // Points to the results
	socklen_t addr_size; 

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
	if((status = getaddrinfo(NULL, PORT_NUMBER, &hints, &servinfo)) != 0 ){
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

	// Bind socket to the port we passed in to getaddrinfo():
	if(bind(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen) == -1){
		printf("There is an error in function: bind().\n");
		close(socket_fd); 
		return 1;
	}

	// Listen for connection incoming connections
	if(listen(socket_fd, BACKLOG) == -1){
		printf("There is an error in function: listen().\n");
		close(socket_fd);
		return 1; 
	}

	// Print waiting message for the user to see
	printf("Server is waiting for connection on port %s...\n", PORT_NUMBER);

	// Wait for incoming connection
	addr_size = sizeof(their_addr);
	socket_is_connected = accept(socket_fd, (struct socketaddr *)&their_addr, &addr_size);

	// Ready to communicate on new socket connection "socket_is_connected"
	if(socket_is_connected == -1){
		printf("There is an error in function: accept().\n");
		return -1; 
	}

	

	/*
	if(accept() == -1){
		printf("There is an error in function: accept().\n");
	}
	*/

	// Free the linked list 
	freeaddrinfo(servinfo);

}