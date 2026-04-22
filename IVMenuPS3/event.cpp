#include "event.h"
#include "caller.h"

//F3E460

uint32_t should_process_event_bitset;

Detour<void>* CNetworkEventMgr_HandleEvent_detour;
void CNetworkEventMgr_HandleEvent(void* manager, CNetworkEvent* pEvent, CMessageBuffer* message, int peer, short messageSeq, int eventId)
{
	NetGameEventTypes type = pEvent->m_EventType;
	opd_s* vftable = *reinterpret_cast<opd_s**>(pEvent);
	int seek_bits = message->GetPos();
	bool skip_processing = false;
	static opd_s modified_vftable[25];

	if((1 << type) & should_process_event_bitset) //Event should be processed so lets do our checks
	{
		switch(type)
		{
			case REQUEST_PICKUP_EVENT:
			{
				
			}
		}
	}
}