#include "cRainDear.h"

cRainDear::cRainDear()
{
	_pLogger = cLogger::Get();
	_pKernal = cKernal::Get();
	_pMemory = cMemory::Get();
}

cRainDear::~cRainDear()
{
}

cPortableExecutable_t cRainDear::CreatePEParser(const std::string imagePath)
{
	return std::make_shared<cPortableExecutable>(imagePath);
}

cKernal_t cRainDear::GetKernalInterface()
{
	return _pKernal;
}

cMemory_t cRainDear::GetMemoryInterface()
{
	return _pMemory;
}

void cRainDear::DisableAntiDebugger()
{
	if (!_pMemory->IsProcessAttatched())
	{
		_pLogger->PrintVar("No process attatched to disable anti-debugger", true);
		return;
	}

	void* pebTable = _pKernal->GetPEBFromHandle(_pMemory->GetCurrentProcessHandle());

	// 0x2 is PEB's isBeingDebugged flag which we set to 0 to bypass IsDebuggerPresent()
	_pMemory->WriteMemory<char>(reinterpret_cast<char*>(pebTable) + 0x2, 0);
	_pLogger->PrintVar("Anti-debugger successful");
}
