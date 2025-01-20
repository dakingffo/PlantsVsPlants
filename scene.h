#pragma once
#include "easyx.h"

#ifndef SCENE_H
#define SCENE_H

class Camera;

class Scene {
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter() {};
	virtual void on_update(std::size_t delta) {};
	virtual void on_draw(const Camera& camera) {};
	virtual void on_input(const ExMessage& msg) {};
	virtual void on_exit() {};
	bool is_debug = false;
};
#endif