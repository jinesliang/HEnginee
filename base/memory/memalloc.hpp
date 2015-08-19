#pragma once

#include "logger/logger.h"
#include "mempool.hpp"

#define new(size) Alloc(size, __FILE__, __LINE__)

void * Alloc(size_t size, char* szFileName, int iLineNum)
{
}