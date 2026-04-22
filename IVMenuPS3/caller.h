#pragma once
#include "stdafx.h"

unsigned int jenkinsHash(char* key);
opd_s* LookupCommandSignature(uint32_t hashCode);

class Caller
{
public:
	union scrValue
	{
		int Int;
		uint32_t Uns;
		float Float;
		char* String;
		scrValue* Reference;
		uint32_t Any;
		BOOL Bool;
	};

	struct Info
	{
		scrValue* ResultPtr;
		int ParamCount;
		scrValue* Params;
		int BufferCount;
		float* Orig[4];
		__vector4 Buffer[4];

		void CopyReferencedParametersOut()
		{
			while (BufferCount--)
			{
				Orig[BufferCount][0] = Buffer[BufferCount].x;
				Orig[BufferCount][1] = Buffer[BufferCount].y;
				Orig[BufferCount][2] = Buffer[BufferCount].z;
			}
		}
	};

	int argCount;
	uint32_t Call_Args[16];
	uint32_t Call_Returns[3];

	Caller()
	{
		argCount = 0;
	}

	~Caller()
	{

	}

	template <typename T>
	void Arg(T value)
	{
		*(T*)&Call_Args[argCount] = value;
		argCount++;
	}

	template <typename T, typename... TArgs>
	void Arg(T value, TArgs... args)
	{
		Arg(value);
		Arg(args...);
	}

	template <typename T, typename... TArgs>
	T call(const char* szName, TArgs... args)
	{
		Info cxt;
		cxt.Params = reinterpret_cast<scrValue*>(Call_Args);
		cxt.ResultPtr = reinterpret_cast<scrValue*>(Call_Returns);
		cxt.BufferCount = 0;

		argCount = 0;
		Arg(args...);

		opd_s* op = LookupCommandSignature(jenkinsHash(const_cast<char*>(szName)));
		if (op->toc == TOC)
		{
			((void(*)(Info*))op)(&cxt);
			cxt.CopyReferencedParametersOut();
			return *(T*)&Call_Returns[0];
		}

		return NULL;
	}

	template <typename T, typename... TArgs>
	T call(uint32_t dwHash, TArgs... args)
	{
		Info cxt;
		cxt.Params = reinterpret_cast<scrValue*>(Call_Args);;
		cxt.ResultPtr = reinterpret_cast<scrValue*>(Call_Returns);;
		cxt.BufferCount = 0;

		argCount = 0;
		Arg(args...);

		opd_s* op = LookupCommandSignature(dwHash);
		if (op->toc == TOC)
		{
			((void(*)(Info*))op)(&cxt);
			cxt.CopyReferencedParametersOut();
			return *(T*)&Call_Returns[0];
		}

		return NULL;
	}

	template <typename T>
	T call(const char* szName)
	{
		Info cxt;
		cxt.Params = nullptr;
		cxt.ResultPtr = reinterpret_cast<scrValue*>(Call_Returns);;
		cxt.BufferCount = 0;

		argCount = 0;

		opd_s* op = LookupCommandSignature(jenkinsHash(const_cast<char*>(szName)));
		if (op->toc == TOC)
		{
			((void(*)(Info*))op)(&cxt);
			cxt.CopyReferencedParametersOut();
			return *(T*)&Call_Returns[0];
		}

		return NULL;
	}

	template <typename T>
	T call(uint32_t dwHash)
	{
		Info cxt;
		cxt.Params = nullptr;
		cxt.ResultPtr = reinterpret_cast<scrValue*>(Call_Returns);;
		cxt.BufferCount = 0;

		argCount = 0;

		opd_s* op = LookupCommandSignature(dwHash);
		if (op->toc == TOC)
		{
			((void(*)(Info*))op)(&cxt);
			cxt.CopyReferencedParametersOut();
			return *(T*)&Call_Returns[0];
		}

		return NULL;
	}
};

struct scrThread
{
	void* vftable;
	int m_ThreadId;
	uint32_t m_Prog;
};