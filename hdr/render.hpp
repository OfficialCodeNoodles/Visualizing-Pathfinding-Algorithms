#pragma once

//Dependencies
#include "window.hpp"
#include "grid.hpp"
#include "path.hpp"

namespace engine {
	namespace render {
		// Rendered size of the grid. 
		extern const gs::Vec2f constrainedGridSize; 
		extern const gs::Vec2f gridOffset; 

		extern sf::RectangleShape tile;
		// Current size of each tile. Note: This changes every time the zoom
		// changes. 
		extern gs::Vec2f tileSize; 
		extern bool displayFrontier; 
		extern bool displaySearched; 

		// Checks if the mouse position is within the grids bounds. 
		bool mouseOnGrid(); 
		// Finds what tile the mouse is currently on. 
		gs::Vec2i getMouseTile(const Grid& grid);

		void renderGrid(const Grid& grid); 
		void renderPath(const Grid& grid);
		void renderFrontier(const Grid& grid); 
		void renderPriorityFrontier(const Grid& grid); 
		void renderSearched(const Grid& grid);
		void renderCostMap(const Grid& grid); 
	}
}
