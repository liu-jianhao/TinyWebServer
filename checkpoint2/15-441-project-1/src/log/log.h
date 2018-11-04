#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// 整个项目的全局变量
char *logfile;
FILE *fp;

int init_log();
void dump_log(const char *fmt, ...);
void close_log();

#endif