#include "../hdr/grid.hpp"
#include "../hdr/path.hpp"
#include "../hdr/window.hpp"

namespace engine {
	size_t HashContainer::operator()(const gs::Vec2i& vec) const {
		// Used this post as a reference to make hash for 2 component vector. 
		// https://stackoverflow.com/questions/45395071/hash-for-a-stdpair-for-use-in-an-unordered-map
		return size_t(vec.x) << 32 | vec.y;
	}

	QueueItem::QueueItem() : cost(0) {
	}
	QueueItem::QueueItem(gs::Vec2i position, int cost) : QueueItem() {
		this->position = position; 
		this->cost = cost; 
	}

	bool QueueItem::operator<(const QueueItem& item) const {
		return cost < item.cost;
	}
	bool QueueItem::operator>(const QueueItem& item) const {
		return cost > item.cost;
	}

	Grid grid;
	bool pause = false,
		inc = false; 
	float updateSpeed = 1.0f; 

	Grid::Grid() : endMarker({ 23, 11 }), searching(false), tiles(nullptr),
		blockSize(0) {
	}
	Grid::~Grid() {
		delete tiles; 
	}
	
	void Grid::create(gs::Vec2i size) {
		// Allows for reallocation. 
		if (tiles != nullptr) delete tiles;
		
		blockSize = size.x * size.y; 
		tiles = new Tile[blockSize]; 
		
		realSize = size; 
		this->size = realSize; 
		
		// Fill grid with empty tiles. 
		clear(1); 
	}
	void Grid::update() {
		bool pathGenerated = false;

		if (inc) {
			pause = true; 
			inc = false;	 

			// Just cause it's 2022 doesn't mean I can't use goto statements.
			goto SEARCHING; 
		}

		if (searching && !pause) {
			// If the updateSpeed is larger that 1 update per second then it 
			// is updated multiple per second. 
			if (updateSpeed > 1.0f) {
				for (int cycle = 0; cycle < updateSpeed; cycle++) {
					pathGenerated = floodGrid(*this);

					if (pathGenerated)
						break; 
				}
			}
			else if (ticks % static_cast<int>(1.0f / updateSpeed) == 0) 
SEARCHING:
				pathGenerated = floodGrid(*this);

			if (pathGenerated) {
				generatePath(*this);
				searching = false; 
			}
		}
	}
	void Grid::clear(Tile tile) {
		for (int xpos = 0; xpos < size.x; xpos++) 
			for (int ypos = 0; ypos < size.y; ypos++) 
				setTile({ xpos, ypos }, tile); 

		clearSearch(*this);
		searching = false; 
	}
	void Grid::generateNoise(int coverage) {
		clear(1); 

		for (int xpos = 0; xpos < size.x; xpos++) {
			for (int ypos = 0; ypos < size.y; ypos++) {
				gs::Vec2i position = gs::Vec2i(xpos, ypos);

				if (position == startMarker || position == endMarker)
					continue; 

				// Only covers a certain percentage of the tiles. 
				if (rand() % 100 < coverage)
					setTile(position, 0);
			}
		}
	}

	void Grid::setTile(gs::Vec2i position, Tile tile) {
		if (isValidTile(position))
			tiles[getArrayCoordinate(position)] = tile; 
	}
	void Grid::setSize(gs::Vec2i size) {
		this->size = size;
	} 
	void Grid::setSize(float percentage) {
		// Resizes usable size based on a percentage of the realSize. 
		size.x = realSize.x * (percentage / 100.0f);
		size.y = realSize.y * (percentage / 100.0f); 
	}

	Tile Grid::getTile(gs::Vec2i position) const {
		if (isValidTile(position))
			return tiles[getArrayCoordinate(position)];
		return Tile(-1);
	}
	gs::Vec2i Grid::getSize() const {
		return size;
	}
	gs::Vec2i Grid::getRealSize() const {
		return realSize;
	}
	bool Grid::isValidTile(gs::Vec2i position) const {
		// Note: This uses the size member not realSize that represents the
		// fully allocated size.
		return position.x >= 0 && position.x < getSize().x &&
			position.y >= 0 && position.y < getSize().y;
	}

	int Grid::getArrayCoordinate(gs::Vec2i position) const {
		if (isValidTile(position))
			// Each row is 1width wide so (y * width) + x gets a 1D coordinate.
			return (position.y * getRealSize().x) + position.x; 
		return Tile(-1);
	}
}