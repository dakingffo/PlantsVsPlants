#pragma once
#include "easyx.h"

#include "scene.h"

#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

class SceneManager {
public:
	enum class SceneType {
		Menu,
		Game,
		Selector
	};

private:
	Scene* current_scene = nullptr;

public:
	SceneManager() = default;
	~SceneManager() = default;

	void set_current_scene(Scene* scene) {
		current_scene = scene;
		current_scene->on_enter();
	}

	void switch_to(SceneType type) {
		current_scene->on_exit();
		switch (type) {
	    using enum SceneType;

		case Menu:     current_scene = menu_scene; break;
		case Game:     current_scene = game_scene; break;
		case Selector: current_scene = selector_scene; break;
		default: break;
		}
		current_scene->on_enter();
	}

	void on_update(std::size_t delta) {
		current_scene->on_update(delta);
	}

	void on_draw(const Camera& camera) {
		current_scene->on_draw(camera);
	}

	void on_input(const ExMessage& msg) {
		current_scene->on_input(msg);
	}
};

SceneManager scene_manager;
#endif