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

		bool IsEqual(const Color& c)
		{
			return _colorCode == c._colorCode;
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

		//static unsigned int MakeRGB(unsigned char r, unsigned char g, unsigned char b) { return (r << 16) | (g << 8) | b; }
		static constexpr Color White() { return Color(255u, 255u, 255u); }
		static constexpr Color Black() { return Color(0u, 0u, 0u); }
		static constexpr Color MediumGray() { return Color(0x60u, 0x60u, 0x60u); }
		static constexpr Color Gray() { return Color(0x80u, 0x80u, 0x80u); }
		static constexpr Color LightGray() { return Color(0xD3u, 0xD3u, 0xD3u); }
		static constexpr Color Red() { return Color(255u, 0u, 0u); }
		static constexpr Color Green() { return  Color(0u, 255u, 0u); }
		static constexpr Color Blue() { return  Color(0u, 0u, 255u); }
		static constexpr Color Yellow() { return  Color(255u, 255u, 0u); }
		static constexpr Color Orange() { return Color(255u, 165u, 0u); }
		static constexpr Color Cyan() { return Color(0u, 255u, 255u); }
		static constexpr Color Magenta() { return Color(255u, 0u, 255u); }
		static constexpr Color Purple() { return Color(180u, 0u, 180u); }
		static constexpr Color Granite() { return Color(0x33u, 0x33u, 0x33u); }

	private:
		unsigned int _colorCode;
	};
}