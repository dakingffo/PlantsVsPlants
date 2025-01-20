#pragma once
#include <iostream>
#include "easyx.h"

#include "utility.h"
#include "atlas.h"
#include "animation.h"
#include "scene.h"
#include "scene_manager.h"
#include "camera.h"

#ifndef MENU_SCENE_H
#define MENU_SCENE_H

extern Atlas atlas_peashooter_run_right;
extern SceneManager scene_manager;
extern IMAGE img_menu_background;

class MenuScene : public Scene {
public:
	MenuScene() = default;
	~MenuScene() = default;

	void on_enter() {
		mciSendString(_T("play bgm_menu repeat from 0"), nullptr, 0, nullptr);
	}

	void on_update(std::size_t delta) {
	}

	void on_draw(const Camera& camera) {
		putimage(0, 0, &img_menu_background);
	}

	void on_input(const ExMessage& msg) {
		if (msg.message == WM_KEYUP) {
			mciSendString(_T("play ui_waibibabo from 0"), nullptr, 0, nullptr);
			mciSendString(_T("play ui_confirm from 0"), nullptr, 0, nullptr);
			scene_manager.switch_to(SceneManager::SceneType::Selector);
		}
	}
	
	void on_exit() {

	}

private:

};
#endif
