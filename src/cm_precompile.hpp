// ProjectFilter(core)

#pragma once

#pragma region VK_REQUIRE_FEATURES
//[]===============================[]
//||    VULKAN REQUIRE FEATURES    ||
//[]===============================[]
#define VK_REQUIRE_FEATURE_GEOMETRY_SHADER
#define VK_REQUIRE_FEATURE_TESSELLATION_SHADER
#define VK_REQUIRE_FEATURE_SHADER_CULL_DISTANCE
#define VK_REQUIRE_FEATURE_SHADER_CLIP_DISTANCE
#define VK_REQUIRE_FEATURE_DEPTH_CLAMP
#define VK_REQUIRE_FEATURE_DEPTH_BOUNDS
#define VK_REQUIRE_FEATURE_DEPTH_BIAS_CLAMP
#define VK_REQUIRE_FEATURE_MULTI_VIEWPORT
#define VK_REQUIRE_FEATURE_ROBUST_BUFFER_ACCESS
#define VK_REQUIRE_FEATURE_FILL_MODE_NON_SOLID
#define VK_REQUIRE_FEATURE_INDEPENDENT_BLEND
#define VK_REQUIRE_FEATURE_SHADER_INT_64
#define VK_REQUIRE_FEATURE_SHADER_INT_16
#define VK_REQUIRE_DISCRETE_GPU
#pragma endregion

#pragma region VK_REQUIRE_EXTENSIONS
//[]=================================[]
//||    VULKAN REQUIRE EXTENSIONS    ||
//[]=================================[]
#define VK_REQUIRE_EXTENSION_SWAPCHAIN
#pragma endregion

#include "cc_vector.hpp"
#include "cc_matrix.hpp"
#include "cc_glfw.hpp"
#include "cc_vulkan.hpp"
#include "clockmaker.hpp"
