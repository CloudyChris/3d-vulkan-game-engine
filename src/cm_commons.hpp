// ProjectFilter (utilities)

#pragma once

// RUNMODE , ASSERT
#ifndef NDEBUG
#	define RUNMODE 1
#   define CC_Assert(Expr, Msg) \
    __CC_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#endif
#ifdef NDEBUG
#	define RUNMODE 0
#   define CC_Assert(Expr, Msg) ;
#endif

// STAT , DEBUG , VK_DEBUG
#define CC_STAT_LOAD "[ ... ]"
#define CC_STAT_OK	 "[ OK ]"
#define CC_STAT_WARN "[ WARN ]"
#define CC_STAT_ERR  "[ ERR ]"

#define CC_STAT(Stat, Msg) \
__CC_STAT(Stat, Msg)

#if RUNMODE==0
#	define CC_DEBUG(Msg) ;
#	define CC_VK_DEBUG(Msg) ;
#else
#	define CC_DEBUG(Msg) \
	__CC_DEBUG(Msg)
#	define CC_VK_DEBUG(Msg) \
		__CC_VK_DEBUG(Msg)
#endif 

#define CM_WINDOW_NAME(CM_WIN_NAME) __CM_WINDOW_NAME(CM_WIN_NAME)

#include <cassert>

#pragma region STD_INCLUDES
//[]=================================[]
//||    STANDARD LIBRARY INCLUDES    ||
//[]=================================[]
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <functional>
#include <fstream>
#include <iostream>
#include <list>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <thread>
#include <vector>
#pragma endregion

#pragma region CM_LIBS
//[]==================================[]
//||        CLOCKMAKER INCLUDES       ||
//[]==================================[]
#include "cm_types.hpp"
#include "cm_math.hpp"
#pragma endregion

inline char* CM_WIN_NAME;

inline void __CC_STAT(const char* stat, const char* stat_msg)
{
	std::cout << stat << " " << stat_msg << std::endl;
	return;
}

inline void __CC_DEBUG(const char* debug_msg)
{
	std::cout << "[ DEBUG ] " << debug_msg << std::endl;
	return;
}

inline void __CC_VK_DEBUG(const char* debug_msg)
{
#ifdef VK_DEBUG_FEATURES
	std::cout << "[ VK_DEBUG ] " << debug_msg << std::endl;
#endif
	return;
}

inline void __CC_Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
{
    if (!expr)
	{
        std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}

inline void __CM_WINDOW_NAME(char* aWindowName)
{
	CM_WIN_NAME = aWindowName;
}

inline std::string RUNMODE_STR()
{
	switch (RUNMODE)
	{
		case 0:
			return "RELEASE";
		case 1:
			return "DEBUG";
		default:
			break;
	}
}

