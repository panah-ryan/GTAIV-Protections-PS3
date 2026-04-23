#include "stdafx.h"
#include "helpers.h"
#include "Detour.h"
#include "caller.h"
#include "defines.h"
#include "array.h"
#include "clone.h"
#include "event.h"
#include "other.h"
#include "sync.h"

SYS_MODULE_INFO(PRXTemplate, 0, 1, 1);
SYS_MODULE_START(PRX_ENTRY);
SYS_MODULE_STOP(PRX_EXIT);

uint32_t TOC;
BOOL term_process;
BOOL constant_loop = TRUE;
BOOL booted_game;
BOOL shutdown_title_hooks;

Caller* call = nullptr;

Detour<void>* CWeaponInfo_LoadWeaponXMLFile_detour;
void CWeaponInfo_LoadWeaponXMLFile(const char* weaponxml)
{
	CWeaponInfo_LoadWeaponXMLFile_detour->CallOriginal(weaponxml);

	uint32_t flags = 1 << CAN_AIM | 1 << CAN_FREE_AIM | 1 << ANIM_RELOAD | 1 << ANIM_CROUCH_FIRE; //0xC005
	int anim_id = CAnimAssociations::GetInstance()->GetGroupId("grenade"); //29

	CWeaponInfo* rocket = CWeaponInfo::GetWeaponInfo(WEAPON_ROCKET);
	if (rocket)
	{
		rocket->m_AimFlags |= flags;
		rocket->m_AnimationIndex = anim_id;
	}

	CWeaponInfo* e2_rocket = CWeaponInfo::GetWeaponInfo(WEAPON_EPISODIC_18);
	if (e2_rocket)
	{
		e2_rocket->m_AimFlags |= flags;
		e2_rocket->m_AnimationIndex = anim_id;
	}
}

BOOL Init()
{
	call = new Caller();

	CWeaponInfo_LoadWeaponXMLFile_detour = new Detour<void>;

	CNetworkArrayHandler_DoesPeerHaveAuthorityOverThisElement_detour = new Detour<bool>;
	CPedGroupsArrayHandler_DoesPeerHaveAuthorityOverThisElement_detour = new Detour<bool>;
	CPedGroupsArrayHandler_ReadElement_detour = new Detour<bool>;
	CScriptClientVariablesArrayHandler_ReadElement_detour = new Detour<bool>;

	CNetworkObjectMgr_ProcessCloneCreateData_detour = new Detour<void>;
	CNetObjHeli_CreateClone_detour = new Detour<bool>;

	CNetworkEventMgr_HandleEvent_detour = new Detour<void>;

	CNetworkPeerMgr_HandleGetReadyToStartPlaying_detour = new Detour<void>;
	CNetworkPeerMgr_AddRemotePeer_detour = new Detour<bool>;
	netPeerComplainer_OnNetEvent_detour = new Detour<void>;

	CNetObjPhysical_SyncAttach_detour = new Detour<int>;
	CNetObjPed_SyncMovementGroup_detour = new Detour<int>;
	CNetObjPed_SyncPedAI_detour = new Detour<int>;
	CNetObjPed_SyncGameState_detour = new Detour<int>;
	CNetObjPed_SyncAttach_detour = new Detour<int>;
	CNetObjPlayer_SyncPedAppearance_detour = new Detour<int>;
	CDummyTask_SyncNetworkData_detour = new Detour<int>;

	setup_blacklists();

	*reinterpret_cast<uint32_t*>(0x480788) = 0x4E800020; //File Bypass
	*reinterpret_cast<uint32_t*>(0xD0DE9C) = 0x60000000; //NOP a specific packet type not used on XBOX

	//Array Protections
	//Function is to small to hook so lets just overwrite the virtual call table
	*reinterpret_cast<void**>(0xEECD04) = reinterpret_cast<void*>(CNetworkArrayHandler_GetElementIndex); //CPlayerInfo
	*reinterpret_cast<void**>(0xEECC84) = reinterpret_cast<void*>(CNetworkArrayHandler_GetElementIndex); //CStaticPickups
	*reinterpret_cast<void**>(0xEECC04) = reinterpret_cast<void*>(CNetworkArrayHandler_GetElementIndex); //CDynamicPickups
	*reinterpret_cast<void**>(0xEECB84) = reinterpret_cast<void*>(CNetworkArrayHandler_GetElementIndex); //CDataBlock
	*reinterpret_cast<void**>(0xEECB04) = reinterpret_cast<void*>(CNetworkArrayHandler_GetElementIndex); //CScriptHostVariables
	*reinterpret_cast<void**>(0xEECA84) = reinterpret_cast<void*>(CNetworkArrayHandler_GetElementIndex); //CScriptClientVariables
	*reinterpret_cast<void**>(0xEECA04) = reinterpret_cast<void*>(CNetworkArrayHandler_GetElementIndex); //CPedGroups
	*reinterpret_cast<void**>(0xED492C) = reinterpret_cast<void*>(CNetworkArrayHandler_GetElementIndex); //CDispatch

	//File protections
	CWeaponInfo_LoadWeaponXMLFile_detour->SetupDetour(0x66DA38, reinterpret_cast<void*>(CWeaponInfo_LoadWeaponXMLFile));

	//Array protections
	CNetworkArrayHandler_DoesPeerHaveAuthorityOverThisElement_detour->SetupDetour(0x81C328, reinterpret_cast<void*>(CNetworkArrayHandler_DoesPeerHaveAuthorityOverThisElement));
	CPedGroupsArrayHandler_DoesPeerHaveAuthorityOverThisElement_detour->SetupDetour(0x821BF8, reinterpret_cast<void*>(CPedGroupsArrayHandler_DoesPeerHaveAuthorityOverThisElement));
	CPedGroupsArrayHandler_ReadElement_detour->SetupDetour(0x823BC0, reinterpret_cast<void*>(CPedGroupsArrayHandler_ReadElement));
	CScriptClientVariablesArrayHandler_ReadElement_detour->SetupDetour(0x820D60, reinterpret_cast<void*>(CScriptClientVariablesArrayHandler_ReadElement));

	//Clone Protections
	CNetworkObjectMgr_ProcessCloneCreateData_detour->SetupDetour(0x83C988, reinterpret_cast<void*>(CNetworkObjectMgr_ProcessCloneCreateData));
	CNetObjHeli_CreateClone_detour->SetupDetour(0x857758, reinterpret_cast<void*>(CNetObjHeli_CreateClone));
	
	//Event Protections
	CNetworkEventMgr_HandleEvent_detour->SetupDetour(0x7FBA28, reinterpret_cast<void*>(CNetworkEventMgr_HandleEvent));

	//Other Protections
	CNetworkPeerMgr_HandleGetReadyToStartPlaying_detour->SetupDetour(0x8A2260, reinterpret_cast<void*>(CNetworkPeerMgr_HandleGetReadyToStartPlaying));
	CNetworkPeerMgr_AddRemotePeer_detour->SetupDetour(0x8A0900, reinterpret_cast<void*>(CNetworkPeerMgr_AddRemotePeer));
	netPeerComplainer_OnNetEvent_detour->SetupDetour(0xD014B0, reinterpret_cast<void*>(netPeerComplainer_OnNetEvent));
	PatchInJump(0x84BA18, *reinterpret_cast<uint32_t*>(CMsgReassignConfirm_hook), FALSE);
	PatchInJump(0x84B620, *reinterpret_cast<uint32_t*>(CMsgReassignNegotiate_hook), FALSE);
	PatchInJump(0x84C68C, *reinterpret_cast<uint32_t*>(CMsgReassignResponse_hook), FALSE);

	//Sync Protections
	CNetObjPhysical_SyncAttach_detour->SetupDetour(0x86CCC0, reinterpret_cast<void*>(CNetObjPhysical_SyncAttach));
	CNetObjPed_SyncMovementGroup_detour->SetupDetour(0x8607A0, reinterpret_cast<void*>(CNetObjPed_SyncMovementGroup));
	CNetObjPed_SyncPedAI_detour->SetupDetour(0x85F628, reinterpret_cast<void*>(CNetObjPed_SyncPedAI));
	CNetObjPed_SyncGameState_detour->SetupDetour(0x865220, reinterpret_cast<void*>(CNetObjPed_SyncGameState));
	CNetObjPed_SyncAttach_detour->SetupDetour(0x867D10, reinterpret_cast<void*>(CNetObjPed_SyncAttach));
	CNetObjPlayer_SyncPedAppearance_detour->SetupDetour(0x872AE0, reinterpret_cast<void*>(CNetObjPlayer_SyncPedAppearance));
	CDummyTask_SyncNetworkData_detour->SetupDetour(0x5D71E8, reinterpret_cast<void*>(CDummyTask_SyncNetworkData));

	booted_game = TRUE;
	return TRUE;
}

void UnInit()
{
	if (booted_game && TOC == GTAIV_TOC) //We are unloading while on the game
	{
		
	}

	if (call)
	{
		delete call;
		call = nullptr;
	}

	booted_game = FALSE;
	shutdown_title_hooks = FALSE;
}

void MasterThread(uint64_t stub)
{
	TOC = GetGameTOC(); //Grab our TOC for verifying we are on the game
	_sys_printf("Got TOC: 0x%X\n", TOC);

	while (!term_process)
	{
		if (!shutdown_title_hooks)
		{
			if (TOC == GTAIV_TOC) //Check if TOC is same as BLUS GTA IV TOC
			{
				if (!booted_game && *reinterpret_cast<uint32_t*>(0x960230) == 0x813C00D4) //Check if we haven't booted the game yet and we are on correct update
				{
					if (!Init())
						Sleep(1000);
				}
			}
		}
		else
		{
			UnInit();
		}

		Sleep(33);
	}

	UnInit();
	constant_loop = FALSE;

	sys_ppu_thread_exit(stub);
}

extern "C" int PRX_EXIT()
{
	term_process = TRUE;
	shutdown_title_hooks = TRUE;

	while(constant_loop)
		Sleep(33);

	Sleep(500);
	return SYS_PRX_STOP_OK;
}

extern "C" int PRX_ENTRY(size_t args, void *argp)
{
	sys_ppu_thread_t main_thread_id;

	sys_ppu_thread_create(&main_thread_id, MasterThread, NULL, 3000, 2048, NULL, "PRX Template");
    return SYS_PRX_RESIDENT;
}

