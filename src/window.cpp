#include "../hdr/window.hpp"

namespace engine {
	namespace render {
		namespace window {
			const gs::Vec2i windowSize = gs::Vec2i(1920, 1080);

			sf::RenderWindow* winmain = nullptr;
			bool isFullscreen;

			int framerate = 60; 
			int virtualFramerate = 60;
			int mouseDelta;

			sf::Image icon;

			void create(bool fullscreen) {
				isFullscreen = fullscreen; 

				if (winmain == nullptr) {
					winmain = new sf::RenderWindow(
						sf::VideoMode(windowSize.x, windowSize.y),
						"Pathfinding Visualization", mGetStyle(isFullscreen)
					);
				}
				else {
					winmain->create(
						sf::VideoMode(windowSize.x, windowSize.y),
						"Pathfinding Visualization", mGetStyle(isFullscreen)
					);
				}

				initStates(); 
			}
			void initStates() {
				gs::input::setWindow(winmain); 

				// Scales window to desktop size. 
				winmain->setSize(gs::Vec2u(
					sf::VideoMode::getDesktopMode().width,
					sf::VideoMode::getDesktopMode().height
				));

				if (icon.getSize().x == 0) 
					icon.loadFromFile("icon.png");
				
				winmain->setIcon(
					icon.getSize().x, icon.getSize().y, icon.getPixelsPtr()
				);
			}
			void update() {
				sf::Event action; 

				gs::input::updateInputs(); 

				mouseDelta = 0; 

				while (winmain->pollEvent(action)) {
					gs::input::updateEvents(action); 

					switch (action.type) {
					case sf::Event::Closed:
						winmain->close();
						break; 
					case sf::Event::MouseWheelScrolled:
						mouseDelta = action.mouseWheelScroll.delta; 
						break; 
					case sf::Event::KeyPressed:
						switch (action.key.code) {
						case sf::Keyboard::Escape:
							winmain->close(); 
							break;
						case sf::Keyboard::F11:
							create(!isFullscreen); 
							break;
						}
						break; 
					}
				}
			}
			void close() {
				delete winmain; 
			}

			void begin(gs::Color backgroundColor) {
				winmain->clear(backgroundColor); 
			}
			void end() {
				winmain->display(); 
			}
		}
	}
}