#pragma once

#include "stdafx.h"
#include "helpers.h"

extern uint8_t DetourAsm[0x500];
extern uint32_t DetourAsmIndex;

void PatchInJump(uint32_t Address, uint32_t Dest, BOOL Linked, int Register = 11);
int Int24ToInt32(int Value);

template<class _ClassType>
class Detour
{
private:
	uint8_t OriginalAsm[0x10];
	uint32_t DetourIndex;

	virtual uint32_t DetourFunctionStart(uint32_t dwFunctionAddress, uint32_t dwStubAddress, uint32_t pDestFunc, int Register)
	{
		uint32_t dwTemp;
		uint32_t dwTempFuncAddr;
		uint32_t dwLength = 0;
		BOOL bTemp;

		for (int i = 0; i < 4; i++)
		{
			dwTempFuncAddr = dwFunctionAddress + (i * 4);
			uint8_t b = *(uint8_t*)dwTempFuncAddr;

			if (b == 0x48 || b == 0x4B)
			{
				dwTemp = dwTempFuncAddr + Int24ToInt32(*(uint32_t*)dwTempFuncAddr);
				bTemp = (*(uint32_t*)dwTempFuncAddr & 1) != 0;
				PatchInJump(dwStubAddress + dwLength, dwTemp, bTemp, Register);
				dwLength += 0x10;

				if (!bTemp)
					goto DoHook;
			}
			else if (*(uint32_t*)dwTempFuncAddr == 0)
				break;
			else
			{
				sys_dbg_write_process_memory((void*)(dwStubAddress + dwLength), (void*)dwTempFuncAddr, sizeof(uint32_t));
				dwLength += 4;
			}
		}

		PatchInJump(dwStubAddress + dwLength, dwFunctionAddress + 0x10, FALSE, Register);
		dwLength += 0x10;

	DoHook:
		PatchInJump(dwFunctionAddress, pDestFunc, FALSE, Register);
		return dwLength;
	}

public:
	uint32_t Addr;
	uint32_t SaveStub;
	opd_s HookOpd;

	Detour()
	{
		Addr = 0;
		SaveStub = 0;
		DetourIndex = 0;
	};

	virtual ~Detour() {};

	virtual BOOL SetupDetour(uint32_t Address, void* Destination, int Register = 11)
	{
		uint32_t Dest = *(uint32_t*)Destination;

		if (Addr != Address || SaveStub == 0)
		{
			DetourIndex = DetourAsmIndex;
			SaveStub = (uint32_t)&DetourAsm[DetourIndex];
			HookOpd.stub = SaveStub;
			HookOpd.toc = TOC;
			Addr = Address;
			_sys_memcpy(OriginalAsm, (void*)Address, 0x10);
			DetourAsmIndex += DetourFunctionStart(Address, SaveStub, Dest, Register);
		}
		else
			DetourFunctionStart(Address, SaveStub, Dest, Register);
		return TRUE;
	}
	
	virtual void TakeDownDetour()
	{
		if (Addr)
			sys_dbg_write_process_memory((void*)Addr, OriginalAsm, 0x10);

		Detour();
	}

	_ClassType CallOriginal()
	{
		return ((_ClassType(*)(...))&HookOpd)();
	}

	template<typename R3>
	_ClassType CallOriginal(R3 r3)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3);
	}

	template<typename R3, typename R4>
	_ClassType CallOriginal(R3 r3, R4 r4)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4);
	}

	template<typename R3, typename R4, typename R5>
	_ClassType CallOriginal(R3 r3, R4 r4, R5 r5)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5);
	}

	template<typename R3, typename R4, typename R5, typename R6>
	_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7>
	_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8>
	_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9>
	_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10>
	_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12, typename S13>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12, S13 s13)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12, s13);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12, typename S13, typename S14>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12, S13 s13, S14 s14)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12, s13, s14);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12, typename S13, typename S14, typename S15>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12, S13 s13, S14 s14,
			S15 s15)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12, s13, s14, s15);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12, typename S13, typename S14, typename S15, typename S16>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12, S13 s13, S14 s14,
			S15 s15, S16 s16)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12, s13, s14, s15, s16);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12, typename S13, typename S14, typename S15, typename S16,
		typename S17>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12, S13 s13, S14 s14,
			S15 s15, S16 s16, S17 s17)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12, s13, s14, s15, s16, s17);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12, typename S13, typename S14, typename S15, typename S16,
		typename S17, typename S18>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12, S13 s13, S14 s14,
			S15 s15, S16 s16, S17 s17, S18 s18)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12, s13, s14, s15, s16, s17, s18);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12, typename S13, typename S14, typename S15, typename S16,
		typename S17, typename S18, typename S19>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12, S13 s13, S14 s14,
			S15 s15, S16 s16, S17 s17, S18 s18, S19 s19)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12, s13, s14, s15, s16, s17, s18, s19);
	}

	template<typename R3, typename R4, typename R5, typename R6, typename R7, typename R8, typename R9, typename R10,
		typename S1, typename S2, typename S3, typename S4, typename S5, typename S6, typename S7, typename S8,
		typename S9, typename S10, typename S11, typename S12, typename S13, typename S14, typename S15, typename S16,
		typename S17, typename S18, typename S19, typename S20>
		_ClassType CallOriginal(R3 r3, R4 r4, R5 r5, R6 r6, R7 r7, R8 r8, R9 r9, R10 r10, S1 s1, S2 s2,
			S3 s3, S4 s4, S5 s5, S6 s6, S7 s7, S8 s8, S9 s9, S10 s10, S11 s11, S12 s12, S13 s13, S14 s14,
			S15 s15, S16 s16, S17 s17, S18 s18, S19 s19, S20 s20)
	{
		return ((_ClassType(*)(...))&HookOpd)(r3, r4, r5, r6, r7, r8, r9, r10, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10,
			s11, s12, s13, s14, s15, s16, s17, s18, s19, s20);
	}
};