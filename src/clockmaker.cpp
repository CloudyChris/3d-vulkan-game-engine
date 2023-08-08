// ProjectFilter (core)
#include "cm_precompile.hpp"
#include "clockmaker.hpp"

Clockmaker::Clockmaker()
	: myWindowManager(new CC_WindowManager(*this))
	, myVulkanManager(new CC_Vulkan(*this))
{}

Clockmaker::~Clockmaker()
{
	delete myWindowManager;
	delete myVulkanManager;
}

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
	CC_STAT(CC_STAT_OK, "Exiting MainLoop");
}

void Clockmaker::Cleanup()
{
	CC_STAT(CC_STAT_LOAD, "Starting cleanup");

	myVulkanManager->Cleanup();
	myWindowManager->Cleanup();

	CC_STAT(CC_STAT_OK, "Finished cleanup");
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
