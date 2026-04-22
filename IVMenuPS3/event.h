#pragma once
#include "stdafx.h"
#include "Detour.h"
#include "defines.h"

extern Detour<void>* CNetworkEventMgr_HandleEvent_detour;
void CNetworkEventMgr_HandleEvent(void* manager, CNetworkEvent* pEvent, CMessageBuffer* message, int peer, short messageSeq, int eventId);