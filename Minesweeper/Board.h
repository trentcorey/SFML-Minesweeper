#pragma once
#include "SFML/Graphics.hpp"
using namespace std;

class Board
{
public:
	class Tile
	{
		Tile* topLeft;
		Tile* top;
		Tile* topRight;
		Tile* right;
		Tile* bottomRight;
		Tile* bottom;
		Tile* bottomLeft;
		Tile* left;
	public:
		unsigned int xPosLeft;
		unsigned int xPosRight;
		unsigned int yPosTop;
		unsigned int yPosBottom;
		bool mine;
		int dangerousNeighbors;
		string tileStatus;
		Tile();
		vector<Tile*> adjacentTiles;
		void AssignNeighbors(unsigned int xPos, unsigned int yPos, Board& board);
	};

	Board();
	void NeighborChecker();
	int mineCount;
	bool debugMode;
	string winState;
	Tile tiles[25][16];
};
