#include "stdafx.h"
#include "defines.h"
#include "array.h"
#include "Detour.h"
#include "helpers.h"
#include "natives.h"

bool should_skip_array[ARRAY_HANDLER_TYPE_COUNT];

int CNetworkArrayHandler_GetElementIndex(networkArrayHandler* handler, int index, int peer)
{
	should_skip_array[handler->m_Type] = false; //Lets clear out our check
	if (index >= handler->m_ArraySize || index < 0) //index is out side of our array
	{
		_sys_printf("[Array] - Invalid index %i would exceed bounds of %s array!\n", index, handler->GetHandlerName());
		should_skip_array[handler->m_Type] = true; //set our array to be skipped
		return 0; //return first index so we are in our bounds (skipping data anyways so it doesn't matter)
	}

	//Lets do the original code, simple function so no need to call original
	if(handler->m_Type == DISPATCH_ORDER_ARRAY_HANDLER || handler->m_Type == CLIENT_VARIABLES_ARRAY_HANDLER)
		return ((handler->m_ArraySize >> 4) * peer) + index;

	return index;
}

Detour<bool>* CNetworkArrayHandler_DoesPeerHaveAuthorityOverThisElement_detour;
bool CNetworkArrayHandler_DoesPeerHaveAuthorityOverThisElement(networkArrayHandler* handler, int index, int peer, bool empty)
{
	if(index == 0 && should_skip_array[handler->m_Type]) //We found a message to skip, lets say we can't apply the data
		return false;

	return CNetworkArrayHandler_DoesPeerHaveAuthorityOverThisElement_detour->CallOriginal(handler, index, peer, empty); //Was safe so lets continue
}

Detour<bool>* CPedGroupsArrayHandler_DoesPeerHaveAuthorityOverThisElement_detour;
bool CPedGroupsArrayHandler_DoesPeerHaveAuthorityOverThisElement(networkArrayHandler* handler, int index, int peer, bool empty)
{
	if(index == 0 && should_skip_array[handler->m_Type]) //We found a message to skip, lets say we can't apply the data
		return false;

	return CPedGroupsArrayHandler_DoesPeerHaveAuthorityOverThisElement_detour->CallOriginal(handler, index, peer, empty); //Was safe so lets continue
}

Detour<bool>* CPedGroupsArrayHandler_ReadElement_detour;
bool CPedGroupsArrayHandler_ReadElement(networkArrayHandler* handler, CMessageBuffer* message, int index)
{
	if (index >= 16 || (ms_PeerMgr.GetMyPeer()->GetPeerID() != index && CNetwork::IsGameInProgress()))
	{
		CPed* ourped = CWorld::GetMainPlayerPed();
		if (ourped && ourped->GetNetworkObject())
		{
			//skip reading created by and seperation value (not important)
			int seek_bits = message->GetPos() + 2 + 7;
			for (int i = 0; i < 8; i++)
			{
				uint16_t netid = message->PeekObjectId(seek_bits);
				if (netid == ourped->GetNetworkObject()->GetNetworkID()) //Network ID is the same as ours (means we are being added to a group)
				{
					if(index < 16 && ms_PeerMgr.GetPeerFromPeerId(index)->IsValid())
						_sys_printf("[Ped Group Array] - Removing ourself from %s's group...\n", CWorld::GetPlayerInfo(index)->GetPlayerName());
					else
						_sys_printf("[Ped Group Array] - Removing ourself from %i group...\n", index);

					handler->SkipElement(message, index); //Tell game to skip this update since its a group hack against us
					return false;
				}

				seek_bits += 12; //Jump to next network id in the message
			}

			return CPedGroupsArrayHandler_ReadElement_detour->CallOriginal(handler, message, index);

		}
	}
	
	//Normally wouldn't run under these conditions so lets not either
	handler->SkipElement(message, index);
	return false;
}

struct freemode_client_vars_original
{
	int m_status;
	int _0x04;
	int _0x08;
	int m_readyUpStatus;
	int m_gameModeSuggestion;
	int m_voteToKickIndex;
};

struct freemode_client_vars_tlad //didn't verify these just assuming they are the same as xbox
{
	int _0x00;
	int _0x04;
	int _0x08;
	int m_voteToKickIndex;
	int m_gameModeSuggestion;
	int _0x14;
	int _0x18;
};

struct freemode_client_vars_tbogt //didn't verify these just assuming they are the same as xbox
{
	int _0x00;
	int _0x04;
	int _0x08;
	int m_voteToKickIndex;
	int m_gameModeSuggestion;
	int _0x14;
	int _0x18;
	int _0x1C;
	int _0x20;
	int _0x24;
	int _0x28;
	int _0x2C;
	int _0x30;
	int _0x34;
	int _0x38;
	int _0x3C;
	int _0x40;
	int _0x44;
	int _0x48;
	int _0x4C;
	int _0x50;
	int _0x54;
	int _0x58;
	int _0x5C;
	int _0x60;
	int _0x64;
	int _0x68;
	int _0x6C;
	int _0x70;
	int _0x74;
	int _0x78;
	int _0x7C;
};

Detour<bool>* CScriptClientVariablesArrayHandler_ReadElement_detour;
bool CScriptClientVariablesArrayHandler_ReadElement(CScriptVariablesArrayHandler* handler, CMessageBuffer* message, int index)
{
	CScriptClientVariablesArrayHandler_ReadElement_detour->CallOriginal(handler, message, index);

	if (handler->m_Identifier == GAME_MODE_FREE_MODE)
	{
		EPISODES episode = static_cast<EPISODES>(GET_CURRENT_EPISODE());

		if (episode == EPISODE_ORIGINAL)
		{
			freemode_client_vars_original* vars = reinterpret_cast<freemode_client_vars_original*>(handler->m_Variables);
			int player_index = index / 6; //Get player index and array index for the specific client for easy modifying
			int array_index = index % 6;

			if (array_index == 4 && (vars[player_index].m_gameModeSuggestion > GAME_MODE_FREE_MODE || vars[player_index].m_gameModeSuggestion < GAME_MODE_SINGLE_PLAYER)) //Value is going to be outside of our array in freemode_cr
			{
				if (ms_PeerMgr.GetPeerFromPeerId(player_index)->IsValid())
					_sys_printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", CWorld::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_gameModeSuggestion = GAME_MODE_SINGLE_PLAYER; //Setting vote to singleplayer which game script just ignores
			}
			else if (array_index == 5 && (vars[player_index].m_voteToKickIndex > 15 || vars[player_index].m_voteToKickIndex < -1)) //Vote to kick also has the same bug
			{
				if (ms_PeerMgr.GetPeerFromPeerId(player_index)->IsValid())
					_sys_printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", CWorld::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_voteToKickIndex = -1; //Setting vote to -1 which game script ignores
			}
		}
		else if (episode == EPISODE_TLAD)
		{
			freemode_client_vars_tlad* vars = reinterpret_cast<freemode_client_vars_tlad*>(handler->m_Variables);
			int player_index = index / 7;
			int array_index = index % 7;

			if (array_index == 3 && (vars[player_index].m_voteToKickIndex > 24 || vars[player_index].m_voteToKickIndex < -1))
			{
				if (ms_PeerMgr.GetPeerFromPeerId(player_index)->IsValid())
					_sys_printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", CWorld::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_voteToKickIndex = -1;
			}
			else if (array_index == 4 && (vars[player_index].m_gameModeSuggestion > GAME_MODE_FREE_MODE || vars[player_index].m_gameModeSuggestion < GAME_MODE_SINGLE_PLAYER))
			{
				if (ms_PeerMgr.GetPeerFromPeerId(player_index)->IsValid())
					_sys_printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", CWorld::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_gameModeSuggestion = GAME_MODE_SINGLE_PLAYER;
			}
		}
		else if (episode == EPISODE_TBOGT)
		{
			freemode_client_vars_tbogt* vars = reinterpret_cast<freemode_client_vars_tbogt*>(handler->m_Variables);
			int player_index = index / 32;
			int array_index = index % 32;

			if (array_index == 3 && (vars[player_index].m_voteToKickIndex > 15 || vars[player_index].m_voteToKickIndex < -1))
			{
				if (ms_PeerMgr.GetPeerFromPeerId(player_index)->IsValid())
					_sys_printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", CWorld::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_voteToKickIndex = -1;
			}
			else if (array_index == 4 && (vars[player_index].m_gameModeSuggestion > 30 || vars[player_index].m_gameModeSuggestion < GAME_MODE_SINGLE_PLAYER))
			{
				if (ms_PeerMgr.GetPeerFromPeerId(player_index)->IsValid())
					_sys_printf("[Client Broadcast Vars] - %s tried to send you to singleplayer!\n", CWorld::GetPlayerInfo(player_index)->GetPlayerName());

				vars[player_index].m_gameModeSuggestion = GAME_MODE_SINGLE_PLAYER;
			}
		}
	}

	return true; //Game returns true it doesn't have a fail condition anyways
}