/* 
 * File: P1/main.c
 * Desc: This file initializes the server and client and facilitates communication. 
 * Author: Jasjeet Dhaliwal
 * Date Created:  1/27/2016
 * Date Modified: 2/8/2016
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Server.h"
#include "Client.h"
#include <pthread.h>
#include "Args.h"

/*Client and Server Objects */ 
Server * server = NULL; 
Client * client = NULL; 


/*Prototypes */ 
void * server_f(void *);
void err (char * str);  


int main (int argc, char * argv[]) {
	
	/* Declare thread for server and client/server argument structures */ 
	pthread_t s_thread; 	
	serv_args* sArgs; 
	client_args* cArgs; 
	char* in_file, * out_file, * serv_ip, * serv_port;  	
	

	
	if (argc < 5) {
		printf("Invalid arguement count...\n"); 
		printf("Correct format: P1 <input_file> <output_file> <server_ip> <server_port>\n"); 
		exit(EXIT_SUCCESS);
	}

	in_file = argv[1]; 
	out_file= argv[2]; 
	serv_ip = argv[3]; 
	serv_port = argv[4]; 	
	
	/* Allocate memory for argument structures */ 
        if (!(sArgs = malloc(sizeof(serv_args)))) err("Error: mem allocation for serv_arg failed."); 
	if (!(cArgs = malloc(sizeof(client_args)))) err("Error: mem allocation for client_arg failed."); 

	/* Populate argument structures */ 
	cArgs->ip = serv_ip; 
	cArgs->in_file = in_file; 
	cArgs->out_file = out_file; 
	cArgs->port = serv_port; 
		
	sArgs->port = serv_port; 


	/* Allocate memory for Client and Server */ 
	if (! (server = malloc(sizeof(Server))) ) err ("Error: mem allocation for Server object failed."); 

	if (! (client = malloc(sizeof(Client)))) err ("Error: mem allocation for Client object failed."); 
	

	/* Launch server thread and initialize server */ 
	if ((pthread_create(&s_thread, NULL, server_f, (void *)sArgs) ) < 0 ) err("Error: thread creation for client failed."); 	    
	
	sleep(1); 


	/* Initialize client */ 	
	initClient(client, cArgs);

	/* Start file transmission */  
	sendFile(client, cArgs); 

	/* Join server thread */ 
	pthread_join(s_thread, NULL); 
	

	/* Close sockets */
	closeClient(client);  
	closeServer(server);  
	
	/*Free memory*/ 
	free(client); 
	free(server); 
	free(sArgs); 
	free(cArgs); 
	exit(EXIT_SUCCESS); 
	
	 
}

/* Initializes server */ 
void* server_f (void* args) {
	serv_args* sArgs = (serv_args*)(args); 
	initServer(server, sArgs); 	
	return;  
} 



void err(char * str) {
	printf("%s\n", str); 
	exit(EXIT_FAILURE); 
}
