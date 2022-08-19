#include "../hdr/path.hpp"

namespace engine {
	PathfindingMethod pathfindingMethod = PathfindingMethod::Breadth;
	bool earlyExit = true; 
	gs::Vec2i focusPoint;

	float heuristic(gs::Vec2i base, gs::Vec2i target) {
		return static_cast<float>(
			std::abs(base.x - target.x) + std::abs(base.y - target.y)
		);
	}

	void initSearch(Grid& grid) {
		clearSearch(grid); 

		// Get started with the starting location. 
		grid.frontier.push(grid.startMarker); 
		grid.priorityFrontier.push(QueueItem(grid.startMarker, 0)); 
		grid.searched.push_back(grid.startMarker); 
		grid.pathMap[grid.startMarker] = grid.startMarker;
		grid.costMap[grid.startMarker] = 0; 
	}
	void clearSearch(Grid& grid) {
		// Function to clear the frontier queues. 
		static auto clearFrontiers = [&]() {
			while (!grid.frontier.empty())
				grid.frontier.pop(); 
			while (!grid.priorityFrontier.empty())
				grid.priorityFrontier.pop(); 
		};

		clearFrontiers(); 
		grid.searched.clear(); 
		grid.pathMap.clear(); 
		grid.costMap.clear(); 
		grid.path.clear();
	}
	bool floodGrid(Grid& grid) {
		bool match; 

		switch (pathfindingMethod) {
		case PathfindingMethod::Breadth:
			match = breadthFlood(grid); 
			break; 
		case PathfindingMethod::Greedy:
			match = greedyFlood(grid); 
			break; 
		case PathfindingMethod::AStar:
			match = aStarFlood(grid); 
			break; 
		}

		// Distance from start to end in the grid. 
		float pathDistance = gs::util::distance(
			grid.startMarker, grid.endMarker
		);
		// Distance from the focus point to the end. 
		float focusPointDistance = gs::util::distance(
			focusPoint, grid.endMarker
		);
		
		// Makes sure there is no dividing by zero. 
		if (pathDistance > 0.0f) {
			// Normalized distance on the path. 
			float ratio = focusPointDistance / pathDistance; 
			// Invert to give lower pitches first. 
			ratio = 1.0f - ratio; 

			// Plays pitch from 0.1 to 5.0. 
			audio::playSound(0.1f + (ratio * 5.0f)); 
		}

		return match; 
	}
	bool breadthFlood(Grid& grid) {
		// Can only search if there are spots left to search. 
		if (!grid.frontier.empty()) {
			// Next position in the frontier queue. 
			gs::Vec2i currentPosition = grid.frontier.front();
			// Removes position from queue. 
			grid.frontier.pop();
			focusPoint = currentPosition; 

			// Allows searching to stop early if the end position is reached. 
			if (currentPosition == grid.endMarker && earlyExit)
				return true; 

			gs::Vec2i nextPositions[4];
			int neighborCount = 0;

			for (int neighborIndex = 0; neighborIndex < 4; neighborIndex++) {
				gs::Vec2i testingPosition = currentPosition;

				// Fancy code to generate the positions around the center. 
				if (neighborIndex % 2 == 0)
					testingPosition.y += neighborIndex == 0 ? -1 : 1;
				else
					testingPosition.x += neighborIndex == 1 ? 1 : -1;

				// If the testingPositions is a valid tile and not solid then 
				// add it to the positions to be looked at. 
				if (grid.isValidTile(testingPosition) &&
					grid.getTile(testingPosition) != 0
				) {
					nextPositions[neighborCount] = testingPosition;
					neighborCount++;
				}
			}

			for (int nextIndex = 0; nextIndex < neighborCount; nextIndex++) {
				gs::Vec2i nextPosition = nextPositions[nextIndex];

				// Checks if nextPosition has already been searched. 
				if (std::find(grid.searched.begin(), grid.searched.end(),
					nextPosition) == grid.searched.end()
				) {
					grid.frontier.push(nextPosition);
					grid.searched.push_back(nextPosition);
					// Creates a link between the next position and the 
					// previous. 
					grid.pathMap[nextPosition] = currentPosition;
				}
			}

			return false; 
		}
		
		return true; 
	}
	bool greedyFlood(Grid& grid) {
		// Can only search if there are spots left to search. 
		if (!grid.priorityFrontier.empty()) {
			// Next position in the frontier queue. 
			gs::Vec2i currentPosition = grid.priorityFrontier.top().position;
			// Removes position from queue. 
			grid.priorityFrontier.pop(); 
			focusPoint = currentPosition;

			// Allows searching to stop early if the end position is reached. 
			if (currentPosition == grid.endMarker && earlyExit)
				return true;

			gs::Vec2i nextPositions[4];
			int neighborCount = 0;

			for (int neighborIndex = 0; neighborIndex < 4; neighborIndex++) {
				gs::Vec2i testingPosition = currentPosition; 

				// Fancy code to generate the positions around the center. 
				if (neighborIndex % 2 == 0)
					testingPosition.y += neighborIndex == 0 ? -1 : 1;
				else
					testingPosition.x += neighborIndex == 1 ? 1 : -1;

				// If the testingPositions is a valid tile and not solid then 
				// add it to the positions to be looked at. 
				if (grid.isValidTile(testingPosition) &&
					grid.getTile(testingPosition) != 0
				) {
					nextPositions[neighborCount] = testingPosition;
					neighborCount++;
				}
			}

			for (int nextIndex = 0; nextIndex < neighborCount; nextIndex++) {
				gs::Vec2i nextPosition = nextPositions[nextIndex];

				// Checks if nextPosition has already been searched. 
				if (std::find(grid.searched.begin(), grid.searched.end(),
					nextPosition) == grid.searched.end()
				) {
					grid.priorityFrontier.push(QueueItem(
						nextPosition, -heuristic(grid.endMarker, nextPosition)
					)); 
					grid.searched.push_back(nextPosition);
					// Creates a link between the next position and the 
					// previous. 
					grid.pathMap[nextPosition] = currentPosition;
				}
			}

			return false;
		}

		return true;
	}
	bool aStarFlood(Grid& grid) {
		// Can only search if there are spots left to search. 
		if (!grid.priorityFrontier.empty()) {
			// Next position in the frontier queue. 
			gs::Vec2i currentPosition = grid.priorityFrontier.top().position;
			// Removes position from queue. 
			grid.priorityFrontier.pop();
			focusPoint = currentPosition;

			// Allows searching to stop early if the end position is reached. 
			if (currentPosition == grid.endMarker && earlyExit)
				return true;

			gs::Vec2i nextPositions[4];
			int neighborCount = 0;

			for (int neighborIndex = 0; neighborIndex < 4; neighborIndex++) {
				gs::Vec2i testingPosition = currentPosition;

				// Fancy code to generate the positions around the center. 
				if (neighborIndex % 2 == 0)
					testingPosition.y += neighborIndex == 0 ? -1 : 1;
				else
					testingPosition.x += neighborIndex == 1 ? 1 : -1;

				// If the testingPositions is a valid tile and not solid then 
				// add it to the positions to be looked at. 
				if (grid.isValidTile(testingPosition) &&
					grid.getTile(testingPosition) != 0
				) {
					nextPositions[neighborCount] = testingPosition;
					neighborCount++;
				}
			}

			for (int nextIndex = 0; nextIndex < neighborCount; nextIndex++) {
				gs::Vec2i nextPosition = nextPositions[nextIndex]; 
				float cost = grid.costMap[currentPosition] + 1.0f;

				if (grid.costMap.count(nextPosition) == 0 ||
					cost < grid.costMap[nextPosition]
				) {
					grid.priorityFrontier.push(QueueItem(
						nextPosition,
						-cost + -heuristic(grid.endMarker, nextPosition) * 1.4f
					));
					// Creates a link between the next position and the 
					// previous. 
					grid.pathMap[nextPosition] = currentPosition;
					grid.costMap[nextPosition] = cost;
				}
			}

			return false;
		}

		return true;
	}
	void generatePath(Grid& grid) {
		// Position on the path. Note: It starts at the end to work backwards
		// when generating a path. 
		gs::Vec2i currentPosition = grid.endMarker;
		gs::Vec2i prvsPosition = gs::Vec2i(-1, -1);

		// Resets the path. 
		grid.path.clear(); 

		// Makes sure the position is actually in the pathMap. 
		if (grid.pathMap.count(currentPosition) != 0) {
			while (currentPosition != grid.startMarker) {
				grid.path.push_back(currentPosition);
				// Follows path to the next position. 
				currentPosition = grid.pathMap[currentPosition];

				// If the prvsPosition is the same as the currentPosition then
				// there is not a valid path that can be generated. 
				if (prvsPosition == currentPosition) {
					grid.path.clear();
					return;
				}

				prvsPosition = currentPosition;
			}

			// Adds final position to the path. 
			grid.path.push_back(currentPosition);
		}
	}
}