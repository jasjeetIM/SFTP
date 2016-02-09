/*
 * File: P1/Client.c
 * Desc: Implements the functions declared for the client. 
 * Author: Jasjeet Dhaliwal
 * Date: 1/27/2016
 *
 */

#include "Client.h"


int initClient (Client* client, client_args* args) {
	
	/*Create file descriptor for client socket */ 
	if ((client->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			error("Error opening client socket."); 
			return 0; 
	}

	/* Terminate program if server ip is invalid */ 
	if (args->ip == NULL) {
		error("Error: server IP address is not valid."); 
		return 0; 
	}
	
	/* Populate client sockaddr_in struct with IP and port of server */ 
	client->serv_addr.sin_family = AF_INET; 
	client->port = atoi(args->port); 
	client->serv_addr.sin_port = htons(client->port);
        client->serv_addr.sin_addr.s_addr = inet_addr(args->ip);
	client->syn = "SYN"; 
	client->fin="FIN"; 	
	
	/* Initialize connection */ 
	if (connect (client->sockfd, (struct sockaddr *) & client->serv_addr, sizeof(client->serv_addr)) < 0) {
		error ("Error: connection to server failed."); 
		return 0; 
	}
	
	printf("Connection established ...\n");  	
	return 1; 
}
	


	void error (char * str ) {
	printf ("%s\n", str); 
	exit(EXIT_FAILURE); 

}



int sendFile(Client * client, client_args* args) {
	
	/* Initialize file stream */ 
	FILE * inFile;
	int j = 0; // num bytes read from file
	  
	if (!(inFile = fopen(args->in_file, "r"))) error("Error: failed to open input inFile"); 
	

	/* Send initial SYN */
	if( send(client->sockfd, client->syn, strlen(client->syn), 0) < 0)  error("Error: failed to send Syn to server");
 
	/* Wait for an ACK */ 
	while (1) {
		if (recv(client->sockfd, client->buffer, BUFFER_SZ, 0) < 0) continue; 
		if (strstr(client->buffer, "ACK")) break; 
	}	

	/* Send Output inFile name */
	if (send(client->sockfd, args->out_file, strlen(args->out_file), 0) <0) error("Error: failure to send inFile name to server."); 
	
	/* Wait for an ACK */ 
	while(1) {
		if(recv(client->sockfd, client->buffer, BUFFER_SZ, 0) < 0) continue; 
		if (strstr(client->buffer, "ACK")) break; 
	}
	  
	/* Main loop : send file data in 10 byte frames, proceed with the next frame after receiving ACK for the sent frame. */
	while(!feof(inFile)) {
	
		bzero(client->buffer, BUFFER_SZ);
			 
		/* Read from file and send 10 byte frames */ 
	     	j = fread(client->buffer,1, BUFFER_SZ , inFile); 
			
		if(send (client->sockfd, client->buffer, j , 0) < 0) error("Error: Send inFile failed.");   
	
		/* Wait for ACK from server */
		while (1) {
			if (recv(client->sockfd, client->buffer, BUFFER_SZ, 0) < 0) continue; 
			if (strstr(client->buffer, "ACK")) break; 
			}
	}
		

	/* Send FIN to server  */ 
	if (send(client->sockfd, client->fin, strlen(client->fin), 0) < 0) error("Error: failed to send FIN"); 

	int ij = 0; 
	/* Wait for FIN ACK for 20 cycles*/ 
	while(1) {
		ij++; 
		if (ij>20) break; 
		if (recv(client->sockfd, client->buffer, BUFFER_SZ, 0) < 0) continue; 
		if (strstr(client->buffer, "FIN ACK")) break; 
	} 

	fclose(inFile);  
	return 1; 
}



int closeClient (Client * client) {
	/*Close file descriptors */ 
	close(client->sockfd); 
	return 1; 
} 
