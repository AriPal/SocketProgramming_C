/*
 * Author: 		Dler Hasan
 * Filename: 	server.c
 * Description: Sockets Programming
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
#define size_limit 10000 // Size limit of array 


// Main code 
int main(){

	// Define variables 
	struct sockaddr_storage their_addr;
	int socket_fd, socket_is_connected;
	struct addrinfo hints; 
	struct addrinfo *servinfo; // Points to the results
	socklen_t addr_size; 
	int array[size_limit];
	int receive_value;
	int send_value; 
	int run = 1; 
	int sum; 
	int counter = 0; 
	int return_status;
	int send_status = 0; 
	int totalSum = 0; 
	uint32_t host_byte_order; 

	// States used to control Server-side
	enum states{
		wait,
		calculateSum, 
		send_Sum,
		exit  
	} state;  

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

	// If communication fails
	if(socket_is_connected == -1){
		printf("There is an error in function: accept().\n");
		return 1; 
	}

	// Print if connection is established 
	printf("Connection between server and client established.\n");

	// Enter first state "wait"
	state = wait; 

	while(run == 1){
		
		// State Machine Architectural approach
		switch(state){
			case wait: 
				// Receive Value from client-side
				return_status = recv(socket_is_connected, &receive_value, sizeof(receive_value), 0);

				// Check if value has been received
				if(return_status > 0){
				 // Different computers use different byte orderings internally, solution is to use htonl() and ntohl() 
				 receive_value = ntohl(receive_value);

				 // Jump to next state
				 state = calculateSum; 
				} else {
					state = exit;
				}
				break;

			case calculateSum: 
				// Add value into array
				array[counter] = receive_value;
				sum += array[counter];  
				counter++; 

				// Jump to next state
				state = send_Sum; 
				break;

			case send_Sum: 
				// Pack data before sending to client
				host_byte_order = htonl(sum); 
				// Send data to client
				send_status = send(socket_is_connected, &host_byte_order, sizeof(uint32_t), 0);

				// Check if server sends data
				if(send_status > 0){
					// Jump to next state
					state = wait; 
				} 
				break;

			case exit:
				// Free the linked list and exit while loop
				freeaddrinfo(servinfo);
				close(socket_fd); 
				close(socket_is_connected); 
				run = 0; 
				break;
		} // End switch case
	} // End while loop


	return 0; 

}
