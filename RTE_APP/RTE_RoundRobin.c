#include "RTE_Include.h"
/*****************************************************************************
*** Author: Shannon
*** Version: 2.1 2018.8.31
*** History: 1.0 �������޸���tivaware
             2.0 ΪRTE�����������䣬����ģ������
						 2.1 ����̬��Ϸ�ʽ����
*****************************************************************************/
#if RTE_USE_ROUNDROBIN == 1
#if RTE_USE_OS == 1
#include "RTE_Components.h"
#include CMSIS_device_header
#endif
/*************************************************
*** ����RoundRobin�Ľṹ���������̬����
*************************************************/
static RTE_RoundRobin_t RoundRobinHandle = 
{
	.SoftTimerCnt = 0,
	.RoundRobinRunTick = 0,
	.SoftTimerTable = (void *)0,
};
/*************************************************
*** Args:   Timer ������ʱ��
*** Function: SoftTimer��ʱ����
*************************************************/
static void RTE_RoundRobin_CheckTimer(uint8_t Timer)
{
	/* Check if count is zero */
	if (RoundRobinHandle.SoftTimerTable[Timer].CNT == 0) {
		/* Call user callback function */
		RoundRobinHandle.SoftTimerTable[Timer].Callback(RoundRobinHandle.SoftTimerTable[Timer].UserParameters);
		/* Set new counter value */
		RoundRobinHandle.SoftTimerTable[Timer].CNT = RoundRobinHandle.SoftTimerTable[Timer].ARR;
		/* Remove timer if auto reload feature is not used */
		if (!RoundRobinHandle.SoftTimerTable[Timer].AREN) {
			RoundRobinHandle.SoftTimerTable[Timer].CNTEN = 0;
		}
	}
}
/*************************************************
*** Args:   Null
*** Function: RoundRobinʱ������
*************************************************/
void RTE_RoundRobin_TickHandler(void)
{
  RoundRobinHandle.RoundRobinRunTick++;
	// Loop through each task in the task table.
	for(uint8_t i = 0; i < RoundRobinHandle.SoftTimerCnt; i++)
	{
    if(RoundRobinHandle.SoftTimerTable[i].CNTEN)
		{
			/* Decrease counter if needed */
			if (RoundRobinHandle.SoftTimerTable[i].CNT)
				RoundRobinHandle.SoftTimerTable[i].CNT--;
#if RTE_USE_OS == 1
			RTE_RoundRobin_CheckTimer(i);
#endif
		}
	}
}
#if RTE_USE_OS == 0
/*************************************************
*** Args:   Null
*** Function: RoundRobin���к��� �ڷǲ���ϵͳ�����µ���
*************************************************/
void RTE_RoundRobin_Run(void)
{
	// Loop through each task in the task table.
	for(uint8_t i = 0; i < RoundRobinHandle.SoftTimerCnt; i++)
	{
		RTE_RoundRobin_CheckTimer(i);
	}
}
#endif
/*************************************************
*** Args:   
					*Name ����Ӷ�ʱ������
					ReloadValue ��װ��ֵ
          ReloadEnable ��װ��ʹ��
          ReloadEnable ��ʱ������ʹ��
          *TimerCallback ��ʱ���ص�����
          *UserParameters �ص������������
*** Function: Ϊ��ǰRoundRobin�������һ����ʱ��
*************************************************/
RTE_RoundRobin_Err_e RTE_RoundRobin_CreateTimer(const char *Name,
	uint32_t ReloadValue, 
	uint8_t ReloadEnable, 
	uint8_t RunEnable, 
	void (*TimerCallback)(void *), 
	void* UserParameters)
{
	if(RoundRobinHandle.SoftTimerTable == (void*)0)
		return RR_HANDLEUNINIT;
	if(RoundRobinHandle.SoftTimerCnt >= HI_ROUNDROBIN_MAX_NUM)
		return RR_NOSPACEFORNEW;
	for(uint8_t i = 0;i<RoundRobinHandle.SoftTimerCnt;i++)
	{
		if(!strcmp(Name,RoundRobinHandle.SoftTimerTable[i].TimerName))
			return RR_ALREADYEXIST;
	}
	RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].TimerName = Name;
	RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].AREN = ReloadEnable;
	RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].CNTEN = RunEnable;
	RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].ARR = ReloadValue;
	RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].CNT = ReloadValue;
	RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].Callback = TimerCallback;
	RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].UserParameters = UserParameters;
	RoundRobinHandle.SoftTimerCnt++;
	return RR_NOERR;
}
/*************************************************
*** Args:   
					*Name ��ɾ����ʱ������
*** Function: Ϊ��ǰRoundRobin����ɾ��һ����ʱ��
*************************************************/
RTE_RoundRobin_Err_e RTE_RoundRobin_RemoveTimer(const char *Name)
{
	int8_t idx = -1;
	for(uint8_t i = 0;i<RoundRobinHandle.SoftTimerCnt;i++)
	{
		if(!ustrcmp(Name,RoundRobinHandle.SoftTimerTable[i].TimerName))
		{
			idx = i;
			break;
		}
	}
	if(idx!=-1)
	{
		for(uint8_t i = idx;i<RoundRobinHandle.SoftTimerCnt-1;i++)
		{
			RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].TimerName 
				= RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt+1].TimerName ;
			RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].AREN 
				= RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt+1].AREN ;
			RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].CNTEN 
				= RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt+1].CNTEN ;
			RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].ARR 
				= RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt+1].ARR ;
			RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].CNT 
				= RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt+1].CNT ;
			RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].Callback 
				= RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt+1].Callback ;
			RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].UserParameters 
				= RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt+1].UserParameters ;
		}
		RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].TimerName = (void*)0;
		RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].AREN = 0;
		RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].CNTEN = 0;
		RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].ARR = 0;
		RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].CNT = 0;
		RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].Callback = (void*)0;
		RoundRobinHandle.SoftTimerTable[RoundRobinHandle.SoftTimerCnt].UserParameters = (void*)0;
		RoundRobinHandle.SoftTimerCnt--;
		return RR_NOERR;
	}
	return RR_NOSUCHTIMER;
}
/*************************************************
*** Args:   
					*Name ����ͣ��ʱ������
*** Function: ��ͣ��ǰRoundRobin�����е�һ����ʱ��
*************************************************/
RTE_RoundRobin_Err_e RTE_RoundRobin_PauseTimer(const char *Name) 
{
	int8_t idx = -1;
	for(uint8_t i = 0;i<RoundRobinHandle.SoftTimerCnt;i++)
	{
		if(!ustrcmp(Name,RoundRobinHandle.SoftTimerTable[i].TimerName))
		{
			idx = i;
			break;
		}
	}
	if(idx!=-1)
	{
		RoundRobinHandle.SoftTimerTable[idx].CNTEN = 0;
		return RR_NOERR;
	}
	return RR_NOSUCHTIMER;
}
/*************************************************
*** Args:   
					*Name ����ͣ��ʱ������
*** Function: �ָ���ǰRoundRobin�����е�һ����ʱ��
*************************************************/
RTE_RoundRobin_Err_e RTE_RoundRobin_ResumeTimer(const char *Name) 
{
	int8_t idx = -1;
	for(uint8_t i = 0;i<RoundRobinHandle.SoftTimerCnt;i++)
	{
		if(!ustrcmp(Name,RoundRobinHandle.SoftTimerTable[i].TimerName))
		{
			idx = i;
			break;
		}
	}
	if(idx!=-1)
	{
		RoundRobinHandle.SoftTimerTable[idx].CNTEN = 1;
		return RR_NOERR;
	}
	return RR_NOSUCHTIMER;
}
/*************************************************
*** Args:   
					Null
*** Function: ��ȡ��ǰRoundRobin������Ϣ
*************************************************/
void RTE_RoundRobin_Demon(void)
{
	RTE_Printf("[RR]    ��ǰ��ת����TIMERʹ����Ŀ��%d �����Ŀ��%d\r\n",RoundRobinHandle.SoftTimerCnt,HI_ROUNDROBIN_MAX_NUM);
	for(uint8_t i = 0;i<RoundRobinHandle.SoftTimerCnt;i++)
	{
		RTE_Printf("[RR]    TIMER:%16s----�Զ����أ�%x ����ֵ��%6d ��ǰ������%6d ���У�%x\r\n",
			RoundRobinHandle.SoftTimerTable[i].TimerName,RoundRobinHandle.SoftTimerTable[i].AREN,RoundRobinHandle.SoftTimerTable[i].ARR
			,RoundRobinHandle.SoftTimerTable[i].CNT,RoundRobinHandle.SoftTimerTable[i].CNTEN);
	}
}
/*************************************************
*** Args:   
					Null
*** Function: ��ȡ��ǰRoundRobin��������ʱ��
*************************************************/
uint32_t RTE_RoundRobin_GetTick(void) 
{
	/* Return current time in milliseconds */
#if RTE_USE_OS == 1
	#ifdef RTE_CMSIS_RTOS
	if (osKernelRunning () == true) 
	{
    return APPSysRunTime;
  }
	#else
	if (osKernelGetState () == osKernelRunning) 
	{
    return osKernelGetTickCount();
  }
	#endif
	else
	{
		static uint32_t ticks = 0U;
					 uint32_t i;
		/* If Kernel is not running wait approximately 1 ms then increment 
			 and return auxiliary tick counter value */
		for (i = (SystemCoreClock >> 14U); i > 0U; i--) {
			__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
			__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
		}
		return ++ticks;
	}
#else
	return RoundRobinHandle.RoundRobinRunTick;
#endif 
}
#endif
/*************************************************
*** Args:   NULL
*** Function: RoundRobin��ʼ��
*************************************************/
void RTE_RoundRobin_Init(void)
{
	RoundRobinHandle.SoftTimerTable = (RTE_SoftTimer_t *)RTE_BGetz(MEM_RTE,(HI_ROUNDROBIN_MAX_NUM + 1) * sizeof(RTE_SoftTimer_t));
	RTE_AssertParam(RoundRobinHandle.SoftTimerTable);
}
/*************************************************
*** Args:   
					*Name ����λ��ʱ������
*** Function: ��λ��ǰRoundRobin�����е�һ����ʱ��
*************************************************/
RTE_RoundRobin_Err_e RTE_RoundRobin_ResetTimer(const char *Name) 
{
	int8_t idx = -1;
	for(uint8_t i = 0;i<RoundRobinHandle.SoftTimerCnt;i++)
	{
		if(!ustrcmp(Name,RoundRobinHandle.SoftTimerTable[i].TimerName))
		{
			idx = i;
			break;
		}
	}
	if(idx!=-1)
	{
		RoundRobinHandle.SoftTimerTable[idx].CNT = RoundRobinHandle.SoftTimerTable[idx].ARR;
		RoundRobinHandle.SoftTimerTable[idx].CNTEN = 1;
	}
	else
		return RR_NOSUCHTIMER;
	return RR_NOERR;
}
