/******************************************************************************
* echo_server.c                                                               *
*                                                                             *
* Description: This file contains the C source code for an echo server.  The  *
*              server runs on a hard-coded port and simply write back anything*
*              sent to it by connected clients.  It does not support          *
*              concurrent clients.                                            *
*                                                                             *
* Authors: Athula Balachandran <abalacha@cs.cmu.edu>,                         *
*          Wolf Richter <wolf@cs.cmu.edu>                                     *
*                                                                             *
*******************************************************************************/

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <sys/select.h>

#define ECHO_PORT 9999
#define BUF_SIZE 4096

int close_socket(int sock)
{
    if (close(sock))
    {
        fprintf(stderr, "Failed closing socket.\n");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int sock, client_sock[FD_SETSIZE];
    ssize_t readret;
    socklen_t cli_size;
    struct sockaddr_in addr, cli_addr;
    char buf[BUF_SIZE];

    fprintf(stdout, "----- Echo Server -----\n");

    /* all networked programs must create a socket */
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "Failed creating socket.\n");
        return EXIT_FAILURE;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(ECHO_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    /* servers bind sockets to ports---notify the OS they accept connections */
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)))
    {
        close_socket(sock);
        fprintf(stderr, "Failed binding socket.\n");
        return EXIT_FAILURE;
    }

    if (listen(sock, 5))
    {
        close_socket(sock);
        fprintf(stderr, "Error listening on socket.\n");
        return EXIT_FAILURE;
    }

    fd_set allset, rset;
    int maxfd = sock;
    int maxi = -1;
    int nready, i;
    int connfd, sockfd;
    for(i = 0; i < FD_SETSIZE; ++i)
    {
        client_sock[i] = -1;
    }
    FD_ZERO(&allset);
    FD_SET(sock, &allset);

    /* finally, loop waiting for input and then write it back */
    while (1)
    {
        rset = allset;
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);

        if(FD_ISSET(sock, &rset)) {
            cli_size = sizeof(cli_addr);
            if ((connfd = accept(sock, (struct sockaddr *)&cli_addr,
                                      &cli_size)) == -1)
            {
                close(connfd);
                fprintf(stderr, "Error accepting connection.\n");
                return EXIT_FAILURE;
            }

            for(i = 0; i < FD_SETSIZE; ++i)
            {
                if(client_sock[i] < 0)
                {
                    client_sock[i] = connfd;
                    break;
                }
            }

            if(i == FD_SETSIZE)
            {
                fprintf(stderr, "client_sock is full\n");
                return EXIT_FAILURE;
            }
            FD_SET(connfd, &allset);
            if(connfd > maxfd)
                maxfd = connfd;
            if(i > maxi)
                maxi = i;
            if(--nready <= 0)
                continue;
        }

        for(i = 0; i <= maxi; ++i)
        {
            if((sockfd = client_sock[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset))
            {
                readret = 0;

                // while ((readret = recv(sockfd, buf, BUF_SIZE, 0)) >= 1)
                if ((readret = recv(sockfd, buf, BUF_SIZE, 0)) >= 1)
                {
                    if (send(sockfd, buf, readret, 0) != readret)
                    {
                        close_socket(sockfd);
                        close_socket(sock);
                        FD_CLR(sockfd, &allset);
                        client_sock[i] = -1;
                        fprintf(stderr, "Error sending to client.\n");
                        return EXIT_FAILURE;
                    }
                    memset(buf, 0, BUF_SIZE);
                }
                
                if (readret == -1)
                {
                    close_socket(sockfd);
                    close_socket(sock);
                    FD_CLR(sockfd, &allset);
                    client_sock[i] = -1;
                    fprintf(stderr, "Error reading from client socket.\n");
                    return EXIT_FAILURE;
                }
                
                if (--nready <= 0)
                    break;
            }
        }

        // if (close_socket(sockfd))
        // {
        //     close_socket(sock);
        //     fprintf(stderr, "Error closing client socket.\n");
        //     return EXIT_FAILURE;
        // }
    }

    close_socket(sock);

    return EXIT_SUCCESS;
}
