/**
 *
 * 与文件有关的处理的函数
 *
 *
 *
 */
#include "shared.h"



/* Sets the name of the starting program */
void OS_SetName(char *name)
{
    __local_name = name;
    return;
}



/* goDaemon: Daemonize a process..
 *
 */
void goDaemon()
{
    int fd;
    pid_t pid;

    pid = fork();

    if(pid < 0)
    {
        merror(FORK_ERROR, __local_name);
        return;
    }
    else if(pid)
    {
        exit(0);
    }

    /* becoming session leader */
    if(setsid() < 0)
    {
        merror(SETSID_ERROR, __local_name);
        return;
    }

    /* forking again */
    pid = fork();
    if(pid < 0)
    {
        merror(FORK_ERROR, __local_name);
        return;
    }
    else if(pid)
    {
        exit(0);
    }


    /* Dup stdin, stdout and stderr to /dev/null */
    if((fd = open("/dev/null", O_RDWR)) >= 0)
    {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
    }


    /* Going to / */
    chdir("/");
    /* Closing stdin, stdout and stderr */
    /*
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
    */

    /* Openining stdin, stdout and stderr to /dev/null */
    /*
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
    */

    return;
}




