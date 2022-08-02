#include "Board.h"

Board::Board()
{
	mineCount = 0;
	winState = "In Progress";
	debugMode = false;

	for (unsigned int i = 0; i < 16; i++) // Filling board
	{
		for (unsigned int j = 0; j < 25; j++)
		{
			Tile tempTile;
			tiles[j][i] = tempTile;
		}
	}
	for (unsigned int i = 0; i < 16; i++) // Filling board
	{
		for (unsigned int j = 0; j < 25; j++)
		{
			tiles[j][i].AssignNeighbors(j, i, *this);
		}
	}
}

Board::Tile::Tile()
{
	mine = false;
	dangerousNeighbors = 0;
	tileStatus = "Hidden";
	xPosLeft = 0;
	xPosRight = 32;
	yPosTop = 0;
	yPosBottom = 32;
}

void Board::Tile::AssignNeighbors(unsigned int xPos, unsigned int yPos, Board& board)
{
	xPosRight += xPos * 32;
	xPosLeft += xPos * 32;
	yPosTop += yPos * 32;
	yPosBottom += yPos * 32;

	if (xPos == 0 && yPos == 0)
	{
		right = &board.tiles[xPos + 1][yPos]; // right = &board...
		adjacentTiles.push_back(right);
		bottomRight = &board.tiles[xPos + 1][yPos + 1];
		adjacentTiles.push_back(bottomRight);
		bottom = &board.tiles[xPos][yPos + 1];
		adjacentTiles.push_back(bottom);
	}
	else if (xPos == 0 && yPos != 0 && yPos != 15)
	{
		top = &board.tiles[xPos][yPos - 1];
		adjacentTiles.push_back(top);
		topRight = &board.tiles[xPos + 1][yPos - 1];
		adjacentTiles.push_back(topRight);
		right = &board.tiles[xPos + 1][yPos];
		adjacentTiles.push_back(right);
		bottomRight = &board.tiles[xPos + 1][yPos + 1];
		adjacentTiles.push_back(bottomRight);
		bottom = &board.tiles[xPos][yPos + 1];
		adjacentTiles.push_back(bottom);
	}
	else if (xPos == 0 && yPos == 15)
	{
		top = &board.tiles[xPos][yPos - 1];
		adjacentTiles.push_back(top);
		topRight = &board.tiles[xPos + 2][yPos - 1];
		adjacentTiles.push_back(topRight);
		right = &board.tiles[xPos + 1][yPos];
		adjacentTiles.push_back(right);
	}
	else if (xPos != 0 && xPos != 24 && yPos == 0)
	{
		left = &board.tiles[xPos - 1][yPos];
		adjacentTiles.push_back(left);
		bottomLeft = &board.tiles[xPos - 1][yPos + 1];
		adjacentTiles.push_back(bottomLeft);
		bottom = &board.tiles[xPos][yPos + 1];
		adjacentTiles.push_back(bottom);
		bottomRight = &board.tiles[xPos + 1][yPos + 1];
		adjacentTiles.push_back(bottomRight);
		right = &board.tiles[xPos + 1][yPos];
		adjacentTiles.push_back(right);
	}
	else if (xPos != 0 && xPos != 24 && yPos == 15)
	{
		left = &board.tiles[xPos - 1][yPos];
		adjacentTiles.push_back(left);
		topLeft = &board.tiles[xPos - 1][yPos - 1];
		adjacentTiles.push_back(topLeft);
		top = &board.tiles[xPos][yPos - 1];
		adjacentTiles.push_back(top);
		topRight = &board.tiles[xPos + 1][yPos - 1];
		adjacentTiles.push_back(topRight);
		right = &board.tiles[xPos + 1][yPos];
		adjacentTiles.push_back(right);
	}
	else if (xPos == 24 && yPos == 0)
	{
		left = &board.tiles[xPos - 1][yPos];
		adjacentTiles.push_back(left);
		bottomLeft = &board.tiles[xPos - 1][yPos + 1];
		adjacentTiles.push_back(bottomLeft);
		bottom = &board.tiles[xPos][yPos + 1];
		adjacentTiles.push_back(bottom);
	}
	else if (xPos == 24 && yPos != 0 && yPos != 15)
	{
		top = &board.tiles[xPos][yPos - 1];
		adjacentTiles.push_back(top);
		topLeft = &board.tiles[xPos - 1][yPos - 1];
		adjacentTiles.push_back(topLeft);
		left = &board.tiles[xPos - 1][yPos];
		adjacentTiles.push_back(left);
		bottomLeft = &board.tiles[xPos - 1][yPos + 1];
		adjacentTiles.push_back(bottomLeft);
		bottom = &board.tiles[xPos][yPos + 1];
		adjacentTiles.push_back(bottom);
	}
	else if (xPos == 24 && yPos == 15)
	{
		top = &board.tiles[xPos][yPos - 1];
		adjacentTiles.push_back(top);
		topLeft = &board.tiles[xPos - 1][yPos - 1];
		adjacentTiles.push_back(topLeft);
		left = &board.tiles[xPos - 1][yPos];
		adjacentTiles.push_back(left);
	}
	else
	{
		topLeft = &board.tiles[xPos - 1][yPos - 1];
		adjacentTiles.push_back(topLeft);
		top = &board.tiles[xPos][yPos - 1];
		adjacentTiles.push_back(top);
		topRight = &board.tiles[xPos + 1][yPos - 1];
		adjacentTiles.push_back(topRight);
		right = &board.tiles[xPos + 1][yPos];
		adjacentTiles.push_back(right);
		bottomRight = &board.tiles[xPos + 1][yPos + 1];
		adjacentTiles.push_back(bottomRight);
		bottom = &board.tiles[xPos][yPos + 1];
		adjacentTiles.push_back(bottom);
		bottomLeft = &board.tiles[xPos - 1][yPos + 1];
		adjacentTiles.push_back(bottomLeft);
		left = &board.tiles[xPos - 1][yPos];
		adjacentTiles.push_back(left);
	}
}

void Board::NeighborChecker()
{	
	int counter;

	for (unsigned int i = 0; i < 16; i++) // Setting dangerous neighbors to 0 for all tiles
	{
		for (unsigned int j = 0; j < 25; j++)
		{
			tiles[j][i].dangerousNeighbors = 0;
		}
	}

	for (unsigned int i = 0; i < 16; i++) // Assigning dangerous neighbors
	{
		for (unsigned int j = 0; j < 25; j++)
		{
			for (unsigned int k = 0; k < tiles[j][i].adjacentTiles.size(); k++)
			{
				if (tiles[j][i].adjacentTiles[k]->mine)
				{
					tiles[j][i].dangerousNeighbors++;
				}
			}
		}
	}
}