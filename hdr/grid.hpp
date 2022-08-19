#pragma once

//Dependencies 
#include "resources.hpp"

namespace engine {
	// Alias for int. Note: If using this for a more complex situation this 
	// type can be replaced for a more advanced data type. 
	typedef int Tile; 

	// Used for hashing 2D vectors. 
	struct HashContainer {
		size_t operator()(const gs::Vec2i& vec) const;
	};
	// Used in the priority frontier to store a position and sort based on a
	// cost value. 
	struct QueueItem {
		gs::Vec2i position; 
		// Value used to sort item in a priority queue. 
		float cost; 

		QueueItem(); 
		QueueItem(gs::Vec2i position, int cost); 

		bool operator<(const QueueItem& item) const; 
		bool operator>(const QueueItem& item) const; 
	};

	// A allocated 2D array data structure. 
	class Grid {
	public:
		// Current usable size of grid. Note: Allocated size may be larger. 
		gs::Vec2i size; 
		// Starting location of the pathfinding. 
		gs::Vec2i startMarker; 
		// Target location of the pathfinding. 
		gs::Vec2i endMarker; 

		bool searching;
		// Tiles needing to be searched. 
		std::queue<gs::Vec2i> frontier; 
		std::priority_queue<QueueItem> priorityFrontier; 
		// Tiles already searched. 
		std::vector<gs::Vec2i> searched;
		// Contains a map of locations that is used to construct a path. 
		std::unordered_map<gs::Vec2i, gs::Vec2i, HashContainer> pathMap;
		// Contains a map of locations that have been searched and a value that
		// is used to prioritize certain pathways. 
		std::unordered_map<gs::Vec2i, float, HashContainer> costMap; 
		// Collection of positions that build a cohesive path. 
		std::vector<gs::Vec2i> path; 

		Grid(); 
		~Grid(); 

		// Allocates tiles and prepares grid for use. 
		void create(gs::Vec2i size); 
		void update(); 
		// Will clear the (entire - meaning allocated) grid to a given value. 
		void clear(Tile tile); 
		// Fills grid with random tiles based on the coverage amount. 
		void generateNoise(int coverage); 

		void setTile(gs::Vec2i position, Tile tile); 
		void setSize(gs::Vec2i size); 
		// Sets the usable size based on a percentage of the allocated size. 
		void setSize(float percentage); 

		Tile getTile(gs::Vec2i position) const; 
		gs::Vec2i getSize() const; 
		// Returns the allocated size of the grid. 
		gs::Vec2i getRealSize() const; 
		// Checks if a given position is in the current accessable grid. 
		bool isValidTile(gs::Vec2i position) const; 
	private:
		Tile* tiles; 
		// Size of grid as allocated. 
		gs::Vec2i realSize; 
		// Number of tiles in the grid. Note: Equal to the width * height. 
		int blockSize; 

		// Converts a 2D coordinate to a 1D coordinate for accessing the tiles. 
		int getArrayCoordinate(gs::Vec2i position) const; 
	};

	extern Grid grid;
	// Used to interact with the grid in real time. 
	extern bool pause, inc; 
	// Speed of the grid flooding. 
	extern float updateSpeed; 
}