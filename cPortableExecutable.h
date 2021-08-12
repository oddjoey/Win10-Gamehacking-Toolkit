#pragma once

#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <memory>
#include <vector>

#include "cLogger.h"

struct RELOC_INFO
{
	DWORD* virtualAddress = nullptr;
	DWORD* firstItemAddress = nullptr;
	unsigned int numberOfItems = 0;
};

typedef std::vector<RELOC_INFO> RelocationTable_t;

class cPortableExecutable
{
public:
	cPortableExecutable(const std::string imagePath);
	~cPortableExecutable();

	std::shared_ptr<RelocationTable_t> GetRelocations();
private:
	std::string						_path = "";

	WORD							_imageNumberOfSections = 0;

	void*							_imageBase = nullptr;
	IMAGE_DOS_HEADER*				_dosHeader = nullptr;
	IMAGE_NT_HEADERS*				_peHeader = nullptr;
	IMAGE_FILE_HEADER*				_coffHeader = nullptr;
	IMAGE_OPTIONAL_HEADER*			_optionalHeader = nullptr;
	IMAGE_SECTION_HEADER*			_sectionHeader = nullptr;

	cLogger_t						_logger = nullptr;
};

typedef std::shared_ptr<cPortableExecutable> cPortableExecutable_t;
