#pragma once

//Dependencies
#include "render.hpp"

namespace engine {
	namespace render {
		namespace ui {
			extern sf::Font font; 
			extern gs::Text text; 
			extern sf::Texture checkBoxOffTexture, checkBoxOnTexture; 
			extern gs::Menu menu;
			// Slider used to adjust the active size of the grid. 
			extern gs::Slider gridSizeSlider;
			// Slider used to adjust the speed of the grid filling animation. 
			extern gs::Slider animationSpeedSlider; 
			// Checkbox buttons used to change the pathfinding algorithm. 
			extern gs::Checkbox methodButtons[3];
			// Used to disable early exiting. 
			extern gs::Checkbox exitEarlyButton; 
			// Buttons used to handle the animations. 
			extern gs::Button animationButtons[4]; 
			// Names of the animation buttons. 
			extern std::string animationButtonText[4]; 
			// Used to hide the frontier from rendering. 
			extern gs::Checkbox displayFrontierButton; 
			// Used to hide the seached vector from rendering. 
			extern gs::Checkbox displaySearchedButton; 
			// Used to change the framerate. 
			extern gs::Button framerateButton; 
			// Buttons used to handle the grid. 
			extern gs::Button gridButtons[4];
			// Names of the grid buttons. 
			extern std::string gridButtonText[4];
			// Button to close application; 
			extern gs::Button exitButton; 
			// Slider used to change the sound volume. 
			extern gs::Slider soundSlider; 
			extern bool startMarkerLocked; 
			extern bool endMarkerLocked;

			void loadAssets(); 
			void handleAssets(); 

			void updateUI(); 

			void renderUI(); 
			void renderText(
				const std::string& string, gs::Vec2f position, float scale,
				gs::Color fillColor = gs::Color::White,
				float outlineThickness = 0.0f,
				gs::Color outlineColor = gs::Color()
			);
		}
	}
}