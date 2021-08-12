#pragma once

#include <iostream>
#include <string>
#include <Windows.h>

#include "cLogger.h"

typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	PVOID PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcessId;
	PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;

typedef DWORD(WINAPI* pNtQueryInformationProcess)(HANDLE, DWORD, PVOID, DWORD, PVOID);

class cKernal
{
	static std::shared_ptr<cKernal> _instance;
public:
	static std::shared_ptr<cKernal> Get();

	cKernal();
	~cKernal();

	void* GetPEBFromHandle(const HANDLE& handle);

private:
	DWORD NtQueryInformationProcess(HANDLE ProcessHandle, DWORD ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);

	HMODULE _ntDLL = nullptr;

	pNtQueryInformationProcess _fnNtQueryInformationProces = nullptr;

	cLogger_t _logger = nullptr;
};

typedef std::shared_ptr<cKernal> cKernal_t;