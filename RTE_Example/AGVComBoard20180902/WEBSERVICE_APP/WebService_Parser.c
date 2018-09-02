#include "WebService_Parser.h"
#include "mxml.h"
uint8_t ParserName = 0;
ApplyTempInfor_t ApplyTempInfor = {0};
ExcuteTempInfor_t ExcuteTempInfor = {0};
ReleaseTempInfor_t ReleaseTempInfor = {0};
static void ApplyResParser(uint8_t *ApplyBuffer);
static void TaskExcuteParser(uint8_t *ExcuteBuffer);
static void ReleaseResParser(uint8_t *ReleaseBuffer);
void WebServiceDataParser(uint8_t *Data,uint16_t DataLenth)
{
	char *SoapDataAdd;
	SoapDataAdd = strstr((char *)Data,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	if(SoapDataAdd==NULL)
	{
		printf("[WebService]    һ֡��Ч���ݣ�\n");
	}
	else
	{
		mxml_node_t *tree,*nodeSOAP_ENV = NULL;
		//��soapbuf�е����ݼ��ص�mxl����ȥ
		tree = mxmlLoadString(NULL, (const char *)SoapDataAdd, MXML_TEXT_CALLBACK);
		//Ѱ��body����
		nodeSOAP_ENV = mxmlFindElement(tree, tree, "SOAP-ENV:Envelope", NULL, NULL, MXML_DESCEND);
		if(nodeSOAP_ENV==NULL)
		{
			printf("[WebService]    ��������SOAP-ENV:Envelopeû���ҵ���\n");
			mxmlRemove(tree);
			mxmlDelete(tree);
			return;
		}
		mxml_node_t *nodeSOAP_ENV_BODY = NULL;
		nodeSOAP_ENV_BODY = mxmlFindElement(nodeSOAP_ENV, tree, "SOAP-ENV:Body", NULL, NULL, MXML_DESCEND);
		if(nodeSOAP_ENV_BODY==NULL)
		{
			printf("[WebService]    ��������SOAP-ENV:Bodyû���ҵ�\n");
			mxmlRemove(nodeSOAP_ENV);
			mxmlDelete(nodeSOAP_ENV);
			mxmlRemove(tree);
			mxmlDelete(tree);
			return;
		}	
		mxml_node_t *nodeFuncName = NULL;
		//�Ӷ���ĺ����б���Ѱ�Һ���
		for(uint8_t i=0;i<SOAPFUCCOUNT;i++)
		{
			nodeFuncName = mxmlFindElement(nodeSOAP_ENV_BODY, tree, SoapRequestFucList[i], NULL, NULL, MXML_DESCEND);
			if(nodeFuncName!=NULL)
			{
				ParserName = i+1;
				printf("[WebService]    ����һ֡���ݣ�%s\n",SoapRequestFucList[i]);
				break;
			}
		}
		if(nodeFuncName==NULL)
		{
			ParserName = 0;
			printf("[WebService]    ���������û�з��������ĵ��ú���\n");
			mxmlRemove(nodeFuncName);
			mxmlDelete(nodeFuncName);
			mxmlRemove(nodeSOAP_ENV_BODY);
			mxmlDelete(nodeSOAP_ENV_BODY);
			mxmlRemove(nodeSOAP_ENV);
			mxmlDelete(nodeSOAP_ENV);
			mxmlRemove(tree);
			mxmlDelete(tree);
			return;
		}
		mxml_node_t *nodeStrInfo = mxmlFindElement(nodeFuncName, tree, "strReqInfo", NULL, NULL, MXML_DESCEND);
		switch(ParserName)
		{
			case ApplyResource:
			{
				ApplyResParser((uint8_t *)nodeStrInfo->child->value.text.string);
			}break;
			case ApplyForExecution:
			{
				TaskExcuteParser((uint8_t *)nodeStrInfo->child->value.text.string);
			}break;
			case ReleaseResource:
			{
				ReleaseResParser((uint8_t *)nodeStrInfo->child->value.text.string);
			}break;
		}
		mxmlRemove(nodeStrInfo);
		mxmlDelete(nodeStrInfo);
		mxmlRemove(nodeFuncName);
		mxmlDelete(nodeFuncName);
		mxmlRemove(nodeSOAP_ENV_BODY);
		mxmlDelete(nodeSOAP_ENV_BODY);
		mxmlRemove(nodeSOAP_ENV);
		mxmlDelete(nodeSOAP_ENV);
		mxmlRemove(tree);
		mxmlDelete(tree);
	}
}
static void ApplyResParser(uint8_t *ApplyBuffer)
{
	uint8_t i=0;
	char *strstart;
	char *strend;
	//��ȡ�����ֶεľ���ֵ
	for(i=0;i<ARFUCARGCOUNT;i++)
	{
		strstart = strstr((char *)ApplyBuffer,ApplyResourceArgList[i]);
		strstart = strstart+strlen(ApplyResourceArgList[i])+1;
		strend = strstr(strstart,ApplyResourceArgList[i])-2;
		switch(i)
		{
			case 0:
			{
				memset(ApplyTempInfor.MsgTypeCode,0,50);
				memcpy(ApplyTempInfor.MsgTypeCode,strstart,strend-strstart);
			}break;
			case 1:
			{
				memset(ApplyTempInfor.LiftId,0,50);
				memcpy(ApplyTempInfor.LiftId,strstart,strend-strstart);
			}break;
			case 2:
			{
				memset(ApplyTempInfor.TaskDetailKey,0,50);
				memcpy(ApplyTempInfor.TaskDetailKey,strstart,strend-strstart);
			}break;
			case 3:
			{
				memset(ApplyTempInfor.Destination,0,50);
				memcpy(ApplyTempInfor.Destination,strstart,strend-strstart);
			}break;
			case 4:
			{
				memset(ApplyTempInfor.InstantLocation,0,50);
				memcpy(ApplyTempInfor.InstantLocation,strstart,strend-strstart);
			}break;
		}
	}
}
static void TaskExcuteParser(uint8_t *ExcuteBuffer)
{
	//ֱ�ӽ�������
	uint8_t i=0;
	char *strstart;
	char *strend;
	for(i=0;i<AFFUCARGCOUNT;i++)
	{
		strstart = strstr((char *)ExcuteBuffer,ApplyForExecutionArgList[i]);
		strstart = strstart+strlen(ApplyForExecutionArgList[i])+1;
		strend = strstr(strstart,ApplyForExecutionArgList[i])-2;
		//��ȡ�����ֶεľ���ֵ
		switch(i)
		{
			case 0:
			{
				memset(ExcuteTempInfor.MsgTypeCode,0,50);
				memcpy(ExcuteTempInfor.MsgTypeCode,strstart,strend-strstart);
			}break;
			case 1:
			{
				memset(ExcuteTempInfor.OldLiftId,0,50);
				memcpy(ExcuteTempInfor.OldLiftId,strstart,strend-strstart);
			}break;
			case 2:
			{
				memset(ExcuteTempInfor.TaskDetailKey,0,50);
				memcpy(ExcuteTempInfor.TaskDetailKey,strstart,strend-strstart);
			}break;
			case 3:
			{
				memset(ExcuteTempInfor.Destination,0,10);
				memcpy(ExcuteTempInfor.Destination,strstart,strend-strstart);
			}break;
			case 4:
			{
				memset(ExcuteTempInfor.InstantLocation,0,10);
				memcpy(ExcuteTempInfor.InstantLocation,strstart,strend-strstart);
			}break;
			case 5:
			{
				uint8_t temp[80];
				char *strstart1,*strend1;
				memset(ExcuteTempInfor.ServiceUrl,0,50);
				memcpy(ExcuteTempInfor.ServiceUrl,strstart,strend-strstart);
				strstart1 = strstr((char *)ExcuteTempInfor.ServiceUrl,"http://")+7;
				if(strstr(strstart1,"/"))
					strend1 = strstr(strstart1,"/");
				else
					strend1 = strstr(strstart1,":")+5;
				memcpy(temp,strstart1,strend1-strstart1);
				memset(ExcuteTempInfor.ServiceUrl,0,50);
				strcat((char *)ExcuteTempInfor.ServiceUrl,(char *)temp);
				memset(temp,0,80);
				strstart1 = (char *)ExcuteTempInfor.ServiceUrl;
				strend1 = strstr(strstart1,".")+1;
				memcpy(temp,strstart1,strend1-strstart1);
				ExcuteTempInfor.AGVServerIP[0] = atoi((char *)temp);
				memset(temp,0,80);
				strstart1 = strend1;
				strend1 = strstr(strstart1,".")+1;
				memcpy(temp,strstart1,strend1-strstart1);
				ExcuteTempInfor.AGVServerIP[1] = atoi((char *)temp);
				memset(temp,0,80);
				strstart1 = strend1;
				strend1 = strstr(strstart1,".")+1;
				memcpy(temp,strstart1,strend1-strstart1);
				ExcuteTempInfor.AGVServerIP[2] = atoi((char *)temp);
				memset(temp,0,80);
				strstart1 = strend1;
				strend1 = strstr(strstart1,":")+1;
				memcpy(temp,strstart1,strend1-strstart1);
				ExcuteTempInfor.AGVServerIP[3] = atoi((char *)temp);
				memset(temp,0,80);
				strstart1 = strend1;
				memcpy(temp,strstart1,4);
				ExcuteTempInfor.AGVServerPortNum = atoi((char *)temp);
			}break;
		}
	}
}
static void ReleaseResParser(uint8_t *ReleaseBuffer)
{
	//ֱ�ӽ�������
	uint8_t i=0;
	char *strstart;
	char *strend;
	for(i=0;i<RRFUCARGCOUNT;i++)
	{
		strstart=strstr((char *)ReleaseBuffer,ReleaseResourceArgList[i]);
		strstart=strstart+strlen(ReleaseResourceArgList[i])+1;
		strend=strstr(strstart,ReleaseResourceArgList[i])-2;
		//��ȡ�����ֶεľ���ֵ
		switch(i)
		{
			case 0:
			{
				memset(ReleaseTempInfor.MsgTypeCode,0,50);
				memcpy(ReleaseTempInfor.MsgTypeCode,strstart,strend-strstart);
			}break;
			case 1:
			{
				memset(ReleaseTempInfor.LiftId,0,50);
				memcpy(ReleaseTempInfor.LiftId,strstart,strend-strstart);
			}break;
			case 2:
			{
				memset(ReleaseTempInfor.TaskDetailKey,0,50);
				memcpy(ReleaseTempInfor.TaskDetailKey,strstart,strend-strstart);
			}break;
		}
	}
}

