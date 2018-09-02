#include "RTE_Include.h"
/*****************************************************************************
*** Author: Shannon
*** Version: 2.0 2018.8.11
*** History: 1.0 ����
             2.0 ΪRTE�����������䣬����ģ������
*****************************************************************************/
#if RTE_USE_STATEMACHINE == 1
/*************************************************
*** Args:   
					thisStateMachine ������״̬��
					State ״̬���
					thisFunction ״̬����
*** Function: Ϊ״̬���Ĳ�ͬ״̬����״̬����
*************************************************/
void StateMachine_SetFuction(StateMachine_t *thisStateMachine,uint8_t State,StateFunction thisFunction)
{
	if(thisStateMachine!=(void *)0&&thisStateMachine->FunctionListTable!=(void *)0&&State<thisStateMachine->StateNum)
		thisStateMachine->FunctionListTable[State] = thisFunction;
}
/*************************************************
*** Args:   
					thisStateMachine ������״̬��
					InputArgs ״̬���������
*** Function: ����һ��״̬��
*************************************************/
void StateMachine_Run(StateMachine_t *thisStateMachine,void * InputArgs)
{
	thisStateMachine->RunningState = thisStateMachine->FunctionListTable[thisStateMachine->RunningState](InputArgs);
}
/*************************************************
*** Args:   
					thisStateMachine ������״̬��
*** Function: ɾ��һ��״̬��
*************************************************/
void StateMachine_Delete(StateMachine_t *thisStateMachine)
{
	thisStateMachine->StateNum = 0;
	thisStateMachine->FunctionListTable = (void *)0;
}
/*************************************************
*** Args:   
					thisStateMachine ������״̬��
					StateNum ״̬��Ŀ
*** Function: ��ʼ��һ��״̬��
*************************************************/
void StateMachine_Init(StateMachine_t *thisStateMachine,uint8_t StateNum,StateFunction *thisFuctionList)
{
	thisStateMachine->StateNum = StateNum;
	thisStateMachine->FunctionListTable = thisFuctionList;
}
#endif
