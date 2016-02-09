/* 
 * File: P1/Server.c
 * Desc: Implements the functions related to the Server structure. 
 * Author: Jasjeet Dhaliwal
 * Date: 1/27/2016
 *
 */

#include "Server.h"


int initServer(Server * server, serv_args* args) {
	
	/* Create a file descriptor for the server socket */
	if ((server->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		sError ("Error in opening socket for the server. "); 
		return 0; 
	}


	/* Populate the sockaddr_in struct with IP and port from the serv_args struct */
	bzero((char*)&(server->serv_addr), sizeof(server->serv_addr)); 
	server->port = atoi(args->port); 	
	server->serv_addr.sin_family = AF_INET; 
	server->serv_addr.sin_addr.s_addr = INADDR_ANY; 
	server->serv_addr.sin_port = htons(server->port); 
	server->ack = "ACK"; 
	server->fin_ack = "FIN ACK"; 


	/* Bind the server to the socket */
	if (    bind(server->sockfd, (struct sockaddr *)&(server->serv_addr), sizeof(server->serv_addr)) < 0) {
		sError ("Error: server failed to bind."); 
		return 0; 
	}

	listen(server->sockfd, 5); 

	/* Create a socket to get the file from the client */ 
	server->clilen = sizeof(server->cli_addr); 
	server->newsockfd = accept (server->sockfd, (struct sockaddr *) &(server->cli_addr), &(server->clilen)); 

	if (server->newsockfd < 0) {
		sError ("Error: Server failed to accpet."); 
		return 0; 
	}

	bzero(server->init_buffer, INITBUFFER_SIZE); 
	
	/* Wait for SYN from client */ 
	while(1) {	
		if (recv(server->newsockfd, server->init_buffer, INITBUFFER_SIZE, 0) < 0) continue;   
		if (strstr(server->init_buffer, "SYN")) break; 
	} 

	/* Send ACK to client */ 
	if (send (server->newsockfd, server->ack, strlen(server->ack), 0) < 0) sError("Error: failed to send ACK to client."); 
	

	bzero(server->init_buffer, INITBUFFER_SIZE); 

	/* Wait for File name from client */  
	while(1) {
		if (recv(server->newsockfd, server->init_buffer, INITBUFFER_SIZE, 0) < 0) continue; 
		else break; 
	}
	
	/* Send ACK for the File name frame */ 
	if (send(server->newsockfd, server->ack, strlen(server->ack), 0) < 0) sError ("Error: failed to send ACK to client.");
	 	
	/* Open file with the received name */ 
	FILE * file; 
	file = fopen(server->init_buffer, "w"); 
	int n = 0; // recv count 



	/* Main loop: receives data frames of size 10 bytes, responds with an ACK, and writes data to file */ 
	while (1) {
	
		bzero(server->buffer, BUFFER_SIZE); 
		
		/* Get data frame */ 
		while(1) {
			if ((n = recv(server->newsockfd, server->buffer, BUFFER_SIZE, 0)) <0)  continue;
			else break; 
		}
		
		/* Check for FIN  */ 
		if (strstr(server->buffer, "FIN")) {
			/* Send FIN ACK */ 
			if (send(server->newsockfd, server->fin_ack, strlen(server->fin_ack), 0) < 0) sError("Error: failed to send FIN ACK."); 			  else break; 
		}
		
		/* Write to file logic: 
 		   Case 1: n == 10 --> write to file in 2 iterations of 5 bytes each. 
		   Case 2: n < 10 -->  
					Case 2.1: n > 5 --> write to file in 2 iterations
					Case 2.2: n <=5 --> write to file in 1 iteration
		*/ 
		
		if (n == 10) {
			fwrite(server->buffer,1, 5, file);	
			fwrite(&(server->buffer[5]), 1, 5, file); 
 		}

 
		else {
			if (n > 5) {
				 fwrite(server->buffer, 1, 5, file);
				 fwrite (&(server->buffer[5]), 1, n-5, file); 
			}

			else fwrite(server->buffer, 1, n, file);  
		}
		
		/* Send ACK to Client */ 
		if ( send(server->newsockfd, server->ack, strlen(server->ack), 0) < 0) sError("Error: failed to send ACK to client."); 
	}
	
	printf("File received.\n"); 	
	fclose(file); 
	return 1; 
}


int closeServer(Server * server) {
	/*Close file descriptors */ 
	close(server->newsockfd); 
	close(server->sockfd); 
	return 1; 

}


void sError ( char * str) {
	printf ("%s\n", str); 
	exit(EXIT_FAILURE); 
}
