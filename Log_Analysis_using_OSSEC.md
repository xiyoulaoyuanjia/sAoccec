Log Analysis using OSSEC
==

**目录**

>* 定义LIDS:Log-Based IDS (基于日志的IDS分析)
>* OSSEC 总览
>* 安装演示demo
>* 用OSSEC 解码与分析日志
>* 学会写decoders
>* 学会写rules
>* ossec在分析警告与规则的实例

**基本概念**

>* OSSEC 是安全日志分析工具

>>* 它不是日志管理工具
>>* 并不是每一条日志都储存，它只存贮那些达到一定级别的日志
>>* 不过还是建议管理与长期储存所有的日志

>* 安全日志分析被称为LID(s)

>>* 基于日志的入侵检测系统
>>* 因为很多用户仅仅使用ossec的日志分析方面，所以甚至可以称为OSSEC LIDS


**定义LIDS**

>* 基于日志的入侵检测
>>* 在某些日志作为主要来源信息的环境下，基于日志的检测与分析就是检测攻击的主要方法与技术
>>* LIDS 还可以用来检测用户的不恰当的操作、脆弱的策略配置、与其它的一些不合适的活动。

**LIDS的优点**

>* 实施很廉价
>>* 例如:OSSEC 源码免费
>>* 安装OSSEC并不需要很昂贵的硬件要求

>* 已知的高安全加密协议
>>* 例如通信使用SSHD 与 SSL 就是很好的例子

>* Visibility of system activity (kernel, internal daemons,..)

>* 它可以包括几乎所有的系统与组建
>>* 因为它们几乎都有各自类别的日志
>>* 已经包含防火墙、路由器、web服务器、中间件等

**OSSEC是什么？**

>* 基于主机的开源HIDS
>* 官网：http://www.ossec.net
>* 可以如下的任务
>>* 日志分析
>>* linux与windows等各系统的文件完整性检测
>>* windows 的注册表完整性检测
>>* 主机的异常检测 例如对于unix的 rootkit 检测
>>* 联动反应机制

_ossec具有强大的关联分析引擎、日志分析、文件完整性检测、windows注册表监控、集中的策略执行与实时的关联反应的特点。_

**为什么使用OSSEC？**

>* 它可以很好的解决一些实际问题例如日志分析
>* 免费 (as in cookies and speech)
>* 容易安装
>* 容易定制 (基于xml的更改配置与增加规则)
>* 易扩展性(c/s体系)
>* 基于windows Solaris linux BSD系等等的多平台应用
>* 默认情况下使用它是安全的
>* 自带了上百种的 decoders/rules 例如：

        Unix Pam, sshd (OpenSSH), Solaris telnetd, Samba, Su, Sudo, Proftpd,
        Pure-ftpd, vsftpd, Microsoft FTP server, Solaris ftpd, Imapd, Postfix,
        Sendmail, vpopmail, Microsoft Exchange, Apache, IIS5, IIS6, Horde IMP,
        Iptables, IPF. PF, Netscreen, Cisco PIX/ASA/FWSM, Snort, Cisco IOS,
        Nmap, Symantec AV, Arpwatch, Named, Squid, Windows event logs, etc
        ,etc,

**其它参考**



**安装OSSEC**

>* 两种方式
>>* 本地化 (当你只需要监控一个系统的时候)
>>* 客户端/服务器 多数集中化的分析(推荐这种)

>* 安装Demo(1.2版本之后)
    tar -zxvf ossec*.tar.gz
    cd ossec*
    ./install.sh
    ... (answer all questions)
    /var/ossec/bin/ossec-control start (after completed)

**理解OSSEC的工作方式**

>* 两种方式工作方式
>>* 本地化 (当你只需要监控一个系统的时候)
>>* 客户端/服务器(推荐这种)

>* 默认安装在/var/ossec 路径下
>* /var/ossec/etc/ossec.conf 是主要的配置文件
>* decoders 在/var/ossec/etc/decoders.xml 文件下
>* 可执行文件在 /var/ossec/bin/ 目录下
>* /var/ossec/rules/*.xml 所有的规则文件
>* /var/ossec/logs/alerts.log 是警告日志
>* ossec-control 控制左右的组建程序

**理解内部进程**

>* 记住默认的安装是安全的
>>* 默认的安装方式需要用户的root权限允许
>>* 不使用root权限(更安全)是没有办法安装的

>* 每一个进程都是在限制的用户权限下运行任务的
>>* 每一个都是chroot到ossec根目录下工作的
>>* 每一个都是在ossec用户(包括ossec建立的几个用户帐号与组帐号)下工作的

>* 进程列表

>>* Analysisd chroot到ossec用户运行
>>* Remoted   chroot到ossecr 用户运行
>>* Maild     chroot到ossecm 用户运行
>>* Logcollector        作为root用户读日志文件，但是并不分析日志
>>* Agentd              仅仅作为客户端时有用 并且作为ossec用户运行

>* 每一个守护进程仅仅完成十分有限的任务

>>* Analysisd 主函数 完成所有的日志分析工作
>>* Remoted  从 多个远程客户端处接受日志
>>* Logcollector 读入包括 syslog windows  event log IIS 等所有日志
>>* Agentd  向 服务器发送日志
>>* Maild    向邮箱发送警告信息
>>* Execd  执行 相应的联动反应机制
>>* Monitord 监视远程客户端状态(在线或者掉线) 压缩与标记日志文件

>* ossec-control 用来管理所有的守护进程的开始与停止的

>* 本地安装ossec日志分析流程图

![](http://vdisk-thumb-2.wcdn.cn/frame.1024x768/data.vdisk.me/55890007/b4aaed20753d42d1c09646b7841479c8cdb76257?ip=1363191600,10.75.7.26&ssig=QKhHGzm4mu&Expires=1363190400&KID=sae,l30zoo1wmz)

>* 客户机/服务器 体系 日志分析流程

>>* ossec-logcollector 收集日志
>>* 解码与分析是ossec-analysisd
>>* ossec-maild 发送警告
>>* ossec-execd 执行联动反应

**网络通信**

![](http://vdisk-thumb-4.wcdn.cn/frame.1024x768/data.vdisk.me/55890007/55bc3892d5ce3f367e2258e856f1db857bb1fbd3?ip=1363278000,10.73.26.25&ssig=OAwnu7yxIB&Expires=1363276800&KID=sae,l30zoo1wmz)

>* 客户端与服务器端的网络通信
>>* 使用zlib压缩
>>* 使用blowfish的共享key加密算法
>>* 默认使用 1514 端口加密
>>* 多平台 linux windows unix 等等


**深入理解日志分析**

>* 关注理解主进程(ossec-ananlysisd)
>>* 它做日志解码与解析
>>* 它做了很多工作(???)

>* 日志  pre-decoding
>* 日志  decoding
>* 日志分析
>* 一些警告的例子

**日志的内部流程**

![](http://vdisk-thumb-2.wcdn.cn/frame.1024x768/data.vdisk.me/55890007/1f28aad302061ff1b1d9cf481ce4974a58a7eee7?ip=1363278000,10.73.26.25&ssig=86n6j8FVKI&Expires=1363276800&KID=sae,l30zoo1wmz)

>* analysisd 函数内部日志的流程

>* 包含3部分内容
>>* Pre-decoding (分离包括 time等部分)
>>* Decoding(使用用户定义的表达式)
>>* Signatures(使用用户定义的规则)

**Pre-decoding 预处理**

>* 从log日志中提取基本信息
>>* Hostname,program name ，和 sysylog日志的头部的日期
>>* 日志必须高度格式化的

>* OSSEC 如何做的?
>>* 接收到的日志的格式
    Apr 13 13:00:01 enigma syslogd: restart
>>* OSSEC 内部提取到的有用的信息
>>>* time/date -> Apr 13 13:00:01
>>>* hostname -> enigma
>>>* program_name -> syslogd
>>>* log -> restart

>* 对于SSHD信息的 Decoding 处理
>>* Apr 14 17:32:06 enigma sshd[1025]: Accepted password for root from 192.168.2.190 port 1618 ssh2
>>* 经过预处理之后提取到的什么有用的信息?
>>>* time/date -> Apr 14 17:32:06
>>>* hostname -> enigma
>>>* program_name -> sshd
>>>* log -> Accepted password for root from 192.168.2.190 port ...

>* 对于 一条ASL 信息的Decoding处理
>>* 接收到的日志为：
    [Time 2006.12.28 15:53:55 UTC] [Facility auth] [Sender sshd] [PID 483]
    [Message error: PAM: Authentication failure for username from
    192.168.0.2] [Level 3] [UID -2] [GID -2] [Host mymac]
>>* 经过预处理之后提取到的什么有用的信息?
>>>* time/date -> Dec 28, 2006 15:53:55
>>>* hostname -> mymac
>>>* program_name -> sshd
>>>* log -> error: PAM: Authentication failure for username from 192.168.0.2

**日志 Decoding**

>* 从一条日志中识别关键信息的过程
>>* 许多时间信息我们并不是很关注
>>* 一般而言ossec具有上百条的decoders信息   
>>* 一般而言我们提取到诸如 源ip、名称、id 等信息
>>*  用户在decoder.xml 中自定义了许多decoders信息
>>* OSSEC包含3个主要的部分

>* 日志decoder之后能提取出什么信息?
    Apr 14 17:32:06 enigma sshd[1025]: Accepted password for root from 192.168.2.190 port 1618 ssh2

    time/date -> Apr 14 17:32:06
    hostname -> enigma
    program_name -> sshd
    log -> Accepted password for root from 192.168.2.190 port ...
    srcip -> 192.168.2.190
    user -> root

**学会写decoders**

>* 写decoders需要什么？
>>* decoders 存放在etc/decoders.xml 文件中
>>* 给写的decoder取一个有意的的名称以便在rules中可以来引用
>>* 提取在rules中可以使用的有用的信息

>* sshd 例子
>>* 我们想要提取出使用者的姓名与源ip
>>* 如果在预处理阶段提取的program name 是 sshd 可以把 decoder 写成如下的 正则表达式

    <decoder name="sshd-success">
    <program_name>sshd</program_name>
    <regex>^Accepted \S+ for (\S+) from (\S+) port </regex>
    <order>user, srcip</order>
    </decoder>


>* 写 decoders 的一些原则
>>* decoders 最好含有 prematch 或者 program_name
>>* 写的正则表达式的部分是用来提取有用的部分的
>>* order 部分是用来指示 提取的每一部分的含义
>>* order 可以的取值为 id, srcip, dstip, srcport, dstport, url, action, status, user, location, etc

>>* 正则表达式的属性 offset 可以是 after_prematch 或者 after_parent

>* vsftpd 的例子

Sun Jun 4 22:08:39 2006 [pid 21611] [dcid] OK LOGIN: Client
"192.168.2.10"

>>* decoder 的例子

    <decoder name="vsftpd">
    <prematch>^\w\w\w \w\w\w\s+\d+ \S+ \d+ [pid \d+] </prematch>
    <regex offset="after_prematch">Client "(\d+.\d+.\d+.\d+)"$</regex>
    <order>srcip</order>
    </decoder>

>* 用 parent 元素来指示多个 decoders 为一组(深层次的)
>>* 使用 parent 标签(也就是元素) 确定 decoder的 parent 
>>* 这样子会创建一个树状结构，只有父亲节点匹配上之后才会去匹配孩子几点

>* sshd 的第二个例子

    <decoder name="sshd">
    <program_name>^sshd</program_name>
    </decoder>
    <decoder name="sshd-success">
    <parent>sshd</parent>
    <prematch>^Accepted</prematch>
    <regex offset="after_prematch">^ \S+ for (\S+) from (\S+) port </regex>
    <order>user, srcip</order>
    </decoder>

>* sshd 的第三个例子

    <decoder name="sshd">
    <program_name>^sshd</program_name>
    </decoder>
    <decoder name="sshd-success">
    <parent>sshd</parent>
    <prematch>^Accepted</prematch>
    <regex offset="after_prematch">^ \S+ for (\S+) from (\S+) port </regex>
    <order>user, srcip</order>
    </decoder>
    <decoder name="ssh-failed">
    <parent>sshd</parent>
    <prematch>^Failed \S+ </prematch>
    <regex offset="after_prematch">^for (\S+) from (\S+) port </regex>
    <order>user, srcip</order>
    </decoder>

>* Apache 的访问日志例子

从  192.168.2.190 - - [18/Jan/2006:13:10:06 -0500] "GET /xxx.html HTTP/1.1" 200 1732   日志中可以 提取出 srcip, id and url

使用如下的 docoders

    <decoder name="web-accesslog">
    <type>web-log</type>
    <prematch>^\d+.\d+.\d+.\d+ </prematch>
    <regex>^(\d+.\d+.\d+.\d+) \S+ \S+ [\S+ \S\d+] </regex>
    <regex>"\w+ (\S+) HTTP\S+ (\d+) </regex>
    <order>srcip, url, id</order>
    </decoder>


**日志Rules规则**

>* 在 decoding(解码)之后 就该去匹配规则了
>>* 内部是树状结构存贮的
>>* 用户定义在xml文件中
>>* 简单写
>>* 匹配 在 decoderd 中提取来的信息
>>* 因为需要解码所以日志的格式必须在初期固定
>>* ossec 现在默认具有多于400条的规则库

>* 两种类型的规则
>>* 单条的原子结构的规则
>>* 多条树状结构的日志规则


**写自己的规则**

>* 写第一个规则时的一些问题?
>>* 一个唯一的整数的rule id 
>>* 范围从0(最低)到15(最高) 之间的level 值
>>* 默认情况下level 为0 是忽视的不会触发警告
>>* 用正则匹配 srcip id user 等

>*简单的sshd 的 规则的第一个例子

    <rule id = "111" level = "5">
    <decoded_as>sshd</decoded_as>
    <description>Logging every decoded sshd message</description>
    </rule>

如果日志被解析为 sshd 则会开始匹配 id为 111的rules了

第二个 sshed 失败信息的例子

    <rule id = "111" level = "5">
    <decoded_as>sshd</decoded_as>
    <description>Logging every decoded sshd message</description>
    </rule>
    <rule id=”122” level=”7”>
    <if_sid>111</if_sid>
    <match>^Failed password</match>
    <description>Failed password attempt</description>
    </rule>

>>* 这里写了依赖第一条规则的第二条规则
>>* 等级为7的较危险的等级为7
>>* 由于if_sid的影响这里只有在第一个匹配的情况下才会开始这里的匹配
>>* match 是教简单的匹配方式

>* rule 更多的元素选项
>>* 我们会写 依赖上面两个的第三个规则例子
>>* 只有在上面两个个匹配的情况下才会开始这里的匹配
>>* 这里会判断 decoder解析下来的 hostname 是不是已mainserver 开头的
>>* 这里还会判断decoders解析到的ip地址范围在不在这里(不再才会匹配)

    <rule id=”122” level=”7”>
    <if_sid>111</if_sid>
    <match>^Failed password</match>
    <description>Failed password attempt</description>
    </rule>
    <rule id=”133” level=”13”>
    <if_sid>122</if_sid>
    <hostname>^mainserver</hostname>
    <srcip>!192.168.2.0/24</srcip>
    <description>Higher severity! Failure on the main server</description>
    </rule>

>* apache 的网络日志 的规则例子

>* 先写一个基本的网络日志的格式(id 为 501)
>* 在它之前应该匹配的HTTP的错误日志的id 为 4××| 5 ××
>* 这里在rule里面使用了id 元素 的例子

    <rule id=”501” level=”3”>
    <decoded_as>web_log</decoded_as>
    <description>Generic rule for apache logs</description>
    </rule>
    <rule id=”502” level=”6”>
    <if_sid>501</if_sid>
    <id>^4|^5</id>
    <description>Log with id 4xx or 5xx</description>
    </rule>

**规则匹配的框架**

![](http://vdisk-thumb-1.wcdn.cn/frame.1024x768/data.vdisk.me/55890007/53db9f22e2532775b019927e60ded7a7720c03c1?ip=1363278000,10.73.26.25&ssig=lHrAWYZaoY&Expires=1363276800&KID=sae,l30zoo1wmz)

>* 前5个日志规则的流程
>>* 向大多数其它的日志分析工具一样这里并不是水平的
>>* 匹配速度很快
>>* 平均每7/8个规则是一种的日志的(这里一共有400条日志规则)

>* 介绍几种更先进的日志规则选项

>>* sshd 成功登录的规则
>>* 匹配时间、日期等基本的策略的 选项
>>* 可以使用group 更好的管理规则

    <rule id = "153" level = "5">
    <if_sid>111</if_sid>
    <match>Accepted password </match>
    <description>Successful login</description>
    <group>login_ok</group>
    </rule>
    <rule id=”154” level=”10”>
    <if_sid>153</if_sid>
    <time>6 pm - 8:30 am</time>
    <description>Alert! Logins outside business hours!</description>
    <group>login_ok,policy_violation</group>
    </rule>

>* 多条树状结构的日志
>>* 多次尝试登录失败的日志
>>* 设置 frequency 与 timeframe 选项的规则
>>* if_matched_sid: 用来表示在Y时间范围内 匹配 规则  X次
>>*  same_source_ip 来自相同的ip

    <rule id=”133” level=”7”>
    <if_sid>111</if_sid>
    <match>^Failed password</match>
    <description>Failed password attempt</description>
    </rule>
    <rule id=”1050” level=”11” frequency=”5” timeframe=”120”>
    <if_matched_sid>133</if_matched_sid>
    <same_source_ip />
    <description>Multiple failed attempts from same IP!</description>
    </rule>

**现实世界真实的规则**

>* 不要修改 默认的规则
>>* 在每次版本升级都会被覆盖
>>* 在 local_rules.xml 中修改 而不是直接修改
>>* 习惯使用 if_sid, if_group 等选项
>>* ID 使用范围 在 100000-109999 (这段是用户自定义的)

>* 如果新增加了日志格式或者日志规则
>>* 将它们发送给我们以便我们在后期版本中更新ossec
>>* 我们可以给你的日志格式分配一段 ID 范围

>* 对于非工作时间的登录认证成功发送警告

    <rule id="100005" level="10">
    <if_group>authentication_success</if_group>
    <time>6 pm - 7:30 am</time>
    <description>Login during non-business hours.</description>
    </rule>

>>* 每一个认证正常的信息分为 “authentication_success” 组 这也是这里为什么使用 <if_group> 的原因

>>* 增加到 local_rules.xml

>* 对于一个特殊的规则需要更改 frequency 或者 severity 的选项

>>* id 为 5712 的规则是 在 尝试多于6次 的 sshd 暴力登录
>>* 想增加 frequency, 仅仅需要在 rule的 属性里面增加 overwrite 属性(yes) 就会覆盖这条 规则.当然同理使用于 frequency

    <rule id="5712" level="10" frequency="20" overwrite=”yes”>
    <if_matched_sid>5710</if_matched_sid>
    <description>SSHD brute force trying to get access to </description>
    <description>the system.</description>
    <group>authentication_failures,</group>
    </rule>


**LID 实例 Squid 日志**

>* 检测内部主机被外面扫描的日志规则

>>* 扫描到恶意的用户或者蠕虫等行为

>>* 如果内部系统生成了多大500/600的 在不同的URL的错误代码 这也是很危险的

    <rule id="35009" level="5">
    <id>^5|^6</id>
    <description>Squid 500/600 error code (server error).</description>
    </rule>
    <rule id="35058" level="10" frequency="6" timeframe="240">
    <if_matched_sid>35009</if_matched_sid>
    <same_source_ip />
    <different_url />
    <description>Multiple 500/600 error codes (server error).</description>
    </rule>


**LID 实例 Web 日志**

>* 检测 过长的 URL 连接的 规则

>>* URL 多于 2900 个字符都是很值得怀疑的 

    <rule id="31115" level="13" maxsize="2900">
    <if_sid>31100</if_sid>
    <description>URL too long. Higher than allowed on most </description>
    <description>browsers. Possible attack.</description>
    <group>invalid_access,</group>
    </rule>


>* 检测到攻击

    OSSEC HIDS Notification.
    2007 Feb 18 20:52:27
    Received From: (jul) 192.168.2.0->/var/log/apache/access_log
    Rule: 31115 fired (level 13) -> "URL too long. Higher than allowed on most
    browsers."
    Portion of the log(s):
    142.167.9.242
    -
    -
    [18/Feb/2007:21:43:49
    -0400]
    "SEARCH
    /\x90\xc9\xc9\xc9\xc9\xc9
    \xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\
    9\xc99\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\x9
    \xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9
    \xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9\xc9...


**LID 实例 Snort 日志**

>* 来自相同IP的 IDS 事件

    2007 May 08 14:10:58 (jul) 192.168.2.0->/var/log/snort/alert
    Rule: 20152 (level 10) -> 'Multiple IDS alerts from same IP Address.'
    [**] [1:648:7] SHELLCODE x86 NOOP [**][Classification: Executable code was
    detected] [Priority: 1] 142.167.24.154:1238 -> 192.168.2.32:80
    [**] [1:648:7] SHELLCODE x86 NOOP [**][Classification: Executable code was
    detected] [Priority: 1] 142.167.24.154:1238 -> 192.168.2.32:80
    [**] [1:648:7] SHELLCODE x86 NOOP [**][Classification: Executable code was
    detected] [Priority: 1] 142.167.24.154:1238 -> 192.168.2.32:80
    [**] [119:4:1] (http_inspect) BARE BYTE UNICODE ENCODING
    [Classification: Preprocessor] 142.167.24.154:1238 -> 192.168.2.32:80
    [**] [119:15:1] (http_inspect) OVERSIZE REQUEST-URI DIRECTORY
    [**][Classification: access to a potentially vulnerable web application]
    [Priority: 2] 142.167.24.154:1238 -> 192.168.2.32:80
    [**] [1:1070:9] WEB-MISC WebDAV search access Classification: access to a
    potentially vulnerable application] 142.167.24.154:1238 -> 192.168.2.32:80


**LID 实例 Auth 日志**
>* 暴力尝试
>* 不仅仅是 ssh 还有 ftpd, imapd, webmails 等等


    OSSEC HIDS Notification.
    2007 Feb 21 05:37:59
    Received From: enigma->/var/log/authlog
    Rule: 5712 fired (level 10) -> "SSHD brute force trying to get access to the sys
    tem."
    Feb 21 05:37:58 enigma sshd[7235]: Failed password for invalid user admin
    from 125.152.17.236 port 42198 ssh2
    Feb 21 05:37:58 enigma sshd[14507]: Invalid user admin from 125.152.17.236
    Feb 21 05:37:56 enigma sshd[10566]: Failed password for invalid user admin
    from 125.152.17.236 port 42132 ssh2
    Feb 21 05:37:56 enigma sshd[11502]: Invalid user admin from 125.152.17.236

>* 经过暴力尝试正确进入

    Rule: 5720 (level 10) -> 'Multiple SSHD authentication failures.'
    Src IP: 125.192.xx.xx
    Feb 11 09:31:58 wpor sshd[4565]: Failed password for root from
    125.192.xx.xx port 42976 ssh2
    Feb 11 09:31:58 wpor sshd[4565]: Failed password for admin from
    125.192.xx.xx port 42976 ssh2
    Feb 11 09:31:58 wpor sshd[4565]: Failed password for admin from
    125.192.xx.xx port 42976 ssh2
    Rule: 40112 (level 12) -> 'Multiple authentication failures followed by
    a success.'
    Src IP: 125.192.xx.xx
    User: admin
    Feb 11 09:31:58 wpor sshd[7235]: Accepted password for admin
    from 125.192.xx.xx port 42198 ssh2



**总结**






























































































































    
    















































































































































    



























