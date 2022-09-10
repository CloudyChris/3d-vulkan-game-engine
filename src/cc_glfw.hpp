// ProjectFilter(glfw)

#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "cm_commons.hpp"

class CC_WindowManager
{
public:
	CC_WindowManager(Clockmaker& aClockmakerInstance);

	void InitWindow();
	bool ShouldCloseWindow();
	void PollEvents();
	void Cleanup();

	GLFWwindow* GetWindow();

private:
	Clockmaker& theClockmaker;

	uint32_t WIDTH;
	uint32_t HEIGHT;
	GLFWwindow* myWindow;
};
