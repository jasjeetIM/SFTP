/* 
 * File: P1/client.h
 * Desc: The function prototypes and variables used by the client.
 * Author: Jasjeet Dhaliwal
 * Date: 1/27/2016
 *
 */


#ifndef CLIENT_H
#define CLIENT_H


#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Args.h"


#define BUFFER_SZ 10

typedef struct Client {
	int sockfd, port, n;
	struct sockaddr_in serv_addr; 
	 unsigned char buffer [BUFFER_SZ];
	char* syn, * fin;    
} Client; 

int initClient(Client * client, client_args* args); 
int sendFile(Client * client, client_args* args); 
int closeClient(Client * client); 
void error (char * str); 


extern Client * client; 

#endif
