#include "WebService_SoapFuncionName.h"
const char left[] = {"<"};
const char right[] = {">"};
const char slash[] = {"/"};
const char* SoapRequestFucList[SOAPFUCCOUNT] = {
	"cs2:ApplyResource", //��Դ����
	"cs2:ExcuteTask", //����ִ��
	"cs2:ReleaseResource", //��Դ�ͷ�
};
const char* SoapResponseFucList[SOAPFUCCOUNT] = {
	"ns:ApplyResourceRes", //��Դ����
	"ns:ApplyForExecutionRes", //����ִ��	
	"ns:ReleaseResourceRes", //��Դ�ͷ�
};
const char* StringResponseFucList[SOAPFUCCOUNT] = {
	"ApplyResourceRes", //��Դ����
	"ApplyForExecutionRes", //����ִ��	
	"ReleaseResourceRes", //��Դ�ͷ�
};
const char* ApplyResourceArgList[ARFUCARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"LiftId",//���ݱ��
	"TaskDetailKey",//������
	"Destination",//AGVĿ��¥��
	"InstantLocation",//AGC����¥��
};
const char* ApplyResourceResPonseArgList[ARFUCRESARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"Error",//������
	"ErrMsg",//�������Ӧ�Ĵ�����Ϣ
	"LiftId",//��������еĵ��ݱ��
	"TaskDetailKey",//�����е�����Ψһ��ʶ
};
const char* ApplyForExecutionArgList[AFFUCARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"LiftId",//���ݱ��
	"TaskDetailKey",//������
	"Destination",//AGVĿ��¥��
	"InstantLocation",//AGC����¥��
	"ServiceUrl",//WebService�����ַ
};
const char* ApplyForExecutionResPonseArgList[AFFUCRESARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"Error",//������
	"ErrMsg",//�������Ӧ�Ĵ�����Ϣ
	"OldLiftId",//��������еĵ��ݱ��
	"OldTaskDetailKey",//�����е�����Ψһ��ʶ�����ֶ�ֻ�е����ֵ�ǰִ�е�������������ִ�е�����һ��ʱ���Ż��и��ֶΣ���֪�ͻ��ˣ�
};
const char* ReleaseResourceArgList[RRFUCARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"LiftId",//���ݱ��
	"TaskDetailKey",//������
};
const char* ReleaseResourceResArgList[RRFUCRESARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"Error",//������
	"ErrMsg",//�������Ӧ�Ĵ�����Ϣ
};
const char* ClientSoapRequestFucList[CLIENTSOAPFUCCOUNT] = {
	"urn:LiftService-FiniTask", //�������
	"urn:LiftService-ErrMsg", //������Ϣ
};
const char* StringRequestFucList[CLIENTSOAPFUCCOUNT] = {
	"FinishTask", //��Դ����
	"ErrorMessage", //����ִ��	
};
const char* FinishTaskArgList[FTFUCARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"LiftId",//���ݱ��
	"TaskDetailKey",//������
	"InstantLocation",//AGC����¥��
};
const char* FinishTaskResponseArgList[FTFUCRESARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"LiftId",//���ݱ��
	"TaskDetailKey",//������
};
const char* ErrorMessageArgList[EMFUCARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"LiftId",//���ݱ��
	"TaskDetailKey",//������
	"Error",//������
	"ErrMsg",//�������Ӧ�Ĵ�����Ϣ
};
const char* ErrorMessageResponseArgList[EMFUCRESARGCOUNT] = {
	"MsgTypeCode",//��Ϣ����
	"Error",//������
	"ErrMsg",//�������Ӧ�Ĵ�����Ϣ
};
const AGVTaskError_t ErrorCode[]=
{
	{"0","NoError"},
	{"1","TaskDetailKeyDifferent"},
	{"2","FloorInforDifferent"},
	{"3","ApplyWithoutRelease"},
	{"4","NoTaskExcute"},
	{"5","NoTaskRelease"},
	{"6","NotAGVMode"},
	{"7","CanApplyFail"},
	{"8","CanExcuteFail"},
	{"9","CanReleaseFail"},
};
