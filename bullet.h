#pragma once
#include <vector>

#include "utility.h"
#include "player_id.h"
#include "animation.h"

#ifndef BULLET_H
#define BULLET_H

extern Camera main_camera;

class Bullet {
public:
	Bullet() = default;
	~Bullet() = default;

	void set_damage(std::size_t val) {
		damage = val;
	}

	std::size_t get_damage() const {
		return damage;
	}

	void set_position(float x, float y) {
		position = { x,y };
	}

	const vector2& get_position() const {
		return position;
	}

	const vector2& get_size() const {
		return size;
	}

	void set_speed(float x, float y) {
		speed = { x,y };
	}

	void set_target(PlayerId id) {
		target = id;
	}

	PlayerId get_target() const {
		return target;
	}

	void set_callback(std::function<void()> f) {
		callback = f;
	}

	void set_valid(bool flag) {
		valid = flag;
	}

	bool get_valid() const {
		return valid;
	}

	bool check_can_remove() const {
		return remove;
	}

	virtual void on_collide() {
		if (callback)
			callback();
	}

	virtual bool check_collision(const vector2& pos, const vector2& size) {
		return this->position.x + this->size.x / 2 >= pos.x
			&& this->position.x + this->size.x / 2 <= pos.x + size.x
			&& this->position.y + this->size.y / 2 >= pos.y
			&& this->position.y + this->size.y / 2 <= pos.y + size.y;
	}

	virtual void on_update(std::size_t delta){}
	virtual void on_draw(const Camera& camera) const {}

protected:
	vector2 size;
	vector2 position;
	vector2 speed;
	int damage = 10;
	bool valid = true;
	bool remove = false;
	std::function<void()> callback;
	PlayerId target = PlayerId::P1;

	bool out_of_screen() {
		return position.x + size.x <= 0 || position.x >= getwidth() || position.y + size.y <= 0 || position.y >= getheight();
	}
};

extern IMAGE img_pea;
extern Atlas atlas_pea_break;

class PeaBullet : public Bullet {
public:
	PeaBullet() {
		size = { 64,64 };
		damage = 10;
		animation_break.set_atlas(&atlas_pea_break);
		animation_break.set_interval(100);
		animation_break.set_loop(false);
		animation_break.set_callback([&]() {remove = true; });
	}

	~PeaBullet() = default;

	void on_collide() {
		Bullet::on_collide();
		switch (rand() % 3) {
		case 0:mciSendString(_T("play pea_break1 from 0"), nullptr, 0, nullptr);
			break;
		case 1:mciSendString(_T("play pea_break2 from 0"), nullptr, 0, nullptr);
			break;
		case 2:mciSendString(_T("play pea_break3 from 0"), nullptr, 0, nullptr);
			break;
		}
	}

	void on_update(std::size_t delta) {
		position += speed * (float)delta;
		if (!valid)
			animation_break.on_update(delta);
		if (out_of_screen())
			remove = true;
	}

	void on_draw(const Camera& camera) const {
		if (valid)
			putimage_alpha(camera, position.x, position.y, &img_pea);
		else 
			animation_break.on_draw(camera, position.x, position.y);
	}

private:
	Animation animation_break;
};

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;

class SunBullet : public Bullet {
public:
	SunBullet() {
		size = { 95,96 };
		damage = 20;
		animation_idle.set_atlas(&atlas_sun);
		animation_idle.set_interval(50);

		animation_explode.set_atlas(&atlas_sun_explode);
		animation_explode.set_interval(60);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() {remove = true; });
		IMAGE* frame_idle = animation_idle.frame();
		IMAGE* frame_explode = animation_explode.frame();
		render_offset = { (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f ,(frame_idle->getheight() - frame_explode->getheight()) / 2.0f };
	}

	~SunBullet() = default;
	void on_collide() {
		Bullet::on_collide();
		main_camera.shake(5, 250);
		mciSendString(_T("play sun_explode from 0"), nullptr, 0, nullptr);
	}

	void on_update(std::size_t delta) {
		if (valid) {
			speed.y += gravity * delta;
			position += speed * (float)delta;
		}
		if (!valid)
			animation_explode.on_update(delta);
		else
			animation_idle.on_update(delta);
		if (out_of_screen())
			remove = true;
	}

	void on_draw(const Camera& camera) const {
		if (valid)
			animation_idle.on_draw(camera, position.x, position.y);
		else 
			animation_explode.on_draw(camera, position.x + render_offset.x, position.y + render_offset.y);
	}

private:
	Animation animation_idle;
	Animation animation_explode;
	vector2 render_offset;
	static constexpr float gravity = 3.8e-3f;
};

extern Atlas atlas_sun_ex;
extern Atlas atlas_sun_ex_explode;

class SunBulletEx : public Bullet {
public:
	SunBulletEx() {
		size = { 250,250 };
		damage = 35;
		animation_idle.set_atlas(&atlas_sun_ex);
		animation_idle.set_interval(50);

		animation_explode.set_atlas(&atlas_sun_ex_explode);
		animation_explode.set_loop(false);
		animation_explode.set_callback([&]() {
			remove = true; 
		});

		IMAGE* frame_idle = animation_idle.frame();
		IMAGE* frame_explode = animation_explode.frame();
		render_offset = { (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f ,(frame_idle->getheight() - frame_explode->getheight()) / 2.0f };
	}

	~SunBulletEx() = default;

	void on_collide() {
		Bullet::on_collide();
		main_camera.shake(20, 350);
		mciSendString(_T("play sun_explodeEx from 0"), nullptr, 0, nullptr);
	}

	void on_update(std::size_t delta) {
		if (valid) {
			speed.y += gravity * delta;
			position += speed * (float)delta;
		}
		if (!valid)
			animation_explode.on_update(delta);
		else
			animation_idle.on_update(delta);
		if (out_of_screen())
			remove = true;
	}

	void on_draw(const Camera& camera) const {
		if (valid)
			animation_idle.on_draw(camera, position.x, position.y);
		else {
			animation_explode.on_draw(camera, position.x + render_offset.x, position.y + render_offset.y);
		}
	}

	bool check_collision(const vector2& pos, const vector2& size) {
		bool x = (max(position.x + this->size.x, pos.x + size.x) - min(position.x, pos.x) <= this->size.x + size.x);
		bool y = (max(position.y + this->size.y, pos.y + size.y) - min(position.y, pos.y) <= this->size.y + size.y);
		return x && y;
	}

private:
	Animation animation_idle;
	Animation animation_explode;
	vector2 render_offset;
	static constexpr float gravity = 1.0e-3f;
};

std::vector<Bullet*>bullet_list;
#endif 
