#include "cRainDear.h"

int main()
{
	std::unique_ptr<cRainDear> toolkit = std::make_unique<cRainDear>();
	toolkit->GetMemoryInterface()->AttatchProcess("ffxiv_dx11.exe");
	toolkit->DisableAntiDebugger();
}