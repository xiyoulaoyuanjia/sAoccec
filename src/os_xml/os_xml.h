/* @(#) $Id: ./src/os_xml/os_xml.h, 2011/09/08 dcid Exp $
 */

/* Copyright (C) 2009 Trend Micro Inc.
 * All rights reserved.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation
 */

/* os_xml C Library.
 */


#ifndef __OS_XML_H
#define __OS_XML_H

/**
 *   定义xml最大行数 ??? 好像不是
 */
#ifndef XML_MAXSIZE
   #define XML_MAXSIZE          2048
#endif /* XML_MAXSIZE */

#ifndef XML_VAR
   #define XML_VAR              "xml_var"
#endif /* XML_VAR */

/**
 *  xml 的类型  包含有 elem 与 attr
 */
#define XML_ELEM                101
#define XML_ATTR                102
#define XML_VARIABLE_BEGIN      '$'

/* XML Node structure */
typedef struct _xml_node
{
    int key;
    int line;
    char *element;
    char *content;
    char **attributes;
    char **values;
}xml_node;

/* XML structure */
typedef struct _OS_XML
{
/***
 *  记录当前的元素或者属性的个数。。因为每一个属性或者元素都需要空间。。当新读入一个时都需要重新分派大小(cur+1)
 */
    int cur;		/* Currently position (and last after reading) */
/**
 *  干什么用??
 */
    int fol;		/* Currently position for the xml_access */

/**
 *  tp Item 类型 分别为 XML_ELEM XML_ATTR  XML_VARIABLE_BEGIN     
 *       
 *      
 *  ct 内容
 *  el element或者attribute的名称 
 */
    int *tp;		/* Item type	*/
 //  rl xml 关系 把 xml看成树结构 记录父节点的行号??? 
    int *rl;		/* Relation in the XML */
 //  ck 判断item 是 closed的吗
    int *ck;		/* If the item was closed or not */
 //  ln 当前的文件行数
    int *ln;        /* Currently xml file line */
/**
 *  存值
 *  属性的值 当然对于 元素没有值???
 */
    char **ct;		/* Content is stored */
/**
 *  <var name="lala">xx</var>
 *  var is element name
 *  name is attribute
 *  <a></a>
 *  
 *  var is element name
 */
    char **el;		/* The element/attribute name is stored */
/**
 *  错误信息
 *  1. 行号
 *  2. 记录错误信息
 *  
 */
    char err[128];	/* Error messages are stored in here */
    int err_line;   /* Line number of the possible error */


}OS_XML;

typedef xml_node ** XML_NODE;

/* Start the XML structure reading a file */
int OS_ReadXML(char *file, OS_XML *lxml);

/* Clear the XML strucute memory */
void OS_ClearXML(OS_XML *_lxml);

/* clear a node */
void OS_ClearNode(xml_node **node);


/* Functions to read the XML */

/* Return 1 if element_name is a root element */
int OS_RootElementExist(OS_XML *_lxml, char *element_name);

/* Return 1 if the element_name exists */
int OS_ElementExist(OS_XML *_lxml, char **element_name);

/* Return the elements "children" of the element_name */
char **OS_GetElements(OS_XML *_lxml, char **element_name);

/* Return the elements "children" of the element_name */
xml_node **OS_GetElementsbyNode(OS_XML *_lxml, xml_node *node);

/* Return the attributes of the element name */
char **OS_GetAttributes(OS_XML *_lxml, char **element_name);

/* Return one value from element_name */
char *OS_GetOneContentforElement(OS_XML *_lxml, char **element_name);

/* Return an array with the content of all entries of element_name */
char **OS_GetElementContent(OS_XML *_lxml, char **element_name);

/* Return an array with the contents of an element_nane */
char **OS_GetContents(OS_XML *_lxml, char **element_name);

/* Return the value of a specific attribute of the element_name */
char *OS_GetAttributeContent(OS_XML *_lxml, char **element_name,
        char *attribute_name);

/* Apply the variables to the xml */
int OS_ApplyVariables(OS_XML *_lxml);

#endif

/* EOF */
