#include "../client_pool/client_pool.h"
#include "../io/io.h"
#include "../log/log.h"
#include <string.h>

// 日志文件
char *logfile;
// 静态网页文件目录
char *WWW;


int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("Usage: %s ports logfilename WWW\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);

    logfile = argv[3];
    init_log();

    WWW = argv[5];

    client_pool pool;
    int listenfd, newfd;
    sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(sockaddr_in);

    printf("[Main] ----- Liso Web Server -----\n");
    dump_log("[Main] ----- Liso Web Server -----");

    if((listenfd = open_listenfd(port)) < 0)
    {
        dump_log("open_listen error");
        exit(1);
    }
    dump_log("[Main] Create listenfd sucessfully");

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
            dump_log("[Main] Select error");
        }

        if(FD_ISSET(listenfd, &pool.read_fds))
        {
            newfd = accept(listenfd, (sockaddr *)&clientaddr, &addrlen);

            if(newfd < 0)
            {
                dump_log("[Main] Accept error");
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

    close_log();

    return 0;
}
