#include "Detour.h"

__attribute__((section(".text"))) uint8_t DetourAsm[0x500] = { 0 };
uint32_t DetourAsmIndex = 0;

int Int24ToInt32(int Value)
{
	uint8_t bValue = (Value >> 24) & 0xFF;
	Value &= 0x00FFFFFF;

	if (Value & 0x800000 && bValue != 0x48)
		Value |= 0xFF000000;

	if (Value & 1)
		Value -= 1;
	return Value;
}

void PatchInJump(uint32_t Address, uint32_t Dest, BOOL Linked, int Register)
{
	uint32_t Bytes[4];

	Bytes[0] = (0x3C000000 + (Register * 0x200000)) | (Dest >> 16) & 0xFFFF;
	Bytes[1] = (0x60000000 + (Register * 0x210000)) | (Dest & 0xFFFF);
	Bytes[2] = 0x7C0903A6 + (Register * 0x200000);
	Bytes[3] = 0x4E800420;

	if (Linked)
		Bytes[3] += 1;
	
	sys_dbg_write_process_memory((void*)Address, Bytes, 4 * 4);
}