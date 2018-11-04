#include "log.h"

#define LOG_BUFFER_SIZE 100


// 初始化日志文件
int init_log()
{
    fp = fopen(logfile, "a");
    if (fp == NULL)
    {
        printf("[ERROR] log file can not be created");
        return -1;
    }
    return 0;
}

// 写入日志文件
void dump_log(const char *fmt, ...)
{
    time_t tmp = time(NULL);
    struct tm* cur_time = localtime(&tmp);
    if (!cur_time)
    {
        return;
    }
    char arg_buffer[LOG_BUFFER_SIZE];
    memset(arg_buffer, '\0', LOG_BUFFER_SIZE);
    strftime(arg_buffer, LOG_BUFFER_SIZE - 1, "[ %x %X ] ", cur_time);

    fwrite(arg_buffer, sizeof(arg_buffer), 1, fp);

    va_list(args);
    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
    fprintf(fp, "\n");
    fflush(fp);
}

// 关闭日志文件
void close_log()
{
    fclose(fp);
}
