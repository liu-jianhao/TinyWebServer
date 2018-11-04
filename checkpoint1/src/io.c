#include "io.h"

// 监听指定端口
int open_listenfd(int port)
{
    int listenfd;
    int yes = 1;
    sockaddr_in serveraddr;

    // 创建套接字
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printlog("%s create listener socket", SOCKET_API_ERR_MSG);
        return -1;
    }

    // 消除 "Address already in use" 这种错误
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
    {
        printlog("%s set SO_REUSEADDR", SOCKET_API_ERR_MSG);
        return -1;
    }

    // 绑定
    memset(&serveraddr, 0, sizeof(sockaddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        printlog("%s bind listenser socket", SOCKET_API_ERR_MSG);
        close(listenfd);
        return -1;
    }

    // 监听
    if (listen(listenfd, 10) < 0)
    {
        printlog("%s listen on listener socket", SOCKET_API_ERR_MSG);
        return -1;
    }

    return listenfd;
}

// send的包裹函数
int sendn(int fd, const void *p, int n)
{
    size_t nleft = n;
    ssize_t nwritten = 0;
    const char *ptr = p;

    while (nleft > 0)
    {
        if ((nwritten = send(fd, ptr + nwritten, n, 0)) < 0)
        {
            if (nwritten < 0 && errno == EINTR)
            {
                nwritten = 0;
            }
            else
            {
                return -1;
            }
        }

        nleft -= nwritten;
        ptr += nwritten;
    }

    return n;
}

// sendn的包裹函数
void Sendn(int fd, const void *ptr, int n)
{
    if (sendn(fd, ptr, n) != n)
        err_sys("sendn error");
}
