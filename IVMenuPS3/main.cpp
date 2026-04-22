#include "stdafx.h"
#include "helpers.h"
#include "Detour.h"
#include "caller.h"
#include "defines.h"
#include "array.h"
#include "clone.h"
#include "event.h"

SYS_MODULE_INFO(PRXTemplate, 0, 1, 1);
SYS_MODULE_START(PRX_ENTRY);
SYS_MODULE_STOP(PRX_EXIT);

uint32_t TOC;
BOOL term_process;
BOOL constant_loop = TRUE;
BOOL booted_game;
BOOL shutdown_title_hooks;

Caller* call = nullptr;

Detour<void>* CWeaponInfoManager_LoadXML_detour;
void CWeaponInfoManger_LoadXML(const char* weaponxml)
{
	CWeaponInfoManager_LoadXML_detour->CallOriginal(weaponxml);

	CWeaponInfo* rocket = CWeaponInfoManager::GetInfo(WEAPON_ROCKET);
	if (rocket)
	{
		rocket->m_AimFlags |= 0xC005; //CAN_AIM - CAN_FREE_AIM - ANIM_RELOAD - ANIM_CROUCH_FIRE
		rocket->m_AnimationIndex = 29; //grenade animation group
	}

	CWeaponInfo* e2_rocket = CWeaponInfoManager::GetInfo(WEAPON_EPISODIC_18);
	if (e2_rocket)
	{
		e2_rocket->m_AimFlags |= 0xC005; //CAN_AIM - CAN_FREE_AIM - ANIM_RELOAD - ANIM_CROUCH_FIRE
		e2_rocket->m_AnimationIndex = 29; //grenade animation group
	}
}

BOOL Init()
{
	call = new Caller();

	CWeaponInfoManager_LoadXML_detour = new Detour<void>;

	CNetworkArrayHandler_CanApplyElementData_detour = new Detour<bool>;
	CPedGroupsArrayHandler_CanApplyElementData_detour = new Detour<bool>;
	CPedGroupsArrayHandler_ReadUpdate_detour = new Detour<bool>;
	CScriptClientVariablesArrayHandler_ReadUpdate_detour = new Detour<bool>;

	CNetworkObjectMgr_ProcessCloneCreateData_detour = new Detour<void>;
	CNetObjHeli_SerializeCloneData_detour = new Detour<bool>;

	setup_blacklists();

	*reinterpret_cast<uint32_t*>(0x480788) = 0x4E800020; //File Bypass

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
	CWeaponInfoManager_LoadXML_detour->SetupDetour(0x66DA38, reinterpret_cast<void*>(CWeaponInfoManger_LoadXML));

	//Array protections
	CNetworkArrayHandler_CanApplyElementData_detour->SetupDetour(0x81C328, reinterpret_cast<void*>(CNetworkArrayHandler_CanApplyElementData));
	CPedGroupsArrayHandler_CanApplyElementData_detour->SetupDetour(0x821BF8, reinterpret_cast<void*>(CPedGroupsArrayHandler_CanApplyElementData));
	CPedGroupsArrayHandler_ReadUpdate_detour->SetupDetour(0x823BC0, reinterpret_cast<void*>(CPedGroupsArrayHandler_ReadUpdate));
	CScriptClientVariablesArrayHandler_ReadUpdate_detour->SetupDetour(0x820D60, reinterpret_cast<void*>(CScriptClientVariablesArrayHandler_ReadUpdate));

	//Clone Protections
	CNetworkObjectMgr_ProcessCloneCreateData_detour->SetupDetour(0x83C988, reinterpret_cast<void*>(CNetworkObjectMgr_ProcessCloneCreateData));
	CNetObjHeli_SerializeCloneData_detour->SetupDetour(0x857758, reinterpret_cast<void*>(CNetObjHeli_SerializeCloneData));
		
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

