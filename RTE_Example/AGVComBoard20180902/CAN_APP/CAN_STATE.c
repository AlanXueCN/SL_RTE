#include "CAN_STATE.h"
#include "CAN_SEND.h"
EleStatus_t EleStatus = 
{
	0
};
unsigned int CANCRC8Check(unsigned char *dt, unsigned int length)
{
	unsigned int crc = 0xff;
	unsigned int i, j, flag;
	for (i=0; i<length; i++)
	{
		crc = crc^dt[i];
		for (j=0; j<8; j++)
		{
		  flag = crc&1;
		  crc = crc>>1;
		  if (flag)
		    crc = crc^0xa1;
		}
	}
	return crc;
}
static void MainBoardInforParser(uint8_t *CanData);
static void RunStatusParser(uint8_t *CanData);
static void DoorStatusParser(uint8_t *CanData);
static void ApplyResultParser(uint8_t *CanData);
static void ExcuteResultParser(uint8_t *CanData);
static void FinishResultParser(uint8_t *CanData);
static void ReleaseResultParser(uint8_t *CanData);
static void ModeControlResultParser(uint8_t *CanData);
static void LiftNumResultParser(uint8_t *CanData);
static void EleModeParser(uint8_t *CanData);
void CANDataParser(uint8_t *CanData)
{
	switch(CanData[0])
	{
		//�������յ������� �����ظ���� ���������״̬�й�
		case AGV_MAINBOARD_INFOR_CMD:
		{
			MainBoardInforParser(CanData);
		}break;
		case AGV_RUNSTATUS_CMD:
		{
			RunStatusParser(CanData);
		}break;
		case AGV_DOORSTATUS_CMD:
		{
			DoorStatusParser(CanData);
		}break;		
		case AGV_MODE_CMD:
		{
			EleModeParser(CanData);
		}break;
		case AFV_HEATBEAT_CMD:
		{
			//�յ������� �ظ�����
		  CANDataSend(AFV_HEATBEAT_CMD,0,0);
		}break;
		case AGV_SOFTWARE_CMD:
		{
			//�յ�����汾�� �ظ�����
			CANDataSend(AGV_SOFTWARE_CMD,0,0);
		}break;
		//�������ͺ��յ��Ļظ�
		case AGV_MODE_CTRL_CMD:		
		{
			ModeControlResultParser(CanData);
		}break;
		case AGV_GET_LIFT_NUM_CMD:		
		{
			LiftNumResultParser(CanData);
		}break;
		case AGV_APPLY_CMD:
		{
			ApplyResultParser(CanData);
		}break;
		case AGV_EXCUTE_CMD:
		{
			ExcuteResultParser(CanData);
		}break;
		case AGV_FINISH_CMD:
		{
			//�յ�Finish
			FinishResultParser(CanData);
		}break;
		case AGV_RELEASE_CMD:
		{
			ReleaseResultParser(CanData);
		}break;
	}
}
static void MainBoardInforParser(uint8_t *CanData)
{
	switch(CanData[1])
	{
		case 0x00:
		{
			printf("[ELE]    ���ݵ�վ��\n");
			EleStatus.GetToDestination = true;
		}break;
		case 0x03:
		{
			printf("[ELE]    ���ݿ��ţ�\n");
			EleStatus.DoorOpenAction = true;
		}break;
		case 0x04:
		{
			printf("[ELE]    ���ݹ��ţ�\n");
			EleStatus.DoorClosedAction = true;
		}break;
		case 0x05:
		{
			printf("[ELE]    �������У�\n");
			EleStatus.UpAction = true;
		}break;
		case 0x06:
		{
			printf("[ELE]    �������У�\n");
			EleStatus.DownAction = true;
		}break;
	}
}
static void RunStatusParser(uint8_t *CanData)
{
	EleStatus.RunStatus = (EleRunStatus_t)CanData[1];
	EleStatus.CurrentFloor = CanData[2]+1;
	EleStatus.DestFloor = CanData[5]+1;
//	printf("[ELE]    ��ǰ��������״̬��%x\n",CanData[1]);
//	printf("[ELE]    ��ǰ����¥�㣺%d\n",EleStatus.CurrentFloor);
//	printf("[ELE]    ����Ŀ��¥�㣺%d\n",EleStatus.DestFloor);
}
static void DoorStatusParser(uint8_t *CanData)
{
	static bool FirstFlag = false;
	EleStatus.DoorStatus = CanData[1];
	if(FirstFlag == false)
	{
		EleStatus.LastStatus = EleStatus.DoorStatus;
		FirstFlag = true;
	}
	if(EleStatus.LastStatus != EleStatus.DoorStatus)
	{
		printf("[ELE]    ��ǰ������״̬���£�%x\n",EleStatus.DoorStatus);
		EleStatus.LastStatus = EleStatus.DoorStatus;
		EleStatus.DoorReady = true;
	}
}
static void EleModeParser(uint8_t *CanData)
{
	EleStatus.WorkMode = CanData[1];
//	printf("[ELE]    ��ǰ���ݹ���ģʽ��%x\n",EleStatus.WorkMode);
	if(EleStatus.WorkMode == 0x0F)
	{
		EleStatus.AGVModeFlag = true;
	}
	else
	{
		EleStatus.AGVModeFlag = false;
	}
}
static void ApplyResultParser(uint8_t *CanData)
{
	uint8_t CanState = 0;
	CanState = CanData[3];
	if(CanState == 0)
	{
		EleStatus.ApplyStatus = true;
		printf("[CANRec]    Time:%d ����Apply�ɹ���\n",RTE_RoundRobin_GetTick());
	}
	else
	{
		printf("[CANRec]    Time:%d ����Applyʧ��:%x\n",RTE_RoundRobin_GetTick(),CanState);
	}
}
static void ExcuteResultParser(uint8_t *CanData)
{
	uint8_t CanState = 0;
	CanState = CanData[3];
	if(CanState == 0)
	{
		EleStatus.ExcuteStatus = true;
		printf("[CANRec]    Time:%d ����ִ�п�ʼ��\n",RTE_RoundRobin_GetTick());
	}
	else
	{
		printf("[CANRec]    Time:%d ����ִ��ʧ�ܣ�%x\n",RTE_RoundRobin_GetTick(),CanState);
	}
}
static void FinishResultParser(uint8_t *CanData)
{
	uint8_t CanState = 0;
	CanState = CanData[1];
	if(CanState == 0)
	{
		EleStatus.FinishStatus = true;
		printf("[CANRec]    Time:%d ���������\n",RTE_RoundRobin_GetTick());
	}
	else
	{
		printf("[CANRec]    Time:%d ����ʧ�ܣ�%x\n",RTE_RoundRobin_GetTick(),CanState);
	}
}
static void ReleaseResultParser(uint8_t *CanData)
{
	uint8_t CanState = 0;
	CanState = CanData[3];
	if(CanState == 0)
	{
		EleStatus.ReleaseStatus = true;
		printf("[CANRec]    Time:%d �����ͷųɹ���\n",RTE_RoundRobin_GetTick());
	}
	else
	{
		printf("[CANRec]    Time:%d �����ͷ�ʧ�ܣ�%x\n",RTE_RoundRobin_GetTick(),CanState);
	}
}
static void ModeControlResultParser(uint8_t *CanData)
{
	
}
static void LiftNumResultParser(uint8_t *CanData)
{

}
