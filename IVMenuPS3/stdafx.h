#pragma once

#include <sys/prx.h>
#include <sys/process.h>
#include <sys/ppu_thread.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/sys_types.h>

#undef vector

typedef int BOOL;

extern uint32_t TOC;

#define TRUE 1
#define FALSE 0

struct opd_s
{
	uint32_t stub;
	uint32_t toc;
};

typedef struct  __vector4 
{
	union
	{
		float        vector4_f32[4];
		unsigned int vector4_u32[4];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		float        v[4];
		unsigned int u[4];
	};
} __vector4;

struct vector3
{
	float x;
	float y;
	float z;
};

//BLUS IV Original
#define GTAIV_TOC 0xF6CCE8 
