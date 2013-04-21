关于 rules 中的各种xml 
==

**一个小的实例**

    <group name="pam,syslog,">
      <rule id="5500" level="0" noalert="1">
        <decoded_as>pam</decoded_as>
        <description>Grouping of the pam_unix rules.</description>
      </rule>      
    
      <rule id="5501" level="3">
        <if_sid>5500</if_sid>
        <match>session opened for user </match>
        <description>Login session opened.</description>
        <group>authentication_success,</group>
      </rule>
    
      <rule id="5502" level="3">
        <if_sid>5500</if_sid>
        <match>session closed for user </match>
        <description>Login session closed.</description>
      </rule>      
      
      <rule id="5503" level="5">
        <if_sid>5500</if_sid>
        <match>authentication failure; logname=</match>
        <description>User login failed.</description>
        <group>authentication_failed,</group>
      </rule>
    
    </group>


    一个 <new></new> 为一个实例对象 所以上面为4个 RuleInfo 对象

![](http://vdisk-thumb-2.wcdn.cn/frame.1024x768/data.vdisk.me/55890007/dfd18413f148605a006461d0ccd6b857b495af66?ip=1363278000,10.73.26.25&ssig=yDIzY1gHK6&Expires=1363276800&KID=sae,l30zoo1wmz)


以下包含 所有的rules的xml可以写的元素或者属性 后面包含其与一个 RuleInfo 对象 的属性的对应关系

>* group    
>>*  直接对应====> RuleInfo.gruop   也就是有很多rules的group都是相同的

注意底下还可以有group 选项 如果含有group选择则会覆盖开始的group选项？

**********
>* rule 以下列出的是 rule 可能的属性取值 当然后面包含了与其对应的RuleInfo 对象的属性
>>* id                ===> RuleInfo.sigid
>>* level             ===> RuleInfo.level
>>* maxsize           ===> RuleInfo.maxsize
>>* timeframe         ====> RuleInfo.timeframe
>>* frequency         ====> RuleInfo.frequency
>>* accuracy         =====>  这个没有直接对应的 如果 accurary 为 非0 则 RuleInfo.level 会乘以100 当然在list的最后还是会变成原来的  __这样子做的用意还没有弄懂?__

>>* noalert          =====> RuleInfo.alert_opts (包含 noalert ,overwrite)
>>* ignore           =====> RuleInfo.ignore
>>* overwrite        =====> RuleInfo.alert_opts (包含 noalert ,overwrite)

******
**以下都是在 <rule></rule>里面的元素与属性**
>* regex

这个对应 RuleInfo里的 regex 它的类型是 OSRegex   
    
    OSRegex_Compile(regex, config_ruleinfo->regex, 0)

>* match

这个意思与上面的大体相同 不同之处在与最后的编译调用的是不同的函数
    
    OSMatch_Compile(match, config_ruleinfo->match, 0)

>* decoded_as  ==>  对应 RuleInfo.decoded_as

    <decoded_as>pam</decoded_as>
    config_ruleinfo->decoded_as =getDecoderfromlist(rule_opt[k]->content);

从 decoderlist 查找 decoded_as的值 对应的id
>* category

_category 包含四种 如下几种_
>>* firewall
    
        config_ruleinfo->category = FIREWALL;
        
>>* ids

    config_ruleinfo->category = IDS;
    
>>* syslog

    config_ruleinfo->category = SYSLOG;
    
>>* web-log

    config_ruleinfo->category = WEBLOG;
    
>>* squid

    config_ruleinfo->category = SQUID;

>>* windows

    config_ruleinfo->category = WINDOWS;

>>* ossec
    
    config_ruleinfo->category = OSSEC_RL;
    
>* cve

这里看了下 这个与 __config_ruleinfo->info_details__  有关心 但是查看 rules 中没有看到
cve 元素 唯一含有的就是下面的这个 先放这 以后有时间再说

    attack_rules.xml:    <info type='cve'>2002-0033</info>
    sshd_rules.xml:    <info type="cve">2001-0144</info>



>* info

从上面可以看出 info 的一个例子  上面 包含了一个 info的属性 以下列出 info所有可能的属性

>>* cve
>>* text
>>* link
>>* osvdb

在这里与  __config_ruleinfo->info_details__ 有关系 先放这以后在说

>* time

例如 可以接受的格式为如下

     * hh:mm - hh:mm (24 hour format)
     * !hh:mm -hh:mm (24 hour format)
     * hh - hh (24 hour format)
     * hh:mm am - hh:mm pm (12 hour format)
     * hh am - hh pm (12 hour format)

    <time>6 pm - 8:30 am</time>

从上面可以看出来 时间段分为两个时间   __first_hour__ 与 __second_hour__

    RuleInfo.day_time=first_hour.second_hour
    RuleInfo.day_time=first_hour!second_hour

>* weekday

底下是一些常见的可以接受的格式

     * weekdays, weekends, monday, tuesday, thursday,..
     * monday,tuesday
     * mon,tue wed

>* description

    config_ruleinfo->comment  貌似是将 上面含有的"\n" 替换掉 ""

>* srcip

    config_ruleinfo->srcip[]  当然肯能有多个 每一个的类型都是  os_ip 的类型

>* dstip

    config_ruleinfo->dstip[]  当然肯能有多个 每一个的类型都是  os_ip 的类型


>* user 


>* id 

注意上面也有一个id 注意两个的区别

    OSMatch_Compile(id, config_ruleinfo->id, 0)

难道要匹配  id?

>* srcport

    OSMatch_Compile(srcport, config_ruleinfo->srcport, 0)

难道要匹配  srcport?

>* dstport

    OSMatch_Compile(dstport, config_ruleinfo->dstport, 0)

难道要匹配  dstport?

>* status

    OSMatch_Compile(status, config_ruleinfo->status, 0)
    
 难道要匹配  status?
    
>* hostname

    OSMatch_Compile(hostname, config_ruleinfo->hostname,0)

>* extra_data

    OSMatch_Compile(extra_data,config_ruleinfo->extra_data, 0)

>* program_name

    OSMatch_Compile(program_name,config_ruleinfo->program_name,0)
>* action

    config_ruleinfo->action 直接写入

>* list

这项看着还较复杂。先放着

>* url

>* compiled_rule

先放这

>* if_sid

直接对应着 __config_ruleinfo->if_sid__

>* if_level

直接对应着 __config_ruleinfo->if_level__

>* if_group

直接对应着 __config_ruleinfo->if_group__

>* __if_matched_group__

如果有 __if_matched_group__ 则必须要有 __if_sid__ 或者 __if_group__


                    if(!config_ruleinfo->if_sid && !config_ruleinfo->if_group)
                    {
                        os_strdup(if_matched_group,
                                  config_ruleinfo->if_group);
                    }


>* __if_matched_sid__

                /* If_matched_sid, we need to get the if_sid */
                if(config_ruleinfo->if_matched_sid &&
                   !config_ruleinfo->if_sid &&
                   !config_ruleinfo->if_group)
                {
                    os_calloc(16, sizeof(char), config_ruleinfo->if_sid);
                    snprintf(config_ruleinfo->if_sid, 15, "%d",
                             config_ruleinfo->if_matched_sid);
                }


>* __same_source_ip__

    config_ruleinfo->context_opts|= SAME_SRCIP   config_ruleinfo->context_opts 包括(__same_source_ip__ __same_src_port__ __check_diff__)

>* __same_src_port__

    config_ruleinfo->context_opts|= SAME_SRCPORT;  包括范围同上所示

>* __check_diff__
    
    config_ruleinfo->context_opts|= SAME_DODIFF; 

>* __not_same_source_ip__

    config_ruleinfo->context_opts&= NOT_SAME_SRCIP;

>* same_dst_port

    config_ruleinfo->context_opts|= SAME_DSTPORT;

>* same_id

    config_ruleinfo->context_opts|= SAME_ID;

>* different_url

    config_ruleinfo->context_opts|= DIFFERENT_URL;

>* not_same_id

    config_ruleinfo->context_opts&= NOT_SAME_ID;
    
>* if_fts

    config_ruleinfo->alert_opts |= DO_FTS;

>* same_user

    config_ruleinfo->context_opts|= SAME_USER;

>* not_same_user

    config_ruleinfo->context_opts&= NOT_SAME_USER;

>* same_location

    config_ruleinfo->context_opts|= SAME_LOCATION;

>* not_same_agent

    config_ruleinfo->context_opts&= NOT_SAME_AGENT;

>* options

这个留后再说

>* ignore

这个留后再说

>* check_if_ignored

留后再说

********************************

__后面这些暂时没有发现有用___

>* lookup
>* field
>* check_value
>* match_key
>* not_match_key
>* match_key_value
>* address_match_key
>* not_address_match_key
>* address_match_key_value
>* if_matched_regex


























