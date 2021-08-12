#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <iomanip>

#define VARNAME(x) #x

enum ePrintType
{
	INFO,
	ERROR,

};

class cLogger
{
	static std::shared_ptr<cLogger> _instance;
public:
	static std::shared_ptr<cLogger> Get();

	cLogger* SetWidth(const std::streamsize width);
	cLogger* SetChar(const char& character);

	template <typename T>
	void PrintVar(const std::string& name, const T& var, const bool& hex = false, const bool& newLine = false)
	{

	}
	
	cLogger();
	~cLogger();
private:
	std::streamsize _formatWidth;
	char _formatChar;
};

typedef std::shared_ptr<cLogger> cLogger_t;