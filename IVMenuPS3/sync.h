#pragma once
#include "Detour.h"
#include "defines.h"

extern Detour<int>* CNetObjPhysical_SyncAttach_detour;
int CNetObjPhysical_SyncAttach(CNetworkObject* obj, SyncType type, CMessageBuffer* message);

extern Detour<int>* CNetObjPed_SyncMovementGroup_detour;
int CNetObjPed_SyncMovementGroup(CNetworkObject* obj, SyncType type, CMessageBuffer* message);

extern Detour<int>* CNetObjPed_SyncPedAI_detour;
int CNetObjPed_SyncPedAI(CNetworkObject* obj, SyncType type, CMessageBuffer* message);

extern Detour<int>* CNetObjPed_SyncGameState_detour;
int CNetObjPed_SyncGameState(CNetworkObject* obj, SyncType type, CMessageBuffer* message);

extern Detour<int>* CNetObjPed_SyncAttach_detour;
int CNetObjPed_SyncAttach(CNetworkObject* obj, SyncType type, CMessageBuffer* message);

extern Detour<int>* CNetObjPlayer_SyncPedAppearance_detour;
int CNetObjPlayer_SyncPedAppearance(CNetworkObject* obj, SyncType type, CMessageBuffer* message, int unused);

extern Detour<int>* CDummyTask_SyncNetworkData_detour;
int CDummyTask_SyncNetworkData(void* _this, SyncType type, CMessageBuffer* message);