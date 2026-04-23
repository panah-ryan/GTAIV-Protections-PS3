#include "helpers.h"
#include <sys/timer.h>

void* operator new(size_t size)
{
	return _sys_malloc(size);
}

void* operator new[](size_t size)
{
	if (size == 0)
		size = 1;

	return _sys_malloc(size);
}

void operator delete(void* alloc)
{
	_sys_free(alloc);
}

void operator delete[](void* ptr)
{
	_sys_free(ptr);
}

uint32_t sys_dbg_write_process_memory(void* destination, const void* source, size_t size)
{
	system_call_6(8, 0x7777, 0x32, (uint64_t)sys_process_getpid(), (uint64_t)destination, (uint64_t)source, (uint64_t)size);
	return_to_user_prog(uint32_t);
}

uint32_t GetGameTOC()
{
	return *reinterpret_cast<uint32_t*>(*reinterpret_cast<uint32_t*>(0x1001C) + 4);
}

int tolower(int c)
{
	if (c - 'A' < 26)
		return c | 32;

	return c;
}

void Sleep(uint32_t time)
{
	sys_timer_usleep(time * 1000);
}

char* _strstr(const char* s1, const char* s2)
{
	const size_t len = _sys_strlen(s2);

	while (*s1)
	{
		if (!_sys_memcmp(s1, s2, len))
			return (char*)s1;

		++s1;
	}
	return 0;
}

int sys_get_random_number(uint8_t* buf, int len)
{
	system_call_3(865, 2, (uint64_t)buf, len);
	return_to_user_prog(int);
}

int _sys_process_get_paramsfo(uint8_t buffer[0x40])
{
	system_call_1(30, (uint64_t)buffer);
	return_to_user_prog(int);
}

char* getTitleID()
{
	static uint8_t buffer[0x40];

	_sys_process_get_paramsfo(buffer);
	return (char*)buffer + 1;
}

int	atoi(char *str)
{
	int neg;
	int num;
	int i;

	i = 0;
	neg = 1;
	num = 0;

	while (str[i] <= ' ')
		i++;

	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			neg *= -1;
		
		i++;
	}

	while (str[i] >= '0' && str[i] <= '9')
	{
		num = num * 10 + (str[i] - 48);
		i++;
	}

	return (num * neg);
}

int isspace(int c)
{
	return ((c == ' ') || (c == '\n') || (c == '\t'));
}

#define isdigit(c) (c >= '0' && c <= '9')

int	toupper(int c)
{
	if (c >= 97 && c <= 122)
		c -= 32;

	return (c);
}

float atof(char * s)
{
	float value, fraction;
	char iexp;
	char sign;

	//Skip leading blanks
	while (isspace(*s)) 
		s++;

	//Get the sign
	if (*s == '-')
	{
		sign = 1;
		s++;
	}
	else
	{
		sign = 0;
		if (*s == '+') 
			s++;
	}

	//Get the integer part
	for (value = 0.0; isdigit(*s); s++)
		value = 10.0*value + (*s - '0');

	//Get the fraction
	if (*s == '.')
	{
		s++;

		for (fraction = 0.1; isdigit(*s); s++)
		{
			value += (*s - '0')*fraction;
			fraction *= 0.1;
		}
	}

	//Finally, the exponent (not very efficient, but enough for now*/
	if (toupper(*s) == 'E')
	{
		s++;

		iexp = (char)atoi(s);
		{
			while (iexp != 0)
			{
				if (iexp < 0)
				{
					value *= 0.1;
					iexp++;
				}
				else
				{
					value *= 10.0;
					iexp--;
				}
			}
		}
	}

	if (sign) 
		value *= -1.0;

	return (value);
}

int abs(int i)
{
	return i < 0 ? -i : i;
}

int itoa_s(int value, char *buf) 
{
	int index = 0;
	int i = value % 10;

	if (value >= 10) 
		index += itoa_s(value / 10, buf);

	buf[index] = i + 0x30;
	index++;
	return index;
}

char *ftoa(float value, int decimals, char *buf) 
{
	int index = 0;

	// Handle negative values
	if (value < 0) 
	{
		buf[index] = '-';
		index++;
		value = -value;
	}

	// Rounding
	float rounding = 0.5;
	for (int d = 0; d < decimals; rounding /= 10.0, d++);
	value += rounding;

	// Integer part
	index += itoa_s((int)(value), buf + index);
	buf[index++] = '.';

	// Remove everything except the decimals
	value = value - (int)(value);

	// Convert decmial part to integer
	int ival = 1;
	for (int d = 0; d < decimals; ival *= 10, d++);
	ival *= value;

	// Add decimal part to string
	index += itoa_s(ival, buf + index);
	buf[index] = '\0';
	return buf;
}

uint64_t _byteswap_uint64(uint64_t value)
{
	return (value & 0x00000000000000FFUL) << 56 | (value & 0x000000000000FF00UL) << 40 |
		(value & 0x0000000000FF0000UL) << 24 | (value & 0x00000000FF000000UL) << 8 |
		(value & 0x000000FF00000000UL) >> 8 | (value & 0x0000FF0000000000UL) >> 24 |
		(value & 0x00FF000000000000UL) >> 40 | (value & 0xFF00000000000000UL) >> 56;
}

uint32_t _byteswap_ulong(uint32_t value)
{
	return (value & 0x000000FF) << 24 | (value & 0x0000FF00) << 8 |
		(value & 0x00FF0000) >> 8 | (value & 0xFF000000) >> 24;
}

uint16_t _byteswap_ushort(uint16_t value)
{
	return (short)((value >> 8) | (value << 8));
}

int hexval(int c) 
{
	if (c >= '0' && c <= '9')
		return c-'0';
	else if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	else
		return -1;
}

uint32_t atou(const char *s) 
{
	size_t value = 0;

	while (hexval(*s) != -1)
		value = (value * 16) + (hexval(*s++));

	return value;
}

char* safecat(char* dest, char* src, size_t destSize)
{
	char* result = dest;
	if (destSize > 0)
	{
		size_t i = 0;
		size_t len = _sys_strlen(dest);
		destSize -= (len + 1);
		dest += len;
		
		while (src[i] && destSize > 0)
		{
			*dest++ = src[i];
			--destSize;
			++i;
		}
	}

	*dest = '\0';
	return result;
}