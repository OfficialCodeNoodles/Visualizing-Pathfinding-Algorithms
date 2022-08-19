#pragma once

//Dependencies
#include "resources.hpp"

namespace engine {
	namespace render {
		namespace window {
			extern const gs::Vec2i windowSize; 

			extern sf::RenderWindow* winmain; 
			extern bool isFullscreen; 

			// Framerate used for events. 
			extern int framerate; 
			// Framerate used for rendering. Note: Can be lower than the actual
			// framerate. 
			extern int virtualFramerate; 
			extern int mouseDelta; 

			extern sf::Image icon; 

			void create(bool fullscreen); 
			void initStates(); 
			void update(); 
			void close();

			void begin(gs::Color backgroundColor = gs::Color());
			void end(); 
		}
	}
}