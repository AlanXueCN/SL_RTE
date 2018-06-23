#include "APP_F7.h"
#include "stm32f7xx.h"
//�������ģʽ	  
void APP_Standby(void)
{ 	
	__WFI();			    //ִ��WFIָ��,�������ģʽ		 
}	     
//ϵͳ��λ   
void APP_SoftReset(void)
{   
	SCB->AIRCR =0X05FA0000|(uint32_t)0x04;	  
} 	
//���������ж�
uint32_t APP_IntEnable(void)
{
	uint32_t ui32Ret;
	// Read PRIMASK and enable interrupts.
	__asm("    mrs     r0, PRIMASK\n"
				"    cpsie   i\n"
				"    bx      lr\n"
				: "=r" (ui32Ret));
	return(ui32Ret);
}
uint32_t APP_IntDisable(void)
{
	uint32_t ui32Ret;
	// Read PRIMASK and disable interrupts.
	__asm("    mrs     r0, PRIMASK\n"
				"    cpsid   i\n"
				"    bx      lr\n"
				: "=r" (ui32Ret));
	return(ui32Ret);
}
void APP_Assert(char *file, uint32_t line)
{ 
	APP_STDPrintf("[ASSERT]Wrong parameters value: file %s on line %d\n", (char *)file, line);
	/* ����һ����ѭ��������ʧ��ʱ������ڴ˴��������Ա����û���� */
	while (1)
	{
	}
}
/* Callback function for system */
static void SysTimer_Callback(void* UserParameters) {
#if USE_DEBUG == 1
	APP_Debug_Poll();
#endif
}
void APP_Init(void)
{
#if USE_MPU == 1
	MPU_Memory_Protection();
#endif
	APP_SoftTimer_Init();
	APP_SoftTimer_Create(TIMER_SYS,10, 1, 0, SysTimer_Callback, (void *)0);
	APP_MemInit();
	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); 
#if USE_DEBUG == 1
	APP_Debug_Init();
#endif
	HAL_Init();
}