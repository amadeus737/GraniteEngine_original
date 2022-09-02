#pragma once

#include <Windows.h>
#include <string>
#include <chrono>
#include "Core.h"
#include "Color.h"

namespace Granite
{
	// Global variables
	bool GRANITE_API _keystates[256];

	class GRANITE_API WindowProperties
	{
	public:
		WindowProperties()
			:
			_title(L"Granite Engine"),
			_width(800),
			_height(600),
			_left(10),
			_top(100)
		{ }

		WindowProperties(std::wstring wt, int w, int h, int l, int t)
			:
			_title(wt),
			_width(w),
			_height(h),
			_left(l),
			_top(t)
		{ }

		void Assign(std::wstring wt, int w, int h, int l, int t)
		{
			_title = wt;
			_width = w;
			_height = h;
			_left = l;
			_top = t;
		}

		std::wstring title() { return _title; }
		int width() { return _width; }
		int height() { return _height; }
		int left() { return _left; }
		int top() { return _top; }

	private:
		std::wstring _title;
		int _width;
		int _height;
		int _left;
		int _top;
	};

	class GRANITE_API Window
	{
	public:
		Window() = default;
		Window(HINSTANCE instance, std::wstring title, int width, int height, int left, int top, wchar_t* args);
		~Window();

		bool IsActive() const;
		bool IsMinimized() const;
		void ShowMessageBox(const std::wstring& title, const std::wstring& message, UINT type = MB_OK) const;
		void Kill() { _isRunning = false;  PostQuitMessage(0); }

		void Clear(Color c);
		void DrawPixel(int x, int y, Color c);
		void DrawRect(int x, int y, int width, int height, Color c);

		void BeginFrame();
		void RenderFrame();
		void EndFrame();
		float GetFPS() { return _FPS; }

		bool ProcessMessage();
		const std::wstring& GetArgs() const { return _args; }

		bool IsRunning() { return _isRunning; }

	private:
		static LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT WINAPI HandleMessageHelper(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM w, LPARAM l);

	private:
		static constexpr const wchar_t* _windowName = L"Granite Engine Render Window";
		HINSTANCE _instance = nullptr;
		std::wstring _args;
		HDC _deviceContext;
		HWND _hWnd;
		float _deltaTime;
		float _time;
		int _fpsFrameCount;
		float _fpsFrameDuration;
		float _FPS;
		std::chrono::steady_clock::time_point _fpsFrameBeginTime;
		bool _isRunning = true;
		int _screenWidth;
		int _screenHeight;
	};
}