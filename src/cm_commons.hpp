// ProjectFilter (utilities)

#pragma once

#ifndef NDEBUG
#	define RUNMODE 1
#   define CC_Assert(Expr, Msg) \
    __CC_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#endif
#ifdef NDEBUG
#	define RUNMODE 0
#   define CC_Assert(Expr, Msg) ;
#endif

#define CM_WINDOW_NAME(CM_WIN_NAME) __CM_WINDOW_NAME(CM_WIN_NAME)

#include <cassert>

#pragma region STD_INCLUDES
//[]=================================[]
//||    STANDARD LIBRARY INCLUDES    ||
//[]=================================[]
#include <cmath>
#include <stdexcept>
#include <cstdint>
#include <string>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <chrono>
#include <vector>
#include <list>
#include <thread>
#include <atomic>
#include <fstream>
#include <map>
#include <functional>
#include <cstdint>
#include <limits>
#include <algorithm>
#include <array>
#include <cstring>
#include <optional>
#include <set>
#pragma endregion

#pragma region CM_LIBS
//[]==================================[]
//||        CLOCKMAKER INCLUDES       ||
//[]==================================[]
#include "cm_types.hpp"
#include "cm_math.hpp"
#pragma endregion

inline char* CM_WIN_NAME;

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

