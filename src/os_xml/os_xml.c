/*      $OSSEC, os_xml.c, v0.3, 2005/02/11, Daniel B. Cid$      */

/* Copyright (C) 2009 Trend Micro Inc.
 * All rights reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

/* os_xml Library.
 */



#include "shared.h"

#include "os_xml.h"

#define _R_CONFS 	'<'
#define _R_CONFE 	'>'
#define _R_EQUAL 	'='
#define _R_COM   	'!'
#define _R_VAR      '$'

#define OPEN            51
#define CLOSE           52

#define LEOF		-2

/* Internal functions */
int _oscomment(FILE *fp);
int _writecontent(char *str, unsigned int size, int parent, OS_XML *_lxml);
int _writememory(char *str, short int type, unsigned int size,
                                        int parent, OS_XML *_lxml);
int _checkmemory(char *str,OS_XML *_lxml);
int _ReadElem(FILE *fp, int position, int parent, OS_XML *_lxml);
int _getattributes(FILE *fp,int parent,OS_XML *_lxml);

void xml_error(OS_XML *_lxml, const char *msg,...) __attribute__((format(printf, 2, 3)));

/* Currently line */
int _line;

/****
 *  
 *   这里对读入的每一个字符需要判断是否时换行符号。。
 *   类似一个过滤器
 */
/* Local fgetc */
int _xml_fgetc(FILE *fp)
{
    int c;
    c = fgetc(fp);

    if(c == '\n') /* add new line */
        _line++;

    return(c);
}

#define FGETC(fp) _xml_fgetc(fp)

void xml_error(OS_XML *_lxml, const char *msg,...)
{
#ifdef DEBUG
    time_t tm;
    struct tm *p;
#endif

    va_list args;
    va_start(args,msg);

#ifdef DEBUG	
    tm = time(NULL);
    p = localtime(&tm);
    fprintf(stderr,"%d/%d/%d %d:%d:%d (LINE: %d)",p->tm_year+1900,p->tm_mon,
            p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec,_line);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "\n\n");
#endif

    memset(_lxml->err,'\0', 128);
    vsnprintf(_lxml->err,127,msg,args);
    va_end(args);
    _lxml->err_line = _line;
}



/* OS_ClearXML v0.1
 * Clear the memory used by the XML
 */
void OS_ClearXML(OS_XML *_lxml)
{
    int i;
    for(i=0;i<_lxml->cur;i++)
    {
        if(_lxml->el[i])
            free(_lxml->el[i]);
        if(_lxml->ct[i])
            free(_lxml->ct[i]);
    }
    _lxml->cur = 0;
    _lxml->err_line = 0;
    free(_lxml->el);
    free(_lxml->ct);
    free(_lxml->rl);
    free(_lxml->tp);
    free(_lxml->ck);
    free(_lxml->ln);
    memset(_lxml->err,'\0', 128);

    return;	

}


/* OS_ReadXML v0.1
 * Read a XML file and generate the necessary structs.
 */
int OS_ReadXML(char *file, OS_XML *_lxml)
{
    int r,i;
    FILE *fp;

    fp = fopen(file,"r");
    if(!fp)
    {
        xml_error(_lxml, "XMLERR: File '%s' not found.",file);
        return(-2);
    }
/**
 *  初始化 _lxml
 */
    _lxml->cur = 0;
    _lxml->fol = 0;
    _lxml->el = NULL;
    _lxml->ct = NULL;
    _lxml->tp = NULL;
    _lxml->rl = NULL;
    _lxml->ck = NULL;
    _lxml->ln = NULL;

    _lxml->err_line = 0;
    memset(_lxml->err,'\0',128);

/**
 *  初始化当前行数
 */
    /* Zeroing the line */
    _line = 1;

/**
 *   int _ReadElem(FILE *fp, int position, int parent, OS_XML *_lxml)
 */
    if((r = _ReadElem(fp,0,0,_lxml)) < 0) /* First position */
    {
        if(r != LEOF)
        {
            fclose(fp);
            return(-1);
        }
    }
/**
 * 判断每一个<aatr2 name="gggg">lele<aatr2/> 
 * 完成。。当然 只有读完<attr2/> 才知道完整 
 */
    for(i=0;i<_lxml->cur;i++)
    {
        if(_lxml->ck[i] == 0)
        {
            xml_error(_lxml,"XMLERR: Element '%s' not closed\n", _lxml->el[i]);
            fclose(fp);
            return(-1);
        }
    }

    fclose(fp);
    return(0);
}

/***
 *  
 *  判断以"<" 开头之后是不是 注释的函数  注释格式 <!--   -->
 *  
 *  返回 0 --不是注释   返回 1 -- 是注释  返回  -1 错误
 */
int _oscomment(FILE *fp)
{
    int c;
/***
 *  
 *   注释"<" 之后必须是 "!" 字符串否则 直接ungetc 
 */
    if((c = fgetc(fp)) == _R_COM)
    {
/**
 *  当然 注释可以包含 "\n" 这里需使用FGETC 函数
 */ 
       while((c=FGETC(fp)) != EOF)
        {
            if(c == _R_COM)
            {
                if((c=fgetc(fp)) == _R_CONFE)
                    return(1);
                ungetc(c,fp);
            }
            else if(c == '-')       /* W3C way of finish comments */
            {
                if((c = FGETC(fp)) == '-')
                {
                    if((c = fgetc(fp)) == _R_CONFE) /*判断连续的两个后面是否跟着一个">"*/
                        return(1);
                    ungetc(c,fp);
                }
                ungetc(c,fp);
            }
            else
                continue;
        }
        return(-1);
    }
    else
        ungetc(c,fp);
    return(0);
}


int _ReadElem(FILE *fp, int position, int parent, OS_XML *_lxml)
{

/**
 *  读每一个字符的变量
 */
    int c;
/**
 *
 */
    unsigned int count = 0;
/**
 */ 
   unsigned int _currentlycont = 0;
/*
 * 依次循环每一个元素与属性 也就是每一个元素与属性完时需要初始化。设置为-1???
 * <aatr2 name=999>lele<aatr2/>
 * <aatr2>  location=0
 * lele     location 1
 * <attr2/> location 2
 *
 * ????  
 */
    short int location = -1;
/**
 *
 */
    char prevv = 0;
/**
 * <aatr2 name=999>lele<aatr2/>
 *
 * elem 存贮为aatr2
 * cont 存贮为lele
 * closedelem attr2
 */
    char elem[XML_MAXSIZE +1];
    char cont[XML_MAXSIZE +1];
    char closedelem[XML_MAXSIZE +1];



    memset(elem,'\0',XML_MAXSIZE +1);
    memset(cont,'\0',XML_MAXSIZE +1);
    memset(closedelem,'\0',XML_MAXSIZE +1);

/**
 *  这里采用一个一个字符读入 对每一个字符判断是不是换行符，是 行号加一
 *  
 */
    while((c=FGETC(fp)) != EOF)
    {
        if(c == '\\')
            prevv = c;
        else if(prevv == '\\')
        {
            if(c != _R_CONFS)
                prevv = 0;
        }


        /* Max size */
        if(count >= XML_MAXSIZE)
        {
            xml_error(_lxml,"XML ERR: String overflow. Exiting.");
            return(-1);
        }

/**
 *  定义 "<!---  --> "  格式注释
 */
        /* Checking for comments */
/**
 *  c == "<"
 */
        if(c == _R_CONFS)
        {
            int r = 0;
            if((r = _oscomment(fp)) < 0)  /*判断是不是注释 返回值有3中情况。注意ungetc的使用**/
            {
                xml_error(_lxml,"XML ERR: Comment not closed. Bad XML.");
                return(-1);
            }
            else if(r == 1)  /*是注释 可以读入下一个字符了**/
                continue;
        }

        /* real checking */
        if((location == -1) && (prevv == 0))
        {
            if(c == _R_CONFS)
            {
                if((c=fgetc(fp)) == '/')
                {
                    xml_error(_lxml,"XML ERR: Bad formed XML. Element "
                                    "not opened");
                    return(-1);
                }
                else
                    ungetc(c,fp);
                location = 0;
            }
            else
                continue;
        }

/**
 *    <var name="lala">xx</var>
 *    <a></a>
 *   当读到" "空格与 ">" 都会进入
 *   此处空格的多少有影响吗？
 */
        else if((location == 0) && ((c == _R_CONFE) || (c == ' ')))
        {
            int _ge = 0;
            int _ga = 0;
            elem[count]='\0';

/**
 *  <aatr2/>lele<aatr2/>
 *  此种情况下需要进入。。。
 */
            /* Removing the / at the end of the element name */
            if(elem[count -1] == '/')
            {
                _ge = '/';
                elem[count -1] = '\0';
            }

/**
 *   把(元素/属性)写入 _lxml 申请的内存中
 */
            _writememory(elem, XML_ELEM, count+1, parent, _lxml);
/**
<test2 name="$lala">
  <aatr gaga="$lala">$lala</aatr>
  <aatr1>$lala2</aatr1>
  <aatr2>$lala</aatr2>
  <aatr3>PARA $lala2,$lala ola $davi|$lili$lala.$lala OIIIIII</aatr3>
  <aatr4>lala-abc</aatr4>
</test2>

记录 当前元素的位置 便于 记录 attr 与 attr1 。。 的parent
 */
            _currentlycont=_lxml->cur-1;
/**
 *  可以看成元素读完了。。后面可以读属性了
 */
            if(c == ' ')
            {
                if((_ga = _getattributes(fp,parent,_lxml)) < 0)  // 获得所有(使用递归)的元素后面的属性
                    return(-1);
            }
/**
 *  <aatr2/>lele<aatr2/> 情况卡。。
 */

            /* If the element is closed already (finished in />) */
            if((_ge == '/') || (_ga == '/'))
            {
                _writecontent("\0", 2, _currentlycont,_lxml);
                _lxml->ck[_currentlycont] = 1;
                _currentlycont = 0;
                count = 0;
                location = -1;

                memset(elem,'\0',XML_MAXSIZE);
                memset(closedelem,'\0',XML_MAXSIZE);
                memset(cont,'\0',XML_MAXSIZE);

                if(parent > 0)
                    return(0);
            }
            else  /*一般应该进这。。**/
            {
                count = 0;
                location = 1;
            }
        }
/**
 * <var name="lala">xx</var>
 * <var name="lala2">my name is daniel</var>
 *
 * 第一行读完了 。。递归读第二行
 *
 */
        else if((location == 2) &&(c == _R_CONFE))
        {
            closedelem[count]='\0';
/**
 * 当然需要判断前与后相同不了。。
 */
            if(strcmp(closedelem,elem) != 0)
            {
                xml_error(_lxml,"XML ERR: Element not closed: %s",elem);
                return(-1);
            }
/**
 * 开始递归第二个。
 */  
            _writecontent(cont,strlen(cont)+1,_currentlycont,_lxml);
            _lxml->ck[_currentlycont]=1;	
            memset(elem,'\0',XML_MAXSIZE);
            memset(closedelem,'\0',XML_MAXSIZE);
            memset(cont,'\0',XML_MAXSIZE);
            _currentlycont = 0;
            count = 0;	
            location = -1;
            if(parent > 0)
                return(0);
        }
/**
 *  表示 <aatr2>lele</aatr2> 中  lele读完了
 *
 */
        else if((location == 1) && (c == _R_CONFS) && (prevv == 0))
        {
/**
 *读最后一部分了。。
 */
            if((c=fgetc(fp)) == '/')
            {
/**
 * 读完了需要处理尾部工作与初始化工作
 *
 */
                cont[count] = '\0';
                count = 0;
                location = 2;
            }	
            else
            {
                ungetc(c,fp);
                ungetc(_R_CONFS,fp);

                if(_ReadElem(fp,position+1,parent+1,_lxml)< 0)
                {
                    return(-1);
                }
                count=0;
            }
        }
        else
        {
            if(location == 0)
                elem[count++] = c;
            else if(location == 1)
                cont[count++] = c;
            else if(location == 2)
                closedelem[count++] = c;

            if((_R_CONFS == c) && (prevv != 0))
            {
                prevv = 0;
            }
        }
    }
    if(location == -1)
        return(LEOF);

    xml_error(_lxml,"XML ERR: End of file and some elements were not closed");
    return(-1);
}				

int _writememory(char *str, short int type, unsigned int size,
                                        int parent, OS_XML *_lxml)
{
    /* Allocating for the element */
    _lxml->el = (char **)realloc(_lxml->el,(_lxml->cur+1)*sizeof(char *));
    _lxml->el[_lxml->cur]=(char *)calloc(size,sizeof(char));
    strncpy(_lxml->el[_lxml->cur],str,size-1);

    /* Allocating for the content */	
    _lxml->ct = (char **)realloc(_lxml->ct,(_lxml->cur+1)*sizeof(char *));

    /* Allocating for the type */
    _lxml->tp = realloc(_lxml->tp,(_lxml->cur+1)*sizeof(int));
    _lxml->tp[_lxml->cur] = type;	

    /* Allocating for the relation */
    _lxml->rl = realloc(_lxml->rl,(_lxml->cur+1)*sizeof(int));
    _lxml->rl[_lxml->cur] = parent;

    /* Allocating for the "check" */
    _lxml->ck = realloc(_lxml->ck,(_lxml->cur+1)*sizeof(int));
    _lxml->ck[_lxml->cur] = 0;

    /* Allocating for the line */
    _lxml->ln = realloc(_lxml->ln,(_lxml->cur+1)*sizeof(int));
    _lxml->ln[_lxml->cur] = _line;
/**
 *  注意对于每一个元素 都是需要判断是否关闭的。。  后面需要判断xml的正确性??
 */
    /* Attributes does not need to be closed */
    if(type == XML_ATTR)
        _lxml->ck[_lxml->cur] = 1;
/**
 *  ?????
 */
    /* Checking if it is a variable */
    if(strcasecmp(XML_VAR,str) == 0)
    {
        _lxml->tp[_lxml->cur] = XML_VARIABLE_BEGIN;
    }

    _lxml->cur++;
    return(0);
}

int _writecontent(char *str, unsigned int size, int parent, OS_XML *_lxml)
{
    _lxml->ct[parent]=(char *)calloc(size,sizeof(char));
    strncpy(_lxml->ct[parent],str,size-1);

    return(0);
}


int _checkmemory(char *str,OS_XML *_lxml)
{
    int i;
    for(i=0;i<_lxml->cur;i++)
    {
        if(_lxml->ck[i] == 0)
        {
            if(strcmp(str,_lxml->el[i]) == 0)
            {
                _lxml->ck[i] = 1;
                return(0);
            }
            else
                continue;
        }
    }
    return(-1);
}

/* getattributes (Internal function): v0.1: 2005/03/03
 * Read the attributes of an element
 */

/**
 *   读入属性的函数
 */
int _getattributes(FILE *fp,int parent,OS_XML *_lxml)
{

/**
 *  location 为0 表示读 属性名称
 *           为1 表示读 属性的值
 */
    int location = 0;
    int count = 0;
    int c;
/**
 * 对于每一个属性的值都已"""或者"'" 开始。。 
 * 而这里就是记录这个属性的值的开始处。便于后面判断结束
 */
    int c_to_match = 0;

    char attr[XML_MAXSIZE+1];
    char value[XML_MAXSIZE+1];

    memset(attr,'\0',XML_MAXSIZE+1);
    memset(value,'\0',XML_MAXSIZE+1);

    while((c=FGETC(fp)) != EOF)
    {
        if(count >= XML_MAXSIZE)
        {
            attr[count-1] = '\0';
            xml_error(_lxml,
                    "XMLERR: Overflow attempt at attribute '%s'.",attr);
            return(-1);
        }

        else if((c == _R_CONFE) || (c == '/'))
        {
            if((location == 1)||((location == 0)&&(count > 0)))
            {
                xml_error(_lxml, "XMLERR: Attribute '%s' not closed.",
                                 attr);
                return(-1);
            }
            else if(c == '/')
                return(c);
            else
                return(0);
        }	
        else if((location == 0)&&(c == '='))
        {
            attr[count]='\0';
            c = FGETC(fp);
/**
 *  "=" 后面 不是 " 或者 ' 的情况 是什么?
 */ 
           if((c != '"')&&(c != '\''))
            {
                unsigned short int _err=1;
                if(c == ' ')
                {
                    while((c=FGETC(fp))!= EOF)
                    {
                        if(c == ' ')
                            continue;
                        else if((c == '"')||(c == '\''))
                        {
                            _err = 0;
                            break;
                        }
                        else
                            break;
                    }
                }
                if(_err != 0){
                    xml_error(_lxml,
                            "XMLERR: Attribute '%s' not followed by a \" or \'."
                            ,attr);
                    return(-1); }
            }

            c_to_match = c;
            location = 1;
            count = 0;
        }
        else if((location == 0)&&(c == ' '))
            continue;

/**
 * 
 * 属性值已经读完
 */
        else if((location == 1)&&(c == c_to_match))
        {
            value[count]='\0';
/*
 * 读完当然需要初始化了。。
 */
            location = 0;
            c_to_match = 0;
/**
 * 写入全局变量中
 * 分别写入属性名称与属性值
 */
            _writememory(attr, XML_ATTR, strlen(attr)+1,
                    parent, _lxml);	
            _writecontent(value,count+1,_lxml->cur-1,_lxml);
            c = FGETC(fp);
/**
 *  后面空格表示不是一个属性 之后递归 做其它的属性。。
 */ 
           if(c == ' ')
                return(_getattributes(fp,parent,_lxml));
            else if(c == _R_CONFE) // 表示正常结束  返回
                return(0);
            else // 不是空格就错误了。。那多个空格呢？？？
            {
                xml_error(_lxml,
                        "XMLERR: Bad attribute closing for '%s'='%s'.",
                        attr,value);
                return(-1);
            }
            count = 0;
        }
/**
 *  分别用来设置属性的名字与值
 */
        else if(location == 0)
            attr[count++]=c;
        else if(location == 1)
            value[count++]=c;

    }

    xml_error(_lxml, "XMLERR: End of file while reading an attribute.");
    return(-1);
}

/* EOF */
