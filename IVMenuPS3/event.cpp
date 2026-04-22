#include "event.h"
#include "caller.h"
#include "natives.h"

uint32_t should_process_event_bitset;

Detour<void>* CNetworkEventMgr_HandleEvent_detour;
void CNetworkEventMgr_HandleEvent(void* manager, CNetworkEvent* pEvent, CMessageBuffer* message, int peer, short messageSeq, int eventId)
{
	NetGameEventTypes type = pEvent->m_EventType;
	uint32_t* vftable = *reinterpret_cast<uint32_t**>(pEvent);
	int seek_bits = message->GetPos();
	bool skip_processing = false;
	static uint32_t modified_vftable[25];

	if((1 << type) & should_process_event_bitset) //Event should be processed so lets do our checks
	{
		switch(type)
		{
			case REQUEST_PICKUP_EVENT:
			{
				int pickup_slot = static_cast<int>(message->PeekInt(11, seek_bits));
				int player_index = static_cast<int>(message->PeekInt(5, seek_bits + 11));

				if (pickup_slot > 1514 || pickup_slot < 0 || player_index > 15 || player_index < 0) //Pickup slot or player index is out of bounds
					skip_processing = true;

				break;
			}
			case RESURRECT_PLAYER_EVENT: 
			{
				int player_index = static_cast<int>(message->PeekInt(8, seek_bits));

				if (player_index > 15 || player_index < 0) //Player index is out of bounds
					skip_processing = true;

				break;
			}
			case REMOVE_WEAPON_EVENT: 
			case REMOVE_ALL_WEAPONS_EVENT: 
			{
				short net_id = static_cast<short>(message->PeekObjectId(seek_bits));

				CNetworkObject* net_obj = ms_objectMgr.GetNetworkObject(net_id, false);
				if (net_obj == nullptr) //Invalid network id
					skip_processing = true;

				break;
			}
			case REQUEST_EXPLOSION_EVENT:
			case START_EXPLOSION_EVENT: 
			{
				EXPLOSION_TAG explosion_type = static_cast<EXPLOSION_TAG>(message->PeekSignedInt(6, seek_bits + 12 + 12));

				//seek_bits + 12 is Explosion Owner Unsigned32 read 12 bits
				if (explosion_type == EXPLOSION_SHIP_DESTROY)
					skip_processing = true;

				break;
			}
			case ALTER_WANTED_LEVEL_EVENT: 
			{
				int player_index = static_cast<int>(message->PeekInt(5, seek_bits));

				if (player_index > 15 || player_index < 0) //Player index is out of bounds
					skip_processing = true;

				break;
			}
			case CREATE_PICKUP_EVENT: 
			{
				uint32_t model_hash = message->PeekInt(32, seek_bits + 57);

				if (IS_THIS_MODEL_A_PED(model_hash) || IS_THIS_MODEL_A_VEHICLE(model_hash)) //Pickup model is a ped or vehicle
					skip_processing = true;

				break;
			}
			case CHANGE_RADIO_STATION_EVENT:
			{
				int station_id = static_cast<int>(message->PeekInt(8, seek_bits + 8 + 8 + 24));

				if (station_id > 30 || station_id < 0) //Station ID is out of bounds
					skip_processing = true;

				break;
			}
			default:
				break;
		}
	}

	if((((1 << type) & should_process_event_bitset) == 0) || skip_processing)
	{
		if(skip_processing)
			_sys_printf("[Event - %s] - %s tried to send invalid event data!\n", 
				pEvent->GetEventName(), 
				CWorld::GetPlayerInfo(peer)->GetPlayerName());

		_sys_memcpy(modified_vftable, vftable, sizeof(modified_vftable));

		//HandleExtraData doesn't actually do anything here
		modified_vftable[8] = 0xF3E460; //Overwrite Handle to not do anything
		modified_vftable[9] = 0xF3E460; //Overwrite Decide to not do anything
		*reinterpret_cast<uint32_t**>(pEvent) = modified_vftable; //Set our modified vftable to the class
	}
	
	CNetworkEventMgr_HandleEvent_detour->CallOriginal(manager, pEvent, message, peer, messageSeq, eventId); //We are good to process the event now
}