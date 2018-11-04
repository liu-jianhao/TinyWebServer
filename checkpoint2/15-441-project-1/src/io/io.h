#ifndef __IO_H
#define __IO_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "../utilities/utilities.h"
#include "../log/log.h"

#define SOCKET_API_ERR_MSG "[Error in socket_api]"

#define SIZE 4096

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr_in6 sockaddr_in6;
typedef struct sockaddr_storage sockaddr_storage;
typedef struct addrinfo addrinfo;

int open_listenfd(int port);

// 传数据
int sendn(int, const void *, int);
void Sendn(int, const void *, int);
int send_file_to_client(int clientfd, char* full_path);


// 文件操作
int is_dir(const char *path);
void get_extension(const char *path, char *result);
size_t get_file_len(const char* fullpath);
void get_curr_time(char *time_buf, size_t buf_size);
void get_flmodified(const char*path, char *last_mod_time, size_t buf_size);

#endif
