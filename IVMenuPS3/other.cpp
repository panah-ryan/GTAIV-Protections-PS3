#include "stdafx.h"
#include "defines.h"
#include "other.h"

Detour<void>* CNetworkPeerMgr_HandleGetReadyToStartPlaying_detour;
void CNetworkPeerMgr_HandleGetReadyToStartPlaying(CNetworkPeerMgr* netPeerMgr, netEvent* evt)
{
    if(CNetwork::IsGameInProgress()) //Game is already in progress
    {
        CNetworkPeer* peer = netPeerMgr->GetPeerFromConnectionId(evt->m_CxnId);
        if(peer->IsValid())
            _sys_printf("[Get Ready to Start Playing] - %s tried to send GetReadyToStartPlaying message when we are already playing!\n", 
                peer->GetGamerInfo()->GetPlayerName());

        return;
    }

    CNetworkPeerMgr_HandleGetReadyToStartPlaying_detour->CallOriginal(netPeerMgr, evt);
}

Detour<bool>* CNetworkPeerMgr_AddRemotePeer_detour;
bool CNetworkPeerMgr_AddRemotePeer(CNetworkPeerMgr* mgr, CPlayerInfo* info, void* endpoint, CMsgPeerData* data, int player_status)
{
    char* gamertag = info->m_Gamertag;

    if (data->m_PeerID > 15)
    {
        _sys_printf("[Peer Data] - Host sent us an invalid peer id. Fixing to prevent crash but we should leave game to be safe.\n");
        data->m_PeerID = CWorld::FindSlotForNewPlayer();
    }

    for (int i = 0; i < _sys_strlen(gamertag); i++)
    {
        if (gamertag[i] == '~')
            gamertag[i] = '-';
    }

    return CNetworkPeerMgr_AddRemotePeer_detour->CallOriginal(mgr, info, data, player_status);
}

int CMsgReassign_Import(uint8_t* peer, datBitBuffer* buffer, int packet_size)
{
    if (*peer > 15) //Invalid peer id
    {
        _sys_printf("[Reassign] - Got invalid peer id for reassignment.\n");
        return packet_size + 1; //makes it so we don't return same value so message bails
    }

    return buffer->GetNumBytesRead();
}

void __attribute__((naked)) CMsgReassignConfirm_hook()
{
    //Load what we need
    __asm("mr %r5, %r24");
    __asm("mr %r4, %r20");
    __asm("addi %r3, %r1, 0x84");
    __asm("bl ._Z19CMsgReassign_ImportPhP12datBitBufferi");
    
    //Do Original code we overwrote
    __asm("nop");
    __asm("add %r3, %r28, %r3");

    //Jump Back
    __asm("lis %r0, 0x0084");
    __asm("ori %r0, %r0, 0xBA28");
    __asm("mtctr %r0");
    __asm("bctr");
}

void __attribute__((naked)) CMsgReassignNegotiate_hook()
{
    //Load what we need
    __asm("mr %r5, %r24");
    __asm("mr %r4, %r22");
    __asm("addi %r3, %r1, 0x4D8");
    __asm("bl ._Z19CMsgReassign_ImportPhP12datBitBufferi");

    //Do Original code we overwrote
    __asm("nop");
    __asm("add %r3, %r28, %r3");

    //Jump Back
    __asm("lis %r0, 0x0084");
    __asm("ori %r0, %r0, 0xB630");
    __asm("mtctr %r0");
    __asm("bctr");
}

void CMsgReassignResponse_Import(CMsgReassignResponse* resp)
{
    if (resp->m_PeerID > 15)
    {
        _sys_printf("[Reassign Response] - Got invalid peer id for reassignment.\n");
        resp->m_PeerID = 0; //set valid peer id
        resp->m_ResponseType = 5; //set invalid response type
    }
}

void __attribute__((naked)) CMsgReassignResponse_hook()
{
    //Load what we need
    __asm("addi %r3, %r1, 0x74");
    __asm("bl ._Z27CMsgReassignResponse_ImportP20CMsgReassignResponse");

    //Don't need to do original code, its nullsub stuff

    //Jump Back
    __asm("lis %r5, 0x0084");
    __asm("ori %r5, %r5, 0xC69C");
    __asm("mtctr %r5");
    __asm("bctr");
}

Detour<void>* netPeerComplainer_OnNetEvent_detour;
void netPeerComplainer_OnNetEvent(void* complainer, void* mgr, netEvent* evnt)
{
    return; //Don't even run this event, it prevents offhost kicking players and RCE when host
}