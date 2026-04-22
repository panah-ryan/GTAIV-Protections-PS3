#include "event.h"
#include "caller.h"

bool ReturnEventSuccess() //Nullsub to replace our Handle and Decide virtual calls
{
	return true;
}

Detour<void>* netEventMgr_HandleEvent_detour;
void netEventMgr_HandleEvent(void* manager, CNetworkEvent* pEvent, CMessage* message, int peer, short messageSeq, int eventId)
{
	NetGameEventTypes type = pEvent->m_EventType;
	opd_s* vftable = *reinterpret_cast<opd_s**>(pEvent);
	int seek_bits = message->GetPos();
	bool skip_processing = false;
	static opd_s modified_vftable[24];


}