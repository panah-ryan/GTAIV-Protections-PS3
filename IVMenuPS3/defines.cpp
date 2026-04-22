#include "stdafx.h"
#include "defines.h"

opd_s CWeaponInfoManager_GetInfo_t = { 0x66C248, GTAIV_TOC };
CWeaponInfo*(*CWeaponInfoManager_GetInfo)(WEAPON_TYPE weapon) = (CWeaponInfo*(*)(WEAPON_TYPE))&CWeaponInfoManager_GetInfo_t;
CWeaponInfo* CWeaponInfoManager::GetInfo(WEAPON_TYPE weapon)
{
	return CWeaponInfoManager_GetInfo(weapon);
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

opd_s CMessage_UnpackVector3_t = { 0x8A7A68, GTAIV_TOC };
void(*CMessage_UnpackVector3)(CMessage* message, float* buffer, int bits) = (void(*)(CMessage*, float*, int))&CMessage_UnpackVector3_t;
void CMessage::PeekVector3(int bits, int srcBitOffset, float* vec)
{
	int save_cursor = m_buffer.m_CursorPos;
	int save_read = m_buffer.m_NumBitsRead;
	int save_source = m_buffer.m_BaseBitOffset;

	m_buffer.m_BaseBitOffset = srcBitOffset;
	CMessage_UnpackVector3(this, vec, bits);

	m_buffer.m_CursorPos = save_cursor;
	m_buffer.m_NumBitsRead = save_read;
	m_buffer.m_BaseBitOffset = save_source;
}

opd_s CNetGamePlayer_GetLocalPlayer_t = { 0x89D460, GTAIV_TOC };
CNetGamePlayer*(*CNetGamePlayer_GetLocalPlayer)(uint32_t interface) = (CNetGamePlayer*(*)(uint32_t))&CNetGamePlayer_GetLocalPlayer_t;
CNetGamePlayer* NetworkInterface::GetLocalPlayer()
{
	return CNetGamePlayer_GetLocalPlayer(NETWORK_INTERFACE);
}

opd_s Network_AreWeInNetworkGame_t = { 0x8B96B8, GTAIV_TOC };
bool(*Network_AreWeInNetworkGame)() = (bool(*)())&Network_AreWeInNetworkGame_t;
bool NetworkInterface::AreWeInNetworkGame()
{
	return Network_AreWeInNetworkGame();
}

opd_s CPlayerInfo_GetPlayerInfo_t = { 0x40A248, GTAIV_TOC };
CPlayerInfo*(*CPlayerInfo_GetPlayerInfo)(int index) = (CPlayerInfo*(*)(int))&CPlayerInfo_GetPlayerInfo_t;
CPlayerInfo* NetworkInterface::GetPlayerInfo(int index)
{
	return CPlayerInfo_GetPlayerInfo(index);
}

opd_s CNetGamePlayer_GetNetPlayer_t = { 0x89D668, GTAIV_TOC };
CNetGamePlayer*(*CNetGamePlayer_GetNetPlayer)(uint32_t _interface, int peer) = (CNetGamePlayer*(*)(uint32_t, int))&CNetGamePlayer_GetNetPlayer_t;
CNetGamePlayer* NetworkInterface::GetNetPlayer(int peer)
{
	return CNetGamePlayer_GetNetPlayer(NETWORK_INTERFACE, peer);
}

opd_s CNetworkObjectMgr_GetObjectTypeName_t = { 0x828DE0, GTAIV_TOC };
const char* (*CNetworkObjectMgr_GetObjectTypeName)(NetworkObjectType type, bool isMissionObject) = (const char* (*)(NetworkObjectType, bool))&CNetworkObjectMgr_GetObjectTypeName_t;
const char* CNetworkObjectMgr::GetObjectTypeName(NetworkObjectType type, bool isMissionObject)
{
	return CNetworkObjectMgr_GetObjectTypeName(type, isMissionObject);
}