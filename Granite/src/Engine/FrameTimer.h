#pragma once

#include <chrono>
#include "Core.h"

namespace Granite
{
	class GRANITE_API FrameTimer
	{
	public:
		FrameTimer()
		{
			last = std::chrono::steady_clock::now();
		}

		float Mark()
		{
			const auto old = last;
			last = std::chrono::steady_clock::now();

			const std::chrono::duration<float> frameTime = last - old;

			return frameTime.count();
		}

	private:
		std::chrono::steady_clock::time_point last;
	};
}