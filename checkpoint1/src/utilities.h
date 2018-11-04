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

// 打印日志
void printlog(const char *fmt, ...);

// 错误处理
void err_sys(const char *fmt, ...);

// 设置非阻塞IO
void set_fl(int fd, int flags);
void clr_fl(int fd, int flags);

#endif