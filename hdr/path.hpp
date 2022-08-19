#pragma once

//Dependencies
#include "grid.hpp"
#include "audio.hpp"

namespace engine {
	// The algorithm to be used for pathfinding. 
	enum class PathfindingMethod { Breadth, Greedy, AStar };
	// Currently selected pathfinding algorithm. 
	extern PathfindingMethod pathfindingMethod; 
	// Allows searching to finish early if the end has been found. 
	extern bool earlyExit; 
	extern gs::Vec2i focusPoint; 

	// Gives a manhattan distance between two points. Used to judge how good
	// different moves are. 
	float heuristic(gs::Vec2i base, gs::Vec2i target); 

	// Prepares a grid to be searched in. 
	void initSearch(Grid& grid); 
	// Clears search data from a grid. 
	void clearSearch(Grid& grid);
	// Used to search grid. 
	bool floodGrid(Grid& grid);
	// Used to fill the grid using the breadth first search algorithm. 
	bool breadthFlood(Grid& grid); 
	// Used to fill the grid using the greedy best first search algorithm. 
	bool greedyFlood(Grid& grid); 
	// Used to fill the grid using the A* algorithm. 
	bool aStarFlood(Grid& grid); 
	// Generates a path to an end using the pathMap of a grid. 
	void generatePath(Grid& grid);
}