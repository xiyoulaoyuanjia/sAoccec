/**
 *  操作配置文件 
 *
 */

/* Functions to handle the configuration files
 */ 

#include "shared.h"
#include "config.h"


/* GlobalConf vv0.2: 2005/03/03
 * v0.2: Changing to support the new OS_XML
 */
int GlobalConf(char * cfgfile)
{
    int modules = 0;


    /* Default values */
    Config.logall = 0;
    Config.stats = 4;
    Config.integrity = 8;
    Config.rootcheck = 8;
    Config.hostinfo = 8;
/**
 *<picviz_output> 为yes  则 设置为1 
 */
    Config.picviz = 0;
    Config.prelude = 0;
    Config.memorysize = 1024;
/**
 *  如果 <email_notification>yes</email_notification>  则会设置为1
 */
    Config.mailnotify = -1;
    Config.keeplogdate = 0;
    Config.syscheck_alert_new = 0;
    Config.syscheck_auto_ignore = 1;
    Config.ar = 0;

    Config.syscheck_ignore = NULL;

/**
 *  
 *  设置为 <white_list>127.0.0.1</white_list>
 *         <white_list>192.168.2.1</white_list>
 *  的地址
 */
    Config.white_list = NULL;
    Config.hostname_white_list = NULL;
/**
 *
    <log_alert_level>1</log_alert_level>
    <email_alert_level>7</email_alert_level>
 *
 */
    /* Default actions -- only log above level 1 */
    Config.mailbylevel = 7;
    Config.logbylevel  = 1;

/**
    <include>rules_config.xml</include>
    <include>sshd_rules.xml</include>
    <include>syslog_rules.xml</include>
    <include>pix_rules.xml</include>
*/
    Config.includes = NULL;
    Config.lists = NULL;
    Config.decoders = NULL;

/****
 *
 *  配置文件是按照 CGLOBAL 写的。这里根据 modules的值确定读配置文件那写 element(xml)
 *
 *  此处只关心 CGLOBAL ----<global>  CRULES ---- <rules>   CALERTS ----<alert> 的xml部分
 *         
 */


    modules|= CGLOBAL;
    modules|= CRULES;
    modules|= CALERTS;


/**
 *  文件模式 文件名称(包含路径名) _Config配置文件  配置文件例如emailconfig
 *  
 */
    /* Reading config */
    if(ReadConfig(modules, cfgfile, &Config, NULL)< 0)
    {
        return(OS_INVALID);
    }

    /* Minimum memory size */
    if(Config.memorysize < 64)
        Config.memorysize = 64;


    return(0);
}


