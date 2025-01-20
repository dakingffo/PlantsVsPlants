#pragma once
#include <vector>
#include "easyx.h"

#include "utility.h"
#include "animation.h"

#ifndef PARTICLE
#define PARTICLE
class Particle {
public:
	Particle() = default;
	Particle(const vector2& pos,Atlas* at, std::size_t life):position(pos),atlas(at),lifespan(life){}
	~Particle() = default;
	void set_atlas(Atlas* target) {
		atlas = target;
	}

	void set_position(const vector2& pos) {
		position = pos;
	}

	void set_lifespan(std::size_t ms) {
		lifespan = ms;
	}

	bool is_valid() noexcept {
		return valid;
	}
	void on_update(std::size_t delta) {
		timer += delta;
		if (timer >= lifespan) {
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->size()) {
				idx_frame = atlas->size() - 1;
				valid = false;
			}
		}
	}
	void on_draw(const Camera& camera) {
		putimage_alpha(camera, position.x, position.y, atlas->get_image(idx_frame));
	}
private:
	std::size_t timer = 0;
	std::size_t lifespan = 0;
	std::size_t idx_frame = 0;

	vector2 position;
	bool valid = true;
	Atlas* atlas = nullptr;
};

std::vector<Particle> particle_list;
#endif // ! PARTICLE
