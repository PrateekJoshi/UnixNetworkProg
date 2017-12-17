/*
 * daytime_client.c
 *
 *  Created on: 16-Dec-2017
 *      Author: prateek
 *      Pg: 32
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


int main(int argc, char **argv) {
	int socket_fd, err= EXIT_SUCCESS;
	struct sockaddr_in server_address;
	char response[100];
	int num_read;
	
	//if IP address is not specified in CLA, return error
	if(argc!=2)
	{
		perror("Please specify IP address in CLA");
		err = EXIT_FAILURE;
		goto exit;
	}

	/* create a socket, specify comm style and return its fd */
	socket_fd= socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd < 0)
	{
		perror("socket() error");
		err = EXIT_FAILURE;
		goto exit;
	}

	// empty entire structure server_address
	memset(&server_address,0,sizeof(server_address));

	//set the attributes in sockaddr_in structure
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(13);			//port 13 returns date and time (std)

	//convert IP adress from text to network(binary) format
	err=inet_pton(AF_INET,argv[1],&(server_address.sin_addr));
	if(err<0)
	{
		perror("inet_pton() error");
		goto exit;
	}

	// establish a TCP connection
	err = connect(socket_fd,(struct sockaddr*)&server_address,sizeof(server_address));
	if(err)
	{
		perror("connect() error");
		goto exit;
	}

	//read reply from server
	while((num_read =  read(socket_fd,response,100)) > 0)
	{
		//null terminate response string
		response[num_read]=0;
		//display response in stdout
		err = fputs(response,stdout);
		if( err == EOF)
		{
			perror("fputs() error");
			goto exit;
		}
	}

	if(num_read<0)
	{
		perror("read() error");
		err = EXIT_FAILURE;
		goto exit;
	}

	exit:
		return err;
}



