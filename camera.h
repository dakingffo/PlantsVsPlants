#pragma once

#include "utility.h"

#ifndef CAMERA_H
#define CAMERA_H

class Camera {
public:
	Camera() : position{}, is_shaking(false), shaking_strength(0) {
		shake_timer.set_one_shot(true);
		shake_timer.set_callback([&]{
			    is_shaking = false;
			    reset();
			});
	}
	~Camera() = default;

	const vector2& get_positon() const {
		return position;
	}

	void reset() {
		position.x = 0;
		position.y = 0;
	}

	void on_update(std::size_t delta) {
		shake_timer.on_update(delta);
		if (is_shaking) {
			position.x = (-50 + rand() % 100) / 50.0f * shaking_strength;
			position.y = (-50 + rand() % 100) / 50.0f * shaking_strength;
		}
	}

	void shake(float strength, std::size_t duration) {
		is_shaking = true;
		shaking_strength = strength;
		shake_timer.set_wait_time(duration);
		shake_timer.restart();
	}

private:
	vector2 position;
	Timer shake_timer;
	bool is_shaking = false;
	float shaking_strength = 0;
};

Camera main_camera;
#endif