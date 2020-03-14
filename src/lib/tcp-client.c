//This library is based on the code found at https://ops.tips/gists/a-minimal-tcp-client-in-c/

#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include "tcp-client.h"

 #define MAXDATASIZE 100 /* max number of bytes we can get at once */

void CreateTcpClient(struct Tcp_Client *client)
{
	client->ret=0;
	client->port=9000;
	strcpy(client->address, "127.0.0.1");
	struct sockaddr_in server_addr = {0};
	client->server_addr = server_addr;
	return client;
}

int connectClient(struct Tcp_Client *client)
{
	client->server_addr.sin_family = AF_INET;

	// sent.
	client->server_addr.sin_port = htons(client->port);

	client->ret = inet_pton(AF_INET, client->address, &client->server_addr.sin_addr);
	if (client->ret != 1) {
		if (client->ret == -1) {
			perror("inet_pton");
		}
		printf("\nfailed to convert address to binary net address\n");
		return -1;
	}

	client->conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client->conn_fd == -1) {
		perror("socket");
		return -1;
	}

	client->ret =  connect(client->conn_fd, (struct sockaddr*)&client->server_addr, sizeof(client->server_addr));
	if (client->ret == -1) {
		perror("connect");
		return -1;
	}
return 0;
}

void sendMessage(char* message, struct Tcp_Client *client)
{
    if (send(client->conn_fd, message, strlen(message), 0) == -1){
                      perror("send");
		      printf("error sending");
		}
}

void ReadMessageSuccesful(struct Tcp_Client *client, char *array[])
{
	int numbytes; 
	char buf[MAXDATASIZE];
	numbytes = recv(client->conn_fd, buf, MAXDATASIZE, 0);
	if ( numbytes== -1) 
	{     
		perror("recv");
		exit(1);
	}	
	buf[numbytes] = '\0';

	char delim[] = "\r\n";

	char *ptr = strtok(buf, delim);
	int i = 0;
	while (ptr != NULL && i<3)
	{
		array[i++] = ptr;
		ptr = strtok(NULL, delim);
	}
}

int disconnectClient(struct Tcp_Client *client)
{
    client->ret = shutdown(client->conn_fd, SHUT_RDWR);
	if (client->ret == -1) {
		perror("shutdown");
		return -1;
	}

	client->ret = close(client->conn_fd);
	if (client->ret == -1) {
		perror("close");
		return -1;
	}
	return 0;
}

