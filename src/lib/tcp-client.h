#ifndef __TCP_CLIENT__
#define __TCP_CLIENT__

#include <sys/socket.h>
#include <netinet/in.h>

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
void ReadMessage(struct Tcp_Client *client, char *value);
#endif