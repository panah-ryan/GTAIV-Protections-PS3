#pragma once
#include "stdafx.h"
#include "Detour.h"
#include "defines.h"

void setup_blacklists();

extern Detour<void>* CNetworkObjectMgr_ProcessCloneCreateData_detour;
void CNetworkObjectMgr_ProcessCloneCreateData(void* netObjMgr, uint8_t peer, eNetworkObjectType objectType, short objectID, uint8_t objectFlags, CMessageBuffer* message);

extern Detour<bool>* CNetObjHeli_CreateClone_detour;
bool CNetObjHeli_CreateClone(CNetworkObject* net_heli, CMessageBuffer* message);