/*
 * File: P1/Server.h
 * Desc: This file delcares the Server structure and its related functions. 
 * Author: Jasjeet Dhaliwal
 * Date: 1/27/2016
 *
 */


#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Args.h"

#define BUFFER_SIZE 10
#define INITBUFFER_SIZE 40

typedef struct Server {
	
	int sockfd, newsockfd, port, n; 
	socklen_t clilen; 
	unsigned char buffer[BUFFER_SIZE]; 
	char init_buffer[INITBUFFER_SIZE]; 
	char* ack, *fin_ack;   
	struct sockaddr_in serv_addr, cli_addr; 

} Server; 

int initServer(Server * server, serv_args* args); 
int startListening(Server * server); 
int closeServer(Server * server); 
void sError (char * str); 

extern Server * server; 

#endif



