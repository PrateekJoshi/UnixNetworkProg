/*
 * daytime_server.c
 *
 *  Created on: 17-Dec-2017
 *      Author: prateek
 *      Pg: 41
 *      Desc: Returns date and time to a client
 */

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 100
#define QUEUE_SIZE 1024

int main(int argc, char **argv) {
	int err = EXIT_SUCCESS;
	int listen_fd, connection_fd;
	struct sockaddr_in server_address;
	char buffer[BUFFER_SIZE];
	time_t current_time;

	/* create a socket, specify comm style and return its fd */
	listen_fd= socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd < 0)
	{
		perror("socket() error");
		err = EXIT_FAILURE;
		goto exit;
	}

	// empty entire structure server_address
	memset(&server_address,0,sizeof(server_address));

	//connection of type IPv4
	server_address.sin_family=AF_INET;
	// accept client connection on any interface
	server_address.sin_addr.s_addr= htonl(INADDR_ANY);
	//assign port 13 for comm
	server_address.sin_port=htons(13);

	// bind fd to specified server address and port
	bind(listen_fd,(struct sockaddr*) &server_address,sizeof(server_address));

	//listen for connections
	listen(listen_fd,QUEUE_SIZE);

	while(1)
	{
		// a new fd is returned by accept for each client that connect to server
		connection_fd = accept(listen_fd,NULL,NULL);

		if(connection_fd < 0)
		{
			perror("accept() error");
			err = EXIT_FAILURE;
			goto exit;
		}

		//get current time
		current_time=time(NULL);

		//write time to string
		snprintf(buffer,sizeof(buffer),"%.24s\r\n",ctime(&current_time));

		//write the time to response
		write(connection_fd,buffer,strlen(buffer));

		//close connection
		close(connection_fd);
	}

	exit:
		return err;
}

