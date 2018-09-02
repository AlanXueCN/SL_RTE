#include "AGV_SM.h"
#include "WebService_Parser.h"
#include "WebService_Send.h"
#include "CAN_STATE.h"
#include "CAN_SEND.h"
#include "TCP_Server.h"
#include "TCP_Client.h"
//#define DEBUG_WEBSERVICE == 1
AGV_SM_Arg_t AGV_RunningTask = {0};
static uint8_t AGV_SendBuffer[2048] = {0};
static State AGV_IDLE_Function(void *Args);
static State AGV_APPLY_Function(void *Args);
static State AGV_WAITEXCUTE_Function(void *Args);
static State AGV_EXCUTE_Function(void *Args);
static State AGV_WAITFINISH_Function(void *Args);
static State AGV_FINISH_Function(void *Args);
static State AGV_RELEASE_Function(void *Args);
static State AGV_WAITOVER_Function(void *Args);
static State AGV_ERROR_Function(void *Args);
static StateFunction AGV_FuctionTable[TASK_CNT] = {0};
static void Client_Callback(void* UserParameters) {
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)UserParameters;	
	TCPClientConnect(RunArgs->ServerIP,RunArgs->ServerPortNum);
	printf("[AGV]    ��֪AGV��������%d.%d.%d.%d:%d ������ɣ�\n"
	,RunArgs->ServerIP[0],RunArgs->ServerIP[1],RunArgs->ServerIP[2],RunArgs->ServerIP[3],RunArgs->ServerPortNum);
	if(TCPClientConnect(RunArgs->ServerIP,RunArgs->ServerPortNum) != false)
	{
		WebService_ErrorGenerate(0,RunArgs);
		WebService_ClientDataGenerate(FinishTask,AGV_SendBuffer,RunArgs);
		TCPClientSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
	}
	else
	{
		printf("[AGV]    ��֪AGV������ʧ�ܣ�\n");
	}
}
void AGV_SM_Init(void)
{
	StateMachine_Init(&AGV_RunningTask.AGVStateMachine,TASK_CNT,AGV_FuctionTable);
	AGV_RunningTask.AGVStateMachine.RunningState = TASK_IDLE;
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_IDLE,AGV_IDLE_Function);
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_APPLY,AGV_APPLY_Function);
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_WAITEXCUTE,AGV_WAITEXCUTE_Function);
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_EXCUTE,AGV_EXCUTE_Function);
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_WAITFINISH,AGV_WAITFINISH_Function);
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_FINISH,AGV_FINISH_Function);
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_RELEASE,AGV_RELEASE_Function);
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_WAITOVER,AGV_WAITOVER_Function);
	StateMachine_SetFuction(&AGV_RunningTask.AGVStateMachine,TASK_ERROR,AGV_ERROR_Function);
	RTE_RoundRobin_CreateTimer("ClientTimer",1000,1,0,Client_Callback,&AGV_RunningTask);
}
static State AGV_IDLE_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;	
	switch(ParserName)
	{
		case Nofuncion:
		{
			
		}break;
		case ApplyResource:
		{
			if(RunArgs->TaskDetailKey[0] == 0)
			{
				//��������
				memcpy(RunArgs->LastTaskDetailKey,ApplyTempInfor.TaskDetailKey,50);
				memcpy(RunArgs->TaskDetailKey,ApplyTempInfor.TaskDetailKey,50);
				memcpy(RunArgs->LiftId,ApplyTempInfor.LiftId,50);
				memcpy(RunArgs->MsgTypeCode,ApplyTempInfor.MsgTypeCode,50);
				RunArgs->TaskDest = atoi((char *)ApplyTempInfor.Destination);
				RunArgs->TaskLocation = atoi((char *)ApplyTempInfor.InstantLocation);
				//����ID
				printf("[AGV]    ���µ���������!\n");
				ef_set_env("taskid", (char *)RunArgs->TaskDetailKey);
				ef_save_env();
				//����APPLY
				ParserName = Nofuncion;
				return TASK_APPLY;
			}
			else
			{
				if(!memcmp(RunArgs->TaskDetailKey,ApplyTempInfor.TaskDetailKey,50))
				{
					memcpy(RunArgs->MsgTypeCode,ApplyTempInfor.MsgTypeCode,50);
					printf("[AGV]    ������ͬ���������룡\n");
					printf("[ELE]    ��ǰ����¥�㣺%d\n",EleStatus.CurrentFloor);
					printf("[ELE]    AGV����¥�㣺%d\n",atoi((char *)ApplyTempInfor.InstantLocation));
					if(EleStatus.CurrentFloor == atoi((char *)ApplyTempInfor.InstantLocation))
					{
						printf("[AGV]    ��������ɣ�\n");
						WebService_ErrorGenerate(0,RunArgs);
						WebService_ServerDataGenerate(ApplyResource,AGV_SendBuffer,RunArgs);
						TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
					}
					ParserName = Nofuncion;
					memset(RunArgs->MsgTypeCode,0,50);
					return TASK_IDLE;
				}
				else
				{
					printf("[AGV]    �����ѱ�ռ�ã�æµ�У�\n");
					ParserName = Nofuncion;
					return TASK_IDLE;
				}
			}
		};
		case ApplyForExecution:
		{
			if(!memcmp(RunArgs->TaskDetailKey,ExcuteTempInfor.TaskDetailKey,50))
			{
				printf("[AGV]    ����������ִ��!\n");
				printf("[ELE]    ��ǰ����¥�㣺%d\n",EleStatus.CurrentFloor);
				printf("[ELE]    AGVĿ��¥�㣺%d\n",atoi((char *)ExcuteTempInfor.Destination));
				memcpy(RunArgs->MsgTypeCode,ExcuteTempInfor.MsgTypeCode,50);
				memcpy(RunArgs->OldLiftId,ExcuteTempInfor.OldLiftId,50);
				RunArgs->ServerPortNum = ExcuteTempInfor.AGVServerPortNum;
				memcpy(RunArgs->ServerIP,ExcuteTempInfor.AGVServerIP,4);
				if(EleStatus.CurrentFloor == atoi((char *)ExcuteTempInfor.Destination))
				{
					WebService_ErrorGenerate(0,RunArgs);
					WebService_ServerDataGenerate(ApplyForExecution,AGV_SendBuffer,RunArgs);
					TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
					printf("[AGV]    ִ������ɣ�\n");
					EleStatus.DoorReady = true;
					memset(RunArgs->MsgTypeCode,0,50);
					memset(RunArgs->OldLiftId,0,50);
					ParserName = Nofuncion;
					return TASK_FINISH;
				}
				else
				{
					if(atoi((char *)ExcuteTempInfor.InstantLocation) == RunArgs->TaskLocation&&
						atoi((char *)ExcuteTempInfor.Destination) == RunArgs->TaskDest)
					{
						printf("[AGV]    ��Ϣһ�£���ʼִ��!\n");
						ParserName = Nofuncion;
						return TASK_EXCUTE;
					}
					else if(RunArgs->TaskLocation == 0&&RunArgs->TaskDest == 0)
					{
						memcpy(RunArgs->LiftId,ExcuteTempInfor.OldLiftId,50);
						RunArgs->TaskDest = atoi((char *)ExcuteTempInfor.Destination);
						RunArgs->TaskLocation = atoi((char *)ExcuteTempInfor.InstantLocation);
						ParserName = Nofuncion;
						return TASK_EXCUTE;
					}
					else
					{
						RunArgs->TaskError = ERROR_DIFFERENT_EXCUTE;
						ParserName = Nofuncion;
						return TASK_ERROR;
					}
				}
			}
			else
			{
				printf("[AGV]    ����ִ�е������뵱ǰ����ID��һ�£�\n");
				if(RunArgs->TaskDetailKey[0] == 0)
					printf("[AGV]    ��ǰ������\n");
				ParserName = Nofuncion;
				return TASK_IDLE;
			}
		};
		case ReleaseResource:
		{
			if(!memcmp(RunArgs->TaskDetailKey,ReleaseTempInfor.TaskDetailKey,50))
			{
				printf("[AGV]    �����������ͷ�!\n");
				memcpy(RunArgs->MsgTypeCode,ReleaseTempInfor.MsgTypeCode,50);
				
				ParserName = Nofuncion;
				return TASK_RELEASE;
			}
			else
			{
				printf("[AGV]    �����ͷŵ������뵱ǰ����ID��һ�£�\n");
				if(RunArgs->TaskDetailKey[0] == 0)
				{
					printf("[AGV]    ��ǰ������\n");
					if(!memcmp(RunArgs->LastTaskDetailKey,ReleaseTempInfor.TaskDetailKey,50))
					{
						printf("[AGV]    ���ϴε�����\n");
						memcpy(RunArgs->MsgTypeCode,ReleaseTempInfor.MsgTypeCode,50);
						WebService_ErrorGenerate(0,RunArgs);
						WebService_ServerDataGenerate(ReleaseResource,AGV_SendBuffer,RunArgs);
						TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
						memset(RunArgs->MsgTypeCode,0,50);
					}
				}
				ParserName = Nofuncion;
			}
		}break;
	}
	return RunArgs->AGVStateMachine.RunningState;	
}
static State AGV_APPLY_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;	
#ifdef DEBUG_WEBSERVICE
	EleStatus.RunStatus = WAITGET;
	return TASK_WAITEXCUTE;
#else
	if(EleStatus.AGVModeFlag==true)
	{
		switch(EleStatus.RunStatus)
		{
			case SERVICEOVER:
			{
				printf("[AGV]    ������������ʼ���룡\n");
				EleStatus.DoorReady = false;
				EleStatus.ApplyStatus = false;
				EleStatus.ExcuteStatus = false;
				EleStatus.FinishStatus = false;
				EleStatus.ReleaseStatus = false;
				EleStatus.DoorReady = false;
				EleStatus.GetToDestination = false;
				EleStatus.DoorOpenAction = false;
				EleStatus.DoorClosedAction = false;
				EleStatus.UpAction = false;
				EleStatus.DownAction = false;
				if(CANDataSend(AGV_APPLY_CMD,RunArgs->TaskDest,RunArgs->TaskLocation) == true)
				{
					EleStatus.RunStatus = WAITGET;
					return TASK_WAITEXCUTE;
				}
				else
				{
					WebService_ErrorGenerate(7,RunArgs);
					WebService_ServerDataGenerate(ApplyResource,AGV_SendBuffer,RunArgs);
					TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
					memset(RunArgs->MsgTypeCode,0,50);
					memset(RunArgs->TaskDetailKey,0,50);
					memset(RunArgs->LastTaskDetailKey,0,50);
					memset(RunArgs->LiftId,0,50);
					RunArgs->TaskDest = 0;
					RunArgs->TaskLocation = 0;
					return TASK_IDLE;
				}
			}
			default:
			{
				printf("[ELE]    ��ǰ��������ģʽ��%x\n",EleStatus.RunStatus);
				EleStatus.RunStatus = WAITGET;
				printf("[ELE]    �ȴ���һ������\n");
				memset(RunArgs->MsgTypeCode,0,50);
				memset(RunArgs->TaskDetailKey,0,50);
				memset(RunArgs->LastTaskDetailKey,0,50);
				memset(RunArgs->LiftId,0,50);
				RunArgs->TaskDest = 0;
				RunArgs->TaskLocation = 0;
				return TASK_IDLE;
			};
		}
	}
	else
	{
		printf("[AGV]    ����ʱ����AGVģʽ��\n");
		memset(RunArgs->MsgTypeCode,0,50);
		memset(RunArgs->TaskDetailKey,0,50);
		memset(RunArgs->LastTaskDetailKey,0,50);
		memset(RunArgs->LiftId,0,50);
		RunArgs->TaskDest = 0;
		RunArgs->TaskLocation = 0;
		return TASK_IDLE;
	}
#endif
	return RunArgs->AGVStateMachine.RunningState;	
}
static State AGV_WAITEXCUTE_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;
#ifdef DEBUG_WEBSERVICE	
	WebService_ServerDataGenerate(ApplyResource,AGV_SendBuffer,RunArgs);
	TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
	return TASK_IDLE;	
#else
	if(EleStatus.DownAction == true)
	{
		EleStatus.DownAction = false;
	}
	if(EleStatus.UpAction == true)
	{
		EleStatus.UpAction = false;
	}
	if(EleStatus.GetToDestination == true)
	{
		printf("[AGV]    �����ѵ���AGV����¥�㣡\n");
		EleStatus.GetToDestination = false;
	}
	if(EleStatus.DoorOpenAction == true)
	{
		printf("[AGV]    ��������ɹ���������AGV����¥�㿪�ţ�\n");
		EleStatus.DoorOpenAction = false;
	}
	if(EleStatus.DoorReady == true)
	{
		printf("[AGV]    ����������ϣ�AGV����¥�����Ѿ��򿪣�\n");
		EleStatus.DoorReady = false;
		osDelay(1000);
		WebService_ErrorGenerate(0,RunArgs);
		WebService_ServerDataGenerate(ApplyResource,AGV_SendBuffer,RunArgs);
		TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
		memset(RunArgs->MsgTypeCode,0,50);
		return TASK_IDLE;	
	}
#endif
	return RunArgs->AGVStateMachine.RunningState;	
}
static State AGV_EXCUTE_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;	
	if(EleStatus.AGVModeFlag==true)
	{
		printf("[AGV]    ������������ʼִ�У�\n");
		EleStatus.ApplyStatus = false;
		EleStatus.ExcuteStatus = false;
		EleStatus.FinishStatus = false;
		EleStatus.ReleaseStatus = false;
		EleStatus.DoorReady = false;
		EleStatus.GetToDestination = false;
		EleStatus.DoorOpenAction = false;
		EleStatus.DoorClosedAction = false;
		EleStatus.UpAction = false;
		EleStatus.DownAction = false;
		if(CANDataSend(AGV_EXCUTE_CMD,RunArgs->TaskDest,RunArgs->TaskLocation) == true)
		{
			WebService_ErrorGenerate(0,RunArgs);
			WebService_ServerDataGenerate(ApplyForExecution,AGV_SendBuffer,RunArgs);
			TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
			EleStatus.RunStatus = WAITGET;
			memset(RunArgs->MsgTypeCode,0,50);
			memset(RunArgs->OldLiftId,0,50);
			printf("[AGV]    ��һ�����ӷ�������\n");
			TCPClientConnect(RunArgs->ServerIP,RunArgs->ServerPortNum);
			return TASK_WAITFINISH;
		}
		else
		{
			WebService_ErrorGenerate(8,RunArgs);
			WebService_ServerDataGenerate(ApplyForExecution,AGV_SendBuffer,RunArgs);
			TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
			memset(RunArgs->MsgTypeCode,0,50);
			memset(RunArgs->OldLiftId,0,50);
			RunArgs->ServerPortNum = 0;
			memset(RunArgs->ServerIP,0,4);
			return TASK_IDLE;
		}
	}
	else
	{
		printf("[AGV]    ִ��ʱ����AGVģʽ��\n");
		memset(RunArgs->MsgTypeCode,0,50);
		memset(RunArgs->OldLiftId,0,50);
		RunArgs->ServerPortNum = 0;
		memset(RunArgs->ServerIP,0,4);
		return TASK_IDLE;
	}
	return RunArgs->AGVStateMachine.RunningState;	
}
static State AGV_WAITFINISH_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;	
	if(EleStatus.DownAction == true)
	{
		EleStatus.DownAction = false;
	}
	if(EleStatus.UpAction == true)
	{
		EleStatus.UpAction = false;
	}
	if(EleStatus.GetToDestination == true)
	{
		printf("[AGV]    �����ѵ���Ŀ��¥�㣡\n");
		EleStatus.GetToDestination = false;
	}
	if(EleStatus.DoorOpenAction == true)
	{
		printf("[AGV]    ������Ŀ��¥�㿪�ţ�\n");
		EleStatus.DoorOpenAction = false;
	}
	if(EleStatus.DoorClosedAction == true)
	{
		printf("[AGV]    ���ݹ��ţ���ʼִ������\n");
		EleStatus.DoorClosedAction = false;
	}
	if(EleStatus.DoorReady == true)
	{
		printf("[AGV]    ������״̬�б䣡\n");
		EleStatus.DoorReady = false;
	}
	if(EleStatus.FinishStatus == true)
	{
		printf("[AGV]    ���������н��� �����ѵ���Ŀ��¥�� ׼����֪AGV��\n");
		EleStatus.FinishStatus = false;
		return TASK_FINISH;
	}
	return RunArgs->AGVStateMachine.RunningState;	
}
static State AGV_FINISH_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;	
	if(EleStatus.DoorReady == true)
	{
		printf("[AGV]    �����Ŵ���ϣ��ȴ���֪AGV��������\n");
		if(RTE_RoundRobin_GetTimerState("ClientTimer")==0)
		{
			RTE_RoundRobin_ResumeTimer("ClientTimer");
		}
		EleStatus.DoorReady = false;
	}
	return RunArgs->AGVStateMachine.RunningState;	
}
static State AGV_RELEASE_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;	
	if(EleStatus.AGVModeFlag==true)
	{
		printf("[AGV]    ������������ʼ�ͷţ�\n");
		EleStatus.ApplyStatus = false;
		EleStatus.ExcuteStatus = false;
		EleStatus.FinishStatus = false;
		EleStatus.ReleaseStatus = false;
		EleStatus.DoorReady = false;
		EleStatus.GetToDestination = false;
		EleStatus.DoorOpenAction = false;
		EleStatus.DoorClosedAction = false;
		EleStatus.UpAction = false;
		EleStatus.DownAction = false;
		if(CANDataSend(AGV_RELEASE_CMD,RunArgs->TaskDest,RunArgs->TaskLocation) == true)
		{
			EleStatus.RunStatus = WAITGET;
			return TASK_WAITOVER;
		}
		else
		{
			WebService_ErrorGenerate(9,RunArgs);
			WebService_ServerDataGenerate(ApplyForExecution,AGV_SendBuffer,RunArgs);
			TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
			memset(RunArgs->MsgTypeCode,0,50);
			return TASK_IDLE;
		}
	}
	else
	{
		printf("[AGV]    �ͷ�ʱ����AGVģʽ��\n");
		memset(RunArgs->MsgTypeCode,0,50);
		return TASK_IDLE;
	}
	return RunArgs->AGVStateMachine.RunningState;	
}
static State AGV_WAITOVER_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;	
	if(EleStatus.DoorClosedAction == true)
	{
		printf("[AGV]    �ͷŹ��ţ�\n");
		EleStatus.DoorClosedAction = false;
	}
	if(EleStatus.DoorReady == true)
	{
		printf("[AGV]    ������״̬�б䣡\n");
		EleStatus.DoorReady = false;
		printf("[AGV]    ��������̽�����\n");
		osDelay(1000);
		WebService_ErrorGenerate(0,RunArgs);
		WebService_ServerDataGenerate(ReleaseResource,AGV_SendBuffer,RunArgs);
		TCPServerSendData(AGV_SendBuffer,strlen((char *)AGV_SendBuffer));
		RunArgs->TaskDest = 0;
		RunArgs->TaskLocation = 0;
		memset(RunArgs->TaskDetailKey,0,50);
		memset(RunArgs->LiftId,0,50);
		memset(RunArgs->MsgTypeCode,0,50);
		memset(RunArgs->OldLiftId,0,50);
		RunArgs->ServerPortNum = 0;
		memset(RunArgs->ServerIP,0,4);
		ef_set_env("taskid", "0");
		ef_save_env();
		return TASK_IDLE;
	}
	return RunArgs->AGVStateMachine.RunningState;	
}
static State AGV_ERROR_Function(void *Args)
{
	AGV_SM_Arg_t *RunArgs = (AGV_SM_Arg_t *)Args;	
	switch(RunArgs->TaskError)
	{
		case ERROR_UNEXPECT_APPLY:
		{
			printf("[ERROR]    δ�����������룡\n");
		}break;
		case ERROR_DIFFERENT_EXCUTE:
		{
			printf("[ERROR]    �յ���ִ��������Ϣ�����벻һ�£�\n");
		}break;
		default:
			break;
	}
	return RunArgs->AGVStateMachine.RunningState;	
}
