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

// Main code 
int main(){

	int status; 
	struct addrinfo hints; 
	struct addrinfo *servinfo; // Points to the results

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
		printf("There is an error in function getaddrinfo\n");
		exit(1); 
	}

	// Free the linked list 
	freeaddrinfo(servinfo);

}