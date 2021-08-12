#include "cPortableExecutable.h"

cPortableExecutable::cPortableExecutable(const std::string imagePath) : _path(imagePath)
{
	// Our logger class to output info
	_logger = cLogger::Get();

	// Create input stream to out image file in binary mode
	std::ifstream fileStream(_path, std::ios::binary);
	if (!fileStream.is_open())
		return;

	// Calculating image size on disc
	fileStream.seekg(0, std::ios::end);
	unsigned int sizeOnDisc = static_cast<unsigned int>(fileStream.tellg());
	
	// Creating buffer w/ size found above
	_imageBase = malloc(sizeOnDisc);
	if (!_imageBase)
		return;

	// Set seekg back to beginning of file to read into our buffer
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(reinterpret_cast<char*>(_imageBase), sizeOnDisc);

	// Pointers to different parts of the image
	_dosHeader		= reinterpret_cast<PIMAGE_DOS_HEADER>(_imageBase);
	_peHeader		= reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<char*>(_imageBase) + _dosHeader->e_lfanew);
	_coffHeader		= reinterpret_cast<PIMAGE_FILE_HEADER>(&_peHeader->FileHeader);
	_optionalHeader = reinterpret_cast<PIMAGE_OPTIONAL_HEADER>(&_peHeader->OptionalHeader);
	_sectionHeader = IMAGE_FIRST_SECTION(_peHeader);

	// Creating a new buffer with VirtualAlloc and using the size in memory instead of disk
	//_imageBase = VirtualAlloc(nullptr, _sizeInMem, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	//if (!_imageBase)
	//	return;

	// Copy image headers into our new buffer
	//memcpy(_imageBase, imageLoaded, _sizeOfHeaders);
	//_logger->PrintInfo("Copied headers into buffer", true);

	// Copy image sections
	//PIMAGE_SECTION_HEADER currentSection = _sectionHeader;
	//for (int i = 0; i < _imageNumberOfSections; i++)
	//{
	//	memcpy(	reinterpret_cast<char*>(_imageBase) + currentSection->VirtualAddress,
	//			reinterpret_cast<char*>(imageLoaded) + currentSection->PointerToRawData,
	//			currentSection->SizeOfRawData);

	//	_logger->PrintVar<>("Section name", currentSection->Name);
	//	_logger->PrintVar<>("Section virtual address", currentSection->VirtualAddress);
	//	_logger->PrintVar<>("Section rva to raw data", currentSection->PointerToRawData);
	//	_logger->PrintVar<>("Section size of raw data", currentSection->SizeOfRawData, false, true);

	//	currentSection++;
	//}
}

cPortableExecutable::~cPortableExecutable()
{
	if (_imageBase)
		free(_imageBase);
}

std::shared_ptr<RelocationTable_t> cPortableExecutable::GetRelocations()
{
	if (!_imageBase || !_optionalHeader)
		return nullptr;

	DWORD relocationTableRVA = _optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	if (!relocationTableRVA)
		return nullptr;

	auto currentBaseReloc = reinterpret_cast<PIMAGE_BASE_RELOCATION>(reinterpret_cast<DWORD64>(_imageBase) + relocationTableRVA);

	std::shared_ptr<RelocationTable_t> rtnTable = std::make_shared<RelocationTable_t>();
	while (currentBaseReloc->VirtualAddress 
		< (_optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size 
		+ reinterpret_cast<DWORD64>(currentBaseReloc)))
	{
		_logger->PrintVar<>(VARNAME(currentBaseReloc->VirtualAddress), currentBaseReloc->VirtualAddress, true);
		_logger->PrintVar<>(VARNAME(currentBaseReloc->SizeOfBlock), currentBaseReloc->SizeOfBlock, true);
		currentBaseReloc += currentBaseReloc->SizeOfBlock;
	}

	return rtnTable;
}
