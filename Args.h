/* 
 * File: P1/Args.h
 * Desc: This file defines the serv_args and client_args structs. 
 * Author: Jasjeet Dhaliwal
 * Date: 2/6/2016
 *
 */

#ifndef _ARGS_H
#define _ARGS_H

#include <stdlib.h>

typedef struct client_args {
	char* ip, * in_file, * out_file, * port; 

}client_args; 


typedef struct serv_args {
	char* port; 
} serv_args; 


#endif
