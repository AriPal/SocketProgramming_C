/*
* Author: 		Dler Hasan
* Filename: 	client.c
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

#define PORT_NUMBER "3354" 

// Main code 
int main(){

	struct addrinfo hints; 
	struct addrinfo *servinfo; // Points to the results
	int user_Input_Number; 
	int socket_fd; // Socket file descriptor


	// Method is used to clear struct 
	memset(&hints, 0, sizeof(hints)); 

	// Define communication protocal 
	hints.ai_family = AF_UNSPEC; // Does not matter if it is IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // Protocol is set to TCP 
	hints.ai_flags = AI_PASSIVE; // Set local IP address automatically

	/* Gets important information
	 * Param1 (Node): Host name, or an IP address.
	 * Param2 (Service): Port number, or the name of a particular service http, ftp, smtp.
	 * Param2 (Hints): Points to struct you've already filled up 
	 */
	if(getaddrinfo("localhost", PORT_NUMBER, &hints, &servinfo) != 0 ){
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

	// Connect socket to server
	connected_to_server = connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen); 
	if(connected_to_server == -1){
		printf("There is an error in function: connect().\n");
		return 1; 
	}

	// Print message
	printf("Client is connected!! %d\n", connected_to_server);

	// Free the linked list 
	freeaddrinfo(servinfo);
	close(socket_fd); 

	return 0; 
}