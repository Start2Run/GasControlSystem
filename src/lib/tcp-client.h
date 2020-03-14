#ifndef __TCP_CLIENT__
#define __TCP_CLIENT__

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

extern struct Tcp_Client 
{
    int                ret;
    int                conn_fd;
    int                port;
    char               address[24];
    struct sockaddr_in server_addr;
};

int connectClient(struct Tcp_Client *client);
void sendMessage(char* message, struct Tcp_Client *client);
int disconnectClient(struct Tcp_Client *client);
void ReadMessageSuccesful(struct Tcp_Client *client, char *array[]);
#endif