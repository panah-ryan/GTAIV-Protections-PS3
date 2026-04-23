#pragma once

#include "stdafx.h"

extern "C" int _sys_printf(const char* fmt, ...);
extern "C" int _sys_snprintf(char* buffer, size_t len, const char* fmt, ...);
extern "C" void* _sys_memcpy(void* dest, const void* src, size_t n);
extern "C" void* _sys_memset(void* m, int c, size_t n);
extern "C" void* _sys_malloc(size_t size);
extern "C" void _sys_free(void* ptr);
extern "C" int _sys_strcmp(const char* s1, const char* s2);
extern "C" char* _sys_strncpy(char* dest, const char* src, size_t n);
extern "C" size_t _sys_strlen(const char* str);
extern "C" int _sys_vsprintf(char * s, const char * format, va_list arg);
extern "C" int _sys_memcmp(const void* ptr1, const void* ptr2, size_t num);
extern "C" char* _sys_strchr(char* str, int character);
extern "C" void* _sys_memmove(void* dest, void* source, size_t num);
extern "C" int _sys_vsnprintf(char* s, size_t n, const char* format, va_list arg);

#define min(a,b)            (((a) < (b)) ? (a) : (b))

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr);
void operator delete[](void* ptr);
uint32_t sys_dbg_write_process_memory(void* destination, const void* source, size_t size);
uint32_t GetGameTOC();
int tolower(int c);
void Sleep(uint32_t time);
char* _strstr(const char* s1, const char* s2);
char* getTitleID();
int atoi(char* str);
float atof(char * s);
uint64_t _byteswap_uint64(uint64_t value);
uint32_t _byteswap_ulong(uint32_t value);
uint16_t _byteswap_ushort(uint16_t value);