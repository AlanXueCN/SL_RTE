#include "CAN_SEND.h"
#include "CAN_STATE.h"

extern const uint8_t MontherSoftVer;
extern const uint8_t SonSoftVer;
extern const uint8_t SubSoftVer;
extern const unsigned short UpdateDate;
void CANAGVModeControl(bool AGVMode)
{
	uint8_t SendData[8] = {0};
	if(AGVMode == true)
	{
		SendData[0] = AGV_MODE_CTRL_CMD;
		SendData[1] = 1;
		SendData[2] = CANCRC8Check(SendData,2);
		BSP_CAN1_WriteData(AGV_TRANSBOARD,SendData,3);
	}
	else
	{
		SendData[0] = AGV_MODE_CTRL_CMD;
		SendData[1] = 2;
		SendData[2] = CANCRC8Check(SendData,2);
		BSP_CAN1_WriteData(AGV_TRANSBOARD,SendData,3);
	}
	uint32_t StartTick = RTE_RoundRobin_GetTick();
	while(RTE_RoundRobin_GetTick() - StartTick <= 1000)
	{
		if(EleStatus.ModeCtrlStatus == true)
		{
			printf("[CANSEND]    ������� �ɹ���\n");
			EleStatus.ModeCtrlStatus = false;
			return;
		}
	}
	printf("[CANSEND]    ������� ʧ�ܣ�(ʹ��ָ��ʱ���Դ���Ϣ)\n");
}
bool CANDataSend(uint8_t CanCommand,uint8_t DestFloor,uint8_t CurrentFloor)
{
	uint8_t SendData[8] = {0};
	switch(CanCommand)
	{
		case AGV_GET_LIFT_NUM_CMD:		
		{
			SendData[0] = AGV_GET_LIFT_NUM_CMD;
			SendData[1] = 0;
			SendData[2] = CANCRC8Check(SendData,2);
			BSP_CAN1_WriteData(AGV_TRANSBOARD,SendData,3);
			printf("[CANSend]    Time:%d LiftNumSend\n",RTE_RoundRobin_GetTick());
		}return true;
		case AFV_HEATBEAT_CMD:
		{
			SendData[0] = AFV_HEATBEAT_CMD;
			SendData[1] = AGV_TRANSBOARD;
			SendData[2] = 0;
			SendData[3] = CANCRC8Check(SendData,3);
			BSP_CAN1_WriteData(AGV_TRANSBOARD,SendData,4);
		}return true;
		case AGV_SOFTWARE_CMD:
		{
			SendData[0] = AGV_SOFTWARE_CMD;
			SendData[1] = AGV_TRANSBOARD;
			SendData[2] = MontherSoftVer;
			SendData[3] = SonSoftVer;
			SendData[4]  = SubSoftVer;
			SendData[5] = UpdateDate&0xff;
			SendData[6] = (UpdateDate>>8)&0xff;
			SendData[7] = CANCRC8Check(SendData,7);
			BSP_CAN1_WriteData(AGV_TRANSBOARD,SendData,8);
		}return true;
		case AGV_APPLY_CMD:
		{
			SendData[0] = AGV_APPLY_CMD;
			SendData[1] = DestFloor;
			SendData[2] = CurrentFloor;
			SendData[3] = 0;
			SendData[4] = CANCRC8Check(SendData,4);
			BSP_CAN1_WriteData(AGV_TRANSBOARD,SendData,5);
			printf("[CANSEND]    �������� ��ǰAGV����¥��:%d Ŀ��¥��:%d.\n",CurrentFloor,DestFloor);
			uint32_t StartTick = RTE_RoundRobin_GetTick();
			while(RTE_RoundRobin_GetTick() - StartTick <= 1000)
			{
				if(EleStatus.ApplyStatus == true)
				{
					EleStatus.ApplyStatus = false;
					return true;
				}
			}
			printf("[CANSEND]    �������� ʧ��(ʹ��ָ��ʱ���Դ���Ϣ)��\n");
			return false;
		}
		case AGV_EXCUTE_CMD:
		{
			SendData[0] = AGV_EXCUTE_CMD;
			SendData[1] = DestFloor;
			SendData[2] = CurrentFloor;
			SendData[3] = 0;
			SendData[4] = CANCRC8Check(SendData,4);
			BSP_CAN1_WriteData(AGV_TRANSBOARD,SendData,5);
			printf("[CANSEND]    ����ִ�� ��ǰAGV����¥��:%d Ŀ��¥��:%d.\n",CurrentFloor,DestFloor);
			uint32_t StartTick = RTE_RoundRobin_GetTick();
			while(RTE_RoundRobin_GetTick() - StartTick <= 1000)
			{
				if(EleStatus.ExcuteStatus == true)
				{
					EleStatus.ExcuteStatus = false;
					return true;
				}
			}
			printf("[CANSEND]    ����ִ�� ʧ��(ʹ��ָ��ʱ���Դ���Ϣ)��\n");
			return false;
		}
		case AGV_RELEASE_CMD:
		{
			SendData[0] = AGV_RELEASE_CMD;
			SendData[1] = DestFloor;
			SendData[2] = CurrentFloor;
			SendData[3] = 0;
			SendData[4] = CANCRC8Check(SendData,4);
			BSP_CAN1_WriteData(AGV_TRANSBOARD,SendData,5);
			printf("[CANSEND]    �����ͷ� ��ǰAGV����¥��:%d Ŀ��¥��:%d.\n",CurrentFloor,DestFloor);
			uint32_t StartTick = RTE_RoundRobin_GetTick();
			while(RTE_RoundRobin_GetTick() - StartTick <= 1000)
			{
				if(EleStatus.ReleaseStatus == true)
				{
					EleStatus.ReleaseStatus = false;
					return true;
				}
			}
			printf("[CANSEND]    �����ͷ� ʧ��(ʹ��ָ��ʱ���Դ���Ϣ)��\n");
			return false;
		}
		default:
			return false;
	}
}

