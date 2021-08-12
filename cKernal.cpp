#include "cKernal.h"

cKernal_t cKernal::_instance = nullptr;

cKernal_t cKernal::Get()
{
    if (_instance == nullptr)
        _instance = std::make_shared<cKernal>();

    return _instance;
}

cKernal::cKernal()
{
    if (_logger == nullptr)
        _logger = cLogger::Get();

    _ntDLL = GetModuleHandleA("ntdll");
    if (!_ntDLL)
        return;

    _fnNtQueryInformationProces = reinterpret_cast<pNtQueryInformationProcess>(GetProcAddress(_ntDLL, "NtQueryInformationProcess"));
    if (!_fnNtQueryInformationProces)
        return;


}

cKernal::~cKernal()
{
}

void* cKernal::GetPEBFromHandle(const HANDLE& handle)
{
    PROCESS_BASIC_INFORMATION pbi = {};
    NtQueryInformationProcess(handle, 0, &pbi, sizeof(PROCESS_BASIC_INFORMATION), 0);
    return pbi.PebBaseAddress;
}

DWORD cKernal::NtQueryInformationProcess(HANDLE ProcessHandle, DWORD ProcessInformationClass, PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength)
{
    if (!_fnNtQueryInformationProces)
        return NULL;

    return _fnNtQueryInformationProces(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
}
