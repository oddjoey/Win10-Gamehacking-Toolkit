#include "cLogger.h"

std::shared_ptr<cLogger> cLogger::_instance = nullptr;

std::shared_ptr<cLogger> cLogger::Get()
{
	if (_instance == nullptr)
		_instance = std::make_shared<cLogger>();

	return _instance;
}

cLogger::cLogger() : _formatWidth(0), _formatChar(' ')
{

}

cLogger::~cLogger()
{
}

cLogger* cLogger::SetWidth(const std::streamsize width)
{
	_formatWidth = width;
	return this;
}

cLogger* cLogger::SetChar(const char& character)
{
	_formatChar = character;
	return this;
}
