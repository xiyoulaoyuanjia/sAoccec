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


**Network Intrusion Detection **
 
**基于网络的入侵检测     **

When you hear the term “intrusion detection system,” or “IDS,” you probably think of an NIDS. 

_当你听到ids或者入侵检测时 你第一反应应该时NIDS_

Network intrusion detection systems have become widely used over the past decade
because of the impressive capability to provide a granular view of what is happening on your
network. 

_网络入侵检测得以在过去数十年广泛使用得益于从网络上你可以获得如颗粒般的详细(一帧帧的数据)_

The NIDS monitors network traffic using a network interface card (NIC) that is
directly connected into your network. 

_NIDS 可以使用直接连入网络的网卡监控网络通道_

The monitoring can be implemented by connecting your NIC to a HUB (Figure 1.1), which allows you to monitor all traffic that crosses the hub; 


_如图1.1 那样 监控可以直接连接到网卡到hub上的，可以监控所有经过这个hub(集线器)的网络流量_

connecting to a SPAN port on a switch (Figure 1.2), which mirrors the traffic seen on
another port of the switch; 

_如图1.2 连接到路由器的SPAN端口，可以监控路由器另一个端口的流量_

or connecting to a network tap (Figure 1.3), which is an inline
device that sits between two interfaces and mirrors the traffic that passes between devices.

_或者连接到可以连接到网络并且监控网络上的流量的网络tap上._

![](https://drive.google.com/uc?export=view&id=0B_kkWa4qHwL6NHFZaWtUellwYjQ)

![](https://drive.google.com/uc?export=view&id=0B_kkWa4qHwL6d01Mbnl5dm8xUnM)

![](https://drive.google.com/uc?export=view&id=0B_kkWa4qHwL6d3ByYV9jZHJWbTg)

![](https://drive.google.com/uc?export=view&id=0B_kkWa4qHwL6SXJqZWg3QmUxblU)

![](https://drive.google.com/uc?export=view&id=0B_kkWa4qHwL6SzJBaUF6aHFKQVk)


NIDS is typically deployed to passively monitor a sensitive segment of your network,
such as a DMZ off the firewall where your corporate Web servers are located (Figure 1.4)
or monitoring connections to an internal database that holds your customer credit card
information (Figure 1.5). 

_一般来说NIDS都会部署在网络比较敏感的地方。例如防火墙连接网络的DMZ(如图1.4所示)或者 监控连接的存放客户信用卡信息等重要数据的数据库(如图1.5所示)_

This monitoring allows you to passively watch all communications between your server and the systems attempting to access it.

_这种监控使你可以被动的发现试图进入你系统与服务器的所有连接_

A signature or pattern is used to match specific events, such as an attack attempt, to traffic
seen on your network. 

_可以在网络流量上通过使用签名或者特征去寻找特殊的攻击企图_

If the traffic seen on your network matches your defined IDS signature,
an alert is generated. 

_如果通过网络的流量符合你定义的IDS特征,那么就生成了一条警告_

An alert can also trigger an action, such as logging the alert to a file, sending an email to someone with details of the alert, or following an action to address this alert,
such as adding a firewall rule to block the traffic on another device.

_一条警告可以触发相应的行为。例如 把警告写入文件中，把警告的详细信息发送给某人。或者把产生这个警告的地址加入到防火墙的策略中阻止其通过。_


An NIDS is a powerful monitoring system for your network traffic, but there are some
things to remember before deploying one:

_虽然NIDS是一个很强大的网络流量监控系统。但是在真正部署之前我们需要考虑如下几件事。_

>* What do you do if well-known NIDS evasion techniques are used to bypass your
   NIDS and signatures? Common NIDS evasion techniques such as fragmentation
  attacks, session splicing, and even denial-of-service (DoS) attacks can be used to
 bypass your NIDS, rendering it useless.

>*_如果著名的抵抗NIDS检测的技术被用来绕过你的NIDS系统与特征，你将如何应对？例如常见的片段储存、会话分割(就是把会话数据放到多个数据包中发出) 、拒绝服务攻击等技术绕过你的NIDS的检测使得其失去作用。_


>*What do you do if the communications between hosts are encrypted? With an NIDS
   you are passively monitoring traffic and do not have the ability to look into an encrypted packet.

>* _如果主机之间的会话被加密，你又如何应对？使用NIDS,你只能被动的检测网络流量。而且没有能力查看加密的数据包_

>*What do you do if an attack is used against your server, but it is encrypted? Your
   carefully designed signatures would be unable to catch the attacks that your NIDS
  is deployed to protect against.

>*_如果针对你的服务器的攻击是加密的，你有能怎么样？在你部署防患攻击的NIDS中精心设计的特征库将不能发现这样的攻击行为_

Tuning your NIDS to detect or account for these types of attacks will go a long way to
help you focus your time on actual incidents instead of chasing down false positives. 

_使用NIDS 去检测或者记录这种类型的攻击会花费你很多的时间在误报上而不是真正的危险事件_

Each NIDS must be tuned for the network segment it is monitoring. 

_每一中NIDS系统都必须不断的调整去适应它正在检测的网络部分_

Remember that most NIDS solutions take a top-down approach to comparing traffic against your signature set. 

_记住！大多数NIDS都是采取自上而下的方法把流量与你的特征集去比较_

Reducing  the number of rules in your deployed signature set reduces processor and memory usage on your NIDS solution. 

_对于某一种NIDS方案在部署的特征库上减少规则的数量可以减少内存与处理器的使用率_

If the DMZ your NIDS is deployed on doesn’t contain any Web servers,
you probably do not need to include signatures to detect Web server attacks.

_如果你部署的NIDS的DMZ 不包含任何web服务器,因为不用去检测web攻击所以你可以不包含任何特征库_

Attackers are becoming adept at sidestepping an NIDS, which is why an HIDS is now a
necessary safeguard to supplement your current NIDS deployments. 

_因为攻击者越来越会去逃避NIDS的检测，所以HIDS现在对于你的已经部署的NIDS系统是很有必要的一个安全补充_

Detecting these attacks at the final destination allow you to mitigate the previously mentioned NIDS headaches.

_在最后目的地(被攻击的电脑上)检测到这些攻击可以减轻之前提到NIDS的各种头疼的问题_




**Host-Based Intrusion Detection**

**_基于主机的入侵检测_**

An HIDS detects events on a server or workstation and can generate alerts similar to an
NIDS. 

_HIDS在服务器或者工作站产生的警告信息与NIDS相同_

An HIDS, however, is able to inspect the full communications stream.

_但是，HIDS去可以检测到网络上所有的会话数据包_
 
NIDS evasion techniques, such as fragmentation attacks or session splicing, do not apply because the HIDS is able to inspect the fully recombined session as it is presented to the operating system.

_由于片段储存、会话分割这些抵抗NIDS检测的技术是完全暴露在操作系统上的(会产生相应的日志信息),所以HIDS可以检测到完整的会话信息。也正是这样这些技术也就没有用武之地了。_

Encrypted communications can be monitored because your HIDS inspection can look at
the traffic before it is encrypted. 

_由于HIDS可以在会话加密之前就查看，所以加密会话也是可以监视到的。_

This means that HIDS signatures will still be able to match against common attacks and not be blinded by encryption.

_这也就意味着HIDS 的特征库可以有效的抵抗常见的攻击，即使是加密的。_

An HIDS is also capable of performing additional system level checks that only IDS software
installed on a host machine can do, such as file integrity checking, registry monitoring, log
analysis, rootkit detection, and active response.

_另外,HIDS 也可以实现向安装到主机的IDS才可以做的一样系统级别的检测，例如 完整性检测、监控注册表、日志分析、rootkit 检测、和联动机制_


**File Integrity Checking**

**_文件完整性检测_**

Every file on an operating system generates a unique digital fingerprint, also known as a
cryptographic hash.

_利用常见的hash加密，操作系统中的每一个文件都可以产生一个唯一的 数字指纹_

This fingerprint is generated based on the name and contents of the file (Figure 1.6).

_根据文件的名字与内容可以产生这样的指纹，例如图1.6所示_

 An HIDS can monitor important files to detect changes in this fingerprint when someone, or something, modifies the contents of the file or replaces the file with a completely different version of the file.

_当某人或者其它的什么修改文件的内容或者使用完全不同的版本替换了文件的内容时，HIDS可以通过检测到指纹的变化来监视重要的文件的这些变化。_

![](https://drive.google.com/uc?export=view&id=0B_kkWa4qHwL6dDdwUEFLZGtLRDA)

**Registry Monitoring**

**_注册表监控_**

The system registry is a directory listing of all hardware and software settings, operating system
conﬁgurations, and users, groups, and preferences on a Microsoft Windows system. 

_系统注册表是一个用来存放软硬件的设置、系统配置、windows系统的用户、组和个性化设置的目录_

Changes made by users and administrators to the system are recorded in the system registry keys so that the changes are saved when the user logs out or the system is rebooted. 

_因为用户或者系统管理员对系统所做的更改都会记录到注册表的key中,所以当用户或系统管理员注销或者重启的时候这些更改都会被保存下来。_

The registry also allows you to look at how the system kernel interacts with hardware and software.

_当然注册表也可以帮你观察系统内核是如何与软硬件联系的_

An HIDS can watch for these changes to important registry keys to ensure that a user
or application isn’t installing a new or modifying an existing program with malicious intent.

_HIDS可以用来观察那些重要的注册表的key的改变，以此来确保用户没有恶意的安装或者修改软件_

For example, a password management utility can be replaced with a modiﬁed executable
and the registry key changed to point to the malicious copy (Figure 1.7).

_例如：注册表的key值的改变可以反应电脑的密码管理工具被可执行程序替换这个恶意的行为_


![](https://drive.google.com/uc?export=view&id=0B_kkWa4qHwL6d2Z2dVJwTzJxaW8)

**Rootkit Detection**

**_Rootkit  检测_**

A rootkit is a program developed to gain covert control over an operating system while hiding
from and interacting with the system on which it is installed. 

_rootkit 是一种被设计用来隐藏在操作系统伺机控制系统或者与安装的系统交互的程序_

An installed rootkit can hide services, processes, ports, ﬁles, directories, and registry keys from the rest of the operating system and from the user.

_已经安装的rootkit可以向用户与系统隐藏服务、进程、端口、目录、注册表等项_



**Active Response**

**_联动反应_**

Active response allows you to automatically execute commands or responses when a speciﬁc
event or set of events is triggered. 

_当特别的事件或者事件集被触发时,联动反应自动的执行命令或者反应_

For example, look at Figure 1.8. An attacker launches an attack against your organization’s mail server (1). 
_例如：如果1.8所示,攻击者向你的组织的邮件服务器发动了攻击_

The attack then passes through your ﬁrewall (2), and ﬁnally, transparently, passes by your deployed network tap that inspects all trafﬁc destined for your mail server (3). 

_攻击先通过防火墙,最后通过了部署在邮件服务器上用来检测所有通过的流量的网络trap_


Your NIDS happens to have a signature for this particular attack. 

_NIDS 对于这种攻击有相应的特征库_

The NIDS active response service sends a command to your ﬁrewall (4) to reset the attacker’s session and place a rule blocking that host. 

_这样，NIDS的联动反应机制就会向防火墙发送命令停止会话并加入相应的策略去阻止这台主机的通过防火墙_

When the attacker, whose connection has been reset, tries to initiate the attack again (5), the attacker is blocked.

_当攻击者的连接会话被阻止，他会在去尝试很多次，而由于我们已经修改了防火墙的策略(阻止其ip或者host通过)所以攻击者会被阻止_


The beneﬁts of active response are enormous, but also risky. 

_联动反应的好处与风险是同时存在的。_

For example, legitimate trafﬁc might generate a false positive and block a legitimate user/host if the rules are poorly designed. 

_例如、当策略指定的不恰当时,对于合法的流量可能产生错误的判断从而阻止合法的用户与主机通过防火墙_

If an attacker knows that your HIDS blocks a certain trafﬁc signature, the attacker
could spoof IP addresses of critical servers in your infrastructure to deny you access. 

当然如果攻击者知道你的HIDS的基于某个流量特征库产生相应的策略去阻止这样的ip或者host通过,攻击者就可以伪造你的设备中重要的一些服务器的ip地址去阻止你的访问


This is essentially a DoS attack that prevents your host from interacting with that IP address.

_这实际山就是一种阻止你的主机访问一些ip地址的拒绝服务攻击_

**Introducing OSSEC**

**_OSSEC 简介_ **

OSSEC is a scalable, multiplatform, open source HIDS with more than 5,000 downloads
each month. 

_ossec 是一个可扩展的、跨平台的、每个月超过5000的下载量的开源HIDS_

It has a powerful correlation and analysis engine, log analysis integration, ﬁle
integrity checking, Windows registry monitoring, centralized policy enforcement, rootkit
detection, real-time alerting, and active response. 

_ossec具有强大的关联分析引擎、日志分析、文件完整性检测、windows注册表监控、集中的策略执行与实时的关联反应的特点。_

In addition to being deployed as an HIDS,it is commonly used strictly as a log analysis tool, monitoring and analyzing ﬁrewalls, IDSs,Web servers, and authentication logs. 

_另外ossec通常作为HIDS被部署。这其中应用较多的是作为一种日志分析工具,监控与分析防火墙、IDS系列、web服务器、审计等的日志_

OSSEC runs on most operating systems, includingLinux, OpenBSD, FreeBSD, Mac OS X, Sun Solaris, and Microsoft Windows.

_ossec可以运行在大多数操作系统上面,这其中包括linux、OpenBSD、FreeBSD、Mac OS X、Sun Solaris, and Microsoft Windows 等系统 _

OSSEC is free software and will remain so in the future.

_OSSEC 不管是在现在或者将来都是自由软件_

 You can redistribute it and/or modify it under the terms of the GNU General Public License (version 3) as published by the Free Software Foundation (FSF). 

_你可以在FSF发布的GNU GPL3的协议条款下，自由的发布或者修改ossec_
ISPs, universities, governments, and large corporate data centers are using OSSEC as their main HIDS solution.

_许多ISP,大学，机构，大量的公司数据中心应用OSSEC作为它们的主要的HIDS的解决方案_

The project has contributors from all over the globe and a quarterly release schedule
for major ﬁxes, enhancements, and new features. 

_该项目的代码贡献者来自世界各地，并且每一个季度都会发布一些重要的补丁、与新功能_

Bugs and feature requests can be sent through the OSSEC bug submission page (www.ossec.net/bugs/) or OSSEC mailing lists(www.ossec.net/main/support/). 

_有关bug与功能的意见可以发送到OSSEC的bug提交页面(www.ossec.net/bugs/)或者OSSEC的邮件列表(www.ossec.net/main/support/)_

We will do our best to solve the submitted requests.

_我们会尽力去解决这些提交请求的。_

If you are interested in being a part of this project, the OSSEC team is always open to
new contributors. 

_当然如果你乐于称为我们组织的一员，OSSEC团队的大门向你时刻敞开着。_

The easiest way to get involved with OSSEC is by helping to test the
product. 

_另外，最简单容易的参与OSSEC的方式还是帮助我们测试这款软件_

The OSSEC team is always releasing beta versions and requires good quality control
on every supported version before public release. 

_OSSEC 团队总是会先发布beta测试版本并且在正式的版本发布之前总是会进行高质量的测试_


To get involved in the development side you must know C and be willing to take some time (actually quite some time) to understand how the internals work.

_如果你想参加到OSSEC的开发那么你必须要懂C语言并且乐于花费相当长的时间(事实上确实是相当长的时间)去学习网络方面的知识_

**Planning Your Deployment**

**_考虑部署你的OSSEC_**

Before starting your OSSEC HIDS installation you must know the differences between the
installation types and know how plan your deployment. 

_在考虑开始你的OSSEC的安装之前你必须先知道不同的安装类型的区别并且知道如何部署_

The OSSEC HIDS can be installed on one system, on multiple systems to provide protection for a large network, or on a few systems with the plan to scale the deployment later to secure your entire organization.

_OSSEC HIDS可以安装到一个系统上去为网络上大量的其它多个系统提供保护，也可以安装到几个系统上去为整个组织提供安全保护_

We will discuss three OSSEC installation types to help you understand how to deploy
the OSSEC HIDS in your environment:

>* Local installation: Used to secure and protect a single host
>* Agent installation: Used to secure and protect hosts while reporting back to a
   central OSSEC server
>* Server installation: Used to aggregate information from deployed OSSEC agents
   and collect syslog events from third-party devices

_下来我们将要讨论OSSEC的各种不同的安装类型的区别用来帮你将它部署到你的环境中：_

>* 本地安装:用来保护单个主机的安全
>* 客户端安装:用来保护主机的全员并将获取的报告数据发回远端的OSSEC服务器
>* 服务器安装:用于收集汇总分析来自各个部署的OSSEC客户端与第三方设备收集的Syslog日志信息




******


**_Appendix A_**

**_Log Data Mining_**

>* Solutions in this chapter:

>>* Introduction
>>* Data Mining Intro
>>* Log Mining Intro
>>* Log Mining Requirements
>>* What We Mine For?
>>* Deeper into Interesting
>>* Conclusion

**_附录A_**

**_日志数据挖掘_**
>* 本章主要内容：

>*  1.介绍
>*  2.数据挖掘介绍
>*  3.日志挖掘介绍
>*  4.日志挖掘需求
>*  5.数据挖掘的目的
>*  6.深入理解其有趣性
>*  7.总结


Introduction
A vast majority of log analysis techniques required that an analyst know something specific about what he is looking for in the logs. For example, he might “scan” the server logs for “known bad” log ( just as OSSEC does!) records that indicate attacks, exploits, server failures, or whatever other infraction of interest by using string matching or regular expressions. One can observe that it requires significant domain knowledge; in the preceding case,expertise in security and specific type of logs available for analysis on all stages of the log analysis process, from reviewing the data to running queries and searches all the way to interpreting the results to acting on the conclusions. In other words, you have to know what questions to ask before you get the answer you want—a tricky proposition at best. In addition, it requires an immense amount of patience to merely start the task, since one can be going through logs for a long time without finding the aberrant line or a group of lines; or, it might not even be there.


_绝大部分日志分析技术都要求分析师知道他正在查找的相关日志的一些特殊信息。例如，一个分析师会通过字符串匹配或者正则表达式在服务器日志记录中浏览预先定义好的错误日志记录（就像OSSEC一样），而这些错误日志记录可以是表示攻击、开发、服务失败或者其他任何对服务不利信息。通过前面的例子，我们注意到，在日志分析的整个阶段，为了根据结果解释结果，从数据回顾到运行查询和搜索，我们都需要有重要的领域知识、安全专业知识以及特殊的日志记录信息。也就是说，在你想得到你想要的答案之前你最好先知道你要问的问题 —— 一个复杂的命题。另外，仅仅是开始日志分析任务就需要巨大的耐心，因为你可能处理日志很长时间了也没发现一行异常或者一组异常，甚至异常根本就不在那。_


In this appendix, we describe methods for discovering interesting patterns in log files for security without specifically knowing what we look for and thus without the onerous “patience requirement” and the expensive “expertise requirements” on all analysis stages. We review some practical results of such methods, demonstrate the tools, and discuss how they can be used in various scenarios that occur in the process of maintaining security and availability of IT operation as well as assisting with compliance initiatives. 

_在本附录中，我们将介绍，不明确知道我们要查找的信息条件下，在安全日志文件中查找我们感兴趣模块的一系列方法。由于不明确要查找的信息，在日志分析的各个阶段也就不用繁重的“耐心要求”和昂贵的“专业要求”。同时，我们也回顾了这些方法的实践结果，介绍了工具，并讨论了这些方法在维持安全、IT操作的可行性以及协助合规计划过程中出现的各种不同场景中的应用。_


Since the techniques we will cover are similar in many regards to data mining, we
need to step back and provide a brief data mining overview for those readers not familiar with it.













