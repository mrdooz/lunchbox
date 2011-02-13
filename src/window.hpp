#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <d3d11.h>

class Camera;
class Scene;

struct Viewport {
	Viewport() : top(0), left(0), width(0), height(0) {}
	Viewport(int top, int left, int width, int height) : top(top), left(left), width(width), height(height) {}
	int top, left;
	int width, height;
};

class Window {
public:
	Window();
	Window(const Viewport &viewport);
	bool active() const { return _active; }
	void set_active(bool active) { _active = active; }
	void set_camera(Camera *camera);
	void render();
	bool handle_event();
	void set_viewport(const Viewport &v) { _viewport = v; }
protected:
	Camera *_camera;
	Scene *_scene;
	bool _active;
	Viewport _viewport;
};

#endif
