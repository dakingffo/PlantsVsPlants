#pragma once
#include "easyx.h"
#include "utility.h"
#include "animation.h"

#ifndef STATUS_BAR_H
#define STATUS_BAR_H

IMAGE* img_player1_avatar = nullptr;
IMAGE* img_player2_avatar = nullptr;

class Statusbar {
public:
	Statusbar() = default;
	~Statusbar() = default;
	void set_avator(IMAGE* img) {
		img_avator = img;
	}

	void set_position(int x, int y) {
		position = { x, y };
	}

	void on_draw(const Camera& camera) {
		putimage_alpha(camera, position.x, position.y, img_avator);

		setfillcolor(RGB(5, 5, 5));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8);
		//solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3 * 2, position.y + 71, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width * 3 / 5 + 3 * 2 , position.y + 65, 8, 8);

		setfillcolor(RGB(67, 47, 47));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3, position.y + 33, 8, 8);
		//solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3, position.y + 68, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width * 3 / 5 + 3 , position.y + 62, 8, 8);

		float hp_width = width * max(0, hp) / 200.0f;
		float mp_width = width * min(100, mp) / 100.0f;
		setfillcolor(RGB(215, 61, 67));
		solidroundrect(position.x + 100, position.y + 10, position.x + 100 + (int)hp_width + 3, position.y + 33, 8, 8);

		setfillcolor(mp == 100? RGB(0,220,220): RGB(83, 131, 195));
		//solidroundrect(position.x + 100, position.y + 45, position.x + 100 + (int)mp_width + 3, position.y + 68, 8, 8);
		solidroundrect(position.x + 100, position.y + 45, position.x + 100 + (int)mp_width * 3 / 5 + 3, position.y + 62, 8, 8);
	}

	void set_hp_mp(int h, int m) {
		hp = h;
		mp = m;
	}

private:
	static constexpr std::size_t width = 275;
	int hp;
	int mp;
	POINT position = { 0 };
	IMAGE* img_avator = nullptr;
};
#endif