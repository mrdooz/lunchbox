#include "stdafx.h"
#include "window.hpp"
#include "scene.hpp"


Window::Window()
	: _camera(nullptr)
{
}

Window::Window(const Viewport &viewport)
	: _camera(nullptr)
	, _viewport(viewport)
{
}


void Window::render()
{

}

bool Window::handle_event()
{
	if (!_scene)
		return false;

	return _scene->handle_event();
}
