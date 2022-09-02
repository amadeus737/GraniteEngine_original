#pragma once

#include <stdint.h>
#include "Core.h"

namespace Granite
{
	class GRANITE_API Color
	{
	public:
		constexpr Color() : _colorCode() {}
		constexpr Color(const Color& c) : _colorCode(c._colorCode) {}
		constexpr Color(unsigned int code) : _colorCode(code) {}
		constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
			: _colorCode((x << 24u) | (r << 16u) | (g << 8u) | b) {}
		constexpr Color(unsigned char r, unsigned char g, unsigned char b)
			: _colorCode((r << 16u) | (g << 8u) | b) {}
		constexpr Color(Color c, unsigned char x)
			: Color((x << 24u) | c.GetColorCode()) {}

		Color& operator =(Color c)
		{
			_colorCode = c.GetColorCode();
			return *this;
		}

		unsigned int GetColorCode() { return _colorCode; }

		constexpr unsigned char x() const { return _colorCode >> 24u; }
		constexpr unsigned char a() const { return x(); }
		constexpr unsigned char r() const { return (_colorCode >> 16u) & 0xFFu; }
		constexpr unsigned char g() const { return (_colorCode >> 8u) & 0xFFu; }
		constexpr unsigned char b() const { return _colorCode & 0xFFu; }

		void set_x(unsigned char x) { _colorCode = (_colorCode & 0xFFFFFFu) | (x << 24u); }
		void set_a(unsigned char a) { set_x(a); }
		void SetR(unsigned char r) { _colorCode = (_colorCode & 0xFF00FFFFu) | (r << 16u); }
		void SetG(unsigned char g) { _colorCode = (_colorCode & 0xFFFF00FFu) | (g << 8u); }
		void SetB(unsigned char b) { _colorCode = (_colorCode & 0xFFFFFF00u) | b; }

		static unsigned int MakeRGB(unsigned char r, unsigned char g, unsigned char b) { return (r << 16) | (g << 8) | b; }
		static unsigned int White() { return MakeRGB(255u, 255u, 255u); }
		static unsigned int Black() { return MakeRGB(0u, 0u, 0u); }
		static unsigned int Gray() { return MakeRGB(0x80u, 0x80u, 0x80u); }
		static unsigned int LightGray() { return MakeRGB(0xD3u, 0xD3u, 0xD3u); }
		static unsigned int Red() { return MakeRGB(255u, 0u, 0u); }
		static unsigned int Green() { return  MakeRGB(0u, 255u, 0u); }
		static unsigned int Blue() { return  MakeRGB(0u, 0u, 255u); }
		static unsigned int Yellow() { return  MakeRGB(255u, 255u, 0u); }
		static unsigned int Cyan() { return MakeRGB(0u, 255u, 255u); }
		static unsigned int Magenta() { return MakeRGB(255u, 0u, 255u); }

	private:
		unsigned int _colorCode;
	};
}