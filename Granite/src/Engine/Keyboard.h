#pragma once

#include "Core.h"
#include <queue>
#include <bitset>

namespace Granite
{
	class GRANITE_API Keyboard
	{
		friend class Window;

	public:
		class Event
		{
		public:
			enum class Type
			{
				Press,
				Release,
				Invalid
			};

		private:
			Type _type;
			unsigned char _keyCode;

		public:
			Event()
				:
				_type(Type::Invalid),
				_keyCode(0u)
			{}

			Event(Type type, unsigned char code)
				:
				_type(type),
				_keyCode(code)
			{}

			bool IsPress() const
			{
				return _type == Type::Press;
			}

			bool IsRelease() const
			{
				return _type == Type::Release;
			}

			bool IsValid() const
			{
				return _type != Type::Invalid;
			}

			unsigned char GetKeyCode() const
			{
				return _keyCode;
			}
		};

	public:
		Keyboard() = default;
		//Keyboard(const Keyboard&) = delete;
		//	Keyboard& operator=(const Keyboard&) = delete;

		bool KeyIsPressed(unsigned char keycode) const;
		Event ReadKey();
		bool KeyIsEmpty() const;
		char ReadChar();
		bool CharIsEmpty() const;
		void FlushKey();
		void FlushChar();
		void Flush();
		void EnableAutorepeat();
		void DisableAutorepeat();
		bool AutorepeatIsEnabled() const;

	private:
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		void OnChar(char character);
		void ClearState();
		template<typename T>
		void TrimBuffer(std::queue<T>& buffer);

	public:
		static constexpr unsigned int _nKeys = 256u;

	private:
		static constexpr unsigned int _bufferSize = 4u;
		bool _autorepeatEnabled = false;
		std::queue<Event> _keybuffer;
		std::queue<char> _charbuffer;
	};
}