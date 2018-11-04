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
        dump_log("%s create listener socket", SOCKET_API_ERR_MSG);
        return -1;
    }

    // 消除 "Address already in use" 这种错误
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
    {
        dump_log("%s set SO_REUSEADDR", SOCKET_API_ERR_MSG);
        return -1;
    }

    // 绑定
    memset(&serveraddr, 0, sizeof(sockaddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)port);
    if (bind(listenfd, (sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        dump_log("%s bind listenser socket", SOCKET_API_ERR_MSG);
        close(listenfd);
        return -1;
    }

    // 监听
    if (listen(listenfd, 10) < 0)
    {
        dump_log("%s listen on listener socket", SOCKET_API_ERR_MSG);
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


// 发送客户请求的文件
// 返回值
// -1 表示文件不存在
int send_file_to_client(int clientfd, char* full_path)
{
    int fd = open(full_path, O_RDONLY);
    if (fd < 0)
    {
        dump_log("[IO][ERROR] Error when opening file %s for reading", full_path);
        return -1;
    }

    size_t file_length = get_file_len(full_path);

    ssize_t ret = sendfile(clientfd, fd, NULL, file_length);
    if (ret == -1)
    {
        err_sys("[io] sendfile error");
    }

    return 0;
}

// 检查是否是目录
// 返回值
// 是目录返回1
// 不是就返回0
int is_dir(const char *path)
{
    size_t len = strlen(path);
    if (path[len - 1] == '/')
    {
        return 1;
    }
    return 0;
}

// 返回文件扩展名
void get_extension(const char *path, char *result)
{
    size_t len = strlen(path);
    for (int i = len - 1; i >= 0; i--)
    {
        int curr_len = len - i;
        if (path[i] == '.')
        {
            strncpy(result, path + (len - curr_len) + 1, curr_len - 1);
            return;
        }
    }
    strncpy(result, "none", 4);
}

// 返回文件长度
size_t get_file_len(const char* fullpath) {
  struct stat st;
  stat(fullpath, &st);
  return st.st_size;
}

// 获得当前时间
void get_curr_time(char *time_buf, size_t buf_size) {
  time_t raw_time;
  struct tm * timeinfo;

  time(&raw_time);
  timeinfo = localtime(&raw_time);
  strftime(time_buf, buf_size, "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
}

// 获得文件最后修改时间
void get_flmodified(const char*path, char *last_mod_time, size_t buf_size) {
  struct stat st;
  struct tm *curr_gmt_time = NULL;
  stat(path, &st);
  curr_gmt_time = gmtime(&st.st_mtime);
  strftime(last_mod_time, buf_size, "%a, %d %b %Y %H:%M:%S %Z", curr_gmt_time);
}

