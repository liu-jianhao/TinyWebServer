#ifndef __UTILITIES_H
#define __UTILITIES_H

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// 错误处理
void err_sys(const char *fmt, ...);

// 设置和清除非阻塞IO标志
void set_fl(int fd, int flags);
void clr_fl(int fd, int flags);

#endif