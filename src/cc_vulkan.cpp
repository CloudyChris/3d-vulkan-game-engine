// ProjectFilter (vulkan)

#include "cm_precompile.hpp"
#include "cc_vulkan.hpp"

CC_Vulkan::CC_Vulkan(Clockmaker& aClockmakerInstance)
	: theClockmaker(aClockmakerInstance)
	, myVulkanInstance(VK_NULL_HANDLE)
	, mySurface(VK_NULL_HANDLE)
{
#ifdef VK_REQUIRE_EXTENSION_DISPLAY
	myRequiredDeviceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#endif

#ifdef VK_REQUIRE_EXTENSION_SURFACE
	myRequiredDeviceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#endif

#ifdef VK_REQUIRE_EXTENSION_SWAPCHAIN
	myRequiredDeviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
#endif
}

void CC_Vulkan::InitVulkan()
{
	CreateInstance();
	CreateSurface();
	PickPhysicalDevice();
	CreateLogicalDevice();
	CreateSwapChain();
	CreateImageViews();
}

void CC_Vulkan::CreateInstance()
{
	VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = (char*)"Clockmaker";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = (char*)"Clockmaker";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &myVulkanInstance);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("[ VkError ] Failed to create Vulkan instance");
	}
#if RUNMODE == 1	
	std::cout << "VkInstance created" << std::endl;
#endif
}

void CC_Vulkan::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(myPhysicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

	VkPhysicalDeviceFeatures deviceFeatures{};

	// TODO: #ifdef like you did in PickPhysicalDevice to assign features to deviceFeatures
	
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(myRequiredDeviceExtensions.size());
	createInfo.ppEnabledExtensionNames = myRequiredDeviceExtensions.data();

	createInfo.enabledLayerCount = 0;

	VkResult result = vkCreateDevice(myPhysicalDevice, &createInfo, nullptr, &myLogicalDevice);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("[ VkError ]Failed to create logical device");
	}

#if RUNMODE == 1
	std::cout << "Created logical device" << std::endl;
#endif

	vkGetDeviceQueue(myLogicalDevice, indices.graphicsFamily.value(), 0, &myGraphicsQueue);

#if RUNMODE == 1
	std::cout << "Extracted graphics queue" << std::endl;
#endif

	vkGetDeviceQueue(myLogicalDevice, indices.presentFamily.value(), 0, &myPresentQueue);

#if RUNMODE == 1
	std::cout << "Extracted present queue" << std::endl;
#endif
}

void CC_Vulkan::CreateSwapChain()
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(myPhysicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.myFormats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.myPresentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.myCapabilities);

	uint32_t imageCount = swapChainSupport.myCapabilities.minImageCount + 1;

	if (swapChainSupport.myCapabilities.maxImageCount > 0 && imageCount > swapChainSupport.myCapabilities.maxImageCount)
	{
		imageCount = swapChainSupport.myCapabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = mySurface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(myPhysicalDevice);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = swapChainSupport.myCapabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VkResult result = vkCreateSwapchainKHR(myLogicalDevice, &createInfo, nullptr, &mySwapChain);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("[ VkError ] Failed to create swap chain");
	}

#if RUNMODE == 1
	std::cout << "Created swap chain" << std::endl;
#endif

	vkGetSwapchainImagesKHR(myLogicalDevice, mySwapChain, &imageCount, nullptr);

	mySwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(myLogicalDevice, mySwapChain, &imageCount, mySwapChainImages.data());

#if RUNMODE == 1
	std::cout << "Retrieved swap chain images" << std::endl;
#endif

	mySwapChainImageFormat = surfaceFormat.format;
	mySwapChainExtent = extent;

#if RUNMODE == 1
	std::cout << "Saved swap chain format and extent" << std::endl;
#endif
}

void CC_Vulkan::CreateImageViews()
{
	mySwapChainImageViews.resize(mySwapChainImages.size());

	for (size_t i = 0; i < mySwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = mySwapChainImages[i];

		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = mySwapChainImageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(myLogicalDevice, &createInfo, nullptr, &mySwapChainImageViews[i]);
		if(result != VK_SUCCESS)
		{
			throw std::runtime_error("[ VkError ] Failed to create image views");
		}
	}
#if RUNMODE == 1
	std::cout << "Created image views" << std::endl;
#endif
}

void CC_Vulkan::CreateSurface()
{
	VkResult result = glfwCreateWindowSurface(myVulkanInstance, theClockmaker.GetWindowManager().GetWindow(), nullptr, &mySurface);
	
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("[ VkError ] Failed to create window surface");
	}

#if RUNMODE == 1
	std::cout << "Created window surface" << std::endl;
#endif
}

void CC_Vulkan::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(myVulkanInstance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("[ VkError ] No physical device with Vulkan support found");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(myVulkanInstance, &deviceCount, devices.data());

	for (const VkPhysicalDevice& device : devices)
	{
		if(IsDeviceSuitable(device))
		{
			myPhysicalDevice = device;
			break;
		}
	}

	if (myPhysicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("[ VkError ] No physical device is suitable");	
	}

#if RUNMODE == 1
	VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(myPhysicalDevice, &deviceProperties);
	std::cout << "Picked physical device " << deviceProperties.deviceName << std::endl;
#endif
}

bool CC_Vulkan::IsDeviceSuitable(VkPhysicalDevice aDevice)
{
	VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(aDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(aDevice, &deviceFeatures);

#if RUNMODE == 1
	std::cout << "Checking physical device for features: " << deviceProperties.deviceName << std::endl;
#endif

#ifdef VK_REQUIRE_FEATURE_ROBUST_BUFFER_ACCESS
	if (!deviceFeatures.robustBufferAccess)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] robustBufferAccess" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_FULL_DRAW_INDEX_UINT_32
	if(!deviceFeatures.fullDrawIndexUint32)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] fullDrawIndexUint32" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_IMAGE_CUBE_ARRAY
	if(!deviceFeatures.imageCubeArray)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] imageCubeArray" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_INDEPENDENT_BLEND
	if(!deviceFeatures.independentBlend)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] independentBlend" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_GEOMETRY_SHADER
	if(!deviceFeatures.geometryShader)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] geometryShader" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_TESSELLATION_SHADER
	if(!deviceFeatures.tessellationShader)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] tessellationShader" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SAMPLE_RATE_SHADING
	if(!deviceFeatures.sampleRateShading)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sampleRateShading" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_DUAL_SRC_BLEND
	if(!deviceFeatures.dualSrcBlend)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] dualSrcBlend" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_LOGIC_OP
	if(!deviceFeatures.logicOp)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] logicOp" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_MULTI_DRAW_INDIRECT
	if(!deviceFeatures.multiDrawIndirect)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] multiDrawIndirect" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_DRAW_INDIRECT_FIRST_INSTANCE
	if(!deviceFeatures.drawIndirectFirstInstance)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] drawIndirectFirstInstance" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_DEPTH_CLAMP
	if(!deviceFeatures.depthClamp)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] depthClamp" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_DEPTH_BIAS_CLAMP
	if(!deviceFeatures.depthBiasClamp)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] depthBiasClamp" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_FILL_MODE_NON_SOLID
	if(!deviceFeatures.fillModeNonSolid)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] fillModeNonSolid" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_DEPTH_BOUNDS
	if(!deviceFeatures.depthBounds)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] depthBounds" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_WIDE_LINES
	if(!deviceFeatures.wideLines)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] wideLines" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_LARGE_POINTS
	if(!deviceFeatures.largePoints)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] largePoints" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_ALPHA_TO_ONE
	if(!deviceFeatures.alphaToOne)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] alphaToOne" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_MULTI_VIEWPORT
	if(!deviceFeatures.multiViewport)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] multiViewport" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SAMPLER_ANISOTROPY
	if(!deviceFeatures.samplerAnisotropy)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] samplerAnisotropy" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_TEXTURE_COMPRESSION_ETC2
	if(!deviceFeatures.textureCompressionETC2)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] textureCompressionETC2" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_TEXTURE_COMPRESSION_ASTC_LDR
	if(!deviceFeatures.textureCompressionASTC_LDR)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] textureCompressionASTC_LDR" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_TEXTURE_COMPRESSION_BC
	if(!deviceFeatures.textureCompressionBC)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] textureCompressionBC" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_OCCLUSION_QUERY_PRECISE
	if(!deviceFeatures.occlusionQueryPrecise)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] occlusionQueryPrecise" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_PIPELINE_STATISTICS_QUERY
	if(!deviceFeatures.pipelineStatisticsQuery)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] pipelineStatisticsQuery" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_VERTEX_PIPELINE_STORES_ANDATOMICS
	if(!deviceFeatures.vertexPipelineStoresAndAtomics)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] vertexPipelineStoresAndAtomics" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_FRAGMENT_STORES_AND_ATOMICS
	if(!deviceFeatures.fragmentStoresAndAtomics)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] fragmentStoresAndAtomics" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_TESSELLATION_AND_GEOMETRY_POINT_SIZE
	if(!deviceFeatures.shaderTessellationAndGeometryPointSize)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderTessellationAndGeometryPointSize" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_IMAGE_GATHER_EXTENDED
	if(!deviceFeatures.shaderImageGatherExtended)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderImageGatherExtended" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_STORAGE_IMAGE_EXTENDED_FORMATS
	if(!deviceFeatures.shaderStorageImageExtendedFormats)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderStorageImageExtendedFormats" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_STORAGE_IMAGE_MULTISAMPLE
	if(!deviceFeatures.shaderStorageImageMultisample)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderStorageImageMultisample" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_STORAGE_IMAGE_READ_WITHOUT_FORMAT
	if(!deviceFeatures.shaderStorageImageReadWithoutFormat)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderStorageImageReadWithoutFormat" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_STORAGE_IMAGE_WRITE_WITHOUT_FORMAT
	if(!deviceFeatures.shaderStorageImageWriteWithoutFormat)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderStorageImageWriteWithoutFormat" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_UNIFORM_BUFFER_ARRAY_DYNAMIC_INDEXING
	if(!deviceFeatures.shaderUniformBufferArrayDynamicIndexing)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderUniformBufferArrayDynamicIndexing" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADERVSAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING
	if(!deviceFeatures.shaderSampledImageArrayDynamicIndexing)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderSampledImageArrayDynamicIndexing" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_STORAGE_BUFFER_ARRAY_DYNAMIC_INDEXING
	if(!deviceFeatures.shaderStorageBufferArrayDynamicIndexing)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderStorageBufferArrayDynamicIndexing" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_STORAGE_IMAGE_ARRAY_DYNAMIC_INDEXING
	if(!deviceFeatures.shaderStorageImageArrayDynamicIndexing)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderStorageImageArrayDynamicIndexing" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_CLIP_DISTANCE
	if(!deviceFeatures.shaderClipDistance)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderClipDistance" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_CULL_DISTANCE
	if(!deviceFeatures.shaderCullDistance)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderCullDistance" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_FLOAT_64
	if(!deviceFeatures.shaderFloat64)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderFloat64" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_INT_64
	if(!deviceFeatures.shaderInt64)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderInt64" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_INT_16
	if(!deviceFeatures.shaderInt16)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderInt16" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_RESOURCE_RESIDENCY
	if(!deviceFeatures.shaderResourceResidency)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderResourceResidency" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SHADER_RESOURCE_MIN_LOD
	if(!deviceFeatures.shaderResourceMinLod)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] shaderResourceMinLod" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_BINDING
	if(!deviceFeatures.sparseBinding)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseBinding" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_RESIDENCY_BUFFER
	if(!deviceFeatures.sparseResidencyBuffer)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseResidencyBuffer" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_RESIDENCY_IMAGE_2D
	if(!deviceFeatures.sparseResidencyImage2D)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseResidencyImage2D" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_RESIDENCY_IMAGE_3D
	if(!deviceFeatures.sparseResidencyImage3D)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseResidencyImage3D" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_RESIDENCY_2_SAMPLES
	if(!deviceFeatures.sparseResidency2Samples)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseResidency2Samples" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_RESIDENCY_4_SAMPLES
	if(!deviceFeatures.sparseResidency4Samples)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseResidency4Samples" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_RESIDENCY_8_SAMPLES
	if(!deviceFeatures.sparseResidency8Samples)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseResidency8Samples" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_RESIDENCY_16_SAMPLES
	if(!deviceFeatures.sparseResidency16Samples)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseResidency16Samples" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_SPARSE_RESIDENCY_ALIASED
	if(!deviceFeatures.sparseResidencyAliased)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] sparseResidencyAliased" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_VARIABLE_MULTISAMPLE_RATE
	if(!deviceFeatures.variableMultisampleRate)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] variableMultisampleRate" << std::endl;
	#endif
#endif
#ifdef VK_REQUIRE_FEATURE_INHERITED_QUERIES
	if(!deviceFeatures.inheritedQueries)
	{
		return false;
	}
	#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] inheritedQueries" << std::endl;
	#endif
#endif

	QueueFamilyIndices indices = FindQueueFamilies(aDevice);
	if (!indices.IsComplete())
	{
		return false;
	}
#if VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] has graphics family and present family" << std::endl;
#endif

	if(!CheckDeviceExtensionSupport(aDevice))
	{
		return false;
	}
#if VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] supports required extensions" << std::endl;
#endif

	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(aDevice);
	if (swapChainSupport.myFormats.empty() || swapChainSupport.myPresentModes.empty())
	{
		return false;
	}
#if VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] has swapchain support" << std::endl;
#endif

#ifdef VK_REQUIRE_DISCRETE_GPU
	if(deviceProperties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		return false;
	}
	#if VK_DEBUG_FEATURES
	std::cout << "[ VkDebug ] is discrete GPU" << std::endl;
	#endif
#endif

	return true;
}

bool CC_Vulkan::CheckDeviceExtensionSupport(VkPhysicalDevice aDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(aDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(aDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(myRequiredDeviceExtensions.begin(), myRequiredDeviceExtensions.end());

	for (const VkExtensionProperties& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

CC_Vulkan::QueueFamilyIndices CC_Vulkan::FindQueueFamilies(VkPhysicalDevice aDevice)
{
    QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(aDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(aDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const VkQueueFamilyProperties& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(aDevice, i, mySurface, &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = i;
		}

		if (indices.IsComplete())
		{
			break;
		}

		i++;
	}

    return indices;
}

CC_Vulkan::SwapChainSupportDetails CC_Vulkan::QuerySwapChainSupport(VkPhysicalDevice aDevice)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(aDevice, mySurface, &details.myCapabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(aDevice, mySurface, &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.myFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(aDevice, mySurface, &formatCount, details.myFormats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(aDevice, mySurface, &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.myPresentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(aDevice, mySurface, &presentModeCount, details.myPresentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR CC_Vulkan::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& aSurfaceFormats)
{
	for (const VkSurfaceFormatKHR& surfaceFormat : aSurfaceFormats)
	{
		if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return surfaceFormat;
		}
	}

	return aSurfaceFormats[0];
}

VkPresentModeKHR CC_Vulkan::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& aPresentModes)
{
	for (const VkPresentModeKHR& presentMode : aPresentModes)
	{
		if(presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D CC_Vulkan::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& aCapabilities)
{
	if (aCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return aCapabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(theClockmaker.GetWindowManager().GetWindow(), &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, aCapabilities.minImageExtent.width, aCapabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, aCapabilities.minImageExtent.height, aCapabilities.maxImageExtent.height);
	
		return actualExtent;
	}
}

void CC_Vulkan::DestroyImageViews()
{
	for (VkImageView imageView : mySwapChainImageViews)
	{
		vkDestroyImageView(myLogicalDevice, imageView, nullptr);
	}
}

void CC_Vulkan::Cleanup()
{
	DestroyImageViews();
	vkDestroySwapchainKHR(myLogicalDevice, mySwapChain, nullptr);
	vkDestroyDevice(myLogicalDevice, nullptr);
	vkDestroySurfaceKHR(myVulkanInstance, mySurface, nullptr);
	vkDestroyInstance(myVulkanInstance, nullptr);

#if RUNMODE == 1
	std::cout << "Vulkan cleanup complete" << std::endl;
#endif
}
