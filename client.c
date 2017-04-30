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
	int socket_fd; // Socket file descriptor
	int connected_to_server;  


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
	printf("Client is connected to server!!\n");

	// Variables used for testing
	int run = 1; 
	int status_send;  
	int user_Input_Number; 
	uint32_t network_byte_order; 

	enum states{
		wait,
		calculate, 
		send  
	}state;

	state = wait; 

	printf("Current state is %d\n", state);




	// while(run == 1){

	// 	// User enters a number
	// 	printf("Please enter a number: ");
	// 	scanf("%d", &user_Input_Number);

	// 	/*
	// 	 * Different computers use different byte orderings internally, one way to solve this:  	
	// 	 * htonl = Host to Network Long (Client-side)
	// 	 * ntohl = Network to Host Long (Server-side)
	// 	 */
	// 	network_byte_order = htonl(user_Input_Number);

	// 	// Send value to server 
	// 	status_send = send(socket_fd, &network_byte_order, sizeof(uint32_t), 0);

	// 	// Verify if value is sent
	// 	if(status_send > 0){
	// 		printf("Client sent value %d to server\n", user_Input_Number);
	// 	} else {

	// 	}

	// 	/* State Machine Diagram */ 

	// 	// State 1: Wait to receive number
	// 	// State 2: Receives Number
		


	// }

	// Free the linked list 
	freeaddrinfo(servinfo);
	close(socket_fd); 

	return 0; 
}