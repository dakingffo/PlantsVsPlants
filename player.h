#pragma once
#include <iostream>
#include "easyx.h"

#include "utility.h"
#include "animation.h"
#include "player_id.h"
#include "bullet.h"
#include "particle.h"

#ifndef PLAYER_H
#define PLAYER_H

extern std::vector<Platform> platform_list;
extern std::vector<Bullet*> bullet_list;

class Player {
public:
	Player(PlayerId id) {
		using enum PlayerId;

		animation_jump_effect.set_atlas(&atlas_jump_effect);
		animation_jump_effect.set_interval(25);
		animation_jump_effect.set_loop(false);
		animation_jump_effect.set_callback([&]() {
			jump_effect_visible = false;
			});

		animation_land_effect.set_atlas(&atlas_land_effect);
		animation_land_effect.set_interval(50);
		animation_land_effect.set_loop(false);
		animation_land_effect.set_callback([&]() {
			land_effect_visible = false;
			});

		is_facing_right = (id == P1 ? true : false);
		current_animation = (id == P1 ? &animation_idle_right : &animation_idle_left);
		this->id = id;
		size.x = size.y = 96;

		timer_recover.set_wait_time(1000);
		timer_recover.set_callback([&]() {
			hp++;
			});

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]() {
			can_attack = true;
			});

		timer_invulnerable.set_wait_time(600);
		timer_invulnerable.set_one_shot(true);
		timer_invulnerable.set_callback([&]() {
			is_invulnerable = false;
			});

		timer_showing_sketch.set_wait_time(75);
		timer_showing_sketch.set_callback([&]() {
			is_showing_sketch = !is_showing_sketch;
			});

		timer_run_effect.set_wait_time(75);
		timer_run_effect.set_callback([&]() {
			vector2 particle_position;
			IMAGE* frame = atlas_run_effect.get_image(0);
			particle_position = {
				position.x + (size.x - frame->getwidth()) / 2,
				position.y + size.y - frame->getheight()
			};
			particle_list.emplace_back( particle_position,&atlas_run_effect, 70 );
			});

		timer_die_effect.set_wait_time(35);
		timer_die_effect.set_callback([&]() {
			vector2 particle_position;
			IMAGE* frame = atlas_run_effect.get_image(0);
			particle_position = {
				position.x + (size.x - frame->getwidth()) / 2,
				position.y + size.y - frame->getheight()
			};
			particle_list.emplace_back(particle_position, &atlas_run_effect, 150);
			});
	}

	~Player() = default;

	virtual void on_update(std::size_t delta) {
		int direction = is_right_key_down - is_left_key_down;
		if (direction) {
			is_facing_right = (direction > 0);
			current_animation = (is_facing_right ? &animation_run_right : &animation_run_left);
			on_run(direction * run_speed * delta);
		}
		else {
			current_animation = (is_facing_right ? &animation_idle_right : &animation_idle_left);
			timer_run_effect.pause();
		}
		if (hp <= 0) {
			current_animation = (last_hurt.x ? &animation_die_left : &animation_die_right);
		}
		timer_recover.on_update(delta);
		hp = min(hp, 200);
		if (is_attacking_ex)
			current_animation = (is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left);
		current_animation->on_update(delta);
		move(delta);
		timer_attack_cd.on_update(delta);

		timer_invulnerable.on_update(delta);
		timer_showing_sketch.on_update(delta);
		if (is_showing_sketch)
			sketch_image(current_animation->frame(), &img_sketch);

		timer_run_effect.on_update(delta);
		if (hp <= 0)
			timer_die_effect.on_update(delta);
		
		animation_jump_effect.on_update(delta);
		animation_land_effect.on_update(delta);
	}

	virtual void on_draw(const Camera& camera) {
		if (jump_effect_visible)
			animation_jump_effect.on_draw(camera, position_jump_effect.x, position_jump_effect.y);
		if (land_effect_visible)
			animation_land_effect.on_draw(camera, position_land_effect.x, position_land_effect.y);

		if (hp > 0 && is_invulnerable && is_showing_sketch)
			putimage_alpha(camera, position.x, position.y, &img_sketch);
		else
			current_animation->on_draw(camera, position.x, position.y);
	}

	virtual void on_input(const ExMessage& msg) {
		using enum PlayerId;

		switch (msg.message) {
		case WM_KEYDOWN:
			switch (id) {
			case P1: {
				switch (msg.vkcode) {
				case 0x41:
					is_left_key_down = true; break;
				case 0x44:
					is_right_key_down = true; break;
				case 0x57:
					on_jump(); break;
				case 0x46:
					if (can_attack) {
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}break;
				case 0x47:
					if (mp >= 100) {
						on_attack_ex();
					}break;
				default:
					break;
				}
				break;
			}break;
			case P2: {
				switch (msg.vkcode) {
				case VK_LEFT:
					is_left_key_down = true; break;
				case VK_RIGHT:
					is_right_key_down = true; break;
				case VK_UP:
					on_jump(); break;
				case VK_OEM_PERIOD:
					if (can_attack) {
						on_attack();
						can_attack = false;
						timer_attack_cd.restart();
					}break;
				case VK_OEM_2:
					if (mp == 100) {
						on_attack_ex();
					}break;
				default:
					break;
				}
				break;
			}break;
			default:
				break;
			}
			break;
		case WM_KEYUP:
			switch (id) {
			case P1: {
				switch (msg.vkcode) {
				case 0x41:
					is_left_key_down = false; break;
				case 0x44:
					is_right_key_down = false; break;
				default:
					break;
				}
				break;
			}break;
			case P2: {
				switch (msg.vkcode) {
				case VK_LEFT:
					is_left_key_down = false; break;
				case VK_RIGHT:
					is_right_key_down = false; break;
				default:
					break;
				}
				break;
			}break;
			default:
				break;
			}
			break;
		}
	}

	void set_position(float x, float y) {
		position.x = x, position.y = y;
	}

	const vector2& get_position() const {
		return position;
	}

	const vector2& get_size() const {
		return size;
	}

	virtual void on_run(float distance) {
		position.x += distance;
		timer_run_effect.resume();
	}

	virtual void on_jump() {
		if (speed.y)
			return;
		jump_effect_visible = true;
		animation_jump_effect.reset();
		mciSendString(_T("play jump from 0 to 400"), nullptr, 0, nullptr);
		speed.y += jump_speed;

		IMAGE* frame = animation_jump_effect.frame();
		position_jump_effect = {
			position.x + (size.x - frame->getwidth()) / 2, position.y + size.y - frame->getheight()
		};
	}

	virtual void on_land() {
		land_effect_visible = true;
		animation_land_effect.reset();

		IMAGE* frame = animation_land_effect.frame();
		position_land_effect = {
			position.x + (size.x - frame->getwidth()) / 2, position.y + size.y - frame->getheight()
		};
	}

	int get_hp() const {
		return hp;
	}
	int get_mp() const {
		return mp;
	}

	void set_hp(int h) {
		hp = h;
	}
	virtual void on_attack() {}
	virtual void on_attack_ex(){}

protected:
	Animation animation_idle_left;
	Animation animation_idle_right;
	Animation animation_run_left;
	Animation animation_run_right;
	Animation animation_jump_effect;
	Animation animation_land_effect;
	Animation animation_attack_ex_left;
	Animation animation_attack_ex_right;
	Animation animation_die_left;
	Animation animation_die_right;
	Animation* current_animation = nullptr;
	IMAGE img_sketch;
	bool jump_effect_visible = false;
	bool land_effect_visible = false;

	vector2 position_jump_effect;
	vector2 position_land_effect;

	PlayerId id;

	vector2 size;
	vector2 position;
	vector2 speed;
	vector2 last_hurt;

	bool is_left_key_down = false;
	bool is_right_key_down = false;
	bool is_facing_right = true;
	static constexpr float run_speed = 0.5f;
	static constexpr float jump_speed = -1.05f;
	static constexpr float gravity = 2.8e-3f;

	int hp = 200;
	int mp = 0;
	Timer timer_recover;

	bool can_attack = true;
	std::size_t attack_cd = 350;
	Timer timer_attack_cd;
	bool is_attacking_ex = false;
	
	bool is_invulnerable = false;
	bool is_showing_sketch = false;
	Timer timer_invulnerable;
	Timer timer_showing_sketch;

	Timer timer_run_effect;
	Timer timer_die_effect;

	void move(std::size_t delta) {
		float last_speed_y = speed.y;
		speed.y += gravity * delta;
		position += speed * delta;
		if (hp <= 0)
			return;
		if (speed.y > 0) {
			for (Platform& p : platform_list) {
				const Platform::CollisionShape& shape = p.shape;
				bool is_collide_x = (max(position.x + size.x, shape.r) - min(position.x, shape.l) <= size.x + (shape.r - shape.l));
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);
				if (is_collide_x && is_collide_y) {
					float dy = speed.y * delta;
					float prey = position.y + size.y - dy;
					if (prey <= shape.y) {
						position.y = shape.y - size.y;
						speed.y = 0;
						if (last_speed_y != 0) 
							on_land();
						break;
					}
				}
			}
		}

		if (is_invulnerable) return;
		for (Bullet* b : bullet_list) {
			if (!b->get_valid() || b->get_target() != id)
				continue;
			if (b->check_collision(position, size)) {
				to_invulnerable();
				b->on_collide();
				b->set_valid(false);
				hp -= b->get_damage();
				last_hurt = { b->get_position().x - position.x ,b->get_position().y - position.y };
				if (hp <= 0) {
					speed.x = (last_hurt.x < 0 ? 0.75f : -0.75f);
					speed.y = -1.5f;
				}
			}
		}
	}

	void to_invulnerable() {
		is_invulnerable = true;
		timer_invulnerable.restart();
	}
};

Player* player1 = nullptr;
Player* player2 = nullptr;


extern Atlas atlas_peashooter_idle_left;               // 婉逗射手朝向左的默认动画图集
extern Atlas atlas_peashooter_idle_right;              // 婉逗射手朝向右的默认动画图集
extern Atlas atlas_peashooter_run_left;                // 婉逗射手朝向左的跑步动画图集
extern Atlas atlas_peashooter_run_right;               // 婉逗射手朝向右的跑步动画图集
extern Atlas atlas_peashooter_attack_ex_left;          // 婉逗射手朝向左的攻击动画图集
extern Atlas atlas_peashooter_attack_ex_right;         // 婉逗射手朝向右的攻击动画图集
extern Atlas atlas_peashooter_die_left;                // 婉逗射手朝向左的死亡动画图集
extern Atlas atlas_peashooter_die_right;               // 婉逗射手朝向右的死亡动画图集

class PeashooterPlayer :public Player {
public:
	PeashooterPlayer(PlayerId id) :Player(id) {
		using enum PlayerId;

		animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
		animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
		animation_run_left.set_atlas(&atlas_peashooter_run_left);
		animation_run_right.set_atlas(&atlas_peashooter_run_right);
		animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
		animation_die_left.set_atlas(&atlas_peashooter_die_left);
		animation_die_right.set_atlas(&atlas_peashooter_die_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);

		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);

		timer_attack_ex.set_wait_time(attack_ex_duration);
		timer_attack_ex.set_one_shot(true);
		timer_attack_ex.set_callback([&]() {
			is_attacking_ex = false; 
		});

		timer_fire_ex.set_wait_time(110);
		timer_fire_ex.set_one_shot(false);
		timer_fire_ex.set_callback([&]() {
			fire(pea_ex_speed); 
		});
		attack_cd = 100;

	}

	void on_update(std::size_t delta) {
		Player::on_update(delta);
		if (is_attacking_ex) {
			main_camera.shake(5, 20);
			timer_attack_ex.on_update(delta);
			timer_fire_ex.on_update(delta);
		}
	}

	void on_attack() {
		if (is_attacking_ex)
			return;
		fire(pea_speed);
		switch (rand() % 2) {
		case 0:
			mciSendString(_T("play pea_shoot1 from 0"), nullptr, 0, nullptr);
			break;
		case 1:
			mciSendString(_T("play pea_shoot2 from 0"), nullptr, 0, nullptr);
			break;
		}
	}

	void on_attack_ex() {
		mp = 0;
		is_attacking_ex = true;
		timer_attack_ex.restart();
		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();
		mciSendString(_T("play pea_shootEx from 0"), nullptr, 0, nullptr);
	}

private:
	void fire(float speed) {
		using enum PlayerId;

		Bullet* bullet = new PeaBullet();
		vector2 bullet_pos, bullet_speed;
		const vector2& bullet_size = bullet->get_size();
		bullet_pos = { is_facing_right ? position.x + size.x - bullet_size.x / 2 - 25 : position.x - bullet_size.x / 2 + 15, position.y + 3 };
		bullet_speed = { (is_facing_right ? speed : -speed), 0 };
		bullet->set_position(bullet_pos.x, bullet_pos.y);
		bullet->set_speed(bullet_speed.x, bullet_speed.y);
		bullet->set_target(id == P1 ? P2 : P1);
		bullet->set_callback([&]() { if (!is_attacking_ex) mp += 8; mp = min(100, mp); });
		bullet_list.push_back(bullet);
	}
	static constexpr float pea_speed = 0.90f;
	static constexpr float pea_ex_speed = 1.5f;
	
	Timer timer_attack_ex;
	Timer timer_fire_ex;
	const int attack_ex_duration = 2500;
};

extern Atlas atlas_sunflower_idle_left;                // 龙日葵朝向左的默认动画图集
extern Atlas atlas_sunflower_idle_right;               // 龙日葵朝向右的默认动画图集
extern Atlas atlas_sunflower_run_left;                 // 龙日葵朝向左的跑步动画图集
extern Atlas atlas_sunflower_run_right;                // 龙日葵朝向右的跑步动画图集
extern Atlas atlas_sunflower_attack_ex_left;           // 龙日葵朝向左的攻击动画图集
extern Atlas atlas_sunflower_attack_ex_right;          // 龙日葵朝向右的攻击动画图集
extern Atlas atlas_sunflower_die_left;                 // 龙日葵朝向左的死亡动画图集
extern Atlas atlas_sunflower_die_right;                // 龙日葵朝向右的死亡动画图集
extern Atlas atlas_sun_text;

class SunflowerPlayer :public Player {
public:
	SunflowerPlayer(PlayerId id) :Player(id) {
		using enum PlayerId;

		animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
		animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
		animation_run_left.set_atlas(&atlas_sunflower_run_left);
		animation_run_right.set_atlas(&atlas_sunflower_run_right);
		animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
		animation_attack_ex_right.set_atlas(&atlas_sunflower_attack_ex_right);
		animation_sun_text.set_atlas(&atlas_sun_text);
		animation_die_left.set_atlas(&atlas_sunflower_die_left);
		animation_die_right.set_atlas(&atlas_sunflower_die_right);

		animation_idle_left.set_interval(75);
		animation_idle_right.set_interval(75);
		animation_run_left.set_interval(75);
		animation_run_right.set_interval(75);
		animation_attack_ex_left.set_interval(100);
		animation_attack_ex_right.set_interval(100);
		animation_sun_text.set_interval(100);
		animation_die_left.set_interval(150);
		animation_die_right.set_interval(150);

		animation_die_left.set_loop(false);
		animation_die_right.set_loop(false);
		animation_attack_ex_left.set_loop(false);
		animation_attack_ex_right.set_loop(false);
		animation_sun_text.set_loop(false);

		animation_attack_ex_left.set_callback([&]() {
			is_attacking_ex = false;
			is_sun_text_visibile = false;
			});
		animation_attack_ex_right.set_callback([&]() {
			is_attacking_ex = false;
			is_sun_text_visibile = false;
			});
		attack_cd = 250;
	}
	
	void on_update(std::size_t delta) {
		Player::on_update(delta);
		if (is_sun_text_visibile)
			animation_sun_text.on_update(delta);
	}

	void on_draw(const Camera& camera) {
		Player::on_draw(camera);
		if (is_sun_text_visibile) {
			IMAGE* frame = animation_sun_text.frame();
			vector2 text_pos = { position.x - (size.x - frame->getwidth()) / 2, position.y - frame->getheight()};
			animation_sun_text.on_draw(camera, text_pos.x, text_pos.y);
		}
	}

	void on_attack() {
		using enum PlayerId;

		Bullet* bullet = new SunBullet();
		const vector2& bullet_size = bullet->get_size();
		vector2 bullet_position = { position.x + (size.x - bullet_size.x) / 2, position.y };
		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_speed(is_facing_right ? sun_speed.x : -sun_speed.x, sun_speed.y);
		bullet->set_target(id == P1 ? P2 : P1);
		bullet->set_callback([&]() {mp += 25; mp = min(100, mp); });
		bullet_list.push_back(bullet);
		mciSendString(_T("play sun_shoot from 350"), nullptr, 0, nullptr);
	}

	void on_attack_ex() {
		using enum PlayerId;

		mp = 0;
		is_attacking_ex = true;
		is_sun_text_visibile = true;
		animation_sun_text.reset();
		is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

		Bullet* bullet = new SunBulletEx();
		Player* enemy = (id == P1 ? player2 : player1);
		vector2 bullet_position, bullet_speed;
		const vector2& bullet_size = bullet->get_size();
		const vector2& target_size = enemy->get_size(), &target_position = enemy->get_position();
		bullet_position = {
			target_position.x + (target_size.x - bullet_size.x) / 2, -size.y - 120
		};
		bullet_speed = {
			0, sun_ex_speed
		};

		bullet->set_position(bullet_position.x, bullet_position.y);
		bullet->set_speed(bullet_speed.x, bullet_speed.y);
		bullet->set_target(id == P1 ? P2 : P1);
		bullet->set_callback([&]() {mp += 35; mp = min(100, mp); });
		bullet_list.push_back(bullet);

		mciSendString(_T("play sun_text from 0"), nullptr, 0, nullptr);
	}
private:
	Animation animation_sun_text;

	const vector2 sun_speed = { 0.6f,-0.90f };
	bool is_sun_text_visibile = false;
	static constexpr std::size_t sun_ex_speed = 0.10f;
}; 
#endif