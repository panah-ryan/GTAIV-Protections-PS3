#pragma once
#include "stdafx.h"
#include "helpers.h"

template <typename T>
class vector
{
public:
	T* arr;
	int capacity;
	int n;
	size_t element_size;

	vector(size_t size)
	{
		capacity = 10;
		n = 0;
		arr = new T[capacity];
		element_size = size;
	}

	~vector()
	{
		delete[] arr;
	}

	void increase_capacity(int sz)
	{
		if (sz <= capacity)
			return;
		T* new_arr = new T[sz];
		for (int i = 0; i < capacity; ++i)
			new_arr[i] = arr[i];
		capacity = sz;
		delete[] arr;
		arr = new_arr;
	}

	int size()
	{
		return n;
	}

	void push_back(T x)
	{
		if (n >= capacity)
			increase_capacity(2 * capacity);
		arr[n] = x;
		++n;
	}

	void pop_back()
	{
		n--;
	}

	void set(int i, T val)
	{
		if (i < 0 || i >= n)
			return;
		arr[i] = val;
	}

	T get(int i)
	{
		return arr[i];
	}

	T* at(int i)
	{
		return &arr[i];
	}

	void erase(int i)
	{
		if (n == 0)
			return;
		_sys_memmove(&arr[i], &arr[i + 1], element_size * (n - i));
		n--;
	}
};