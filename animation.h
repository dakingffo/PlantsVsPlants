#pragma once

#include "utility.h"
#include "camera.h"

#ifndef ANIMATION_H
#define ANIMATION_H

inline void putimage_alpha(const Camera& camera, int x, int y, IMAGE* img) {
	int w = img->getwidth(), h = img->getheight();
	const vector2& camera_pos = camera.get_positon();
	AlphaBlend(GetImageHDC(GetWorkingImage()), x - camera_pos.x, y - camera_pos.y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

inline void putimage_alpha(int x, int y, int width , int height, IMAGE* img, int src_x, int src_y) {
	int w = width > 0 ? width : img->getwidth();
	int h = height > 0 ? height : img->getheight();
	AlphaBlend(GetImageHDC(GetWorkingImage()), x, y, w, h,
		GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2) {
	const vector2& pos_camera = camera.get_positon();
	line(x1 - pos_camera.x, y1 - pos_camera.y, x2 - pos_camera.x, y2 - pos_camera.y);
}

inline void sketch_image(IMAGE* src, IMAGE* dest) {
	int w = src->getwidth(), h = src->getheight();
	Resize(dest, w, h);
	DWORD* src_buf = GetImageBuffer(src);
	DWORD* dest_buf = GetImageBuffer(dest);
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++) {
			int idx = y * w + x;
			dest_buf[idx] = BGR(RGB(255, 255, 255)) | (src_buf[idx] & 0xFF000000);
		}
}

class Animation {
public:
	Animation() = default;
	~Animation() = default;

	void reset() {
		timer = 0;
		idx_frame = 0;
	}

	void set_atlas(Atlas* target) {
		reset();
		atlas = target;
	}

	void set_loop(bool flag) {
		is_loop = flag;
	}

	void set_interval(std::size_t ms) {
		interval = ms;
	}

	std::size_t index() noexcept {
		return idx_frame;
	}

	IMAGE* frame() {
		return atlas->get_image(idx_frame);
	}

	bool check_finished() {
		if (is_loop) return false;
		else return (idx_frame == atlas->size() - 1);
	}

	void on_update(std::size_t delta) {
		timer += delta;
		if (timer >= interval) {
			timer = 0;
			idx_frame++;
			if (idx_frame >= atlas->size()) {
				idx_frame = (is_loop ? 0 : atlas->size() - 1);
				if (!is_loop && callback)
					callback();
			}
		}
	}

	void on_draw(const Camera& camera, std::size_t x, std::size_t y) const {
		putimage_alpha(camera, x, y, atlas->get_image(idx_frame));
	}

	void set_callback(std::function<void()> target) {
		callback = target;
	}
private:
	std::size_t timer = 0;
	std::size_t interval= 0;
	std::size_t idx_frame = 0;
	bool is_loop = true;
	Atlas* atlas = nullptr;
	std::function<void()> callback;
};
#endif