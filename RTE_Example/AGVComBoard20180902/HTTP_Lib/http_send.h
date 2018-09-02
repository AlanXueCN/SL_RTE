
/*
 * dlove_HttpSend.h
 *
 *  Created on: 2013��12��25��
 *      Author: root
 */
 
#ifndef DLOVE_HTTPSEND_H_
#define DLOVE_HTTPSEND_H_
 
#include "http_string.h"
 
struct ___Dlove_HTTPSend{
    struct ___Dlove_String httpText;
    struct ___Dlove_String postArgbuf;      /* post ���������ȷ�������ط� */
};
 
/* ���ܣ���ʼ��һ��httpSend���� */
void ___Dlove_httpSendInit(struct ___Dlove_HTTPSend *_httpSend);
 
/* ���ܣ�����http�����ĵ�������
 * ������
 *  ver,http�汾��,"1.0"/"1.1"
 * ע�⣺url ��ֱ��׷�ӵ� http ���ĺ����,���Ը�ʽһ��Ҫ��ȷ */
void ___Dlove_httpSendSetReqLine(struct ___Dlove_HTTPSend *_httpSend,
        const char *_method,
        const char *_url,
        const char *_ver);
 /* ���ܣ�����http��Ӧ���ĵ���Ӧ��
 * ������
 *  ver,�汾��,"1.0"/"1.1"  num�����
 * ע�⣺url ��ֱ��׷�ӵ� http ���ĺ����,���Ը�ʽһ��Ҫ��ȷ */
void ___Dlove_httpSendSetHTTPLine(struct ___Dlove_HTTPSend *_httpSend,
        const char *_ver,
        const char *_num);
/* ���ܣ���� http ���ײ��ֶ� */
void ___Dlove_httpSendAddHead(struct ___Dlove_HTTPSend *_httpSend,
        const char *_name,
        const char *_value);
 
/* ���ܣ����post���� */
void ___Dlove_httpSendAddPostArg(struct ___Dlove_HTTPSend *_httpSend,
        const char *_name,
        const char *_value);
 
/* ���ܣ��������������־��http���Ĺ�������
 *  ����û������post�����������ģ�����ֻ�Ǽ򵥵�׷��"\r\n"
 *  ����������post�����������ģ��������Զ����� content-type,content-length��Ȼ��post����׷�ӵ�����֮�� */
void ___Dlove_httpSendSetOver(struct ___Dlove_HTTPSend *_httpSend);
 
///* ���ܣ���������ɵ�http������ͨ���׽��ַ��ͳ�ȥ
// * ���أ����ͳɹ�����0�����򷵻�1������errno*/
//int ___Dlove_httpSendTo(struct ___Dlove_HTTPSend *_httpSend,int sock);
// 
/* ���ܣ��ͷ�httpSend��ռ�õ���Դ */
void ___Dlove_httpSendClr(struct ___Dlove_HTTPSend *_httpSend);
 
#endif /* DLOVE_HTTPSEND_H_ */
