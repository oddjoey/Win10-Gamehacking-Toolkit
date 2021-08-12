#pragma once

#include "cLogger.h"
#include "cMemory.h"
#include "cKernal.h"
#include "cPortableExecutable.h"

class cRainDear
{
public:
	cRainDear();
	~cRainDear();
	cPortableExecutable_t	CreatePEParser(const std::string imagePath);
	cKernal_t				GetKernalInterface();
	cMemory_t				GetMemoryInterface();
	void					DisableAntiDebugger();
private:
	cKernal_t				_pKernal		= nullptr;
	cMemory_t				_pMemory		= nullptr;
	cLogger_t				_pLogger		= nullptr;
};