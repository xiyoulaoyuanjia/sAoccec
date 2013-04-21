/**
 *
 *  调试选项文件
 *
 *
 *
 */

#include "headers/shared.h"

/*   调试标志
 */
int dbg_flag = 0;
int chroot_flag=0;
int daemon_flag=0;

/**
 *
 *
 */
void nowDaemon()
{
    daemon_flag = 1;
}


/*
 * 
 */
/* For internal logs */
#ifndef LOGFILE
  #ifndef WIN32
    #define LOGFILE   "/logs/ossec.log"
  #else
    #define LOGFILE "ossec.log"
  #endif
#endif

/*
 * 开始调试
 */
void nowDebug()
{
    dbg_flag++;
}


/**
 *
 */
void print_out(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    /* Print to stderr */
    (void)vfprintf(stderr, msg, args);

    #ifdef WIN32
    (void)fprintf(stderr, "\r\n");
    #else
    (void)fprintf(stderr, "\n");
    #endif

    va_end(args);
}

/* _log function */
void _log(const char * msg,va_list args)
{
    time_t tm;
    struct tm *p;

    /* For the stderr print */
    va_list args2;

    FILE *fp;

    tm = time(NULL);
    p = localtime(&tm);

    /* Duplicating args */
    va_copy(args2, args);


    /* If under chroot, log directly to /logs/ossec.log */
    if(chroot_flag == 1)
    {
        fp = fopen(LOGFILE, "a");
    }
    else
    {
        char _logfile[256];
        #ifndef WIN32
        snprintf(_logfile, 256, "%s%s", DEFAULTDIR, LOGFILE);
        #else
        snprintf(_logfile, 256, "%s", LOGFILE);
        #endif
        fp = fopen(_logfile, "a");
    }

    /* Maybe log to syslog if the log file is not available. */
    if(fp)
    {
        (void)fprintf(fp,"%d/%02d/%02d %02d:%02d:%02d ",
                      p->tm_year+1900,p->tm_mon+1,
                      p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);
        (void)vfprintf(fp, msg, args);
        #ifdef WIN32
        (void)fprintf(fp, "\r\n");
        #else
        (void)fprintf(fp, "\n");
        #endif
        fflush(fp);
        fclose(fp);
    }
    else
    {


    }


    /* Only if not in daemon mode */
    if(daemon_flag == 0)
    {
	    /* Print to stderr */
        (void)fprintf(stderr,"%d/%02d/%02d %02d:%02d:%02d ",
                      p->tm_year+1900,p->tm_mon+1 ,p->tm_mday,
                      p->tm_hour,p->tm_min,p->tm_sec);
        (void)vfprintf(stderr, msg, args2);
        #ifdef WIN32
        (void)fprintf(stderr, "\r\n");
        #else
        (void)fprintf(stderr, "\n");
        #endif
    }

    /* args2 must be ended here */
    va_end(args2);
}
/**
 *  debug 模式 标志大于1时测试
 *
 */
void debug1(const char * msg,...)
{   
    if(dbg_flag >= 1)
    {
        va_list args;
        va_start(args, msg);
    
        _log(msg, args); 
        
        va_end(args);
    }
}   


void nowChroot()
{   
    chroot_flag = 1;
}       

/**
 *
 */
void verbose(const char * msg,... )
{
    va_list args;
    va_start(args, msg);
    _log(msg, args);
    va_end(args);
}

/**
 *  打印错误并退出 
    exit(1);
 *  
 */
void ErrorExit(const char *msg, ...)
{
    va_list args;

    #ifdef WIN32
        /* If not MA */
        #ifndef MA
        WinSetError();
        #endif
    #endif

    va_start(args, msg);
    _log(msg, args);
    va_end(args);

    exit(1);
}
/**
输出错误信息
*/
void merror(const char * msg,... )
{   
    va_list args;
    va_start(args, msg);
    _log(msg, args);
    va_end(args);
}   


