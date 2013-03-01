ossec 基于主机的入侵检测指南 手册
-----------------
**note** 作者：Andrew Hay 
##目录##
* [关于这本书]()
* [关于dvd]()
* [前沿]()
* [第一章 ossec 即日开始]()
    * [介绍。。]()
    * [关于入侵检测的介绍](#1.2.0)
        * [基于网络的入侵检测]()
        * [基于主机的入侵检测]()
            *   [验证文件完整性]()
            *   [监控注册表]()
            *   [Rootkit 检测]()
            *   [危险响应]()
    * [关于ossec的介绍]()
    * [考虑ossec的安装部署]()
        * [本地安装]()
        * [客户端安装]()
        * [服务器安装]()
        * [那一种适合我?]()
    * [安装之前需要考虑的事项]()
        * [支持的操作系统]()
        * [特别注意事项]()
            *  Microsoft Windows
            *  Sun Solaris
            *  Ubuntu Linux
            *  Mac Os 
    * [总结]()
    * [Solution Fast Track]()
    * [Frequently Asked Question]()
* [第二章安装]()
    * [介绍]()
    * [下载ossec]()
        * [编译与安装]()
        * [完成本地安装]()
    * [基于服务器-客户端安装]()
        * [安装服务器端]()
        * [管理agent]()
        * [安装agent]()
            * [安装unix agent]()
            * [安装windows agent]()
    * [简化安装]()
        * [一次安装 到处copy]()
            * [unix linux bsd]()
        * [导出key]()
            * [unix linux BSD]()
    * [总结]()
* [第三章OSSEC HIDS 配置]()
    * [介绍]()
    * [理解ossec 的配置文件]()
    * [配置日志/警告选项]()
        * [使用电子邮件发出警告]()
        * [电子邮件配置]()
            * [使用syslog接收远端的日志]()
            * [配置数据库]()
        * [规则文件声明]()
        * [阅读日志文件]()
        * [配置完整性检测]()
        * [配置一个agent]()
        * [高级的配置选项]()
        * [总结]()
* [第四章ossec 规则的使用]()
    * [介绍]()
    * [介绍规则]()
    * [理解ossec的分析步骤]()
    * [预处理日志]()
    * [解码日志]()
        * [sshd 日志的例子]()
        * [vsftpd 日志的解码例子]()
        * [使用<parent>选项]()
        * [cisco Pix 日志例子]()
        * [cisco acl 日志例子]()
    * [理解规则]()
        * [原子规则(atmic rules)]()
            * [写规则]()
        * [关联规则]()
    * [运用到真实世界的一个例子]()
        * [规则的严重程度 level 选项]()
        * [规则的频繁程度 frequency 选项]()
        * [忽视一条规则]()
        * [忽视一个ip地址]()
        * [关联复杂的snort警告]()
        * [忽视确认的更改事件]()
    * [为自定义的应用程序写规则/解码文件]()
        * [确认需要提取什么信息]()
        * [创建解码文件]()
        * [创建规则文件]()
        * [监控日志文件]()
    * [总结]()
* [第五章系统完整性验证与Rootkit 检测]()
    * [介绍]()
    * [理解系统完整性检测(syscheck)]()
    * [调整syscheck]()
        * [使用syscheck 规则]()
        * [忽视指定的文件夹]()
        * [为重要的文件增加危险警告级别]()
        * [为在周末的变化增加危险警告级别]()
        * [配置自定义的syscheck检测]()
    * [检测 Rootkits 和 监控策略]()
    * [总结]()
* [第六章警告动作响应]()
    * [介绍]()
    * [关于动作响应()
    * [检测动作响应]()
        * [命令]()
        * [主动响应]()
        * [试着将它们合起来]()
    * [创建一个简单的响应]()
        * [可执行]()
        * [命令]()
        * [响应]()
    * [配置响应超时]()
        * [拒绝执行命令]()
        * [拒绝执行响应]()
    * [总结]()
* [使用ossec的web界面]()
    
* [结尾]()
* [附录A 日志数据挖掘]()
* [附录B 正确配置ossec策略]()
* [附录C 基于主机的IDS的Rootkit 检测]()
* [附录D The OSSEC VMware Guest Image]()
* 




<h2 id="1.2.0">Introducing Intrusion Detection(关于入侵检测的介绍)</h2>

Have you ever wondered what was happening on your network at any given time? 

_在某个时间你是否想过你的网络发生了什么事情。。_

What about the type of traffic trying to get to a server on your network? 

_某种特殊的流量试图通过你的网络到达服务器？_

Intrusion detection is the act of detecting events that have been deemed inappropriate or unwelcome by the business,
organizational unit, department, or group. 

_入侵检测被商业、机构单位、部门、组织用来检测那些不是很合适或者令人讨厌的行为事件_

This can be anything from the emailing of company
secrets to a competitor, to malicious attacks from a host on the Internet, to the viewing of
inappropriate Web content during your lunch break.

_当然这些行为事件包括很多。从发送商业机密到竞争对手的邮件 到来自互联网主机的破坏攻击,再到在你午休都偷看私人网络内容_

Intrusion detection can be performed manually, by inspecting network traffic and logs from
access resources, or automatically, using tools. 

_入侵检测可以手动或者通过工具自动的检测网络流量特性与访问主机而产生的日志文件_

A tools used to automate the processing of intrusion-related information is typically classified as an intrusion detection system (IDS).

_能自动处理与入侵检测相关信息的工具可以称为入侵检测系统_

Before understanding how the Open Source Security (OSSEC) host intrusion detection
system (HIDS) works, we should first review the differences between an HIDS and a network
intrusion detection system (NIDS).

_在了解开源软件主机入侵检测系统(ossec)如何工作之前。我们最好能先来看下 主机入侵检测hids与网络入侵检测nids的区别._




















