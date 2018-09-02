#include "TCP_Server.h"
#include "Net_Config.h"
static uint8_t TCPServerSocket;
uint8_t TCPServerRealData[2048] = {0};
uint16_t TCPServerRealDataLenth = 0;
extern osThreadId_t ThreadIDWebService;
static uint16_t TCPServerCallback (uint8_t soc, uint8_t evt, uint8_t *ptr, uint16_t par)
{
	char buf[50];
	/* ȷ����socket_tcp�Ļص� */
	if (soc != TCPServerSocket) 
	{
		return (0);
	}
	switch (evt) 
	{
		/*
			Զ�̿ͻ���������Ϣ
		    1������ptr�洢Զ���豸��IP��ַ��par�д洢�˿ںš�
		    2��������ֵ1�������ӣ�������ֵ0��ֹ���ӡ�
		*/
		case TCP_EVT_CONREQ:
				sprintf(buf, "[SERVER]    �пͻ������� IP: %d.%d.%d.%d", ptr[0], ptr[1], ptr[2], ptr[3]);
				printf("%s �˿ں�:%d\n", buf, par);
			return (1);
		/* ������ֹ */
		case TCP_EVT_ABORT:
			break;
		/* SocketԶ�������Ѿ����� */
		case TCP_EVT_CONNECT:
				printf("[SERVER]    ���ӽ���\n");
			break;
		/* ���ӶϿ� */
		case TCP_EVT_CLOSE:
		   	printf("[SERVER]    ���ӶϿ�\n");
			break;
		/* ���͵������յ�Զ���豸Ӧ�� */
		case TCP_EVT_ACK:
			break;
		/* ���յ�TCP����֡��ptrָ�����ݵ�ַ��par��¼���ݳ��ȣ���λ�ֽ� */
		case TCP_EVT_DATA:
		{
			char EndString[30] = {0};
			memcpy(TCPServerRealData + TCPServerRealDataLenth, ptr, par);
			TCPServerRealDataLenth = TCPServerRealDataLenth + par;
			memcpy(EndString , TCPServerRealData + TCPServerRealDataLenth - 30 , 30);
			if(strstr((char *)EndString,"</SOAP-ENV:Envelope>"))
			{
				printf("[WebService]    �յ�һ֡���ݣ�\n");
				osThreadFlagsSet(ThreadIDWebService,0x0001U);
			}
		}break;
	}
	return (0);
}
void TCPServerInit(void)
{
	/* 
	   ����TCP Socket�������������ͻ������ӷ�������10����������ͨ�Ž��Ͽ����ӡ�
	   ������������ʹ����TCP_TYPE_KEEP_ALIVE����һֱ�������ӣ�����10���ʱ�����ơ�
	*/
  TCPServerSocket = tcp_get_socket (TCP_TYPE_SERVER | TCP_TYPE_KEEP_ALIVE, 0, 10, TCPServerCallback);
	if(TCPServerSocket != 0)
	{
		printf("[SERVER]    ����������Socket:%d ����:%d\n",TCPServerSocket,tcp_listen (TCPServerSocket, 8888));
	}
}
void TCPServerSendData(uint8_t *Data,uint16_t Lenth)
{
	int32_t iCount=Lenth;
	uint8_t *dataadd=Data;
	uint16_t maxlen;
	uint8_t *sendbuf;
	while(tcp_get_state(TCPServerSocket)!=TCP_STATE_CONNECT)
	{
		main_TcpNet();
	}
	do
	{
		main_TcpNet();
		if (tcp_check_send (TCPServerSocket) == true) 
		{
			maxlen = tcp_max_dsize (TCPServerSocket);
			iCount -= maxlen;
			if(iCount < 0)
			{
				/* ��ô����û����� */
				maxlen = iCount + maxlen;
			}
			sendbuf = tcp_get_buf(maxlen);
			memcpy(sendbuf,dataadd,maxlen);
			/* ���Է���ֻ��ʹ�û�ȡ���ڴ� */
			tcp_send (TCPServerSocket, sendbuf, maxlen);
			if(iCount > 0)
				dataadd=dataadd+maxlen;
		}
	}while(iCount > 0);		
}

