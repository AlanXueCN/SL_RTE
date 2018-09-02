/*
 * ___Dlove_String.c
 *
 *  Created on: 2013��12��24��
 *      Author: root
 */
 
#include "http_string.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
 
/* ���ܣ����� string ������ */
void ___Dlove_String_remalloc(struct ___Dlove_String *string){
    string->buf=realloc(string->buf,string->capacity+=string->increment);
    return ;
}
 
void ___Dlove_String_strcatch(struct ___Dlove_String *string,const char *str){
    /* ����ռ䲻��,�����·��� */
    int tmp=string->size;
    char *newbuf=string->buf+tmp;
    string->size+=strlen(str);
    while(string->size > string->capacity )
        ___Dlove_String_remalloc(string);

 
    /* �� str ׷�ӵ� string ��ĩβ */
    for(;(*newbuf=*str) !='\0' ;++newbuf,++str)
        ;
 
    return ;
}
 
void ___Dlove_String_strcatstr(struct ___Dlove_String *string,const struct ___Dlove_String *str){
    /* ����ռ䲻��,�����·��� */
    int tmp=string->size;
    char *newbuf=string->buf+tmp;
    /* �� str ׷�ӵ� string ��ĩβ */
    char *strbuf=str->buf;
    string->size+=str->size;
    while(string->size > string->capacity )
        ___Dlove_String_remalloc(string);
    for(;(*newbuf=*strbuf) !='\0' ;++newbuf,++strbuf)
        ;
 
    return ;
 
}
 
void ___Dlove_String_init(struct ___Dlove_String *_str,int _capacity,int _increment){
    _str->buf=(char*)malloc( _str->capacity=_capacity);
    _str->increment=_increment;
    _str->size=0;
    *_str->buf='\0'; /* ��ʼ��Ϊ����Ϊ0��C�ַ��� */
    return ;
}
 
void ___Dlove_String_release(struct ___Dlove_String *_str){
    if(_str->buf != NULL ){
        free(_str->buf);
        _str->buf=NULL;
    }
 
    _str->size=0;
    _str->capacity=0;
 
    return ;
}
