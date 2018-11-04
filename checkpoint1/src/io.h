#ifndef __IO_H
#define __IO_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "utilities.h"

#define SOCKET_API_ERR_MSG "[Error in socket_api]"

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;
typedef struct sockaddr_storage sockaddr_storage;
typedef struct addrinfo addrinfo;

int open_listenfd(int port);
int sendn(int, const void *, int);
void Sendn(int, const void *, int);

#endif