#pragma once

#include <malloc.h>
#include <memory.h>

template<class T>
class CMemPool
{
public:
	static T* alloc(T, int count);

private:
	CMemPool();
	~CMemPool();

private:

};


