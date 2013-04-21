/**
 *
 *
 *
 */

#ifndef _CCONFIG__H
#define _CCONFIG__H
#include "shared.h"


/* Configuration structure */
typedef struct __Config
{
/**
 *  共29个 GeoIP support  3个  所以一般而言为26个
 * 
 *   
 */  

    u_int8_t logall;
    u_int8_t stats;
    u_int8_t integrity;
    u_int8_t syscheck_auto_ignore;
    u_int8_t syscheck_alert_new;
    u_int8_t rootcheck;
    u_int8_t hostinfo;
    u_int8_t mailbylevel;
    u_int8_t logbylevel;
/**
 *  好像没有用
 */
    u_int8_t logfw;

/**
 *   什么是 Prelude  support ???? 
 */
    /* Prelude support */
    u_int8_t prelude;
    /* which min. level the alert must be sent to prelude */
    u_int8_t prelude_log_level;
    /* prelude profile name */
    char *prelude_profile;

/**
 *    
 *   什么是 Picviz support
 */
    /* Picviz support */
    u_int8_t picviz;
    char *picviz_socket;

    /* Not currently used */
    u_int8_t keeplogdate;

    /* Mail alerting */
    short int mailnotify;

    /* For the active response */
    int ar;

    /* For the correlation */
    int memorysize;

/**
 *
 *
 *
 */

    /* List of files to ignore (syscheck) */
    char **syscheck_ignore;

    /* List of ips to never block */
    os_ip **white_list;

    /* List of hostnames to never block */
    OSMatch **hostname_white_list;

    /* List of rules */
    char **includes;

    /* List of Lists */
    char **lists;

    /* List of decoders */
    char **decoders;
/**
 *  好像也没有用
 */
    /* Global rule hash. */
    void *g_rules_hash;

#ifdef GEOIP
    /* GeoIP support */
    u_int8_t loggeoip;
    char *geoip_db_path;
    char *geoip6_db_path;
#endif

}_Config;


#endif
