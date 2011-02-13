#ifndef _APP_HPP_
#define _APP_HPP_

#include <vector>
#include <windows.h>

class Window;
class Scene;

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
	void handle_idle();
	void render();
	void find_app_root();
	static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static App *_instance;

	Scene *_scene;
	std::vector<Window *> _windows;

	HINSTANCE _hinstance;
	HWND _hwnd;
	DWORD _width;
	DWORD _height;
	string2 _app_root;
};


#endif
