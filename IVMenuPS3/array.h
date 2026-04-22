#pragma once
#include "stdafx.h"
#include "Detour.h"
#include "defines.h"

enum NetworkGameArrayTypes : int
{
	PLAYER_INFO_ARRAY_HANDLER,
	STATIC_PICKUPS_ARRAY_HANDLER,
	DYNAMIC_PICKUPS_ARRAY_HANDLER,
	HOST_VARIABLES_ARRAY_HANDLER,
	CLIENT_VARIABLES_ARRAY_HANDLER,
	PED_GROUPS_ARRAY_HANDLER,
	DISPATCH_ORDER_ARRAY_HANDLER,
	ARRAY_HANDLER_TYPE_COUNT
};

class networkArrayHandler
{
public:
	int _0x04;
	int _0x08;
	NetworkGameArrayTypes m_Type; //0x0C
	int m_Identifier; //Game ID for REGISTER_CLIENT_BROADCAST_VARS
	int m_ArraySize; //0x14

	networkArrayHandler()
	{

	}

	virtual ~networkArrayHandler();
	virtual void call_0x04();
	virtual void call_0x08();
	virtual void call_0x0C();
	virtual void call_0x10();
	virtual void call_0x14();
	virtual void call_0x18();
	virtual void call_0x1C();
	virtual const char* GetHandlerName(); //0x20
	virtual int GetSizeOfElementIndex(); //0x24
	virtual void call_0x28();
	virtual int GetElementIndex(int index, int peer); //0x2C
	virtual void call_0x30();
	virtual void call_0x34();
	virtual void call_0x38();
	virtual bool CanApplyElementData(int index, int peer, bool empty); //0x3C
	virtual void call_0x40();
	virtual void call_0x44();
	virtual void call_0x48();
	virtual void call_0x4C();
	virtual bool WriteUpdate(CMessage* buffer, int index); //0x50
	virtual bool ReadUpdate(CMessage* buffer, int index); //0x54
	virtual void call_0x58();
	virtual void SkipUpdate(CMessage* buffer, int index); //0x5C
	virtual void LogElementData(bool r4, int index); //0x60
};

class CScriptVariablesArrayHandler : public networkArrayHandler
{
public:
	char _0x18[0x20070];
	int* m_Variables; //0x20088
	int m_VariableLength; //0x20090 - hard set to 4
};

int CNetworkArrayHandler_GetElementIndex(networkArrayHandler* handler, int index, int peer);

extern Detour<bool>* CNetworkArrayHandler_CanApplyElementData_detour;
bool CNetworkArrayHandler_CanApplyElementData(networkArrayHandler* handler, int index, int peer, bool empty);

extern Detour<bool>* CPedGroupsArrayHandler_CanApplyElementData_detour;
bool CPedGroupsArrayHandler_CanApplyElementData(networkArrayHandler* handler, int index, int peer, bool empty);

extern Detour<bool>* CPedGroupsArrayHandler_ReadUpdate_detour;
bool CPedGroupsArrayHandler_ReadUpdate(networkArrayHandler* handler, CMessage* message, int index);

extern Detour<bool>* CScriptClientVariablesArrayHandler_ReadUpdate_detour;
bool CScriptClientVariablesArrayHandler_ReadUpdate(CScriptVariablesArrayHandler* handler, CMessage* message, int index);