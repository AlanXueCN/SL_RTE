#include "TCP_Client.h"
#include "Net_Config.h"
#include "AGV_SM.h"
static uint8_t TCPClientrSocket;
static uint16_t LocalPortNum = 0;
uint8_t TCPClientRealData[2048] = {0};
uint16_t TCPClientRealDataLenth = 0;
/*
*********************************************************************************************************
*	�� �� ��: tcp_callback
*	����˵��: TCP Socket�Ļص�����
*	��    ��: soc  TCP Socket����
*             evt  �¼�����
*             ptr  �¼�������TCP_EVT_DATA��ptrָ��Ļ�������¼�Ž��յ���TCP���ݣ������¼���¼IP��ַ
*             par  �¼�������TCP_EVT_DATA����¼���յ������ݸ����������¼���¼�˿ں�
*	�� �� ֵ: 
*********************************************************************************************************
*/
static uint16_t TCPClientCallback (uint8_t soc, uint8_t evt, uint8_t *ptr, uint16_t par)
{
	char buf[50];
	/* ȷ����socket_tcp�Ļص� */
	if (soc != TCPClientrSocket) 
	{
		return (0);
	}

	switch (evt) 
	{
		/*
			���ӷ�����������Ϣ
		    1������ptr�洢Զ���豸��IP��ַ��par�д洢�˿ںš�
		    2��������ֵ1�������ӣ�������ֵ0��ֹ���ӡ�
		*/
		case TCP_EVT_CONREQ:
				sprintf(buf, "���ӵ������� IP: %d.%d.%d.%d", ptr[0], ptr[1], ptr[2], ptr[3]);
				printf("[CLIENT]    %s �˿ں�Port:%d\n", buf, par);
			return (1);
		/* ������ֹ */
		case TCP_EVT_ABORT:
		{
			printf("[CLIENT]    ������ֹ\n");
		}break;
		/* SocketԶ�������Ѿ����� */
		case TCP_EVT_CONNECT:
		{
			printf("[CLIENT]    ���ӽ���\n");
		}break;
		/* ���ӶϿ� */
		case TCP_EVT_CLOSE:
		{
			printf("[CLIENT]    ���ӱ��ر�\n");
		}break;
		/* ���͵������յ�Զ���豸Ӧ�� */
		case TCP_EVT_ACK:
			break;
		/* ���յ�TCP����֡��ptrָ�����ݵ�ַ��par��¼���ݳ��ȣ���λ�ֽ� */
		case TCP_EVT_DATA:
		{
			printf("[CLIENT]    �������Ѿ��յ���\n");
			extern AGV_SM_Arg_t AGV_RunningTask;
			AGV_RunningTask.ServerPortNum = 0;
			memset(AGV_RunningTask.ServerIP,0,4);
			RTE_RoundRobin_PauseTimer("ClientTimer");
			AGV_RunningTask.AGVStateMachine.RunningState = TASK_IDLE;
		}break;
	}
	return (0);
}
void TCPClientInit(void)
{
	/* 
	   ����TCP Socket�����ӣ��ͻ������ӷ�������10����������ͨ�Ž��Ͽ����ӡ�
	   ������������ʹ����TCP_TYPE_KEEP_ALIVE����һֱ�������ӣ�����10���ʱ�����ơ�
	*/
  TCPClientrSocket = tcp_get_socket (TCP_TYPE_CLIENT | TCP_TYPE_KEEP_ALIVE, 0, 10, TCPClientCallback);
	printf("[CLIENT]    �����ͻ���Socket:%d\n",TCPClientrSocket);
}
bool TCPClientConnect(uint8_t *RemoteIP,uint16_t RemotePort)
{
	if(TCPClientrSocket != 0)
	{
		if(tcp_get_state(TCPClientrSocket)!=TCP_STATE_CONNECT)
		{
			LocalPortNum = (uint16_t)RTE_RoundRobin_GetTick();
			bool rev = tcp_connect (TCPClientrSocket, RemoteIP, RemotePort, LocalPortNum);
			printf("[CLIENT]    ���ӵ���������%d.%d.%d.%d:%d ״̬:%d\n",RemoteIP[0],RemoteIP[1],RemoteIP[2]
			,RemoteIP[3],RemotePort,rev);
			return rev;
		}
		else
			return true;
	}
	return false;
}
void TCPClientSendData(uint8_t *Data,uint16_t Lenth)
{
	int32_t iCount=Lenth;
	uint8_t *dataadd=Data;
	uint8_t SocketState;
	uint16_t maxlen;
	uint8_t *sendbuf;
	SocketState = tcp_get_state(TCPClientrSocket);
	if(SocketState == TCP_STATE_CONNECT)
	{
		do
		{
			main_TcpNet();
			if (tcp_check_send (TCPClientrSocket) == true) 
			{
				maxlen = tcp_max_dsize (TCPClientrSocket);
				iCount -= maxlen;
				if(iCount < 0)
				{
					/* ��ô����û����� */
					maxlen = iCount + maxlen;
				}
				sendbuf = tcp_get_buf(maxlen);
				memcpy(sendbuf,dataadd,maxlen);
				/* ���Է���ֻ��ʹ�û�ȡ���ڴ� */
				tcp_send (TCPClientrSocket, sendbuf, maxlen);
				if(iCount > 0)
					dataadd=dataadd+maxlen;
			}
		}while(iCount > 0);		
	}
}
