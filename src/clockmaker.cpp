// ProjectFilter (core)
#include "cm_precompile.hpp"
#include "clockmaker.hpp"

Clockmaker::Clockmaker()
	: myWindowManager(new CC_WindowManager(*this))
	, myVulkanManager(new CC_Vulkan(*this))
{}

void Clockmaker::Init()
{
	myWindowManager->InitWindow();
	myVulkanManager->InitVulkan();	
}

void Clockmaker::MainLoop()
{
	while (!myWindowManager->ShouldCloseWindow())
	{
		myWindowManager->PollEvents();
	}
#if RUNMODE == 1
	std::cout << "Exiting MainLoop" << std::endl;
#endif
}

void Clockmaker::Cleanup()
{
#if RUNMODE == 1
	std::cout << "Starting cleanup" << std::endl;
#endif

	myVulkanManager->Cleanup();
	myWindowManager->Cleanup();

#if RUNMODE == 1
	std::cout << "Finished cleanup" << std::endl;
#endif
}

CC_WindowManager& Clockmaker::GetWindowManager()
{
	return *myWindowManager;
}

void Clockmaker::Run()
{
	Init();
	MainLoop();
	Cleanup();
}
