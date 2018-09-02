#include "RTE_Include.h"
/*****************************************************************************
*** Author: Shannon
*** Version: 2.1 2018.8.31
*** History: 1.0 �������޸���tivaware
             2.0 ΪRTE�����������䣬����ģ������
						 2.1 ����̬��Ϸ�ʽ����
*****************************************************************************/
#if HI_USE_SHELL == 1
/*************************************************
*** ����Shell�Ľṹ���������̬����
*************************************************/
static RTE_Shell_Control_t ShellHandle = 
{
	.CmdCnt = 0,
	.CmdBuffer = (void *)0,
	.g_psCmdTable = (void *)0,
};
/*************************************************
*** Args:   *pcCmdLine ������������
*** Function: Shell�����д���
*************************************************/
static RTE_Shell_Err_e RTE_Shell_CommandProcess(char *pcCmdLine)
{
	char *pcChar;
	uint_fast8_t ui8Argc;
	bool bFindArg = true;
	// Initialize the argument counter, and point to the beginning of the
	// command line string.
	ui8Argc = 0;
	pcChar = pcCmdLine;
	// Advance through the command line until a zero character is found.
	while(*pcChar)
	{
		// If there is a space, then replace it with a zero, and set the flag
		// to search for the next argument.
		if(*pcChar == ' ')
		{
			*pcChar = 0;
			bFindArg = true;
		}
		// Otherwise it is not a space, so it must be a character that is part
		// of an argument.
		else
		{
			// If bFindArg is set, then that means we are looking for the start
			// of the next argument.
			if(bFindArg)
			{
				// As long as the maximum number of arguments has not been
				// reached, then save the pointer to the start of this new arg
				// in the argv array, and increment the count of args, argc.
				if(ui8Argc < HI_SHELL_MAX_ARGS)
				{
						ShellHandle.g_ppcArgv[ui8Argc] = pcChar;
						ui8Argc++;
						bFindArg = false;
				}
				// The maximum number of arguments has been reached so return
				// the error.
				else
				{
						return(SHELL_TOOMANYARGS);
				}
			}
		}
		// Advance to the next character in the command line.
		pcChar++;
	}
	// If one or more arguments was found, then process the command.
	if(ui8Argc)
	{
		for(uint8_t i = 0;i<ShellHandle.CmdCnt;i++)
		{
			// If this command entry command string matches argv[0], then call
			// the function for this command, passing the command line
			// arguments.
			if(!strcmp(ShellHandle.g_ppcArgv[0], ShellHandle.g_psCmdTable[i].pcCmd))
			{
				RTE_Shell_Err_e retval;
				if(ShellHandle.g_psCmdTable[i].pfnCmdLine !=(void *)0)
				{
					retval = (RTE_Shell_Err_e)ShellHandle.g_psCmdTable[i].pfnCmdLine(ui8Argc, ShellHandle.g_ppcArgv);
				}
				return retval;
			}
		}
	}
	return(SHELL_NOVALIDCMD);
}
/*************************************************
*** Args:   *cmd �������ַ���
            *func ������ִ�к���
            *help �����ַ���
*** Function: ����һ��Shell�����д���
*************************************************/
RTE_Shell_Err_e RTE_Shell_AddCommand(const char *cmd,RTE_Shell_Err_e (*func)(int argc, char *argv[]),const char *help)
{
	if(ShellHandle.g_psCmdTable == (void*)0)
		return SHELL_UNINITHANDLE;
	if(ShellHandle.CmdCnt >= HI_SHELL_MAX_NUM)
		return SHELL_NOSPACEFORNEW;
	for(uint8_t i = 0;i<ShellHandle.CmdCnt;i++)
	{
		if(!strcmp(cmd,ShellHandle.g_psCmdTable[i].pcCmd))
			return SHELL_ALREADYEXIST;
	}
	ShellHandle.g_psCmdTable[ShellHandle.CmdCnt].pcCmd = cmd;
	ShellHandle.g_psCmdTable[ShellHandle.CmdCnt].pfnCmdLine = func;
	ShellHandle.g_psCmdTable[ShellHandle.CmdCnt].pcHelp = help;
	ShellHandle.CmdCnt++;
	return SHELL_NOERR;
}
/*************************************************
*** Args:   *pcCmdLine ������������
*** Function: ɾ��һ��Shell�����д���
*************************************************/
RTE_Shell_Err_e RTE_Shell_DeleteCommand(const char *cmd)
{
	int8_t idx = -1;
	for(uint8_t i = 0;i<ShellHandle.CmdCnt;i++)
	{
		if(!strcmp(cmd,ShellHandle.g_psCmdTable[i].pcCmd))
		{
			idx = i;
			break;
		}
	}
	if(idx!=-1)
	{
		for(uint8_t i = idx;i<ShellHandle.CmdCnt-1;i++)
		{
			ShellHandle.g_psCmdTable[i].pcCmd = 
				ShellHandle.g_psCmdTable[i+1].pcCmd;
			ShellHandle.g_psCmdTable[i].pfnCmdLine = 
				ShellHandle.g_psCmdTable[i+1].pfnCmdLine;
			ShellHandle.g_psCmdTable[i].pcHelp = 
				ShellHandle.g_psCmdTable[i+1].pcHelp;
		}
		ShellHandle.g_psCmdTable[ShellHandle.CmdCnt].pcCmd = (void *)0;
		ShellHandle.g_psCmdTable[ShellHandle.CmdCnt].pfnCmdLine = (void *)0;
		ShellHandle.g_psCmdTable[ShellHandle.CmdCnt].pcHelp = (void *)0;
		ShellHandle.CmdCnt--;
		return SHELL_NOERR;
	}
	return SHELL_NOSUCHCMD;
}
/*************************************************
*** Args:   NULL
*** Function: shell�Դ��İ�������
*************************************************/
static RTE_Shell_Err_e RTE_Shell_CMD_Help(int argc, char *argv[])
{
	RTE_Printf("*********************************\r\n");
	RTE_Printf("[SHELL]    ����ָ��\r\n");
	RTE_Printf("[SHELL]    ------------------\r\n");
	for(uint8_t i = 0;i<ShellHandle.CmdCnt;i++)
	{
		RTE_Printf("[SHELL]    ComandName:%16s----����:%s\r\n", ShellHandle.g_psCmdTable[i].pcCmd
					,ShellHandle.g_psCmdTable[i].pcHelp);
	}
	return(SHELL_NOERR);
}
#if RTE_USE_BGET == 1
/*************************************************
*** Args:   NULL
*** Function: shell�Դ�����Ϣ����
*************************************************/
static RTE_Shell_Err_e RTE_Shell_CMD_RTEInfor(int argc, char *argv[])
{
	RTE_BGet_Monitor_t mon_infor = {0};
	RET_BGet_Monitor(MEM_RTE,&mon_infor);
	RTE_Printf("*********************************\r\n");
	RTE_Printf("[SHELL]    RTE�汾�ţ�%s\r\n",RTE_VERSION);
	RTE_Printf("[SHELL]    ϵͳ��ϸ��Ϣ\r\n");
	RTE_Printf("[SHELL]    ------------------\r\n");
	RTE_Printf("[MEM]    ��ǰ�ڴ�ʹ����� ����/ȫ��:%d%%/%d ������/����:%d/%d ��Ƭ����:%d%%\r\n",
			mon_infor.pct_used,
			mon_infor.size_total,
	    mon_infor.size_free,
			mon_infor.size_free_big,
			mon_infor.pct_frag);
	RTE_Printf("[SHELL]    ------------------\r\n");
	RTE_Printf("[SHELL]    ��ǰSHELLָ��ʹ����Ŀ��%d �����Ŀ��%d\r\n",ShellHandle.CmdCnt,HI_SHELL_MAX_NUM);
	RTE_Printf("[SHELL]    ------------------\r\n");
	RTE_RoundRobin_Demon();
	return(SHELL_NOERR);
}
#endif
/*************************************************
*** Args:   NULL
*** Function: ��ʼ��shell����
*************************************************/
void RTE_Shell_Init(void)
{
	ShellHandle.CmdBuffer = (char *)RTE_BGetz(MEM_RTE,HI_SHELL_MAX_BUFSIZE);
	RTE_AssertParam(ShellHandle.CmdBuffer);
	ShellHandle.g_psCmdTable = (RTE_Shell_t *)RTE_BGetz(MEM_RTE,(HI_SHELL_MAX_NUM + 1) * sizeof(RTE_Shell_t));
	ShellHandle.g_psCmdTable[HI_SHELL_MAX_NUM].pcCmd = (void *)0;
	ShellHandle.g_psCmdTable[HI_SHELL_MAX_NUM].pfnCmdLine = (void *)0;
	ShellHandle.g_psCmdTable[HI_SHELL_MAX_NUM].pcHelp = (void *)0;
	RTE_AssertParam(ShellHandle.g_psCmdTable);
	RTE_AssertParam(RTE_Shell_AddCommand("����",RTE_Shell_CMD_Help,"SHELLʹ�ð���") == SHELL_NOERR);
	RTE_AssertParam(RTE_Shell_AddCommand("ϵͳ��Ϣ",RTE_Shell_CMD_RTEInfor,"��ʾRTEϵͳ��Ϣ") == SHELL_NOERR);
}
/*************************************************
*** Args:   NULL
*** Function: ��ʼ��shell����
*************************************************/
#include "BSP_Com.h"
void RTE_Shell_Poll(void)
{
	int iStatus;
	uint16_t lenth;
	BSP_COM_Data_t *debugdata;
	debugdata = BSP_COM_ReturnQue(COM_1);
	if(RTE_MessageQuene_Out(&debugdata->ComQuene,(uint8_t *)ShellHandle.CmdBuffer,&lenth)!=MSG_NO_ERR)
	{
		return;
	}
	iStatus = RTE_Shell_CommandProcess(ShellHandle.CmdBuffer);
	if(iStatus == SHELL_NOVALIDCMD)
	{
		RTE_Printf("[SHELL]    ����ʶ���CMD!\r\n");
	}
	else if(iStatus == SHELL_TOOMANYARGS)
	{
		RTE_Printf("[SHELL]    ���������Ŀ����!\r\n");
	}
	memset(ShellHandle.CmdBuffer,0,HI_SHELL_MAX_BUFSIZE);
}
#endif
