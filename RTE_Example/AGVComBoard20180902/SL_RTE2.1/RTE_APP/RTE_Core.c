#include "RTE_Include.h"
/*****************************************************************************
*** Author: Shannon
*** Version: 2.0 2018.8.7
*** History: 1.0 ����
             2.0 ΪRTE������������
*****************************************************************************/
/*************************************************
*** Args:   *file  ����ʧ�ܵ��ļ�;
            line ����ʧ�ܵ���;
*** Function: ���Թ���
*************************************************/
void RTE_Assert(char *file, uint32_t line)
{ 
	printf("[ASSERT]    Wrong parameters value: file %s on line %d\n", file, line);
	while (1)
	{
		
	}
}
/*************************************************
*** RTE��������ڴ棬��̬���䣬32λ����
*************************************************/
#if RTE_USE_BGET == 1
#define ALIGN_32BYTES(buf) buf __attribute__ ((aligned (32)))
ALIGN_32BYTES (uint8_t RTE_RAM[BGET_MEMSIZE]) = {0};
#endif
/*************************************************
*** RTE_Shell�Ļص�����
*************************************************/
#if HI_USE_SHELL == 1
static void Shell_TimerCallBack(void *Params)
{
	RTE_Shell_Poll();
}
#endif
/*************************************************
*** Args:   NULL
*** Function: RTE��ʼ��
*************************************************/
void RTE_Init(void)
{
#if RTE_USE_BGET == 1
	RTE_BPool(MEM_RTE,RTE_RAM,BGET_MEMSIZE);
#endif
	
#if RTE_USE_HUMMANINTERFACE == 1
	#if HI_USE_SHELL == 1
		RTE_Shell_Init();
		RTE_RoundRobin_CreateTimer("ShellTimer",10,1,1,Shell_TimerCallBack,(void *)0);
	#endif
#endif
	
}
