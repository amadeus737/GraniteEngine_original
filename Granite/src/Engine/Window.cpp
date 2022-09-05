#include "Window.h"
#include <assert.h>
#include <wchar.h>

namespace Granite
{
	// Global variables
	BITMAPINFO _screenBufferInfo;
	void* _screenBuffer;

	Window::Window(HINSTANCE instance, std::wstring title, int width, int height, int left, int top, wchar_t* args)
		:
		_instance(instance),
		_args(args),
		_deviceContext(nullptr),
		_hWnd(nullptr),
		_deltaTime(0.0f),
		_time(0.0f),
		_fpsFrameCount(0),
		_fpsFrameDuration(0.0f),
		_FPS(0.0f),
		_fpsFrameBeginTime(std::chrono::steady_clock::time_point()),
		_isRunning(true),
		_screenWidth(800),
		_screenHeight(600)
	{
		// Setup screen dimensions
		_screenWidth = width;
		_screenHeight = height;

		// Register window class
		WNDCLASSEXW wc = { sizeof(WNDCLASSEX), CS_CLASSDC, HandleMessageSetup, 0, 0, _instance, nullptr, nullptr, nullptr, nullptr, _windowName, nullptr };

		// Assign icons and cursor
		wc.hIconSm = nullptr;
		wc.hIcon = nullptr;
		wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

		// Register the window class
		RegisterClassExW(&wc);

		// Create the window
		RECT rect;
		rect.left = left;
		rect.right = _screenWidth + rect.left;
		rect.top = top;
		rect.bottom = _screenHeight + rect.top;
		AdjustWindowRect(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
		_hWnd = CreateWindowW(_windowName, title.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, _instance, this);

		// Throw exception if something went terribly wrong
		if (!_hWnd)
		{
			MessageBoxW(0, L"CreateWindowEx failed", 0, 0);
		}

		// Get device context
		_deviceContext = GetDC(_hWnd);

		// Allocate screen buffer
		int bytesPerPixel = 4;
		_screenBuffer = VirtualAlloc(0, _screenWidth * _screenHeight * bytesPerPixel, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		// Setup screen buffer info
		// TODO : Experiment with changing width / height
		_screenBufferInfo.bmiHeader.biSize = sizeof(_screenBufferInfo.bmiHeader);
		_screenBufferInfo.bmiHeader.biWidth = _screenWidth;
		_screenBufferInfo.bmiHeader.biHeight = -_screenHeight;
		_screenBufferInfo.bmiHeader.biPlanes = 1;
		_screenBufferInfo.bmiHeader.biBitCount = 32;
		_screenBufferInfo.bmiHeader.biCompression = BI_RGB;

		// Show and update window
		ShowWindow(_hWnd, SW_SHOWDEFAULT);
		UpdateWindow(_hWnd);
	}

	Window::~Window()
	{
		// unregister window class
		UnregisterClassW(_windowName, _instance);
	}

	bool Window::IsActive() const
	{
		return GetActiveWindow() == _hWnd;
	}

	bool Window::IsMinimized() const
	{
		return IsIconic(_hWnd) != 0;
	}

	void Window::ShowMessageBox(const std::wstring& title, const std::wstring& message, UINT type) const
	{
		MessageBoxW(_hWnd, message.c_str(), title.c_str(), type);
	}

	bool Window::ProcessMessage()
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				return false;
			}
		}

		return true;
	}

	LRESULT WINAPI Window::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
		if (msg == WM_NCCREATE)
		{
			// extract ptr to window class from creation data
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const pWnd = reinterpret_cast<Window*>(pCreate->lpCreateParams);

			// sanity check
			assert(pWnd != nullptr);

			// set WinAPI-managed user data to store ptr to window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

			// set message proc to normal (non-setup) handler now that setup is finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMessageHelper));

			// forward message to window class handler
			return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
		}

		// if we get a message before the WM_NCCREATE message, handle with default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT WINAPI Window::HandleMessageHelper(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		// retrieve ptr to window class
		Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		// forward message to window class handler
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMessage(HWND window, UINT msg, WPARAM wp, LPARAM lp)
	{
		wchar_t out_msg[32];
		LRESULT result;
		switch (msg)
		{
		case WM_CLOSE:
			_isRunning = false;
			break;

		case WM_DESTROY:
			_isRunning = false;
			PostQuitMessage(0);
			break;

		case WM_KILLFOCUS:
			keyboard.ClearState();
			break;

		case WM_KEYDOWN:
			swprintf_s(out_msg, L"WM_KEYDOWN: 0x%x = %d\n", wp, wp);
			OutputDebugStringW(out_msg);
			if (!(lp & 0x40000000) || keyboard.AutorepeatIsEnabled()) // no thank you on the autorepeat
			{
				keyboard.OnKeyPressed(static_cast<unsigned char>(wp));
			}
			break;

		case WM_KEYUP:
			swprintf_s(out_msg, L"WM_KEYUP: 0x%x\n", wp);
			OutputDebugStringW(out_msg);
			keyboard.OnKeyReleased(static_cast<unsigned char>(wp));
			break;

		case WM_CHAR:
			swprintf_s(out_msg, L"WM_CHAR: %c\n", (wchar_t)wp);
			OutputDebugStringW(out_msg);
			keyboard.OnChar(static_cast<unsigned char>(wp));
			break;
		}

		return DefWindowProc(window, msg, wp, lp);
	}

	void Window::BeginFrame()
	{
		_fpsFrameBeginTime = std::chrono::high_resolution_clock::now();
	}

	void Window::Clear(Color color)
	{
		uint32_t* pixel = (uint32_t*)_screenBuffer;
		for (int index = 0; index < _screenWidth * _screenHeight; ++index)
		{
			*pixel++ = color.GetColorCode();
		}
	}

	void Window::DrawPixel(int x, int y, Color color)
	{
		DrawRect(x, y, 1, 1, color);
	}

	void Window::DrawRect(int x, int y, int width, int height, Color color)
	{
		for (int i = x; i < x + width; i++)
		{
			for (int j = y; j < y + height; j++)
			{
				uint32_t* pixel = (uint32_t*)_screenBuffer;
				pixel += j * _screenWidth + i;
				*pixel = color.GetColorCode();
			}
		}
	}

	void Window::DrawLine(int x1, int y1, int x2, int y2, Color c)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1; y = y1; xe = x2;
			}
			else
			{
				x = x2; y = y2; xe = x1;
			}

			DrawPixel(x, y, c);

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				DrawPixel(x, y, c);
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1; y = y1; ye = y2;
			}
			else
			{
				x = x2; y = y2; ye = y1;
			}

			DrawPixel(x, y, c);

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				DrawPixel(x, y, c);
			}
		}
	}

	void Window::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, Color c)
	{
		DrawLine(x0, y0, x1, y1, c);
		DrawLine(x1, y1, x2, y2, c);
		DrawLine(x2, y2, x0, y0, c);
	}

	void Window::RenderFrame()
	{
		StretchDIBits(_deviceContext, 0, 0, _screenWidth, _screenHeight, 0, 0, _screenWidth, _screenHeight, _screenBuffer, &_screenBufferInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	void Window::EndFrame()
	{
		_deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _fpsFrameBeginTime).count() * 0.001f * 0.001f * 0.001f;
		_time += _deltaTime;

		if (_fpsFrameCount >= 100)
		{
			if (_fpsFrameDuration < 0.001f)
				_fpsFrameDuration = 0.001f;

			float FPS = static_cast<float>(_fpsFrameCount) / _fpsFrameDuration;

			_fpsFrameCount = 0;
			_fpsFrameDuration = 0.0f;
		}
		else
		{
			_fpsFrameCount++;
			_fpsFrameDuration += _deltaTime;
		}
	}
}
