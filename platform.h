#pragma once
#include <vector>

#include "camera.h"
#include "animation.h"

#ifndef PLATFORM_H
#define PLATFORM_H

extern Scene* game_scene;

class Platform {
public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) {
		putimage_alpha(camera, render_position.x, render_position.y, img);
#if DE_BUG
		if (game_scene->is_debug) {
			setlinecolor(RGB(255, 0, 0));
			line(camera, shape.l, shape.y, shape.r, shape.y);
		}
#endif
	}
	struct CollisionShape {
		float y;
		float l;
		float r;
	}shape;
	IMAGE* img = nullptr;
	POINT render_position = { 0 };
};

std::vector<Platform> platform_list;
#endif