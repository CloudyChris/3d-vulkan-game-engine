// ProjectFilter (vulkan)

#pragma once

#include "cc_glfw.hpp"
#include "cm_commons.hpp"

class CC_Vulkan
{
public:
	CC_Vulkan(Clockmaker& aClockmakerInstance);

	void InitVulkan();
	void Cleanup();

private:
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool IsComplete()
		{
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR myCapabilities;
		std::vector<VkSurfaceFormatKHR> myFormats;
		std::vector<VkPresentModeKHR> myPresentModes;
	};

	void CreateInstance();
	void CreateSurface();
	void CreateLogicalDevice();
	void CreateSwapChain();
	void CreateImageViews();

	// cleanup
	void DestroyImageViews();

	// picking a suitable physical device
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice aDevice);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice aDevice);

	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice aDevice);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice aDevice);

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& aSurfaceFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& aPresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& aCapabilities);

	Clockmaker& theClockmaker;

	VkInstance myVulkanInstance;

	VkPhysicalDevice myPhysicalDevice;
	std::vector<const char*> myRequiredDeviceExtensions;

	VkDevice myLogicalDevice;
	VkQueue myGraphicsQueue;
	VkQueue myPresentQueue;

	VkSurfaceKHR mySurface;

	VkSwapchainKHR mySwapChain;
	std::vector<VkImage> mySwapChainImages;
	std::vector<VkImageView> mySwapChainImageViews;
	VkFormat mySwapChainImageFormat;
	VkExtent2D mySwapChainExtent;
};
