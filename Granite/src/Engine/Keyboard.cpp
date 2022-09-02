#include "Keyboard.h"

namespace Granite
{
	// Global variables
	std::bitset<Keyboard::_nKeys> GRANITE_API _keystates;

	bool Keyboard::KeyIsPressed(unsigned char keycode) const
	{
		return _keystates[keycode];
	}

	Keyboard::Event Keyboard::ReadKey()
	{
		if (_keybuffer.size() > 0u)
		{
			Keyboard::Event e = _keybuffer.front();
			_keybuffer.pop();

			return e;
		}
		else
		{
			return Keyboard::Event();
		}
	}

	bool Keyboard::KeyIsEmpty() const
	{
		return _keybuffer.empty();
	}

	char Keyboard::ReadChar()
	{
		if (_charbuffer.size() > 0u)
		{
			unsigned char charcode = _charbuffer.front();
			_charbuffer.pop();
			return charcode;
		}
		else
		{
			return 0;
		}
	}

	bool Keyboard::CharIsEmpty() const
	{
		return _charbuffer.empty();
	}

	void Keyboard::FlushKey()
	{
		_keybuffer = std::queue<Keyboard::Event>();
	}

	void Keyboard::FlushChar()
	{
		_charbuffer = std::queue<char>();
	}

	void Keyboard::Flush()
	{
		FlushKey();
		FlushChar();
	}

	void Keyboard::EnableAutorepeat()
	{
		_autorepeatEnabled = true;
	}

	void Keyboard::DisableAutorepeat()
	{
		_autorepeatEnabled = false;
	}

	bool Keyboard::AutorepeatIsEnabled() const
	{
		return _autorepeatEnabled;
	}

	void Keyboard::OnKeyPressed(unsigned char keycode)
	{
		_keystates[keycode] = true;

		if (_keybuffer.size() < _bufferSize)
			_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));

		TrimBuffer(_keybuffer);
	}

	void Keyboard::OnKeyReleased(unsigned char keycode)
	{
		_keystates[keycode] = false;

		if (_keybuffer.size() < _bufferSize)
			_keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));

		TrimBuffer(_keybuffer);
	}

	void Keyboard::OnChar(char character)
	{
		if (_charbuffer.size() < _bufferSize)
			_charbuffer.push(character);

		TrimBuffer(_charbuffer);
	}

	void Keyboard::ClearState()
	{
		_keystates.reset();
	}

	template<typename T>
	void Keyboard::TrimBuffer(std::queue<T>& buffer)
	{
		while (buffer.size() > _bufferSize)
		{
			buffer.pop();
		}
	}
}