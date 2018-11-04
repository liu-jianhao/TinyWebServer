#include "log.h"
#include <stdio.h>

char* LOGFILE;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("usage: %s filename\n", argv[0]);
        return -1;
    }
    char hello[10] = "hello";

    init_log(argv[1]);
    dump_log("%s", hello);

    close_log();

    return 0;
}