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















