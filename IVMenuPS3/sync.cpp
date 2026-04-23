#include "stdafx.h"
#include "sync.h"
#include "defines.h"
#include "natives.h"

int& animation_task_count = *reinterpret_cast<int*>(0x15CF66C);
short& weapon_anim_task_count = *reinterpret_cast<short*>(0x15CF658);

opd_s CNetObjPhysical_SyncGameState_t = { 0x86B9F8, GTAIV_TOC };
int(*CNetObjPhysical_SyncGameState)(CNetworkObject* obj, SyncType type, CMessageBuffer* message) = (int(*)(CNetworkObject*, SyncType, CMessageBuffer*)) & CNetObjPhysical_SyncGameState_t;

Detour<int>* CNetObjPhysical_SyncAttach_detour;
int CNetObjPhysical_SyncAttach(CNetworkObject* obj, SyncType type, CMessageBuffer* message) //Only runs with non peds
{
	int seek_bits = message->GetPos();
	CNetworkObject* net_ped = CWorld::GetMainPlayerPed()->GetNetworkObject();

	if (net_ped && type == SYNC_READ && obj->IsGlobalFlagSet(CNetworkObject::NETOBJGLOBALFLAG_SCRIPTOBJECT))
	{
		if (message->PeekBool(seek_bits)) //Check if we have attachment
		{
			uint16_t net_id = message->PeekObjectId(seek_bits + 1); //Read network id for what obj is being attached to
			CNetworkObject* base = ms_objectMgr.GetNetworkObject(net_id);
			CNetworkPeer* owner = obj->GetPeerOwner();

			if (net_id == net_ped->GetNetworkID()) //if network id is our network id we are being attached to
			{
				if(owner->IsValid())
					_sys_printf("[Attachment] - %s tried to attach something to you!\n", owner->GetGamerInfo()->GetPlayerName());

				return CNetObjPhysical_SyncAttach_detour->CallOriginal(obj, SYNC_SKIP, message);
			}
			else if (base == obj) //Infinite Attach
			{
				if(owner->IsValid())
					_sys_printf("[Attachment] - %s is infinite attaching!\n", owner->GetGamerInfo()->GetPlayerName());

				return CNetObjPhysical_SyncAttach_detour->CallOriginal(obj, SYNC_SKIP, message);
			}
		}
	}

	return CNetObjPhysical_SyncAttach_detour->CallOriginal(obj, type, message);
}

Detour<int>* CNetObjPed_SyncMovementGroup_detour;
int CNetObjPed_SyncMovementGroup(CNetworkObject* obj, SyncType type, CMessageBuffer* message)
{
	int seek_bits = message->GetPos();

	if (type == SYNC_READ)
	{
		int anim_group_id = static_cast<int>(message->PeekInt(16, seek_bits)); //read animation group id
		
		if ((anim_group_id > animation_task_count || anim_group_id < 0) && anim_group_id != -1) //id is out side of the bounds
		{
			if(obj->GetPeerOwner()->IsValid())
				_sys_printf("[Ped Movement Group] - %s tried to set animation group ID out of bounds! [%i]\n", obj->GetPeerOwner()->GetGamerInfo()->GetPlayerName(), anim_group_id);

			return CNetObjPed_SyncMovementGroup_detour->CallOriginal(obj, SYNC_SKIP, message);
		}
	}

	return CNetObjPed_SyncMovementGroup_detour->CallOriginal(obj, type, message);
}

Detour<int>* CNetObjPed_SyncPedAI_detour;
int CNetObjPed_SyncPedAI(CNetworkObject* obj, SyncType type, CMessageBuffer* message)
{
	int seek_bits = message->GetPos();

	if (type == SYNC_READ && obj->IsGlobalFlagSet(CNetworkObject::NETOBJGLOBALFLAG_SCRIPTOBJECT))
	{
		int relationship_group = static_cast<int>(message->PeekInt(6, seek_bits));

		if (relationship_group > 55 || relationship_group < 0)
		{
			if (obj->GetPeerOwner()->IsValid())
				_sys_printf("[Ped AI Data] - %s tried to set relationship group out of bounds! [%i]\n", obj->GetPeerOwner()->GetGamerInfo()->GetPlayerName(), relationship_group);

			return CNetObjPed_SyncPedAI_detour->CallOriginal(obj, SYNC_SKIP, message);
		}
	}

	return CNetObjPed_SyncPedAI_detour->CallOriginal(obj, type, message);
}

Detour<int>* CNetObjPed_SyncGameState_detour;
int CNetObjPed_SyncGameState(CNetworkObject* obj, SyncType type, CMessageBuffer* message)
{
	//Call other Sync GameState
	int ret = CNetObjPhysical_SyncGameState(obj, type, message);
	*reinterpret_cast<uint32_t*>(0x865290) = 0x38600000 | ret; //li %r3, ret replace original call with return value of data

	int seek_bits = message->GetPos() + 1 + 2 + 7;

	if (type == SYNC_READ)
	{
		eWeaponType weapon_index = static_cast<eWeaponType>(message->PeekInt(6, seek_bits));

		if (weapon_index >= WEAPON_ANYWEAPON || weapon_index < WEAPON_UNARMED)
		{
			if (obj->GetPeerOwner()->IsValid())
				_sys_printf("[Ped Game State Data] - %s tried to set out of bounds weapon index! [%i]\n", obj->GetPeerOwner()->GetGamerInfo()->GetPlayerName(), weapon_index);

			return CNetObjPed_SyncGameState_detour->CallOriginal(obj, SYNC_SKIP, message);
		}

		if (obj->IsGlobalFlagSet(CNetworkObject::NETOBJGLOBALFLAG_SCRIPTOBJECT))
		{
			seek_bits += 6 + 1;
			seek_bits += weapon_index == WEAPON_OBJECT ? 12 : 1;
			seek_bits += message->PeekBool(seek_bits) ? 13 : 1;
			seek_bits += message->PeekBool(seek_bits) ? 16 : 1;
			seek_bits += 2 + 13 + 8;

			int animation_group = static_cast<int>(message->PeekInt(16, seek_bits));

			if (animation_group >= animation_task_count || animation_group < 0)
			{
				if (obj->GetPeerOwner()->IsValid())
					_sys_printf("[Ped Game State Data] - %s tried to set out of bounds default animation group index! [%i]\n", obj->GetPeerOwner()->GetGamerInfo()->GetPlayerName(), animation_group);

				return CNetObjPed_SyncGameState_detour->CallOriginal(obj, SYNC_SKIP, message);
			}
		}
	}

	return CNetObjPed_SyncGameState_detour->CallOriginal(obj, type, message);
}

Detour<int>* CNetObjPed_SyncAttach_detour;
int CNetObjPed_SyncAttach(CNetworkObject* obj, SyncType type, CMessageBuffer* message)
{
	int seek_bits = message->GetPos();
	CNetworkObject* net_ped = CWorld::GetMainPlayerPed()->GetNetworkObject();

	if (net_ped && type == SYNC_READ && obj->IsGlobalFlagSet(CNetworkObject::NETOBJGLOBALFLAG_SCRIPTOBJECT))
	{
		if (message->PeekBool(seek_bits)) //Check if ped is attaching to something
		{
			uint16_t net_id = message->PeekObjectId(seek_bits + 1); //Read network id for what obj is being attached to
			CNetworkObject* base = ms_objectMgr.GetNetworkObject(net_id);
			CNetworkPeer* owner = obj->GetPeerOwner();

			if (net_id == net_ped->GetNetworkID()) //if network id is our network id we are being attached to
			{
				if (owner->IsValid())
					_sys_printf("[Attachment] - %s tried to attach something to you!\n", owner->GetGamerInfo()->GetPlayerName());

				return CNetObjPed_SyncAttach_detour->CallOriginal(obj, SYNC_SKIP, message);
			}
			else if (base == obj) //Obj is trying to attach to itself
			{
				if (owner->IsValid())
					_sys_printf("[Attachment] - %s is infinite attaching!\n", owner->GetGamerInfo()->GetPlayerName());

				return CNetObjPhysical_SyncAttach_detour->CallOriginal(obj, SYNC_SKIP, message);
			}
		}
	}

	return CNetObjPed_SyncAttach_detour->CallOriginal(obj, type, message);
}

Detour<int>* CNetObjPlayer_SyncPedAppearance_detour;
int CNetObjPlayer_SyncPedAppearance(CNetworkObject* obj, SyncType type, CMessageBuffer* message, int unused)
{
	int seek_bits = message->GetPos();

	if (type == SYNC_READ)
	{
		uint32_t model_hash = message->PeekInt(32, seek_bits);
		if (!IS_THIS_MODEL_A_PED(model_hash) || model_hash == PED_PLAYER || model_hash == PED_SUPERLOD)
		{
			if (obj->GetPeerOwner()->IsValid())
				_sys_printf("[Player Apperance Data] - %s tried to set model to crash model! [0x%X]\n", obj->GetPeerOwner()->GetGamerInfo()->GetPlayerName(), model_hash);

			return CNetObjPlayer_SyncPedAppearance_detour->CallOriginal(obj, SYNC_SKIP, message, unused);
		}
	}

	return CNetObjPlayer_SyncPedAppearance_detour->CallOriginal(obj, type, message, unused);
}

Detour<int>* CDummyTask_SyncNetworkData_detour;
int CDummyTask_SyncNetworkData(void* _this, SyncType type, CMessageBuffer* message)
{
	int seek_bits = message->GetPos();

	if (type == SYNC_READ)
	{
		short anim_index = static_cast<short>(message->PeekInt(16, seek_bits));
		if (anim_index < 0 || anim_index > weapon_anim_task_count)
		{
			_sys_printf("[Dummy Task] - Invalid Animation Index!\n");
			return CDummyTask_SyncNetworkData_detour->CallOriginal(_this, SYNC_SKIP, message);
		}
	}

	return CDummyTask_SyncNetworkData_detour->CallOriginal(_this, type, message);
}