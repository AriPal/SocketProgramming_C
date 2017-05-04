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
#define IP_ADDRESS "localhost" // If you use external device, change this to e.g 192.168.0.1

// Main code 
int main(){
	struct addrinfo hints; 
	struct addrinfo *servinfo; // Points to the results  
	int socket_fd; // Socket file descriptor
	int connected_to_server; 
	int run = 1; // while loop
	int sum;  
	int status_send;  
	int receive_status; 
	int user_Input_Number; 
	uint32_t network_byte_order; 

 	// // States used to control Client-side
	enum states{
		wait_For_Input,
		sendVal, 
		wait_For_Sum,
		printSum, 
		exit,  
	} state;


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
	if(getaddrinfo(IP_ADDRESS, PORT_NUMBER, &hints, &servinfo) != 0 ){
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

	// If evertying works, this message will be printed
	printf("Client is connected to server!!\n");

	// Enter first state "waiting" 
	state = wait_For_Input;

	while(run == 1){

		// State Machine Architectural approach
		switch(state){

			case wait_For_Input:
				printf("Please enter a number: "); // Print waiting message
				scanf("%d", &user_Input_Number); // Wait for user input
				network_byte_order = htonl(user_Input_Number); // Convert number from host to network long
				
		 		// Jump to exit state if user enters "0"
			 	if(network_byte_order == 0){
			 		state = exit;
			 	}

				// Jump to next state
				state = sendVal;
				break;

			case sendVal: 
				// Send value to server
				status_send = send(socket_fd, &network_byte_order, sizeof(uint32_t), 0);

				// Verify if value is sent
			 	if(status_send > 0){
			 		printf("Client sent users entered value %d to server\n", user_Input_Number);
			 		
			 		// Jump to next state
			 		state = wait_For_Sum;

			 	} else{
			 		printf("There is an error in function: send().\n");
			 	}
			 	break;

			case wait_For_Sum:
				// Receive sum from server
				receive_status = recv(socket_fd, &sum, sizeof(sum), 0);
				sum = ntohl(sum); // Interpret value sent from server 

				// Check if data is received from server
				if(receive_status > 0 ){
					printf("Client has received this sum %d from server.\n", sum);
					// Jump to next state
					state = wait_For_Input;
				} 
				break;

			case exit:
				// Free the linked list and exit while loop
				freeaddrinfo(servinfo);
				close(socket_fd); 
				run = 0;
				break;

		} // end switch 
	} // end while loop

	return 0; 
}