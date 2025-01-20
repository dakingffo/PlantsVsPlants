#pragma once
#include <iostream>
#include "easyx.h"

#include "scene.h"
#include "scene_manager.h"
#include "platform.h"
#include "player.h"
#include "status_bar.h"

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern std::vector<Platform> platform_list;
extern SceneManager scene_manager;
extern Camera main_camera;

extern Player* player1;
extern Player* player2;
extern IMAGE* img_player1_avatar;
extern IMAGE* img_player2_avatar;

extern IMAGE img_1P_winner;
extern IMAGE img_2P_winner;
extern IMAGE img_winner_bar;

class GameScene : public Scene {
public:
	GameScene() = default;
	~GameScene() = default;

	void on_enter() {
		game_over = false;
		is_slide_out_start = false;

		pos_img_winner_bar = {
			-img_winner_bar.getwidth(),
			(getheight() - img_winner_bar.getheight()) / 2
		};
		pos_img_winner_bar_dest = (getwidth() - img_winner_bar.getwidth()) / 2;
		pos_img_winner_text = {
	        pos_img_winner_bar.x,
	        (getheight() - img_1P_winner.getheight()) / 2
		};
		pos_img_winner_text_dest = (getwidth() - img_1P_winner.getwidth()) / 2;

		timer_winner_slide_in.restart();
		timer_winner_slide_in.set_wait_time(2300);
		timer_winner_slide_in.set_one_shot(true);
		timer_winner_slide_in.set_callback([&]() {
			is_slide_out_start = true;
			});
		timer_winner_slide_out.restart();
		timer_winner_slide_out.set_wait_time(1000);
		timer_winner_slide_out.set_one_shot(true);
		timer_winner_slide_out.set_callback([&]() {
			scene_manager.switch_to(SceneManager::SceneType::Selector);
			});

		mciSendString(_T("play game_waibiwaibiwaibi from 0"), nullptr, 0, nullptr);
		mciSendString(_T("play bgm_game repeat from 0"), nullptr, 0, nullptr);
		bgm_status.resize(20);

		pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
		pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;
		pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
		pos_img_hills.y = (getheight() - img_hills.getheight()) / 2 + 145;

		platform_list.resize(4);
		Platform& large_platform = platform_list[0];
		large_platform.img = &img_platform_large;
		large_platform.render_position = { 122,455 };
		large_platform.shape = { 455.0f + 60, 122.0f + 30, 122.0f + img_platform_large.getwidth() - 30};

		Platform& platform1 = platform_list[1];
		platform1.img = &img_platform_small;
		platform1.render_position = { 175,360 };
		platform1.shape = { 360.0f + img_platform_small.getheight() / 2, 175.0f + 40, 175.0f + img_platform_small.getwidth() - 40};

		Platform& platform2 = platform_list[2];
		platform2.img = &img_platform_small;
		platform2.render_position = { 855,360 };
		platform2.shape = { 360.0f + img_platform_small.getheight() / 2, 855.0f + 40, 855.0f + img_platform_small.getwidth() - 40 };
		
		Platform& platform3 = platform_list[3];
		platform3.img = &img_platform_small;
		platform3.render_position = { 515,225 };
		platform3.shape = { 225.0f + img_platform_small.getheight() / 2, 515.0f + 40, 515.0f + img_platform_small.getwidth() - 40 };

		player1->set_position(215, 225);
		player2->set_position(960, 225);

		status_bar_1P.set_avator(img_player1_avatar);
		status_bar_2P.set_avator(img_player2_avatar);
		status_bar_1P.set_position(235, 625);
		status_bar_2P.set_position(675, 625);
	}

	void on_update(std::size_t delta) {
		player1->on_update(delta);
		player2->on_update(delta);
		main_camera.on_update(delta);

		particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(), [](Particle& particle) {
			return !particle.is_valid();
			}), particle_list.end());

		for (Particle& p : particle_list)
			p.on_update(delta);

		bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), [](Bullet* b) {
			bool flag = b->check_can_remove();
			if (flag)
				delete b;
			return flag;
			}),bullet_list.end());
		for (Bullet* b : bullet_list)
			b->on_update(delta);

		const vector2& player1_pos = player1->get_position();
		const vector2& player2_pos = player2->get_position();

		if (player1_pos.y >= getheight())
			player1->set_hp(0);
		if (player2_pos.y >= getheight())
			player2->set_hp(0);
		status_bar_1P.set_hp_mp(player1->get_hp(), player1->get_mp());
		status_bar_2P.set_hp_mp(player2->get_hp(), player2->get_mp());

		if (player1->get_hp() <= 0 || player2->get_hp() <= 0) {
			if (!game_over) {
				mciSendString(_T("pause bgm_game"), nullptr, 0, nullptr);
				mciSendString(_T("pause bgm_gameX"), nullptr, 0, nullptr);
				mciSendString(_T("play ui_win from 0"), nullptr, 0, nullptr);
			}
			game_over = true;
		}

		if (game_over) {
			pos_img_winner_bar.x += float(speed_winner_bar * delta);
			pos_img_winner_text.x += float(speed_winner_text * delta);
			if (!is_slide_out_start) {
				timer_winner_slide_in.on_update(delta);
				if (pos_img_winner_bar.x > pos_img_winner_bar_dest)
					pos_img_winner_bar.x = pos_img_winner_bar_dest;
				if (pos_img_winner_text.x > pos_img_winner_text_dest)
					pos_img_winner_text.x = pos_img_winner_text_dest;
			}
			else
				timer_winner_slide_out.on_update(delta);
		}

		if (!game_over && !is_bgmX && ((player1->get_hp() <= 50 || player2->get_hp() <= 50))) {
			mciSendString(_T("status pea_shootEx mode"), const_cast<wchar_t*>(bgm_status.c_str()), 20, nullptr);
			if (bgm_status.find(_T("playing")) == std::string::npos) {
				mciSendString(_T("play game_wawarou from 0"), nullptr, 0, nullptr);
				mciSendString(_T("pause bgm_game"), nullptr, 0, nullptr);
				mciSendString(_T("play bgm_gameX repeat from 300"), nullptr, 0, nullptr);
				is_bgmX = true;
			}
		}
	}

	void on_draw(const Camera& camera) {
		putimage_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
		putimage_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

		for (Platform& p : platform_list) {
			p.on_draw(camera);
		}

		for (Particle& p : particle_list)
			p.on_draw(camera);

		if (is_debug) {
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("°´Q¹Ø±Õµ÷ÊÔ"));
		}
		player1->on_draw(camera);
		player2->on_draw(camera);
		for (Bullet* b : bullet_list)
			b->on_draw(camera);
		if (game_over) {
			putimage_alpha(camera, pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
			putimage_alpha(camera, pos_img_winner_text.x, pos_img_winner_text.y, player1->get_hp() > 0 ? &img_1P_winner : &img_2P_winner);
		}
		else {
			status_bar_1P.on_draw(camera);
			status_bar_2P.on_draw(camera);
		}
	}

	void on_input(const ExMessage& msg) {
		player1->on_input(msg);
		player2->on_input(msg);
#if DE_BUG
		switch (msg.message) {
		case WM_KEYDOWN:
			if (msg.vkcode == 0x51)
				is_debug = !is_debug;
			break;
		default:
			break;
		}
#endif
	}

	void on_exit() {
		delete player1; player1 = nullptr;
		delete player2; player2 = nullptr;
		is_debug = false;
		for (Bullet* b : bullet_list)
			delete b;
		bullet_list.clear();
		particle_list.clear();
		main_camera.reset();
	}
private:
	static constexpr float speed_winner_bar = 3.0f;
	static constexpr float speed_winner_text = 1.5f;

	POINT pos_img_sky = { 0 };
	POINT pos_img_hills = { 0 };

	bool is_bgmX = false;
	bool game_over = false;

	Statusbar status_bar_1P;
	Statusbar status_bar_2P;

	POINT pos_img_winner_bar = { 0 };
	POINT pos_img_winner_text = { 0 };
	int pos_img_winner_bar_dest = 0;
	int pos_img_winner_text_dest = 0;
	Timer timer_winner_slide_in;
	Timer timer_winner_slide_out;
	bool is_slide_out_start = false;

	std::wstring bgm_status;
};
#endif
