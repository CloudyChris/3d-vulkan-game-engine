// ProjectFilter (core)

#pragma once

#include "cm_commons.hpp"

class Clockmaker
{
public:
	Clockmaker();
	void Init();
	void MainLoop();
	void Cleanup();
	void Run();

	CC_WindowManager& GetWindowManager();

private:
CC_WindowManager* myWindowManager;
CC_Vulkan* myVulkanManager;
};
