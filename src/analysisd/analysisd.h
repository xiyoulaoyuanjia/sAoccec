/****
 *
 *
 */


#ifndef _LOGAUDIT__H

#define _LOGAUDIT__H


/**
 *
 *  虽然现在还不知道这是怎么用的
 *
 */

/* Time structures */
int today;
int thishour;

int prev_year;
char prev_month[4];

int __crt_hour;
int __crt_wday;




/* Local host name */
char __shost[512];

/**
 *  当 没有获得服务器名称时默认设置为此名称
 */
#define OSSEC_SERVER    "ossec-server"

#endif
