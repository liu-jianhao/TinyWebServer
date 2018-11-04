#ifndef __SERVER_H
#define __SERVER_H

#include "io.h"

// 客户池
typedef struct
{
    // 全部
    fd_set all_set;
    // 准备要读的
    fd_set read_fds;
    // 最大值
    int maxfd;
    // 已经准备的个数
    int nready;
    // 在数组中可获得的最大下标
    int maxi;
    // 客户数组
    int client_fd[FD_SETSIZE];
} client_pool;

void init_pool(int listenfd, client_pool *p);
void add_client_to_pool(int newfd, client_pool *p);
void handle_clients(client_pool *p);
void clear_client(int clientfd, int idx, client_pool *p);

#endif