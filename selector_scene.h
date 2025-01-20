#pragma once
#include <iostream>
#include "easyx.h"

#include "scene.h"
#include "scene_manager.h"
#include "animation.h"
#include "player.h"
#include "player_id.h"
#include "status_bar.h"

#ifndef SELECTOR_SCENE_H
#define SELECTOR_SCENE_H

extern Player* player1;
extern Player* player2;
extern IMAGE* img_player1_avatar;
extern IMAGE* img_player2_avatar;

extern SceneManager scene_manager;

extern IMAGE img_VS;                                   // VS “’ ı◊÷Õº∆¨
extern IMAGE img_1P;                                   // 1P Œƒ±æÕº∆¨
extern IMAGE img_2P;                                   // 2P Œƒ±æÕº∆¨

extern IMAGE img_1P_desc;                              // 1P º¸Œª√Ë ˆÕº∆¨
extern IMAGE img_2P_desc;                              // 2P º¸Œª√Ë ˆÕº∆¨

extern IMAGE img_gravestone_left;                      // ≥ØœÚ◊Ûµƒƒπ±ÆÕº∆¨
extern IMAGE img_gravestone_right;                     // ≥ØœÚ”“µƒƒπ±ÆÕº∆¨

extern IMAGE img_selector_tip;                         // —°Ω«ΩÁ√ÊÃ· æŒƒ±æÕº∆¨
extern IMAGE img_selector_background;                  // —°Ω«ΩÁ√Ê±≥æ∞Õº∆¨

extern IMAGE img_1P_selector_btn_idle_left;            // 1P œÚ◊Û—°‘Ò∞¥≈•ƒ¨»œ◊¥Ã¨Õº∆¨
extern IMAGE img_1P_selector_btn_idle_right;           // 1P œÚ”“—°‘Ò∞¥≈•ƒ¨»œ◊¥Ã¨Õº∆¨
extern IMAGE img_1P_selector_btn_down_left;            // 1P œÚ◊Û—°‘Ò∞¥≈•∞¥œ¬◊¥Ã¨Õº∆¨
extern IMAGE img_1P_selector_btn_down_right;           // 1P œÚ”“—°‘Ò∞¥≈•∞¥œ¬◊¥Ã¨Õº∆¨

extern IMAGE img_2P_selector_btn_idle_left;            // 2P œÚ◊Û—°‘Ò∞¥≈•ƒ¨»œ◊¥Ã¨Õº∆¨
extern IMAGE img_2P_selector_btn_idle_right;           // 2P œÚ”“—°‘Ò∞¥≈•ƒ¨»œ◊¥Ã¨Õº∆¨
extern IMAGE img_2P_selector_btn_down_left;            // 2P œÚ◊Û—°‘Ò∞¥≈•∞¥œ¬◊¥Ã¨Õº∆¨
extern IMAGE img_2P_selector_btn_down_right;           // 2P œÚ”“—°‘Ò∞¥≈•∞¥œ¬◊¥Ã¨Õº∆¨

extern IMAGE img_peashooter_selector_background_left;  // —°Ω«ΩÁ√Ê≥ØœÚ◊ÛµƒÕÒ∂∫…‰ ÷±≥æ∞Õº∆¨
extern IMAGE img_peashooter_selector_background_right; // —°Ω«ΩÁ√Ê≥ØœÚ”“µƒÕÒ∂∫…‰ ÷±≥æ∞Õº∆¨

extern IMAGE img_sunflower_selector_background_left;   // —°Ω«ΩÁ√Ê≥ØœÚ◊Ûµƒ¡˙»’ø˚±≥æ∞Õº∆¨
extern IMAGE img_sunflower_selector_background_right;  // —°Ω«ΩÁ√Ê≥ØœÚ”“µƒ¡˙»’ø˚±≥æ∞Õº∆¨

extern Atlas atlas_peashooter_idle_left;               // ÕÒ∂∫…‰ ÷≥ØœÚ◊Ûµƒƒ¨»œ∂Øª≠ÕººØ
extern Atlas atlas_peashooter_idle_right;              // ÕÒ∂∫…‰ ÷≥ØœÚ”“µƒƒ¨»œ∂Øª≠ÕººØ
extern Atlas atlas_sunflower_idle_left;                // ¡˙»’ø˚≥ØœÚ◊Ûµƒƒ¨»œ∂Øª≠ÕººØ
extern Atlas atlas_sunflower_idle_right;               // ¡˙»’ø˚≥ØœÚ”“µƒƒ¨»œ∂Øª≠ÕººØ
extern IMAGE img_avatar_peashooter;                    // ÕÒ∂∫…‰ ÷Õ∑œÒÕº∆¨
extern IMAGE img_avatar_sunflower;                     // ¡˙»’ø˚Õ∑œÒÕº∆¨

class SelectorScene : public Scene {
public:
	SelectorScene() = default;
	~SelectorScene() = default;

	void on_enter() {
		animation_peashooter_right.set_atlas(&atlas_peashooter_idle_right);
		animation_sunflower_right.set_atlas(&atlas_sunflower_idle_right);
		animation_peashooter_right.set_interval(100);
		animation_sunflower_right.set_interval(100);

		animation_peashooter_left.set_atlas(&atlas_peashooter_idle_left);
		animation_sunflower_left.set_atlas(&atlas_sunflower_idle_left);
		animation_peashooter_left.set_interval(100);
		animation_sunflower_left.set_interval(100);

		static const int OFFSET_X = 50;

		pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;

		pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;

		pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;

		pos_img_tip.y = getheight() - 125;

		pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;

		pos_img_1P.y = 35;

		pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;

		pos_img_2P.y = pos_img_1P.y;

		pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;

		pos_img_1P_desc.y = getheight() - 150;

		pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;

		pos_img_2P_desc.y = pos_img_1P_desc.y;

		pos_img_1P_gravestone.x = (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 - OFFSET_X;

		pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getheight() + 35;

		pos_img_2P_gravestone.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 + OFFSET_X;

		pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;

		pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;

		pos_animation_1P.y = pos_img_1P_gravestone.y + 80;

		pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 + OFFSET_X;

		pos_animation_2P.y = pos_animation_1P.y;

		pos_img_1P_name.y = pos_animation_1P.y + 155;

		pos_img_2P_name.y = pos_img_1P_name.y;

		pos_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1P_selector_btn_idle_left.getwidth();

		pos_1P_selector_btn_left.y = pos_img_1P_gravestone.y + (img_gravestone_right.getheight() - img_1P_selector_btn_idle_left.getheight()) / 2;

		pos_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone_right.getwidth();

		pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;

		pos_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2P_selector_btn_idle_left.getwidth();

		pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;

		pos_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone_left.getwidth();

		pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;

		mciSendString(_T("resume bgm_menu "), nullptr, 0, nullptr);
	}

	void on_update(std::size_t delta) {
		animation_peashooter_left.on_update(delta);
		animation_sunflower_left .on_update(delta);
		animation_peashooter_right.on_update(delta);
		animation_sunflower_right.on_update(delta);
		scorll_offset_x += 5;
		if (scorll_offset_x >= img_peashooter_selector_background_left.getwidth())
			scorll_offset_x = 0;
	}

	void on_draw(const Camera& camera) {
		IMAGE* p1_background = nullptr;
		IMAGE* p2_background = nullptr;
		using enum PlayerType;

		switch (P2_type) {
		case Peashooter:
			p1_background = &img_peashooter_selector_background_right;
			break;
		case Sunflower:
			p1_background = &img_sunflower_selector_background_right;
			break;
		default:
			p1_background = &img_peashooter_selector_background_right;
		}
		switch (P1_type) {
		case Peashooter:
			p2_background = &img_peashooter_selector_background_left;
			break;
		case Sunflower:
			p2_background = &img_sunflower_selector_background_left;
			break;
		default:
			p2_background = &img_peashooter_selector_background_left;
		}

		putimage(0, 0, &img_selector_background);

		putimage_alpha(camera, scorll_offset_x - p1_background->getwidth(), 0, p1_background);
		putimage_alpha(scorll_offset_x, 0, p1_background->getwidth() - scorll_offset_x, 0, p1_background, 0, 0);
		putimage_alpha(getwidth() - p2_background->getwidth(), 0, p2_background->getwidth() - scorll_offset_x, 0, p2_background, scorll_offset_x, 0);
		putimage_alpha(camera, getwidth()- scorll_offset_x, 0, p2_background);

		putimage_alpha(camera, pos_img_VS.x, pos_img_VS.y, &img_VS);
		putimage_alpha(camera, pos_img_1P.x, pos_animation_1P.y, &img_1P);
		putimage_alpha(camera, pos_img_2P.x, pos_animation_2P.y, &img_2P);
		putimage_alpha(camera, pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_right);
		putimage_alpha(camera, pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_left);
		
		switch (P1_type) {
		case Peashooter:
			animation_peashooter_right.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);
			break;
		case Sunflower:
			animation_sunflower_right.on_draw(camera, pos_animation_1P.x, pos_animation_1P.y);
			pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);
			break;
		}
		switch (P2_type) {
		case Peashooter:
			animation_peashooter_left.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);
			break;
		case Sunflower:
			animation_sunflower_left.on_draw(camera, pos_animation_2P.x, pos_animation_2P.y);
			pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;
			outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);
			break;
		}

		putimage_alpha(camera, pos_1P_selector_btn_left.x, pos_1P_selector_btn_left.y, 
			btn_1P_left ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
		putimage_alpha(camera, pos_1P_selector_btn_right.x, pos_1P_selector_btn_right.y,
			btn_1P_right ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
		 putimage_alpha(camera, pos_2P_selector_btn_left.x, pos_2P_selector_btn_left.y,
			btn_2P_left ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
		putimage_alpha(camera, pos_2P_selector_btn_right.x, pos_2P_selector_btn_right.y,
			btn_2P_right ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

		putimage_alpha(camera, pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
		putimage_alpha(camera, pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);
		putimage_alpha(camera, pos_img_tip.x, pos_img_tip.y, &img_selector_tip);
	}

	void on_input(const ExMessage& msg) {
		using enum PlayerType;

		switch (msg.message) {
		case WM_KEYDOWN: {
			switch (msg.vkcode) {
			case 0x41:
				btn_1P_left = true;
				break;
			case 0x44:
				btn_1P_right = true; 
				break;
			case VK_LEFT:
				btn_2P_left = true;
				break;
			case VK_RIGHT:
				btn_2P_right = true;
				break;
			case VK_RETURN:
				scene_manager.switch_to(SceneManager::SceneType::Game);
				mciSendString(_T("play ui_start_game from 0"), nullptr, 0, nullptr);
				break;
			}
			break;
		}
		case WM_KEYUP: {
			switch (msg.vkcode) {
			case 0x41:
				btn_1P_left = false;
				P1_type = (PlayerType)((int(Invalid) + int(P1_type) - 1) % int(Invalid));
				mciSendString(_T("play ui_plants_switch from 0"), nullptr, 0, nullptr);
				break;
			case 0x44:
				btn_1P_right = false;
				P1_type = (PlayerType)((int(P1_type) + 1) % int(Invalid));
				mciSendString(_T("play ui_plants_switch from 0"), nullptr, 0, nullptr);
				break;
			case VK_LEFT:
				btn_2P_left = false;
				P2_type = (PlayerType)((int(Invalid) + int(P2_type) - 1) % int(Invalid));
				mciSendString(_T("play ui_plants_switch from 0"), nullptr, 0, nullptr);
				break;
			case VK_RIGHT:
				btn_2P_right = false;
				P2_type = (PlayerType)((int(P2_type) + 1) % int(Invalid));
				mciSendString(_T("play ui_plants_switch from 0"), nullptr, 0, nullptr);
				break;
			}
			break;
		}
		default:
			break;
		}
	}

	void on_exit() {
		using enum PlayerType;
		using enum PlayerId;

		switch(P1_type) {
		case Peashooter:
			player1 = new PeashooterPlayer(P1); 
			img_player1_avatar = &img_avatar_peashooter;
			break;
		case Sunflower:
			player1 = new SunflowerPlayer(P1); 
			img_player1_avatar = &img_avatar_sunflower;
			break;
		}

		switch (P2_type) {
		case Peashooter:
			player2 = new PeashooterPlayer(P2); 
			img_player2_avatar = &img_avatar_peashooter;
			break;
		case Sunflower:
			player2 = new SunflowerPlayer(P2); 
			img_player2_avatar = &img_avatar_sunflower;
			break;
		}

		mciSendString(_T("pause bgm_menu"), nullptr, 0, nullptr);
	}

private:
	enum class PlayerType {
		Peashooter = 0,
		Sunflower,
		Invalid
	};
	using enum PlayerType;

	POINT pos_img_VS = { 0 };
	POINT pos_img_tip = { 0 };
	POINT pos_img_1P = { 0 };
	POINT pos_img_2P = { 0 };
	POINT pos_img_1P_desc = { 0 };
	POINT pos_img_2P_desc = { 0 };
	POINT pos_img_1P_name = { 0 };
	POINT pos_img_2P_name = { 0 };
	POINT pos_img_1P_gravestone = { 0 };
	POINT pos_img_2P_gravestone = { 0 };
	POINT pos_animation_1P = { 0 };
	POINT pos_animation_2P = { 0 };
	POINT pos_1P_selector_btn_left = { 0 };
	POINT pos_1P_selector_btn_right = { 0 };
	POINT pos_2P_selector_btn_left = { 0 };
	POINT pos_2P_selector_btn_right = { 0 };

	Animation animation_peashooter_left;
	Animation animation_sunflower_left;
	Animation animation_peashooter_right;
	Animation animation_sunflower_right;

	PlayerType P1_type = Peashooter;
	PlayerType P2_type = Sunflower;

	LPCTSTR str_peashooter_name = _T("Õ„∂π…‰ ÷");
	LPCTSTR str_sunflower_name = _T("œÚ»’ø˚");

	int scorll_offset_x = 0;

	bool btn_1P_left = false;
	bool btn_2P_left = false;
	bool btn_1P_right = false;
	bool btn_2P_right = false;

	void outtextxy_shaded(std::size_t x, std::size_t y, LPCTSTR str) {
		settextcolor(RGB(45, 45, 45));
		outtextxy(x + 3, y + 3, str);
		settextcolor(RGB(255, 255, 255));
		outtextxy(x, y, str);
	}
};
#endif
