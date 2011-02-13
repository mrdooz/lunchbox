#include "stdafx.h"
#include "scene.hpp"

Scene::Scene()
	: _camera(nullptr)
{

}

bool Scene::render()
{
	return true;
}

void Scene::set_camera(Camera *camera)
{
	_camera = camera;
	render();
}

bool Scene::handle_event()
{
	return false;
}

void Scene::update(double running_time, double dt, int num_ticks, double frac)
{

}

bool Scene::init()
{
	return true;
}

bool Scene::close()
{
	return true;
}
