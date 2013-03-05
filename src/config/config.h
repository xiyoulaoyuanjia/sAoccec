//命名规范   _H文件名大写__H

#ifndef _HCONFIG__H
#define _HCONFIG__H


// 这个与 ossec_config 中的
//
// <ossec_config> 下一级 匹配(global、rules)

#define CGLOBAL       0000001
#define CRULES        0000002
#define CSYSCHECK     0000004
#define CROOTCHECK    0000010
#define CALERTS       0000020
#define CLOCALFILE    0000040
#define CREMOTE       0000100
#define CCLIENT       0000200
#define CMAIL         0000400
#define CAR           0001000
#define CDBD          0002000
#define CSYSLOGD      0004000
#define CAGENTLESS    0020000
#define CREPORTS      0040000




