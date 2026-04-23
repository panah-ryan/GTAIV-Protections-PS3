#include "stdafx.h"
#include "defines.h"

CNetworkObjectMgr& ms_objectMgr = *reinterpret_cast<CNetworkObjectMgr*>(0x17455FC);
CNetworkPeerMgr& ms_PeerMgr = *reinterpret_cast<CNetworkPeerMgr*>(0x1716D48);

opd_s CWeaponInfo_GetWeaponInfo_t = { 0x66C248, GTAIV_TOC };
CWeaponInfo*(*CWeaponInfo_GetWeaponInfo)(eWeaponType weapon) = (CWeaponInfo*(*)(eWeaponType))&CWeaponInfo_GetWeaponInfo_t;
CWeaponInfo* CWeaponInfo::GetWeaponInfo(eWeaponType weapon)
{
	return CWeaponInfo_GetWeaponInfo(weapon);
}

opd_s datBitBuffer_CanReadBits_t = { 0xA77880, GTAIV_TOC };
bool(*datBitBuffer_CanReadBits)(datBitBuffer* buffer, int bits) = (bool(*)(datBitBuffer*, int))&datBitBuffer_CanReadBits_t;
bool datBitBuffer::CanReadBits(int bits)
{
	return datBitBuffer_CanReadBits(this, bits);
}

opd_s datBitBuffer_CanWriteBits_t = { 0xA778E0, GTAIV_TOC };
bool(*datBitBuffer_CanWriteBits)(datBitBuffer* buffer, int bits) = (bool(*)(datBitBuffer*, int))&datBitBuffer_CanWriteBits_t;
bool datBitBuffer::CanWriteBits(int bits)
{
	return datBitBuffer_CanWriteBits(this, bits);
}

opd_s datBitBuffer_CopyBits_t = { 0xA77BD8, GTAIV_TOC };
void(*datBitBuffer_CopyBits)(uint8_t* dst, uint8_t* src, int bits, int dst_offset, int src_offset) = (void(*)(uint8_t*, uint8_t*, int, int, int))&datBitBuffer_CopyBits_t;
void datBitBuffer::CopyBits(uint8_t* dst, uint8_t* src, int bits, int dst_offset, int src_offset)
{
	datBitBuffer_CopyBits(dst, src, bits, dst_offset, src_offset);
}

opd_s datBitBuffer_ReadUnsigned_t = { 0xA773B0, GTAIV_TOC };
void(*datBitBuffer_ReadUnsigned)(uint8_t* buffer, uint32_t* u, int bits, int bit_offset) = (void(*)(uint8_t*, uint32_t*, int, int))&datBitBuffer_ReadUnsigned_t;
void datBitBuffer::ReadUnsigned(uint32_t* u, int bits, int bit_offset)
{
	datBitBuffer_ReadUnsigned(m_ReadBits, u, bits, bit_offset);
}

opd_s datBitBuffer_WriteUnsigned_t = { 0xA77430, GTAIV_TOC };
void(*datBitBuffer_WriteUnsigned)(uint8_t* buffer, uint32_t u, int bits, int bit_offset) = (void(*)(uint8_t*, uint32_t, int, int))&datBitBuffer_WriteUnsigned_t;
void datBitBuffer::WriteUnsigned(uint32_t u, int bits, int bit_offset)
{
	datBitBuffer_WriteUnsigned(m_WriteBits, u, bits, bit_offset);
}

opd_s datBitBuffer_ReadUns64_t = { 0xA78BB0, GTAIV_TOC };
bool(*datBitBuffer_ReadUns64)(datBitBuffer* buffer, uint64_t* u, int bits) = (bool(*)(datBitBuffer*, uint64_t*, int))&datBitBuffer_ReadUns64_t;
bool datBitBuffer::ReadUns64(uint64_t* u, int bits)
{
	return datBitBuffer_ReadUns64(this, u, bits);
}

opd_s datBitBuffer_ReadBits_t = { 0xA77F00, GTAIV_TOC };
bool(*datBitBuffer_ReadBits)(datBitBuffer* datbit, uint8_t* buffer, int bits, int seekBits) = (bool(*)(datBitBuffer*, uint8_t*, int, int))&datBitBuffer_ReadBits_t;
bool datBitBuffer::ReadBits(uint8_t* buffer, int bits, int seekBits)
{
	return datBitBuffer_ReadBits(this, buffer, bits, seekBits);
}

opd_s datBitBuffer_ReadStr_t = { 0xA78878, GTAIV_TOC };
bool(*datBitBuffer_ReadStr)(datBitBuffer* buffer, char* str, int maxChars) = (bool(*)(datBitBuffer*, char*, int))&datBitBuffer_ReadStr_t;
bool datBitBuffer::ReadStr(char* str, int maxChars)
{
	return datBitBuffer_ReadStr(this, str, maxChars);
}

opd_s datBitBuffer_WriteUns64_t = { 0xA78698, GTAIV_TOC };
bool(*datBitBuffer_WriteUns64)(datBitBuffer* buffer, uint64_t u, int bits) = (bool(*)(datBitBuffer*, uint64_t, int))&datBitBuffer_WriteUns64_t;
bool datBitBuffer::WriteUns64(uint64_t u, int bits)
{
	return datBitBuffer_WriteUns64(this, u, bits);
}

opd_s datBitBuffer_WriteBits_t = { 0xA77E08, GTAIV_TOC };
bool(*datBitBuffer_WriteBits)(datBitBuffer* datbit, uint8_t* buffer, int bits, int seekBits) = (bool(*)(datBitBuffer*, uint8_t*, int, int))&datBitBuffer_WriteBits_t;
bool datBitBuffer::WriteBits(uint8_t* buffer, int bits, int seekBits)
{
	return datBitBuffer_WriteBits(this, buffer, bits, seekBits);
}

opd_s datBitBuffer_WriteStr_t = { 0xA783D8, GTAIV_TOC };
bool(*datBitBuffer_WriteStr)(datBitBuffer* buffer, char* str, int maxChars) = (bool(*)(datBitBuffer*, char*, int))&datBitBuffer_WriteStr_t;
bool datBitBuffer::WriteStr(char* str, int maxChars)
{
	return datBitBuffer_WriteStr(this, str, maxChars);
}

opd_s CNetworkPeerMgr_GetMyPeer_t = { 0x89D460, GTAIV_TOC };
CNetworkPeer*(*CNetworkPeerMgr_GetMyPeer)(CNetworkPeerMgr* mgr) = (CNetworkPeer*(*)(CNetworkPeerMgr*))&CNetworkPeerMgr_GetMyPeer_t;
CNetworkPeer* CNetworkPeerMgr::GetMyPeer()
{
	return CNetworkPeerMgr_GetMyPeer(this);
}

opd_s CNetwork_IsGameInProgress_t = { 0x8B96B8, GTAIV_TOC };
bool(*CNetwork_IsGameInProgress)() = (bool(*)())&CNetwork_IsGameInProgress_t;
bool CNetwork::IsGameInProgress()
{
	return CNetwork_IsGameInProgress();
}

opd_s CWorld_GetPlayerInfo_t = { 0x40A248, GTAIV_TOC };
CPlayerInfo*(*CWorld_GetPlayerInfo)(int index) = (CPlayerInfo*(*)(int))&CWorld_GetPlayerInfo_t;
CPlayerInfo* CWorld::GetPlayerInfo(int index)
{
	return CWorld_GetPlayerInfo(index);
}

opd_s CNetworkPeerMgr_GetPeerFromPeerId_t = { 0x89D668, GTAIV_TOC };
CNetworkPeer*(*CNetworkPeerMgr_GetPeerFromPeerId)(CNetworkPeerMgr* mgr, int index) = (CNetworkPeer*(*)(CNetworkPeerMgr*, int))&CNetworkPeerMgr_GetPeerFromPeerId_t;
CNetworkPeer* CNetworkPeerMgr::GetPeerFromPeerId(int index)
{
	return CNetworkPeerMgr_GetPeerFromPeerId(this, index);
}

opd_s CNetworkObjectMgr_GetObjectTypeName_t = { 0x828DE0, GTAIV_TOC };
const char* (*CNetworkObjectMgr_GetObjectTypeName)(eNetworkObjectType type, bool isMissionObject) = (const char* (*)(eNetworkObjectType, bool))&CNetworkObjectMgr_GetObjectTypeName_t;
const char* CNetworkObjectMgr::GetObjectTypeName(eNetworkObjectType type, bool isMissionObject)
{
	return CNetworkObjectMgr_GetObjectTypeName(type, isMissionObject);
}

opd_s CAnimAssociations_GetGroupId_t = { 0x686528, GTAIV_TOC };
int(*CAnimAssociations_GetGroupId)(const char* animation) = (int(*)(const char*))&CAnimAssociations_GetGroupId_t;
int CAnimAssociations::GetGroupId(const char* animation)
{
	return CAnimAssociations_GetGroupId(animation);
}

opd_s CWorld_FindSlotForNewPlayer_t = { 0x410220, GTAIV_TOC };
int(*CWorld_FindSlotForNewPlayer)() = (int(*)())&CWorld_FindSlotForNewPlayer_t;
int CWorld::FindSlotForNewPlayer()
{
	return CWorld_FindSlotForNewPlayer();
}

opd_s CNetworkObjectMgr_GetNetworkObject_t = { 0x8332C0, GTAIV_TOC };
CNetworkObject*(*CNetworkObjectMgr_GetNetworkObject)(short objectID, bool includeAll) = (CNetworkObject*(*)(short, bool))&CNetworkObjectMgr_GetNetworkObject_t;
CNetworkObject* CNetworkObjectMgr::GetNetworkObject(short objectID, bool includeAll)
{
	return CNetworkObjectMgr_GetNetworkObject(objectID, includeAll);
}

opd_s CNetworkPeerMgr_GetPeerFromConnectionId_t = { 0x89D358, GTAIV_TOC };
CNetworkPeer*(*CNetworkPeerMgr_GetPeerFromConnectionId)(int cxnId) = (CNetworkPeer*(*)(int))&CNetworkPeerMgr_GetPeerFromConnectionId_t;
CNetworkPeer* CNetworkPeerMgr::GetPeerFromConnectionId(int cxnId)
{
	return CNetworkPeerMgr_GetPeerFromConnectionId(cxnId);
}

opd_s CNetworkPeer_GetGamerInfo_t = { 0x89CAD8, GTAIV_TOC };
CPlayerInfo*(*CNetworkPeer_GetGamerInfo)(CNetworkPeer* _this) = (CPlayerInfo*(*)(CNetworkPeer*))&CNetworkPeer_GetGamerInfo_t;
CPlayerInfo* CNetworkPeer::GetGamerInfo()
{
	return CNetworkPeer_GetGamerInfo(this);
}