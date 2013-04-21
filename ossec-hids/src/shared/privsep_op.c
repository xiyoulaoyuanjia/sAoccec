/* Functions for privilege separation.
 */

/*
 *  包含 依据用户名获得用户uid
 *
 *
 */

#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <unistd.h>

#include "headers/os_err.h"

/*
 * 根据用户名获得用户uid
 *
 */
int Privsep_GetUser(char * name)
{
    int os_uid = -1;

    struct passwd *pw;
    pw = getpwnam(name);
    if(pw == NULL)
        return(OS_INVALID);

    os_uid = (int)pw->pw_uid;
    endpwent();

    return(os_uid);
}

/**
 *
 * 根据用户组名获得用户组id 
 *
 *
 */
int Privsep_GetGroup(char * name)
{
    int os_gid = -1;

    struct group *grp;
    grp = getgrnam(name);
    if(grp == NULL)
        return(OS_INVALID);

    os_gid = (int)grp->gr_gid;
    endgrent();

    return(os_gid);
}

/*
 *设置用户组 包括 setgroups 与 setegid 和 setgid
 *
 */
int Privsep_SetGroup(gid_t gid)
{
/*  感觉不需要设置这个setgroups
 */
    if (setgroups(1, &gid) == -1)
        return(OS_INVALID);

/**
 *  HPUX 不能 调用 setegid?
 *  设置有效组id 与 组id
 */
    #ifndef HPUX
    if(setegid(gid) < 0)
        return(OS_INVALID);
    #endif

    if(setgid(gid) < 0)
        return(OS_INVALID);

    return(OS_SUCCESS);
}

/**
 *   更改根目录
 *  
 */

int Privsep_Chroot(char * path)
{
    if(chdir(path) < 0)
        return(OS_INVALID);

    if(chroot(path) < 0)
        return(OS_INVALID);
/**
 * 是不是可以去掉
 */
    chdir("/");

    return(OS_SUCCESS);
}



















