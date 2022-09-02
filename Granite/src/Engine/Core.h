#pragma once

#ifdef GRANITE_PLATFORM_WINDOWS
	#ifdef GRANITE_BUILD_DLL
		#define GRANITE_API __declspec(dllexport)
	#else
		#define GRANITE_API __declspec(dllimport)
	#endif
#else
	#error Granite only supports Windows!
#endif