#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

#include <Windows.h>
#include <TlHelp32.h>

#include "cLogger.h"

class cMemory
{
	static std::shared_ptr<cMemory> _instance;
public:
	static std::shared_ptr<cMemory> Get();

	cMemory();
	~cMemory();

	void DumpAllProcesses();
	void DumpModulesFromProcess(const DWORD& processID);

	std::string GetCurrentProcessName() const;
	DWORD GetCurrentProcessID() const;
	HANDLE GetCurrentProcessHandle() const;

	bool IsProcessAttatched() const;

	bool AttatchProcess(const std::string& processName);
	BYTE* GetModuleAddress(const std::string& moduleName);

	template <typename T>
	void WriteMemory(void* address, const T& value)
	{
		WriteProcessMemory(_processHandle, address, &value, sizeof(T), NULL);
	}
	template <typename T>
	T ReadMemory(void* address)
	{
		T value;
		ReadProcessMemory(_processHandle, address, &value, sizeof(T), NULL);
		return value;
	}
private:
	std::string _processName = "";
	HANDLE _processHandle = nullptr;
	DWORD _processID = NULL;
	bool _attatched = false;
	cLogger_t _logger = nullptr;
};

typedef std::shared_ptr<cMemory> cMemory_t;