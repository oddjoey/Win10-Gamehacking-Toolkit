#include "cMemory.h"

cMemory_t cMemory::_instance = nullptr;

cMemory_t cMemory::Get()
{
	if (_instance == nullptr)
		_instance = std::make_shared<cMemory>();

	return _instance;
}

cMemory::cMemory()
{
	if (_logger == nullptr)
		_logger = cLogger::Get();
}

cMemory::~cMemory()
{
	if (_processHandle != INVALID_HANDLE_VALUE)
		CloseHandle(_processHandle);
}

void cMemory::DumpAllProcesses()
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		size_t counter = 0;

		_logger->PrintVar("BEGIN PROCESS DUMP");
		{
			PROCESSENTRY32 entry = {};
			entry.dwSize = sizeof(PROCESSENTRY32);

			char szProcessName[MAXCHAR];

			Process32First(snapshot, &entry);
			do
			{
				std::wcstombs(szProcessName, entry.szExeFile, sizeof(szProcessName));

				std::string msg = std::to_string(++counter)
					+ " - Name:" + szProcessName
					+ " - ID:" + std::to_string(entry.th32ProcessID)
					+ " - Parent ID:" + std::to_string(entry.th32ParentProcessID);

				_logger->PrintVar(msg);

			} while (Process32Next(snapshot, &entry));
		}
		_logger->PrintVar("PROCESS DUMP COMPLETE", true);
	}
}

void cMemory::DumpModulesFromProcess(const DWORD& processID)
{
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		size_t counter = 0;

		_logger->PrintVar("BEGIN MODULE DUMP");
		{
			MODULEENTRY32 entry = {};
			entry.dwSize = sizeof(MODULEENTRY32);

			char szModulePath[MAXCHAR];
			char szModuleName[MAXCHAR];

			Module32First(snapshot, &entry);
			do
			{
				std::wcstombs(szModulePath, entry.szExePath, sizeof(szModulePath));
				std::wcstombs(szModuleName, entry.szModule, sizeof(szModuleName));

				std::string msg = std::to_string(++counter)
					+ " - Name:" + szModuleName
					+ " - Path:" + szModulePath;
				_logger->PrintVar(msg);

				msg = std::to_string(counter)
					+ " - Address:" + std::to_string((DWORD64)entry.modBaseAddr)
					+ " - Size:" + std::to_string((DWORD)entry.modBaseSize)
					+ " - Module ID:" + std::to_string((DWORD)entry.th32ModuleID)
					+ " - Process ID:" + std::to_string((DWORD)entry.th32ModuleID);
				_logger->PrintVar(msg);

			} while (Module32Next(snapshot, &entry));
		}
		_logger->PrintVar("MODULE DUMP COMPLETE");
	}
}

std::string cMemory::GetCurrentProcessName() const
{
	return _processName;
}

DWORD cMemory::GetCurrentProcessID() const
{
	return _processID;
}

HANDLE cMemory::GetCurrentProcessHandle() const
{
	return _processHandle;
}

bool cMemory::IsProcessAttatched() const
{
	return _attatched;
}

bool cMemory::AttatchProcess(const std::string& processName)
{
	PROCESSENTRY32 entry = {};
	entry.dwSize = sizeof(PROCESSENTRY32);
	char convertedName[MAXCHAR];

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		Process32First(snapshot, &entry);
		do
		{
			std::wcstombs(convertedName, entry.szExeFile, sizeof(convertedName));

			if (!processName.compare(convertedName))
			{
				_processName = processName;
				_processID = entry.th32ProcessID;
				_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _processID);

				CloseHandle(snapshot);

				_logger->PrintVar("Process " + processName + " HANDLE found. ID:" + std::to_string(_processID), true);

				_attatched = true;

				return true;
			}
		} while (Process32Next(snapshot, &entry));
	}

	_logger->PrintVar("Process " + processName + " not found", true);

	CloseHandle(snapshot);
	return false;
}

BYTE* cMemory::GetModuleAddress(const std::string& moduleName)
{
	MODULEENTRY32 entry = {};
	entry.dwSize = sizeof(MODULEENTRY32);
	char _moduleName[MAXCHAR];

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _processID);

	if (snapshot != INVALID_HANDLE_VALUE)
	{
		if (Module32First(snapshot, &entry))
		{
			std::wcstombs(_moduleName, entry.szModule, sizeof(_moduleName));
			if (!moduleName.compare(_moduleName))
			{
				CloseHandle(snapshot);
				return entry.modBaseAddr;
			}

			while (Module32Next(snapshot, &entry))
			{
				std::wcstombs(_moduleName, entry.szModule, sizeof(_moduleName));
				if (!moduleName.compare(_moduleName))
				{
					CloseHandle(snapshot);
					return entry.modBaseAddr;
				}
			}
		}
	}

	CloseHandle(snapshot);
	return nullptr;
}