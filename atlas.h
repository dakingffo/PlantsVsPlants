#pragma once
#include <iostream>
#include <vector>
#include "easyx.h"

#ifndef ATLAS_H
#define ATLAS_H

class Atlas {
public:
	Atlas() = default;
	~Atlas() = default;

	void load_from_file(LPCTSTR path_template, std::size_t count) {
		img_list.clear();
		img_list.resize(count);

		TCHAR path_file[path_size]{};
		for (std::size_t i = 0; i < count; i++) {
			swprintf_s(path_file, path_template, i + 1);
			loadimage(&img_list[i], path_file);
		}
	}

	void clear() {
		img_list.clear();
	}

	std::size_t size() noexcept {
		return img_list.size();
	}

	IMAGE* get_image(std::size_t idx) {
		if (idx >= img_list.size())
			return nullptr;
		return &img_list[idx];
	}

	void add_image(const IMAGE& img) {
		img_list.push_back(img);
	}

private:
	static constexpr int path_size = 256;
	std::vector<IMAGE> img_list;
};
#endif
