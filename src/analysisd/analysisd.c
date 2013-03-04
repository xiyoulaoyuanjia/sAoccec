/*
 *
 * ossec-analysisd 
 *
 * Responsible for correlation and log decoding.
 *
 *
 */


/**
 * 设置默认名称 
 */
#ifndef ARGV0
   #define ARGV0 "ossec-analysisd"
#endif

/***
 *
 *
 *
 *
 *
 */
#include "analysisd.h"
#include "shared.h"




/* Hourly alerts */
int hourly_alerts;
int hourly_events;
int hourly_syscheck;
int hourly_firewall;


/** int main(int argc, char **argv)
 *  */
// 验证规则或者独立deamon进程 使用
#ifndef TESTRULE
int main(int argc, char **argv)
#else
int main_analysisd(int argc, char **argv)
#endif
{

/*
 *  test_config 测试标志 后面有验证其值是否设置退出
 *
 *  run_foreground 与 deamon对应的 非守护进程模式 后面依据其值判断是否设置程序为deamon模式
 *
 *  m_queue
 *
 *  c
 */
    int c = 0, m_queue = 0, test_config = 0,run_foreground = 0;
/*
 *  dir  默认的安装路径
 * 
 *  user 默认的用户名称
 *
 *  group 默认的组名称
 *
 *  uid  gid  用户id与组id
 *
 */
    char *dir = DEFAULTDIR;
    char *user = USER;
    char *group = GROUPGLOBAL;
    int uid = 0,gid = 0;

/*
 *  cfg 默认配置文件
 */
    char *cfg = DEFAULTCPATH;
/*
 *  设置名称
 */
    OS_SetName(ARGV0);

/**
 *
 *
 *
 *
 */

    thishour = 0;
    today = 0;
    prev_year = 0;
    memset(prev_month, '\0', 4);

/**
 *
 *
 *
 */
    hourly_alerts = 0;
    hourly_events = 0;
    hourly_syscheck = 0;
    hourly_firewall = 0;

/**
 *  解析传入进来的参数
 *
 *  -V 打印版本信息
 *  -h 打印帮助信息
 *  -d 调试模式运行 
 *  -f 前台运行
 *  -u <user>   Run as 'user'   以user身份运行
 *  -g <group>  Run as 'group'  以group身份运行
 *  -d <dir>    Chroot to 'dir' 转换根目录
 *  -c <config> Read the 'config' file  读配置文件
 *  -t 测试配置
 */

   while((c = getopt(argc, argv, "Vtdhfu:g:D:c:")) != -1){
        switch(c){
            case 'V':
                print_version();
                break;
            case 'h':
                help(ARGV0);
                break;
            case 'd':
                nowDebug();
                break;
            case 'f':
                run_foreground = 1;
                break;
            case 'u':
                if(!optarg)
                    ErrorExit("%s: -u needs an argument",ARGV0);
                user = optarg;
                break;
            case 'g':
                if(!optarg)
                    ErrorExit("%s: -g needs an argument",ARGV0);
                group = optarg;
                break;
            case 'D':
                if(!optarg)
                    ErrorExit("%s: -D needs an argument",ARGV0);
                dir = optarg;
                break;
            case 'c':
                if(!optarg)
                    ErrorExit("%s: -c needs an argument",ARGV0);
                cfg = optarg;
                break;
            case 't':
                test_config = 1;
                break;
            default:
                help(ARGV0);
                break;
        }

    }

/**
 * 开始程序
 */

    /* Starting daemon */
/*
 *  only dbg_flag >=1 的时候
 */
    debug1(STARTED_MSG,ARGV0);

/**
 *
 */
	DEBUG_MSG("%s: DEBUG: Starting on debug mode - %d ", ARGV0, (int)time(0));
		      

/***
 *  
 *  根据用户名称与组名称获得用户uid与组id 从而通过其返回值验证 用户名与组名是否合法
 */
    /*Check if the user/group given are valid */
    uid = Privsep_GetUser(user);
    gid = Privsep_GetGroup(group);
    if((uid < 0)||(gid < 0))
        ErrorExit(USER_ERROR,ARGV0,user,group);

/***
 *
 *  
 *
 */
    /* Found user */
    debug1(FOUND_USER, ARGV0);

/**
 *
 *
 *
 *
 *
 */
    /* Initializing Active response */
/*
    AR_Init();
    if(AR_ReadConfig(test_config, cfg) < 0)
    {
        ErrorExit(CONFIG_ERROR,ARGV0, cfg);
    }
*/
/**
 *  DEBUG: Active response initialized ...
 */
    debug1(ASINIT, ARGV0);

/**
 *
 *
 */
    /* Reading configuration file */

    if(GlobalConf(cfg) < 0)
    {
        ErrorExit(CONFIG_ERROR,ARGV0, cfg);
    }

/**
 *  DEBUG: Read configuration ...
 */
    debug1(READ_CONFIG, ARGV0);

/***
 *
 *
 *
 */
    /* Fixing Config.ar */
/*
    Config.ar = ar_flag;
    if(Config.ar == -1)
        Config.ar = 0;
*/
/**
 *  获得服务器名称 
 *
 */

    /* Getting servers hostname */
    memset(__shost, '\0', 512);
    if(gethostname(__shost, 512 -1) != 0)
    {
        strncpy(__shost, OSSEC_SERVER, 512 -1);
    }
    else
    {
        char *_ltmp;
/**
 *  例如 去掉 localhost.localdomain 后面的domain
 */	
        /* Remove domain part if available */
        _ltmp = strchr(__shost, '.');
        if(_ltmp)
            *_ltmp = '\0';
    }

    /* going on Daemon mode */
    if(!test_config && !run_foreground)
    {
/**
 *  设置deamon标志
 */
        nowDaemon();
/**
 *   Daemonize a process.. 
 */
        goDaemon();
    }

/**
 *   prelude 没有搞清楚
 *   
 */
    /* Starting prelude */
/*
    #ifdef PRELUDE
    if(Config.prelude)
    {
        prelude_start(Config.prelude_profile, argc, argv);
    }
    #endif
*/
/**
 *   Picviz socket ?????
 *  
 */
    /* Opening the Picviz socket */
/*
    if(Config.picviz)
    {
        OS_PicvizOpen(Config.picviz_socket);
        chown(Config.picviz_socket, uid, gid);
    }

*/

/**
 *  设置用户组 包括 setgroups 与 setegid 和 setgid
 *  下面还要设置用户id 当这两个同时需要设置的时候，先后顺序还是很重要的
 */
    /* Setting the group */
    if(Privsep_SetGroup(gid) < 0)
        ErrorExit(SETGID_ERROR,ARGV0,group);

/**
 *  更改根目录 包括chdir chroot
 */
    /* Chrooting */
    if(Privsep_Chroot(dir) < 0)
        ErrorExit(CHROOT_ERROR,ARGV0,dir);

/*
 * 设置 chroot 标志
 */
    nowChroot();

/***
 *
 *
 *
 *
 */

    /*
     * Anonymous Section: Load rules, decoders, and lists
     *
     * As lists require two pass loading of rules that make use of list lookups
     * are created with blank database structs, and need to be filled in after
     * completion of all rules and lists.
     */
    {
        {
            /* Initializing the decoders list */
            OS_CreateOSDecoderList();
//
//            if(!Config.decoders)
//            { /* Legacy loading */
//                /* Reading decoders */
//                if(!ReadDecodeXML(XML_DECODER))
//                {
//                    ErrorExit(CONFIG_ERROR, ARGV0,  XML_DECODER);
//                }
//
//                /* Reading local ones. */
//                c = ReadDecodeXML(XML_LDECODER);
//                if(!c)
//                {
//                    if((c != -2))
//                        ErrorExit(CONFIG_ERROR, ARGV0,  XML_LDECODER);
//                }
//                else
//                {
//                    if(!test_config)
//                        verbose("%s: INFO: Reading local decoder file.", ARGV0);
//                }
//            }
//            else
//            { /* New loaded based on file speified in ossec.conf */
//                char **decodersfiles;
//                decodersfiles = Config.decoders;
//                while( decodersfiles && *decodersfiles)
//                {
//                    if(!test_config)
//                        verbose("%s: INFO: Reading decoder file %s.", ARGV0, *decodersfiles);
//                    if(!ReadDecodeXML(*decodersfiles))
//                        ErrorExit(CONFIG_ERROR, ARGV0, *decodersfiles);
//
//                    free(*decodersfiles);
//                    decodersfiles++;
//                }
//            }
//
//            /* Load decoders */
//            SetDecodeXML();
//
        }
//        { /* Load Lists */
//            /* Initializing the lists of list struct */
//            Lists_OP_CreateLists();
//            /* Load each list into list struct */
//            {
//                char **listfiles;
//                listfiles = Config.lists;
//                while(listfiles && *listfiles)
//                {
//                    if(!test_config)
//                        verbose("%s: INFO: Reading loading the lists file: '%s'", ARGV0, *listfiles);
//                    if(Lists_OP_LoadList(*listfiles) < 0)
//                        ErrorExit(LISTS_ERROR, ARGV0, *listfiles);
//                    free(*listfiles);
//                    listfiles++;
//                }
//                free(Config.lists);
//                Config.lists = NULL;
//            }
//        }
//        { /* Load Rules */
//            /* Creating the rules list */
//            Rules_OP_CreateRules();
//
//            /* Reading the rules */
//            {
//                char **rulesfiles;
//                rulesfiles = Config.includes;
//                while(rulesfiles && *rulesfiles)
//                {
//                    if(!test_config)
//                        verbose("%s: INFO: Reading rules file: '%s'", ARGV0, *rulesfiles);
//                    if(Rules_OP_ReadRules(*rulesfiles) < 0)
//                        ErrorExit(RULES_ERROR, ARGV0, *rulesfiles);
//
//                    free(*rulesfiles);
//                    rulesfiles++;
//                }
//
//                free(Config.includes);
//                Config.includes = NULL;
//            }
//
//            /* Find all rules with that require list lookups and attache the
//             * the correct list struct to the rule.  This keeps rules from having to
//             * search thought the list of lists for the correct file during rule evaluation.
//             */
//            OS_ListLoadRules();
//        }
//
//
    }


    /* Fixing the levels/accuracy */
/*
    {
        int total_rules;
        RuleNode *tmp_node = OS_GetFirstRule();

        total_rules = _setlevels(tmp_node, 0);
        if(!test_config)
            verbose("%s: INFO: Total rules enabled: '%d'", ARGV0, total_rules);
    }
*/


    /* Creating a rules hash (for reading alerts from other servers). */
/*
    {
        RuleNode *tmp_node = OS_GetFirstRule();
        Config.g_rules_hash = OSHash_Create();
        if(!Config.g_rules_hash)
        {
            ErrorExit(MEM_ERROR, ARGV0);
        }
        AddHash_Rule(tmp_node);
    }

*/

    /* Ignored files on syscheck */
/*
    {
        char **files;
        files = Config.syscheck_ignore;
        while(files && *files)
        {
            if(!test_config)
                verbose("%s: INFO: Ignoring file: '%s'", ARGV0, *files);
            files++;
        }
    }
*/

    /* Checking if log_fw is enabled. */
/*
    Config.logfw = getDefine_Int("analysisd",
                                 "log_fw",
                                 0, 1);
*/

    /* Success on the configuration test */
//    if(test_config)
//        exit(0);


    /* Verbose message */
//    debug1(PRIVSEP_MSG, ARGV0, dir, user);


    /* Signal manipulation      */
//    StartSIG(ARGV0);


    /* Setting the user */
//    if(Privsep_SetUser(uid) < 0)
//        ErrorExit(SETUID_ERROR,ARGV0,user);


    /* Creating the PID file */
//    if(CreatePID(ARGV0, getpid()) < 0)
//        ErrorExit(PID_ERROR,ARGV0);


    /* Setting the queue */
//    if((m_queue = StartMQ(DEFAULTQUEUE,READ)) < 0)
//        ErrorExit(QUEUE_ERROR, ARGV0, DEFAULTQUEUE, strerror(errno));


    /* White list */
/*
    if(Config.white_list == NULL)
    {
        if(Config.ar)
            verbose("%s: INFO: No IP in the white list for active reponse.", ARGV0);
    }
    else
    {
        if(Config.ar)
        {
            os_ip **wl;
            int wlc = 0;
            wl = Config.white_list;
            while(*wl)
            {
                verbose("%s: INFO: White listing IP: '%s'",ARGV0, (*wl)->ip);
                wl++;wlc++;
            }
            verbose("%s: INFO: %d IPs in the white list for active response.",
                    ARGV0, wlc);
        }
    }
*/

    /* Hostname White list */
/*
    if(Config.hostname_white_list == NULL)
    {
        if(Config.ar)
            verbose("%s: INFO: No Hostname in the white list for active reponse.",
            ARGV0);
    }
    else
    {
        if(Config.ar)
        {
            int wlc = 0;
            OSMatch **wl;

            wl = Config.hostname_white_list;
            while(*wl)
            {
                char **tmp_pts = (*wl)->patterns;
                while(*tmp_pts)
                {
                    verbose("%s: INFO: White listing Hostname: '%s'",ARGV0,*tmp_pts);
                    wlc++;
                    tmp_pts++;
                }
                wl++;
            }
            verbose("%s: INFO: %d Hostname(s) in the white list for active response.",
                    ARGV0, wlc);
        }
    }

*/
    /* Start up message */
//    verbose(STARTUP_MSG, ARGV0, (int)getpid());


    /* Going to main loop */
//    OS_ReadMSG(m_queue);

/*
    if (Config.picviz)
    {
        OS_PicvizClose();
    }
*/
    exit(0);
}










