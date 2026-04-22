#include "stdafx.h"
#include "clone.h"
#include "vector.h"
#include "natives.h"

enum eEntityOwnedBy
{
	ENTITY_OWNEDBY_UNKNOWN,
	ENTITY_OWNEDBY_1,
	ENTITY_OWNEDBY_2,
	ENTITY_OWNEDBY_SCRIPT,
	ENTITY_OWNEDBY_4,
	ENTITY_OWNEDBY_5,
	ENTITY_OWNEDBY_DEBUG
};

uint32_t should_process_clone_bitset = 0x3BF; //All clones enabled
PED_MODELS ped_blacklist_models[] = { PED_PLAYER, PED_SUPERLOD, PED_CS_ANDREI, PED_CS_ANGIE, PED_CS_BADMAN, PED_CS_BLEDAR, PED_CS_BULGARIN,
	PED_CS_BULGARINHENCH, PED_CS_CIA, PED_CS_DARDAN, PED_CS_DAVETHEMATE, PED_CS_DMITRI,
	PED_CS_EDTHEMATE, PED_CS_FAUSTIN, PED_CS_FRANCIS, PED_CS_HOSSAN, PED_CS_ILYENA,
	PED_CS_IVAN, PED_CS_JAY, PED_CS_JIMMY_PEGORINO, PED_CS_MEL, PED_CS_MICHELLE,
	PED_CS_MICKEY, PED_CS_OFFICIAL, PED_CS_RAY_BOCCINO, PED_CS_SERGEI, PED_CS_VLAD,
	PED_CS_WHIPPINGGIRL, PED_CS_MANNY, PED_CS_ANTHONY, PED_CS_ASHLEY, PED_CS_ASSISTANT,
	PED_CS_CAPTAIN, PED_CS_CHARLIEUC, PED_CS_DARKO, PED_CS_DWAYNE, PED_CS_ELI_JESTER,
	PED_CS_ELIZABETA, PED_CS_GAYTONY, PED_CS_GERRYMC, PED_CS_GORDON, PED_CS_ISSAC,
	PED_CS_JOHNNYTHEBIKER, PED_CS_JONGRAVELLI, PED_CS_JORGE, PED_CS_KAT, PED_CS_KILLER,
	PED_CS_LUIS, PED_CS_MAGICIAN, PED_CS_MAMC, PED_CS_MELODY, PED_CS_MITCHCOP,
	PED_CS_MORI, PED_CS_PBXGIRL2, PED_CS_PHILB, PED_CS_PLAYBOYX, PED_CS_PRIEST,
	PED_CS_RICKY, PED_CS_TOMMY, PED_CS_TRAMP, PED_CS_BRIAN, PED_CS_CHARISE,
	PED_CS_CLARENCE, PED_CS_EDDIELOW, PED_CS_GRACIE, PED_CS_JEFF, PED_CS_MARNIE,
	PED_CS_MARSHAL, PED_CS_PATHOS, PED_CS_SARAH, PED_CS_ROMAN_D, PED_CS_ROMAN_T,
	PED_CS_ROMAN_W, PED_CS_BRUCIE_B, PED_CS_BRUCIE_T, PED_CS_BRUCIE_W, PED_CS_BERNIE_CRANEC,
	PED_CS_BERNIE_CRANET, PED_CS_BERNIE_CRANEW, PED_CS_LILJACOB_B, PED_CS_LILJACOB_J, PED_CS_MALLORIE_D,
	PED_CS_MALLORIE_J, PED_CS_MALLORIE_W, PED_CS_DERRICKMC_B, PED_CS_DERRICKMC_D, PED_CS_MICHAEL_B,
	PED_CS_MICHAEL_D, PED_CS_PACKIEMC_B, PED_CS_PACKIEMC_D, PED_CS_KATEMC_D, PED_CS_KATEMC_W,
	PED_CS_NIKO, PED_CS_ANDREAS, PED_CS_ANGELGUY, PED_CS_ANGUS, PED_CS_ASHLEYA,
	PED_CS_BILLY, PED_CS_BIKESTRIPPER, PED_CS_BRIANJ, PED_CS_CLAY, PED_CS_DAVE_GROSSMAN,
	PED_CS_DESEAN, PED_CS_ELIZABETAT, PED_CS_FRANKIE, PED_CS_JASON, PED_CS_JIM_FITZ,
	PED_CS_MAITRE_D, PED_CS_MALC, PED_CS_MITCHB, PED_CS_MARTA, PED_CS_MASSEUSE,
	PED_CS_MATTHEWS, PED_CS_MCCORNISH, PED_CS_PAUL_LAMONT, PED_CS_PRISONDLC_01, PED_CS_PRISONGUARD,
	PED_CS_RAYGOON, PED_CS_RAY_LEATHER, PED_CS_RONCERO, PED_CS_RUSSIAN_GOON, PED_CS_RUSSIANGOON2,
	PED_CS_TERRY, PED_CS_SKANK, PED_CS_STUBBS, PED_CS_STUBBS_SUIT, PED_CS_BAZ, PED_CS_JEREMY, PED_CS_ABDUL, PED_CS_ABODYGUARD, PED_CS_ADRIANA,
	PED_CS_AHMAD, PED_CS_ARMANDO, PED_CS_ARNAUD, PED_CS_BATHROOM, PED_CS_BILLY2,
	PED_CS_BULGARIN2, PED_CS_BRUCIE2, PED_CS_DAISY, PED_CS_DERRICK2, PED_CS_DESSIE,
	PED_CS_DOCKGOON, PED_CS_EUGENE, PED_CS_EVAN2, PED_CS_GPARTY_01, PED_CS_GRACIE2,
	PED_CS_HENRIQUE, PED_CS_ISSAC2, PED_CS_JOHNNY2, PED_CS_MARGOT, PED_CS_MAURICE,
	PED_CS_MORI_K, PED_CS_MR_SANTOS, PED_CS_NIKO2, PED_CS_OYVEY, PED_CS_PACKIE2,
	PED_CS_PAPI, PED_CS_ROCCO, PED_CS_ROMAN2, PED_CS_RUS_KN, PED_CS_SHARON,
	PED_CS_TAHIR, PED_CS_TIMUR, PED_CS_TONY, PED_CS_TONYAQ, PED_CS_TRAMP2,
	PED_CS_TRIAD, PED_CS_TROY, PED_CS_VIC, PED_CS_VICGIRL, PED_CS_VINCE, PED_CS_YUSEF 
};

vector<PED_MODELS>* ped_blacklist;
vector<uint32_t>* object_blacklist;

void setup_blacklists()
{
	object_blacklist = new vector<uint32_t>(sizeof(uint32_t));
	ped_blacklist = new vector<PED_MODELS>(sizeof(PED_MODELS));

	for(int i = 0; i < 172; i++)
		ped_blacklist->push_back(ped_blacklist_models[i]);
}

Detour<void>* CNetworkObjectMgr_ProcessCloneCreateData_detour;
void CNetworkObjectMgr_ProcessCloneCreateData(void* netObjMgr, uint8_t peer, NetworkObjectType objectType, short objectID, uint8_t objectFlags, CMessage* message)
{
	bool should_we_create = false;
	BOOL is_mission_object = (objectFlags & GLOBAL_FLAG_SCRIPT_OBJECT) >> 3;

	if (objectType >= NET_OBJ_TYPE_PLAYER && objectType < NET_OBJ_TYPE_COUNT //Object type we are trying to spawn is a valid type
		&& ((1 << objectType) & should_process_clone_bitset) != 0) //We are allowing this type to be spawned
	{
		int seek_bits = message->GetPos();

		switch (objectType)
		{
		case NET_OBJ_TYPE_PLAYER:
		{
			int player_index = static_cast<int>(message->PeekInt(5, seek_bits));
			uint32_t model_hash = message->PeekInt(32, seek_bits + 5);

			if(!NetworkInterface::GetPlayerInfo(peer)->GetPlayerPed() //Check if they already have a ped (duplicate ped)
				&& player_index < 16 && player_index >= 0 //Check if index they are trying to take is valid
				&& player_index != NetworkInterface::GetLocalPlayer()->GetPeerID()) //Check if they are trying to take our index
				should_we_create = true;

			if(model_hash == NULL || (model_hash != PED_M_Y_MULTIPLAYER && model_hash != PED_F_Y_MULTIPLAYER)) //They are trying to spawn not as default multiplayer model, lets fix that
				message->PokeInt(PED_M_Y_MULTIPLAYER, 32, seek_bits + 5);

			break;
		}
		case NET_OBJ_TYPE_PED:
		{
			uint32_t model_hash = message->PeekInt(32, seek_bits + 2);
			seek_bits += 2 + 32 + 16;

			if (is_mission_object) //Read extra data for mission objects
			{
				seek_bits += 8 + 1;
				seek_bits += message->PeekBool(seek_bits) ? 10 + 1 : 1;
				seek_bits += 1 + 1 + 1;
			}

			if(message->PeekBool(seek_bits)) //Message flag if we have a prop
				message->PokeInt(NULL, 32, seek_bits + 1); //Set prop model to NULL

			if (model_hash != NULL && IS_THIS_MODEL_A_PED(model_hash)) //Model is a valid ped
			{
				bool blacklisted = false;
				for (int i = 0; i < ped_blacklist->size(); i++) //Check if model is in our blacklist
				{
					if (ped_blacklist->get(i) == model_hash)
					{
						blacklisted = true;
						break;
					}
				}

				if (!blacklisted)
					should_we_create = true;
			}

			break;
		}
		case NET_OBJ_TYPE_DUMMY_PED: //Basically same as above
		{
			uint32_t model_hash = message->PeekInt(32, seek_bits);

			if (message->PeekBool(seek_bits)) //Message flag if we have a prop
				message->PokeInt(NULL, 32, seek_bits + 32 + 1); //Set prop model to NULL

			if (model_hash != NULL && IS_THIS_MODEL_A_PED(model_hash)) //Model is a valid ped
			{
				bool blacklisted = false;
				for (int i = 0; i < ped_blacklist->size(); i++) //Check if model is in our blacklist
				{
					if (ped_blacklist->get(i) == model_hash)
					{
						blacklisted = true;
						break;
					}
				}

				if (!blacklisted)
					should_we_create = true;
			}

			break;
		}
		case NET_OBJ_TYPE_AUTOMOBILE:
		{
			uint32_t model_hash = message->PeekInt(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_CAR(model_hash)) //Valid model for a car
				should_we_create = true;

			break;
		}
		case NET_OBJ_TYPE_OBJECT:
		{
			eEntityOwnedBy owned_by = static_cast<eEntityOwnedBy>(message->PeekInt(3, seek_bits) + 1);

			if (is_mission_object || owned_by == ENTITY_OWNEDBY_DEBUG || owned_by == ENTITY_OWNEDBY_SCRIPT)
			{
				uint32_t model_hash = message->PeekInt(32, seek_bits + 3);

				if (model_hash != NULL && !IS_THIS_MODEL_A_PED(model_hash))
				{
					bool blacklisted = false;
					for (int i = 0; i < object_blacklist->size(); i++) //Check if model is in our blacklist
					{
						if (object_blacklist->get(i) == model_hash)
						{
							blacklisted = true;
							break;
						}
					}

					if (!blacklisted)
						should_we_create = true;
				}
					
			}
			else
				should_we_create = true; //Its not reading a model so we can assume its safe

			break;
		}
		case NET_OBJ_TYPE_BIKE:
		{
			uint32_t model_hash = message->PeekInt(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_BIKE(model_hash)) //Model is a bike
				should_we_create = true;

			break;
		}
		case NET_OBJ_TYPE_TRAIN:
		{
			uint32_t model_hash = message->PeekInt(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_TRAIN(model_hash)) //Model is a train
				should_we_create = true;

			break;
		}
		case NET_OBJ_TYPE_HELI:
		{
			uint32_t model_hash = message->PeekInt(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_HELI(model_hash)) //Model is a heli
				should_we_create = true;

			break;
		}
		case NET_OBJ_TYPE_BOAT:
		{
			uint32_t model_hash = message->PeekInt(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_BOAT(model_hash)) //Model is a boat
				should_we_create = true;

			break;
		}
		case NET_OBJ_TYPE_PLANE:
		{
			uint32_t model_hash = message->PeekInt(32, seek_bits);

			if (model_hash != NULL && IS_THIS_MODEL_A_VEHICLE(model_hash)) //Can't check if model is a plane (always returns false) so haven't found any issues if its any vehicle
				should_we_create = true;

			break;
		}
		default:
			break;
		}
	}

	if (!should_we_create)
	{
		if(objectType < NET_OBJ_TYPE_COUNT && objectType >= NET_OBJ_TYPE_PLAYER)
			_sys_printf("[Clone Create] - Prevented %s from spawning by player %s!\n", CNetworkObjectMgr::GetObjectTypeName(objectType, false), NetworkInterface::GetPlayerInfo(peer)->GetPlayerName());

		return;
	}

	CNetworkObjectMgr_ProcessCloneCreateData_detour->CallOriginal(netObjMgr, peer, objectType, objectID, objectFlags, message); //Continue original code since its safe to spawn
}

Detour<bool>* CNetObjHeli_SerializeCloneData_detour;
bool CNetObjHeli_SerializeCloneData(CNetworkObject* net_heli, CMessage* message)
{
	bool ret = CNetObjHeli_SerializeCloneData_detour->CallOriginal(net_heli, message);

	if (ret)
	{
		CHeli* heli = net_heli->GetBaseHeli();
		if (heli && heli->m_PlayerIndexSpotlight < -1 || heli->m_PlayerIndexSpotlight > 15)
		{
			_sys_printf("[Network Heli] - Corrected invalid player spot light crash!\n");
			heli->m_PlayerIndexSpotlight = -1;
		}
	}

	return ret;
}