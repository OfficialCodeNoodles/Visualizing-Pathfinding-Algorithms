#include "../hdr/render.hpp"
#include "../hdr/ui.hpp"

namespace engine {
	namespace render {
		const gs::Vec2f constrainedGridSize = gs::Vec2f(1600, 800); 
		const gs::Vec2f gridOffset = gs::Vec2f(3.0f, 3.0f);

		sf::RectangleShape tile;
		gs::Vec2f tileSize;
		bool displayFrontier = true; 
		bool displaySearched = true; 

		bool mouseOnGrid() {
			// Current position of mouse. 
			const gs::Vec2f mousePosition = gs::input::mousePosition; 
			return mousePosition.x >= gridOffset.x &&
				mousePosition.x <= constrainedGridSize.x + gridOffset.x &&
				mousePosition.y >= gridOffset.y &&
				mousePosition.y <= constrainedGridSize.y + gridOffset.y; 
		}
		gs::Vec2i getMouseTile(const Grid& grid) {
			// Size of tile for rendering. 
			tileSize = gs::Vec2f(
				constrainedGridSize.x / grid.size.x,
				constrainedGridSize.y / grid.size.y
			);

			gs::Vec2i blockTile; 
			
			// Constrains tile to bounds of grid. 
			blockTile.x = gs::util::clamp(
				gs::input::mousePosition.x - gridOffset.x, 0.0f,
				constrainedGridSize.x
			);
			blockTile.y = gs::util::clamp(
			    gs::input::mousePosition.y - gridOffset.y, 0.0f,
				constrainedGridSize.y
			);

			// Scales tile to tileSize. 
			blockTile.x /= tileSize.x; 
			blockTile.y /= tileSize.y; 

			// Constrains tile position to grid size. 
			gs::util::clamp(&blockTile.x, 0, grid.size.x - 1); 
			gs::util::clamp(&blockTile.y, 0, grid.size.y - 1);

			return blockTile; 
		}

		void renderGrid(const Grid& grid) {
			// Size of tile for rendering. 
			tileSize = gs::Vec2f(
				constrainedGridSize.x / grid.size.x, 
				constrainedGridSize.y / grid.size.y 
			);
			
			tile.setSize(gs::Vec2f(
				std::floor(tileSize.x) - 3.0f, // Creates a gap between tiles. 
				std::floor(tileSize.y) - 3.0f
			)); 
			// Fills gap with outline. 
			tile.setOutlineThickness(3.0f); 
			tile.setOutlineColor(gs::Color::White);

			for (int xpos = 0; xpos < grid.size.x; xpos++) {
				for (int ypos = 0; ypos < grid.size.y; ypos++) {
					Tile currentTile = grid.getTile({ xpos, ypos }); 
					gs::Color currentTileColor; 

					tile.setPosition(
						gridOffset.x + (xpos * tileSize.x),
						gridOffset.y + (ypos * tileSize.y)
					);

					if (currentTile != -1) {
						switch (currentTile) {
						case 0: // Solid tile 
							currentTileColor = gs::Color(70, 70, 75);
							break; 
						case 1: // Empty tile 
							currentTileColor = gs::Color(200, 200, 180);
							break;
						}

						// Highlights tile when selected. 
						if (getMouseTile(grid) == gs::Vec2i(xpos, ypos) && 
							mouseOnGrid()
						)
							currentTileColor = gs::util::approach(
								currentTileColor, gs::Color::White, 15.0f
							);

						tile.setFillColor(currentTileColor); 

						window::winmain->draw(tile);
					}
				}
			}

			if (displaySearched) {
				// Renders costMap instead of the searched vector when using
				// the A* Algorithm. 
				if (pathfindingMethod == PathfindingMethod::AStar)
					renderCostMap(grid); 
				else
					renderSearched(grid);
			}
			if (displayFrontier) {
				if (pathfindingMethod == PathfindingMethod::Breadth)
					renderFrontier(grid);
				// The other pathfinding algorithms use a priority queue 
				// instead of a normal one. 
				else
					renderPriorityFrontier(grid); 
			}
			renderPath(grid); 

			tile.setOutlineColor(gs::Color::Black);
			
			// Makes sure the start marker is actually on the grid. 
			if (grid.isValidTile(grid.startMarker)) {
				tile.setPosition(
					gridOffset.x + (grid.startMarker.x * tileSize.x),
					gridOffset.y + (grid.startMarker.y * tileSize.y)
				);
				tile.setFillColor(gs::Color(50, 235, 40));

				// Highlights start marker when selected. 
				if (ui::startMarkerLocked || (mouseOnGrid() &&
					getMouseTile(grid) == grid.startMarker)
				)
					tile.setFillColor(gs::util::approach(
						tile.getFillColor(), gs::Color::White, 15.0f
					));

				window::winmain->draw(tile);
			}
			if (grid.isValidTile(grid.endMarker)) {
				tile.setPosition(
					gridOffset.x + (grid.endMarker.x * tileSize.x),
					gridOffset.y + (grid.endMarker.y * tileSize.y)
				);
				tile.setFillColor(gs::Color(235, 50, 40));

				// Highlights end marker when selected. 
				if (ui::endMarkerLocked || (mouseOnGrid() &&
					getMouseTile(grid) == grid.endMarker)
				)
					tile.setFillColor(gs::util::approach(
						tile.getFillColor(), gs::Color::White, 15.0f
					));

				window::winmain->draw(tile); 
			}
		}
		void renderPath(const Grid& grid) {
			static sf::RectangleShape pathSegment; 

			// This code is ugly af. 
			pathSegment.setSize(gs::Vec2f(
				tileSize.x / 3.0f, (tileSize.y / 2.0f) + (tileSize.x / 6.0f)
			));
			pathSegment.setOrigin(
				pathSegment.getSize().x / 2.0f, pathSegment.getSize().y 
					- (tileSize.x / 6.0f)
			);
			pathSegment.setFillColor(gs::Color(200, 0, 255));

			// Loops through the path positions starting on the second. Note:
			// I casted the size to an int to prevent an integer underflow. 
			for (int pathIndex = 1; pathIndex < 
				static_cast<int>(grid.path.size()) - 1; pathIndex++
			) {
				// Previous position in path. 
				gs::Vec2i prvsPosition = grid.path[pathIndex - 1]; 
				// Center position in path. 
				gs::Vec2i currentPosition = grid.path[pathIndex];
				// Next position in path. 
				gs::Vec2i nextPosition = grid.path[pathIndex + 1]; 

				if (!grid.isValidTile(currentPosition))
					continue;
				
				// Vector difference in distance between the previous step and
				// the current. This is used to create on half of the path. 
				gs::Vec2i backDelta = prvsPosition - currentPosition; 
				// Vector difference in distance between the next step and the
				// current. 
				gs::Vec2i frontDelta = nextPosition - currentPosition; 

				// Loops through both deltas to build path. 
				for (gs::Vec2i segmentDelta : { backDelta, frontDelta }) {
					// Upward vertical segment. 
					if (segmentDelta == gs::Vec2i(0, -1))
						pathSegment.setRotation(0.0f);
					// Leftward horizontal segment. 
					else if (segmentDelta == gs::Vec2i(-1, 0))
						pathSegment.setRotation(-90.0f);
					// Downward vertical segment. 
					else if (segmentDelta == gs::Vec2i(0, 1))
						pathSegment.setRotation(-180.0f);
					// Rightward horizontal segment. 
					else
						pathSegment.setRotation(-270.0f); 

					pathSegment.setPosition(
						gridOffset.x + (currentPosition.x * tileSize.x),
						gridOffset.y + (currentPosition.y * tileSize.y)
					);
					// Centers segment in the middle of the tile. 
					pathSegment.move(tileSize * 0.5f); 
					
					window::winmain->draw(pathSegment); 
				}
			}
		}
		void renderFrontier(const Grid& grid) {
			// Copy of the grid frontier. Note: I need to make a copy to access
			// all the elements. 
			std::queue<gs::Vec2i> frontier = grid.frontier; 

			tile.setOutlineColor(gs::Color::Black); 

			while (!frontier.empty()) {
				gs::Vec2i frontierPosition = frontier.front();
				frontier.pop(); 

				if (grid.isValidTile(frontierPosition)) {
					tile.setPosition(
						gridOffset.x + (frontierPosition.x * tileSize.x),
						gridOffset.y + (frontierPosition.y * tileSize.y)
					);
					tile.setFillColor(gs::Color(200, 230, 255, 220));

					window::winmain->draw(tile); 
				}
			}
		}
		void renderPriorityFrontier(const Grid& grid) {
			// Copy of the grid priority frontier. Note: I need to make a copy
			// to access all the elements. 
			std::priority_queue<QueueItem> priorityFrontier = 
				grid.priorityFrontier; 

			tile.setOutlineColor(gs::Color::Black);

			while (!priorityFrontier.empty()) {
				gs::Vec2i frontierPosition = priorityFrontier.top().position; 
				priorityFrontier.pop(); 

				if (grid.isValidTile(frontierPosition)) {
					tile.setPosition(
						gridOffset.x + (frontierPosition.x * tileSize.x),
						gridOffset.y + (frontierPosition.y * tileSize.y)
					);
					tile.setFillColor(gs::Color(200, 230, 255, 220));

					window::winmain->draw(tile);
				}
			}
		}
		void renderSearched(const Grid& grid) {
			for (gs::Vec2i searchedPosition : grid.searched) {
				if (grid.isValidTile(searchedPosition)) {
					tile.setPosition(
						gridOffset.x + (searchedPosition.x * tileSize.x),
						gridOffset.y + (searchedPosition.y * tileSize.y)
					);
					tile.setFillColor(gs::Color(0, 0, 0, 50)); 

					window::winmain->draw(tile); 
				}
			}
		}
		void renderCostMap(const Grid& grid) {
			// Gets the [ Key, Value ] pairs within the costMap. So pair.first
			// corresponds to the position key. 
			for (const auto& pair : grid.costMap) {
				if (grid.isValidTile(pair.first)) {
					tile.setPosition(
						gridOffset.x + (pair.first.x * tileSize.x),
						gridOffset.y + (pair.first.y * tileSize.y)
					);
					tile.setFillColor(gs::Color(0, 0, 0, 50));

					window::winmain->draw(tile);
				}
			}
		}
	}
}