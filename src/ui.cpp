#include "../hdr/ui.hpp"

namespace engine {
	namespace render {
		namespace ui {
			sf::Font font;
			gs::Text text;
			sf::Texture checkBoxOffTexture, checkBoxOnTexture;
			gs::Menu menu; 
			gs::Slider gridSizeSlider;
			gs::Slider animationSpeedSlider;
			gs::Checkbox methodButtons[3];
			gs::Checkbox exitEarlyButton;
			gs::Button animationButtons[4];
			std::string animationButtonText[4] = {
				"Start", "Pause", "Reset", "Inc"
			};
			gs::Checkbox displayFrontierButton;
			gs::Checkbox displaySearchedButton;
			gs::Button framerateButton;
			gs::Button gridButtons[4];
			std::string gridButtonText[4] = {
				"Clear Grid", "Fill Grid", "Light Noise", "Heavy Noise"
			}; 
			gs::Button exitButton;
			gs::Slider soundSlider;
			bool startMarkerLocked = false; 
			bool endMarkerLocked = false; 

			void loadAssets() {
				font.loadFromFile("font.ttf"); 
				checkBoxOffTexture.loadFromFile("checkBoxOff.png"); 
				checkBoxOnTexture.loadFromFile("checkBoxOn.png"); 

				handleAssets(); 
			}
			void handleAssets() {
				// text
				text.setFont(font);

				// gridSizeSlider
				gridSizeSlider.setSize(constrainedGridSize.x - 50.0f, 20.0f); 
				gridSizeSlider.setPosition(gridOffset.x + 25.0f, 880.0f);
				gridSizeSlider.setOnColor(gs::Color::Transparent); 
				gridSizeSlider.setOffColor(gs::Color::Transparent); 
				gridSizeSlider.setOnOutlineThickness(3.0f); 
				gridSizeSlider.setOffOutlineThickness(3.0f); 
				gridSizeSlider.button.setSize(200.0f, 22.0f); 
				gridSizeSlider.button.setSelectedFillColor(gs::Color(
					100, 100, 100)
				);
				gridSizeSlider.button.setScaleModifiers(1.0f); 
				gridSizeSlider.setPercentage(25.0f); 
				menu.add(&gridSizeSlider);

				// animationSpeedSlider
				animationSpeedSlider = gridSizeSlider;
				animationSpeedSlider.setSize(260.0f, 20.0f);
				animationSpeedSlider.move(1600.0f, 0.0f); 
				animationSpeedSlider.button.setSize(50.0f, 22.0f); 
				animationSpeedSlider.setPercentage(25.0f); 
				menu.add(&animationSpeedSlider); 

				// methodButtons
				methodButtons[0].setSize(270.0f, 50.0f); 
				methodButtons[0].setPosition(
					window::windowSize.x - ((window::windowSize.x -
						constrainedGridSize.x - methodButtons[0].getSize().x)
							/ 2.0f) - methodButtons[0].getSize().x,
					188.0f
				); 
				methodButtons[0].setInactiveTextFillColor(gs::Color::White); 
				methodButtons[0].setClickedTextFillColor(gs::Color::White); 
				methodButtons[0].setScaleModifiers(1.0f); 
				methodButtons[0].setFont(font); 
				methodButtons[0].setTextScale(0.65f, 0.65f); 
				methodButtons[0].setString("Breadth First Search"); 
				menu.add(&methodButtons[0]);

				methodButtons[1] = methodButtons[0];
				methodButtons[1].move(0.0f, 70.0f); 
				methodButtons[1].setString("Greedy Best First"); 
				menu.add(&methodButtons[1]); 

				methodButtons[2] = methodButtons[1]; 
				methodButtons[2].move(0.0f, 70.0f); 
				methodButtons[2].setString("A* Algorithm"); 
				menu.add(&methodButtons[2]); 

				methodButtons[0].eventTriggerer = 
					gs::Button::EventTriggerer::None; 
				methodButtons[0].isSelected = true;
				methodButtons[0].isClickedOn = true;
				methodButtons[0].update(); 
				methodButtons[0].eventTriggerer =
					gs::Button::EventTriggerer::ActiveMouse; 

				// exitEarlyButton
				exitEarlyButton.setSize(30.0f, 30.0f); 
				exitEarlyButton.setPosition(1640.0f, 400.0f); 
				exitEarlyButton.clear(); 
				exitEarlyButton.eventTriggerer =
					gs::Button::EventTriggerer::None; 
				exitEarlyButton.isSelected = true; 
				exitEarlyButton.isClickedOn = true; 
				exitEarlyButton.update(); 
				exitEarlyButton.eventTriggerer =
					gs::Button::EventTriggerer::ActiveMouse; 
				menu.add(&exitEarlyButton); 

				// displayFrontierButton
				displayFrontierButton = exitEarlyButton; 
				displayFrontierButton.move(0.0f, 165.0f); 
				menu.add(&displayFrontierButton); 

				// displaySearchedButton
				displaySearchedButton = displayFrontierButton; 
				displaySearchedButton.move(0.0f, 50.0f); 
				menu.add(&displaySearchedButton);

				// framerateButton 
				framerateButton.applyStyle(methodButtons[0].getStyle()); 
				framerateButton.setSize(220.0f, 50.0f); 
				framerateButton.setPosition(1625.0f, 675.0f); 
				framerateButton.setFont(font); 
				framerateButton.setTextScale(0.65f, 0.65f);
				framerateButton.setString("Change Framerate");
				menu.add(&framerateButton); 

				// animationButtons
				for (int animationButtonIndex = 0; animationButtonIndex < 4;
					animationButtonIndex++
				) {
					gs::Button& currentAnimationButton = 
						animationButtons[animationButtonIndex];

					currentAnimationButton.applyStyle(
						methodButtons[0].getStyle()
					);
					currentAnimationButton.setSize(75.0f, 50.0f); 
					currentAnimationButton.setPosition(1606.0f, 753.0f);
					currentAnimationButton.move(
						78.0f * animationButtonIndex, 0.0f
					); 
					currentAnimationButton.setFont(font); 
					currentAnimationButton.setTextScale(0.65f, 0.65f); 
					currentAnimationButton.setString(
						animationButtonText[animationButtonIndex]
					);

					menu.add(&currentAnimationButton);
				}

				// gridButtons
				for (int gridButtonIndex = 0; gridButtonIndex < 4; 
					gridButtonIndex++
				) {
					gs::Button& currentGridButton = 
						gridButtons[gridButtonIndex]; 

					currentGridButton.applyStyle(methodButtons[0].getStyle()); 
					currentGridButton.setSize(350.0f, 80.0f); 
					currentGridButton.setPosition(100.0f, 950.0f); 
					currentGridButton.move(353.0f * gridButtonIndex, 0.0f); 
					currentGridButton.setFont(font); 
					currentGridButton.setTextScale(1.0f, 1.0f); 
					currentGridButton.setString(
						gridButtonText[gridButtonIndex]
					); 

					menu.add(&currentGridButton);
				}

				// exitButton
				exitButton.setSize(100.0f, 50.0f); 
				exitButton.setPosition(1800.0f, 1010.0f); 
				exitButton.setShapeColors(gs::Color::Transparent); 
				exitButton.setScaleModifiers(1.0f); 
				exitButton.setFont(font); 
				exitButton.setString("Exit");
				exitButton.setInactiveTextFillColor(gs::Color::Black); 
				exitButton.setSelectedTextFillColor(gs::Color(100, 100, 100));
				exitButton.setColorAdjustSpeed(100.0f); 
				menu.add(&exitButton); 

				menu.update(); 

				// soundSlider
				soundSlider = animationSpeedSlider; 
				soundSlider.move(0.0f, 100.0f); 
				menu.add(&soundSlider); 
			}

			void updateUI() {
				// Handle UI elements. 

				// Moves slider with scroll wheel. 
				gridSizeSlider.button.move(window::mouseDelta * -32.0f, 0.0f); 
				
				// Sets chech box textures for all of the check boxes. 
				exitEarlyButton.setTexture(
					earlyExit ? checkBoxOnTexture : checkBoxOffTexture
				); 
				displayFrontierButton.setTexture(
					displayFrontier ? checkBoxOnTexture : checkBoxOffTexture
				);
				displaySearchedButton.setTexture(
					displaySearched ? checkBoxOnTexture : checkBoxOffTexture
				); 

				menu.update(); 

				float gridScale = std::floor(gridSizeSlider.getPercentage());

				// Makes sure the percentage is even to prevent uneven tiles. 
				if (static_cast<int>(gridScale) % 2 == 1)
					gridScale -= 1.0f; 

				gridScale = std::max(gridScale, 2.0f); 
				grid.setSize(gridScale);

				float animationSpeed = animationSpeedSlider.getPercentage(); 

				// Updates the updating speed for the grid flooding. 
				if (animationSpeed > 50.0f) {
					animationSpeed -= 49.0f; 
					updateSpeed = animationSpeed; 
				}
				else 
					updateSpeed = animationSpeed / 50.0f; 

				for (int methodButtonIndex = 0; methodButtonIndex < 3;
					methodButtonIndex++
				) {
					// Current method button being checked for a mouse event. 
					gs::Checkbox& methodButton = 
						methodButtons[methodButtonIndex];

					if (methodButton.isSelected && methodButton.isClickedOn) {
						PathfindingMethod prvsMethod = pathfindingMethod; 

						// Updates the pathfinding method. 
						pathfindingMethod = static_cast<PathfindingMethod>(
							methodButtonIndex
						);

						if (pathfindingMethod != prvsMethod) {
							clearSearch(grid);
							grid.searching = false; 

							// This loop disables the other method boxes when a new
							// one is clicked on. 
							for (int buttonInc = 1; buttonInc < 3; buttonInc++) {
								gs::Checkbox& nextButton = methodButtons[
									(methodButtonIndex + buttonInc) % 3
								];

								// Only disables button if already selected. 
								if (nextButton.isClickedOn) {
									nextButton.eventTriggerer =
										gs::Button::EventTriggerer::None;
									nextButton.isSelected = true;
									nextButton.update();
									nextButton.eventTriggerer =
										gs::Button::EventTriggerer::ActiveMouse;
								}
							}
						}
					}
				}

				earlyExit = exitEarlyButton.isClickedOn; 
				displayFrontier = displayFrontierButton.isClickedOn; 
				displaySearched = displaySearchedButton.isClickedOn; 

				// Changes framerate. 
				if (framerateButton.isSelected && framerateButton.isClickedOn)
					window::virtualFramerate = window::virtualFramerate == 30 ?
						60 : 30; 

				// Starts search. 
				if (animationButtons[0].isSelected &&
					animationButtons[0].isClickedOn
				) {
					initSearch(grid);

					grid.searching = true;
					pause = false; 
				}
				// Pauses search. 
				else if (animationButtons[1].isSelected &&
					animationButtons[1].isClickedOn
				)
					pause = !pause;
				// Resets search. 
				else if (animationButtons[2].isSelected &&
					animationButtons[2].isClickedOn
				) {
					clearSearch(grid);
					grid.searching = false; 
				}
				// Increments search. 
				else if (animationButtons[3].isSelected &&
					animationButtons[3].isClickedOn
				)
					inc = true; 

				// Clears grid. 
				if (gridButtons[0].isSelected && gridButtons[0].isClickedOn)
					grid.clear(1);
				// Fills grid with solid tiles. 
				else if (gridButtons[1].isSelected &&
					gridButtons[1].isClickedOn
				) 
					grid.clear(0);
				// Generates noise at a level of 15%. 
				else if (gridButtons[2].isSelected &&
					gridButtons[2].isClickedOn
				)
					grid.generateNoise(15); 
				// Generates noise at a level of 30%. 
				else if (gridButtons[3].isSelected &&
					gridButtons[3].isClickedOn
				)
					grid.generateNoise(30);

				// Closes application if exit is pressed. 
				if (exitButton.isSelected && exitButton.isClickedOn)
					window::winmain->close(); 

				audio::changeVolume(soundSlider.getPercentage());

				// Handle tile code. 

				// Start marker interaction. 
				if (!startMarkerLocked && !endMarkerLocked && 
					getMouseTile(grid) == grid.startMarker &&
					gs::input::activeMouseClickL
				)
					startMarkerLocked = true; 
				if (startMarkerLocked) {
					if (!gs::input::activeMouseClickL || !mouseOnGrid())
						startMarkerLocked = false;
					else {
						grid.startMarker = getMouseTile(grid);
						// Resets grid if start marker is moved. This is due to 
						// the path needing to be regenerated to work so it is
						// instead reset. 
						clearSearch(grid);
						grid.searching = false;
					}
				}
				// End marker interaction. 
				if (!endMarkerLocked && !startMarkerLocked &&
					getMouseTile(grid) == grid.endMarker &&
					gs::input::activeMouseClickL
				)
					endMarkerLocked = true; 
				if (endMarkerLocked) {
					if (!gs::input::activeMouseClickL || !mouseOnGrid())
						endMarkerLocked = false;
					else {
						grid.endMarker = getMouseTile(grid);
						// Updates path after moving end marker. 
						generatePath(grid); 
					}
				}

				// Allows walls to be placed and erased. 
				if (mouseOnGrid()) {
					if (!startMarkerLocked && !endMarkerLocked) {
						// Left mouse click to fill tile. 
						if (gs::input::activeMouseClickL)
							grid.setTile(getMouseTile(grid), 0);
						// Right mouse click to delete tile. 
						else if (gs::input::activeMouseClickR)
							grid.setTile(getMouseTile(grid), 1); 
					}
				}
			}

			void renderUI() {
				// Text information. 

				renderText(
					"Grid Size", { 720.0f, 820.0f }, 1.0f, gs::Color::Black
				);

				renderText(
					"Pathfinding\nSettings", { 1638.0f, 20.0f }, 1.25f,
					gs::Color::Black
				);
				renderText(
					"Method", { 1700.0f, 125.0f }, 1.0f, gs::Color::Black
				);
				renderText(
					"Exit Search Early", { 1680.0f, 403.0f }, 0.65f, 
					gs::Color::Black
				);
				renderText(
					"Display Frontier", { 1680.0f, 568.0f }, 0.65f,
					gs::Color::Black
				);
				renderText(
					"Display Searched", { 1680.0f, 618.0f }, 0.65f,
					gs::Color::Black
				);
				renderText(
					std::to_string(window::virtualFramerate), 
					{ 1868.0f, 687.0f }, 0.75f, gs::Color::Black
				);

				renderText(
					"Animation\nSettings", { 1638.0f, 450.0f }, 1.25f,
					gs::Color::Black
				);
				renderText(
					"Animation Speed", { 1622.0f, 820.0f }, 1.0f, 
					gs::Color::Black
				);
				renderText(
					"Sound Volume", { 1648.0f, 920.0f }, 1.0f,
					gs::Color::Black
				);

				// Note: This renders all components attached to the menu. 
				gs::draw(window::winmain, menu);
			}
			void renderText(
				const std::string& string, gs::Vec2f position, float scale,
				gs::Color fillColor, float outlineThickness, 
				gs::Color outlineColor
			) {
				text.setString(string); 
				text.setPosition(position); 
				text.setScale(scale, scale); 
				text.setFillColor(fillColor); 
				text.setOutlineThickness(outlineThickness); 
				text.setOutlineColor(outlineColor); 

				gs::draw(window::winmain, text); 
			}
		}
	}
}