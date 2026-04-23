#pragma once
#include "stdafx.h"
#include "defines.h"
#include "Detour.h"

struct CMsgPeerData
{
	uint8_t m_PeerID;
	char _padding1[3];
	int m_playerColor;
	int m_playerTeam;
	int _0x0C;
	uint64_t m_SessionID;
	int m_NatType;
	int _0x1C;
	int _0x20;
	bool _0x24;
	char _padding2[3];
};

struct CMsgReassignResponse
{
	uint8_t m_PeerID;
	bool m_GotNegotiateResponse;
	char _padding[2];
	int m_ResponseType;
};

extern Detour<void>* CNetworkPeerMgr_HandleGetReadyToStartPlaying_detour;
void CNetworkPeerMgr_HandleGetReadyToStartPlaying(CNetworkPeerMgr* netPeerMgr, netEvent* evt);

extern Detour<bool>* CNetworkPeerMgr_AddRemotePeer_detour;
bool CNetworkPeerMgr_AddRemotePeer(CNetworkPeerMgr* mgr, CPlayerInfo* info, void* endpoint, CMsgPeerData* data, int player_status);

void CMsgReassignConfirm_hook();
void CMsgReassignNegotiate_hook();
void CMsgReassignResponse_hook();

extern Detour<void>* netPeerComplainer_OnNetEvent_detour;
void netPeerComplainer_OnNetEvent(void* complainer, void* mgr, netEvent* evnt);