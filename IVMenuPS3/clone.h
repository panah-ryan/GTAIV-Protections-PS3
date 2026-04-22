#pragma once
#include "stdafx.h"
#include "Detour.h"
#include "defines.h"

void setup_blacklists();

extern Detour<void>* CNetworkObjectMgr_ProcessCloneCreateData_detour;
void CNetworkObjectMgr_ProcessCloneCreateData(void* netObjMgr, uint8_t peer, NetworkObjectType objectType, short objectID, uint8_t objectFlags, CMessage* message);

extern Detour<bool>* CNetObjHeli_SerializeCloneData_detour;
bool CNetObjHeli_SerializeCloneData(CNetworkObject* net_heli, CMessage* message);