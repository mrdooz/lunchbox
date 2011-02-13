#include "stdafx.h"
#include "app.hpp"
#include "camera.hpp"
#include "window.hpp"
#include "scene.hpp"
#include "scene_bspline.hpp"

using namespace std;

App *App::_instance;

App &App::instance()
{
	if (!_instance)
		_instance = new App();
	return *_instance;
}

LRESULT App::wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static App *self = NULL;

	switch( message ) 
	{
	case WM_CREATE:
		{
			CREATESTRUCT *cs = (CREATESTRUCT *)lParam;
			self = (App *)cs->lpCreateParams;
		}
		break;

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
	, _scene(NULL)
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

	// a border seems out of place, seeing as we're doing everything else ourselves..
	//const UINT window_style = WS_VISIBLE | WS_POPUP | WS_OVERLAPPEDWINDOW;
	const UINT window_style = WS_VISIBLE | WS_POPUP /*| WS_OVERLAPPEDWINDOW*/;

	_hwnd = CreateWindowA(kClassName, "lunchbox editor - magnus österlind - 2010", window_style,
		CW_USEDEFAULT, CW_USEDEFAULT, _width, _height, NULL, NULL,
		_hinstance, (void *)this);

	set_client_size();

	ShowWindow(_hwnd, SW_SHOW);

	return true;

}

void App::find_app_root()
{
	// keep going up directory levels until we find "app.root", or we hit the bottom..
	char *starting_dir = _getcwd(NULL, 0);
	while (true) {
		if (file_exists("app.root")) {
			char *buf = _getcwd(NULL, 0);
			_app_root = buf;
			free(buf);
			return;
		}
		if (_chdir("..") == -1)
			break;
	}

	_app_root = starting_dir;
	free(starting_dir);
}

bool App::init()
{
	find_app_root();

	_width = 1024;
	_height = 768;
	if (!create_window())
		return false;

	if (!Graphics::instance().init_directx(_hwnd, _width, _height))
		return false;

	_scene = new SceneBSpline();
	int w = _width/2;
	int h = _height/2;

	_windows.push_back(new Window(Viewport(0, 0, w, h)));
	_windows.push_back(new Window(Viewport(w, 0, w, h)));
	_windows.push_back(new Window(Viewport(0, h, w, h)));
	_windows.push_back(new Window(Viewport(w, h, w, h)));

	for (size_t i = 0; i < _windows.size(); ++i) {
		_windows[i]->set_active(true);
		_windows[i]->set_scene(_scene);
	}

	_scene->init();

	return true;
}

bool App::close()
{
	delete exch_null(_scene);
	container_delete(_windows);
	
	if (!Graphics::instance().close())
		return false;

	return true;
}

void App::render()
{
	for (size_t i = 0; i < _windows.size(); ++i) {
		Window *cur = _windows[i];
		if (cur->active())
			cur->render();
	}
}

void App::handle_idle()
{
	render();
}

bool App::run()
{
	MSG msg = {0};

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER cur;
	QueryPerformanceCounter(&cur);
	double cur_time = (double)(cur.QuadPart / freq.QuadPart);
	double accumulator = 0;
	double running_time = 0;
	const double dt = 1 / 100.0f;

	while (msg.message != WM_QUIT) {
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {

			QueryPerformanceCounter(&cur);
			double new_time = (double)cur.QuadPart / freq.QuadPart;
			double delta_time = new_time - cur_time;
			cur_time = new_time;
			accumulator += delta_time;

			// calc the number of ticks of the fixed timestep to step
			int num_ticks = (int)(accumulator / dt);
			const double a = delta_time > 0 ? (accumulator - num_ticks * dt) / delta_time : 0;

			if (_scene)
				_scene->update(running_time, dt, num_ticks, a);

			running_time += num_ticks * dt;
			accumulator -= num_ticks * dt;

			Graphics::instance().clear();
			handle_idle();
			Graphics::instance().present();
		}
	}
	return true;
}
