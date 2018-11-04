#include "client_pool.h"
#include "io.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printlog("Usage: %s ports", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    client_pool pool;
    int listenfd, newfd;
    sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(sockaddr_in);

    printlog("[Main] ----- Liso Echo Server -----\n");

    if((listenfd = open_listenfd(port)) < 0)
    {
        printlog("open_listen error");
        exit(1);
    }
    printlog("[Main] Create listenfd sucessfully");

    init_pool(listenfd, &pool);

    while(1)
    {
        pool.read_fds = pool.all_set;
        pool.nready = select(pool.maxfd+1, &pool.read_fds, NULL, NULL, NULL);

        if(pool.nready == 0)
        {
            continue;
        }
        else if(pool.nready < 0)
        {
            printlog("[Main] Select error");
        }

        if(FD_ISSET(listenfd, &pool.read_fds))
        {
            newfd = accept(listenfd, (sockaddr *)&clientaddr, &addrlen);

            if(newfd < 0)
            {
                printlog("[Main] Accept error");
                continue;
            }

            add_client_to_pool(newfd, &pool);

            if(pool.nready <= 0)
            {
                continue;
            }
        }

        // 剩下的就是处理客户端的请求
        handle_clients(&pool);
    }
}