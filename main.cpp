#include "easyx.h"
#include "Windows.h"

#include "utility.h"
#include "atlas.h"
#include "scene_manager.h"
#include "menu_scene.h"
#include "game_scene.h"
#include "selector_scene.h"

//2330 lines in total


int main() {
	ExMessage msg;
	initgraph(1280, 720);
	initresources();
	BeginBatchDraw();
	menu_scene = new MenuScene();
	game_scene = new GameScene();
	selector_scene = new SelectorScene();
	scene_manager.set_current_scene(menu_scene);

	while (true) {
		DWORD frame_begin_time = GetTickCount();
		while (peekmessage(&msg)) {
			scene_manager.on_input(msg);
		}

		static DWORD last_tick = GetTickCount();
		DWORD this_tick = GetTickCount();
		DWORD delta_tick = this_tick - last_tick;
		scene_manager.on_update(delta_tick);
		last_tick = this_tick;

		cleardevice();
		scene_manager.on_draw(main_camera);
		FlushBatchDraw();
		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_begin_time;
		if (frame_delta_time < 1000 / max_FPS)
			Sleep(1000 / max_FPS - frame_delta_time);
	}

	EndBatchDraw();
	delete menu_scene;
	delete game_scene;
	return 0;
}