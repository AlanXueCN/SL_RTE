/*
 * ___Dlove_String.h
 *
 *  Created on: 2013��12��24��
 *      Author: root
 */
 
#ifndef DLOVE_STRING_H_
#define DLOVE_STRING_H_
 
/* C�ַ������� */
struct ___Dlove_String{
    char    *buf;
    int     capacity;       /* �ַ���������,���ַ������������ɵ��ַ����� */
    int     size;           /* �ַ������ַ��� */
    int     increment;      /* ���ַ�����С����ʱ,��Ҫ���·���ռ�,�·��� increment �ֽڵĿռ�  */
};
 
/* ���ܣ���ʼ��һ�� str ����,�����ʼ�ڴ�ķ���
 * ������
 *  _capacity: ָ�����ַ����ĳ�ʼ����,���� 0
 *  _increment: ָ�����ַ�����ÿ�����·���ռ�ʱ������ */
void ___Dlove_String_init(struct ___Dlove_String *_str,int _capacity,int _increment);
 
/* ���ܣ��� str ��ӵ� string ��δβ
 * ע�⣺
 *  ��� string ����������,����Զ����� */
void ___Dlove_String_strcatch(struct ___Dlove_String *string,const char *str);
void ___Dlove_String_strcatstr(struct ___Dlove_String *string,const struct ___Dlove_String *str);
 
/* ���ܣ��ͷ��ַ��� _str ��ռ�õĿռ�
 * ע�⣺�˺� _str->buf==NULL,_str->size��_str->capacity��ʼ��Ϊ0,_str->increment����
 *  ������Ȼ��������ʹ��������� */
void ___Dlove_String_release(struct ___Dlove_String *_str);
 
/* �ӿ� */
#define String          struct ___Dlove_String
#define strCatch        ___Dlove_String_strcatch
#define strCatstr       ___Dlove_String_strcatstr
#define strInit         ___Dlove_String_init
#define strClr          ___Dlove_String_release
 
#endif /* DLOVE_STRING_H_ */
