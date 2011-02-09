// lunchbox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <celsus/error2.hpp>
#include <celsus/Logger.hpp>
#include <celsus/graphics.hpp>

class App
{
public:
	bool init();
	bool close();
	bool run();

	static App &instance();
private:
	App();
	bool create_window();
	void set_client_size();
	static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static App *_instance;

	HINSTANCE _hinstance;
	HWND _hwnd;
	DWORD _width;
	DWORD _height;
};

App *App::_instance;

App &App::instance()
{
	if (!_instance)
		_instance = new App();
	return *_instance;
}

LRESULT App::wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) 
	{
	case WM_SIZE:
		{
			const int w = LOWORD(lParam);
			const int h = HIWORD(lParam);
			Graphics::instance().resize(w, h);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		break;

	case WM_MOUSEMOVE:
		break;

	case WM_MOUSEWHEEL:
		break;

	case WM_KEYDOWN:
		break;

	case WM_CHAR:
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

App::App()
	: _hinstance(NULL)
	, _hwnd(NULL)
{
}

void App::set_client_size()
{
	// Adjust the window size to make the client size match the request size
	RECT client_rect;
	RECT window_rect;
	GetClientRect(_hwnd, &client_rect);
	GetWindowRect(_hwnd, &window_rect);
	window_rect.right -= window_rect.left;
	window_rect.bottom -= window_rect.top;
	window_rect.left = window_rect.top = 0;
	const int dx = window_rect.right - client_rect.right;
	const int dy = window_rect.bottom - client_rect.bottom;

	SetWindowPos(_hwnd, NULL, -1, -1, _width + dx, _height + dy, SWP_NOZORDER | SWP_NOREPOSITION);
}

bool App::create_window()
{
	const char* kClassName = "LunchboxEditor";

	WNDCLASSEXA wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc    = wnd_proc;
	wcex.hInstance      = _hinstance;
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName  = kClassName;

	RETURN_ON_FAIL_BOOL_W(RegisterClassExA(&wcex));

	const UINT window_style = WS_VISIBLE | WS_POPUP | WS_OVERLAPPEDWINDOW;

	_hwnd = CreateWindowA(kClassName, "lunchbox editor - magnus österlind - 2010", window_style,
		CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL,
		_hinstance, NULL);

	set_client_size();

	ShowWindow(_hwnd, SW_SHOW);

	return true;

}

bool App::init()
{
	_width = 1024;
	_height = 768;
	if (!create_window())
		return false;

	if (!Graphics::instance().init_directx(_hwnd, _width, _height))
		return false;

	return true;
}

bool App::close()
{
	if (!Graphics::instance().close())
		return false;

	return true;
}

bool App::run()
{
	MSG msg = {0};

	while (msg.message != WM_QUIT) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			Graphics::instance().present();
		}
	}
	return true;
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	App &app = App::instance();

	if (!app.init())
		return 1;

	if (!app.run())
		return 1;

	if (!app.close())
		return 1;

	return 0;
}


