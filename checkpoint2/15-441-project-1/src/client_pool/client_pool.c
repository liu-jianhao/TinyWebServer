#include "client_pool.h"

const unsigned int BUF_SIZE = 4096;


// 初始化客户池，刚开始只有listenfd才是需要监听的
void init_pool(int listenfd, client_pool *p)
{
    FD_ZERO(&p->all_set);
    FD_ZERO(&p->read_fds);
    FD_SET(listenfd, &p->all_set);
    p->maxfd = listenfd;
    p->nready = 0;
    p->maxi = -1;
    for (int i = 0; i < FD_SETSIZE; i++)
    {
        // -1表示这个位置还没被占用
        p->client_fd[i] = -1;
    }
}

// 添加客户套接字文件描述符到客户池
void add_client_to_pool(int newfd, client_pool *p)
{
    int i;
    p->nready--;

    for (i = 0; i < FD_SETSIZE; i++)
    {
        // 先在数组中找到位置存储新到来的fd
        if (p->client_fd[i] < 0)
        {
            p->client_fd[i] = newfd;
            FD_SET(newfd, &p->all_set);
            p->maxfd = (newfd > p->maxfd) ? newfd : p->maxfd;
            p->maxi = (i > p->maxi) ? i : p->maxi;
            break;
        }
    }

    // 超过限制
    if (i == FD_SETSIZE)
    {
        dump_log("[Client pool] Too many clients!");
    }
}


void handle_clients(client_pool *p)
{
    int i, nbytes, clientfd;
    char buf[BUF_SIZE];

    for (i = 0; (i <= p->maxi) && (p->nready > 0); i++)
    {
        clientfd = p->client_fd[i];
        if (clientfd <= 0)
            continue;

        if (FD_ISSET(clientfd, &p->read_fds))
        {
            // 设置为非阻塞
            set_fl(clientfd, O_NONBLOCK);
            // 接受数据
            nbytes = recv(clientfd, buf, BUF_SIZE, 0);
            if (nbytes > 0)
            {
                dump_log("[Client pool] Receive %d bytes from client on socket %d", nbytes, clientfd);

                handle_http_request(clientfd, buf, nbytes);

                clr_fl(clientfd, O_NONBLOCK);
            }
            else if (nbytes <= 0)
            {
                // 客户端关闭了连接
                if (nbytes == 0)
                {
                    dump_log("[Client pool] Connection closed by client on socket %d", clientfd);
                }
                // 有错误
                else
                {
                    dump_log("[Client pool] Exception on recv() from client on socket %d", clientfd);
                }
                clear_client(clientfd, i, p);
            }
            p->nready--;
            if (p->nready <= 0)
                return;
        }
    }
}

// 从客户池中删除指定fd
void clear_client(int client_fd, int idx, client_pool *p)
{
    close(client_fd);
    FD_CLR(client_fd, &p->all_set);
    p->client_fd[idx] = -1;
}
