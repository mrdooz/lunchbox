#ifndef _SCENE_HPP_
#define _SCENE_HPP_

class Camera;

class Scene {
public:
	Scene();
	virtual bool init();
	virtual bool close();
	virtual bool render();
	virtual void update(double running_time, double dt, int num_ticks, double frac);
	void set_camera(Camera *camera);
	virtual bool handle_event();
private:
	Camera *_camera;
};

#endif

