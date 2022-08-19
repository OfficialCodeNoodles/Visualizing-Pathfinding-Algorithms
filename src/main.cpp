#include "../hdr/ui.hpp"

//Globals

int ticks = 0;

void events() {
	engine::render::window::update(); 

	// Skips events if the window isn't being focused on. 
	if (!engine::render::window::winmain->hasFocus()) {
		ticks--; 
		return;
	}

	engine::grid.update(); 
	engine::render::ui::updateUI(); 
}
void graphics() {
	engine::render::window::begin(gs::Color(220, 220, 220)); 

	engine::render::renderGrid(engine::grid); 
	engine::render::ui::renderUI(); 

	engine::render::window::end(); 
}

int main() {
	srand(time(0));
	gs::util::Clock timer; 

	engine::render::window::create(true);

	HWND hwnd = GetConsoleWindow();
	// Hides console window. 
	ShowWindow(hwnd, 0);

	engine::render::ui::loadAssets(); 
	engine::audio::loadAssets(); 

	// Creates a grid with a maximum size of 100 by 50. 
	engine::grid.create(gs::Vec2i(100, 50));

	while (engine::render::window::winmain->isOpen()) {
		timer.begin(); 
		events();

		if (engine::render::window::virtualFramerate !=
			engine::render::window::framerate
		) {
			if (ticks % (engine::render::window::framerate /
				engine::render::window::virtualFramerate) == 0
			)
				graphics(); 
		}
		else
			graphics(); 

		timer.end(); 
		timer.wait(engine::render::window::framerate); 
		ticks++; 
	}

	engine::render::window::close(); 

	return 0; 
}

/*
Select Pathfinding Method

Early Exit 


Start Pause Reset Inc
Animation Speed Slider
*/