/**

设置了 两个全局变量

config 

ftell 全局到文件偏移量指针？

*/
#ifndef _CONFIG__H
    
#define _CONFIG__H 
    
#include "config/config.h"
#include "config/global-config.h"
    
long int __crt_ftell; /* Global ftell pointer. */
_Config Config;  /* Global Config structure */
    
    
    
#endif
