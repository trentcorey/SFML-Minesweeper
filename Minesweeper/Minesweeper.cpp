#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Random.h"
#include <unordered_map>
#include <string>
#include "TextureManager.h"
#include "Board.h"
using sf::Sprite;
using sf::Mouse;
using namespace std;

void LoadGameData(string fileName, Board &board);
void DisplayMineCounter(Board &gameBoard, Sprite &hiddenTile, sf::RenderWindow &window);
void DisplaySmiley(Board& gameBoard, Sprite& hiddenTile, Sprite& face_happy, Sprite& face_win, Sprite& face_lose, sf::RenderWindow& window);
void DisplayButtons(Board& gameBoard, Sprite& hiddenTile,Sprite& debug, Sprite& test_1, Sprite& test_2, Sprite& test_3, sf::RenderWindow& window);
void ChainReveal(Board::Tile& tile);
void MineCounter(Board& gameBoard);
void VictoryChecker(Board& gameBoard);
void RandomizeMines(Board& gameBoard);

int main()
{
    Board gameBoard;
    RandomizeMines(gameBoard);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");

    /*===== Initialization =====*/
    Sprite hiddenTile(TextureManager::GetTexture("tile_hidden"));
    Sprite mine(TextureManager::GetTexture("mine"));
    Sprite tile_revealed(TextureManager::GetTexture("tile_revealed"));
    Sprite number_1(TextureManager::GetTexture("number_1"));
    Sprite number_2(TextureManager::GetTexture("number_2"));
    Sprite number_3(TextureManager::GetTexture("number_3"));
    Sprite number_4(TextureManager::GetTexture("number_4"));
    Sprite number_5(TextureManager::GetTexture("number_5"));
    Sprite number_6(TextureManager::GetTexture("number_6"));
    Sprite number_7(TextureManager::GetTexture("number_7"));
    Sprite number_8(TextureManager::GetTexture("number_8"));
    Sprite flag(TextureManager::GetTexture("flag"));
    Sprite face_happy(TextureManager::GetTexture("face_happy"));
    Sprite face_win(TextureManager::GetTexture("face_win"));
    Sprite face_lose(TextureManager::GetTexture("face_lose"));
    Sprite debug(TextureManager::GetTexture("debug"));
    Sprite test_1(TextureManager::GetTexture("test_1"));
    Sprite test_2(TextureManager::GetTexture("test_2"));
    Sprite test_3(TextureManager::GetTexture("test_3"));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // If a button was pressed...
            if (event.type == sf::Event::MouseButtonPressed)
            {
                // AND it was the left button...
                if (event.mouseButton.button == Mouse::Left)
                {
                    auto pos = Mouse::getPosition(window);

                    if (debug.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
                    {
                        if (gameBoard.debugMode == true)
                        {
                            gameBoard.debugMode = false;
                        }
                        else
                        {
                            gameBoard.debugMode = true;
                        }
                    }

                    if (face_happy.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y))) // Reset game
                    {
                        for (int i = 0; i < 16; i++)
                        {
                            for (int j = 0; j < 25; j++)
                            {
                                gameBoard.tiles[j][i].tileStatus = "Hidden";
                            }
                        }
                        RandomizeMines(gameBoard);
                        gameBoard.winState = "In Progress";
                    }

                    if (test_1.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y))) // Checking if test button was pressed
                    {
                        for (int i = 0; i < 16; i++)
                        {
                            for (int j = 0; j < 25; j++)
                            {
                                gameBoard.tiles[j][i].tileStatus = "Hidden";
                                gameBoard.tiles[j][i].mine = false;
                                gameBoard.tiles[j][i].dangerousNeighbors = 0;
                            }
                        }
                        gameBoard.winState = "In Progress";
                        LoadGameData("boards/testboard1.brd", gameBoard);
                    }

                    if (test_2.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
                    {
                        for (int i = 0; i < 16; i++)
                        {
                            for (int j = 0; j < 25; j++)
                            {
                                gameBoard.tiles[j][i].tileStatus = "Hidden";
                                gameBoard.tiles[j][i].mine = false;
                                gameBoard.tiles[j][i].dangerousNeighbors = 0;
                            }
                        }
                        gameBoard.winState = "In Progress";
                        LoadGameData("boards/testboard2.brd", gameBoard);
                    }

                    if (test_3.getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
                    {
                        for (int i = 0; i < 16; i++)
                        {
                            for (int j = 0; j < 25; j++)
                            {
                                gameBoard.tiles[j][i].tileStatus = "Hidden";
                                gameBoard.tiles[j][i].mine = false;
                                gameBoard.tiles[j][i].dangerousNeighbors = 0;
                            }
                        }
                        gameBoard.winState = "In Progress";
                        LoadGameData("boards/testboard3.brd", gameBoard);
                    }

                    if (gameBoard.winState == "In Progress")
                    {
                        for (int i = 0; i < 16; i++)
                        {
                            for (int j = 0; j < 25; j++)
                            {
                                if (pos.x > gameBoard.tiles[j][i].xPosLeft && pos.x < gameBoard.tiles[j][i].xPosRight  // Checking if Tile was clicked
                                    && pos.y < gameBoard.tiles[j][i].yPosBottom && pos.y > gameBoard.tiles[j][i].yPosTop)
                                {
                                    if (gameBoard.tiles[j][i].mine)
                                    {
                                        gameBoard.winState = "Lost";

                                        for (int k = 0; k < 16; k++)
                                        {
                                            for (int m = 0; m < 25; m++)
                                            {
                                                if (gameBoard.tiles[m][k].mine && gameBoard.tiles[m][k].tileStatus == "Flagged")
                                                {
                                                    gameBoard.tiles[m][k].tileStatus = "Revealed & Flagged";
                                                }
                                                else if (gameBoard.tiles[m][k].mine)
                                                {
                                                    gameBoard.tiles[m][k].tileStatus = "Revealed";
                                                }
                                            }
                                        }

                                    }
                                    if (gameBoard.tiles[j][i].tileStatus != "Flagged")
                                        gameBoard.tiles[j][i].tileStatus = "Revealed";
                                }
                            }
                        }
                    }
                }
                else if (event.mouseButton.button == Mouse::Right)
                {
                    auto pos = Mouse::getPosition(window);

                    for (int i = 0; i < 16; i++)
                    {
                        for (int j = 0; j < 25; j++)
                        {
                            if (pos.x > gameBoard.tiles[j][i].xPosLeft && pos.x < gameBoard.tiles[j][i].xPosRight  // Checking if Tile was clicked
                                && pos.y < gameBoard.tiles[j][i].yPosBottom && pos.y > gameBoard.tiles[j][i].yPosTop)
                            {
                                if (gameBoard.tiles[j][i].tileStatus == "Flagged")
                                {
                                    gameBoard.tiles[j][i].tileStatus = "Hidden";
                                }
                                else
                                {
                                    gameBoard.tiles[j][i].tileStatus = "Flagged";
                                }
                            }
                        }
                    }

                }
            }
        }

        // 1. Clear screen
        window.clear();

        // 2. Draw stuff
            for (int i = 0; i < 16; i++) // Drawing tiles
            {
                int height = i * hiddenTile.getTextureRect().height;
                for (int j = 0; j < 25; j++)
                {
                    if (gameBoard.winState == "Won")
                    {
                        if (gameBoard.tiles[j][i].mine)
                        {
                            gameBoard.tiles[j][i].tileStatus = "Flagged";
                        }
                    }

                    if (gameBoard.tiles[j][i].tileStatus == "Hidden")
                    {
                        int width = j * hiddenTile.getTextureRect().width;
                        hiddenTile.setPosition(width, height);
                        window.draw(hiddenTile);
                        if (gameBoard.debugMode && gameBoard.tiles[j][i].mine)
                        {
                            mine.setPosition(width, height);
                            window.draw(mine);
                        }
                    }
                    else if (gameBoard.tiles[j][i].tileStatus == "Flagged")
                    {
                        int width = j * hiddenTile.getTextureRect().width;
                        hiddenTile.setPosition(width, height);
                        window.draw(hiddenTile);
                        flag.setPosition(width, height);
                        window.draw(flag);
                    }
                    else if (gameBoard.tiles[j][i].tileStatus == "Revealed & Flagged")
                    {
                        int width = j * tile_revealed.getTextureRect().width;
                        tile_revealed.setPosition(width, height);
                        window.draw(tile_revealed);
                        flag.setPosition(width, height);
                        window.draw(flag);
                        mine.setPosition(width, height);
                        window.draw(mine);
                    }
                    else if (gameBoard.tiles[j][i].tileStatus == "Revealed")
                    {
                        int width = j * hiddenTile.getTextureRect().width;
                        tile_revealed.setPosition(width, height);
                        window.draw(tile_revealed);
                        if (gameBoard.tiles[j][i].mine)
                        {
                            mine.setPosition(width, height);
                            window.draw(mine);
                        }
                        else if (gameBoard.tiles[j][i].dangerousNeighbors == 1)
                        {
                            number_1.setPosition(width, height);
                            window.draw(number_1);
                        }
                        else if (gameBoard.tiles[j][i].dangerousNeighbors == 2)
                        {
                            number_2.setPosition(width, height);
                            window.draw(number_2);
                        }
                        else if (gameBoard.tiles[j][i].dangerousNeighbors == 3)
                        {
                            number_3.setPosition(width, height);
                            window.draw(number_3);
                        }
                        else if (gameBoard.tiles[j][i].dangerousNeighbors == 4)
                        {
                            number_4.setPosition(width, height);
                            window.draw(number_4);
                        }
                        else if (gameBoard.tiles[j][i].dangerousNeighbors == 5)
                        {
                            number_5.setPosition(width, height);
                            window.draw(number_5);
                        }
                        else if (gameBoard.tiles[j][i].dangerousNeighbors == 6)
                        {
                            number_6.setPosition(width, height);
                            window.draw(number_6);
                        }
                        else if (gameBoard.tiles[j][i].dangerousNeighbors == 7)
                        {
                            number_7.setPosition(width, height);
                            window.draw(number_7);
                        }
                        else if (gameBoard.tiles[j][i].dangerousNeighbors == 8)
                        {
                            number_8.setPosition(width, height);
                            window.draw(number_8);
                        }
                        ChainReveal(gameBoard.tiles[j][i]);
                    }
                }
            }

        MineCounter(gameBoard);
        DisplayMineCounter(gameBoard, hiddenTile, window);
        VictoryChecker(gameBoard);
        DisplaySmiley(gameBoard, hiddenTile, face_happy, face_win, face_lose, window);
        DisplayButtons(gameBoard, hiddenTile, debug, test_1, test_2, test_3, window);

        // 3. Display stuff you've drawn
        window.display();
    }

    /*===== Cleanup =====*/
    TextureManager::Clear();

    return 0;
}

void LoadGameData(string fileName, Board &board)
{
    unsigned int lineCount = 0;
    ifstream file(fileName);

    if (file.is_open())
    {
        string line;
        
        while (getline(file, line))
        {
            string tempString;
            stringstream lineStream(line);
            
            getline(lineStream, tempString);
            for (unsigned int i = 0; i < 24; i++)
            {
                if (tempString[i] == '1')
                {
                    board.tiles[i][lineCount].mine = true;
                }
                else
                {
                    board.tiles[i][lineCount].mine = false;
                }
            }
            lineCount++;
        }
    }
    else
    {
        cout << "File " << fileName << "not opened." << endl;
    }
    board.NeighborChecker();
}

void DisplayMineCounter(Board& gameBoard, Sprite& hiddenTile, sf::RenderWindow& window)
{
    if (gameBoard.mineCount >= 0)
    {
        sf::Texture Digit;

        Digit.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
        Sprite zero(Digit);
        zero.setPosition(0, hiddenTile.getTextureRect().height * 16);
        window.draw(zero);

        unsigned int tensPlace = gameBoard.mineCount % 10;
        tensPlace = gameBoard.mineCount - tensPlace;
        Digit.loadFromFile("images/digits.png", sf::IntRect((tensPlace / 10) * 21, 0, 21, 32));
        Sprite tensDigit(Digit);
        tensDigit.setPosition(21, hiddenTile.getTextureRect().height * 16);
        window.draw(tensDigit);

        unsigned int onesPlace = gameBoard.mineCount % 10;
        Digit.loadFromFile("images/digits.png", sf::IntRect(onesPlace * 21, 0, 21, 32));
        Sprite onesDigit(Digit);
        onesDigit.setPosition(42, hiddenTile.getTextureRect().height * 16);
        window.draw(onesDigit);
    }
    else
    {
        sf::Texture Digit;

        Digit.loadFromFile("images/digits.png", sf::IntRect(210, 0, 21, 32));
        Sprite negative(Digit);
        negative.setPosition(0, hiddenTile.getTextureRect().height * 16);
        window.draw(negative);

        Digit.loadFromFile("images/digits.png", sf::IntRect(0, 0, 21, 32));
        Sprite zero(Digit);
        zero.setPosition(21, hiddenTile.getTextureRect().height * 16);
        window.draw(zero);

        int tensPlace = gameBoard.mineCount % 10;
        tensPlace = gameBoard.mineCount - tensPlace;
        Digit.loadFromFile("images/digits.png", sf::IntRect((-(tensPlace) / 10) * 21, 0, 21, 32));
        Sprite tensDigit(Digit);
        tensDigit.setPosition(42, hiddenTile.getTextureRect().height * 16);
        window.draw(tensDigit);

        int onesPlace = gameBoard.mineCount % 10;
        Digit.loadFromFile("images/digits.png", sf::IntRect(-(onesPlace) * 21, 0, 21, 32));
        Sprite onesDigit(Digit);
        onesDigit.setPosition(63, hiddenTile.getTextureRect().height * 16);
        window.draw(onesDigit);
    }
}

void DisplaySmiley(Board& gameBoard, Sprite& hiddenTile, Sprite& face_happy, Sprite& face_win, Sprite& face_lose, sf::RenderWindow& window)
{
    if (gameBoard.winState == "In Progress")
    {
        face_happy.setPosition((11 * hiddenTile.getTextureRect().width) + (hiddenTile.getTextureRect().width / 2), hiddenTile.getTextureRect().height * 16);
        window.draw(face_happy);
    }
    else if (gameBoard.winState == "Won")
    {
        face_win.setPosition((11 * hiddenTile.getTextureRect().width) + (hiddenTile.getTextureRect().width / 2), hiddenTile.getTextureRect().height * 16);
        window.draw(face_win);
    }
    else if (gameBoard.winState == "Lost")
    {
        face_lose.setPosition((11 * hiddenTile.getTextureRect().width) + (hiddenTile.getTextureRect().width / 2), hiddenTile.getTextureRect().height * 16);
        window.draw(face_lose);
    }

}

void DisplayButtons(Board& gameBoard, Sprite& hiddenTile, Sprite& debug, Sprite& test_1, Sprite& test_2, Sprite& test_3, sf::RenderWindow& window)
{
    debug.setPosition((15 * hiddenTile.getTextureRect().width) + (hiddenTile.getTextureRect().width / 2), hiddenTile.getTextureRect().height * 16);
    window.draw(debug);
    test_1.setPosition((15 * hiddenTile.getTextureRect().width) + (hiddenTile.getTextureRect().width / 2) + (debug.getTextureRect().width), hiddenTile.getTextureRect().height * 16);
    window.draw(test_1);
    test_2.setPosition((15 * hiddenTile.getTextureRect().width) + (hiddenTile.getTextureRect().width / 2) + (debug.getTextureRect().width * 2), hiddenTile.getTextureRect().height * 16);
    window.draw(test_2);
    test_3.setPosition((15 * hiddenTile.getTextureRect().width) + (hiddenTile.getTextureRect().width / 2) + (debug.getTextureRect().width * 3), hiddenTile.getTextureRect().height * 16);
    window.draw(test_3);
}

void ChainReveal(Board::Tile& tile)
{
    bool safeNeighbors = true;
    for (unsigned int i = 0; i < tile.adjacentTiles.size(); i++)
    {
        if (tile.adjacentTiles[i]->mine)
        {
            safeNeighbors = false;
        }
    }
    if (safeNeighbors)
    {
        for (unsigned int i = 0; i < tile.adjacentTiles.size(); i++)
        {
            tile.adjacentTiles[i]->tileStatus = "Revealed";
        }
    }
}

void MineCounter(Board& gameBoard)
{
    int flagCount = 0;
    int minesOnBoard = 0;

    for (int i = 0; i < 16; i++) 
    {
        for (int j = 0; j < 25; j++)
        {
            if (gameBoard.tiles[j][i].tileStatus == "Flagged" || gameBoard.tiles[j][i].tileStatus == "Revealed & Flagged")
            {
                flagCount++;
            }

            if (gameBoard.tiles[j][i].mine)
            {
                minesOnBoard++;
            }
        }
    }
    gameBoard.mineCount = minesOnBoard - flagCount;
}

void VictoryChecker(Board& gameBoard)
{
    bool victoryStatus = false;
    int hiddenMines = 0;
    int hiddenTiles = 0;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 25; j++)
        {
            if (gameBoard.tiles[j][i].tileStatus == "Hidden" && gameBoard.tiles[j][i].mine)
            {
                hiddenMines++;
                hiddenTiles++;
            }
            else if (gameBoard.tiles[j][i].tileStatus == "Hidden")
            {
                hiddenTiles++;
            }
        }
    }

    if (hiddenMines == gameBoard.mineCount && hiddenTiles == hiddenMines)
    {
        gameBoard.winState = "Won";
    }
}

void RandomizeMines(Board& gameBoard)
{
    int desiredMines = 100;
    cout << "How many mines? (1 - 99)" << endl;

    while (desiredMines > 99)
    {
        cin >> desiredMines;
        if (desiredMines > 99)
        {
            cout << "Too many mines. Choose a number between 1 and 99" << endl;
        }
    }

    int numOfMines = 0;

    for (int i = 0; i < 16; i++) // Resetting all mines to false
    {
        for (int j = 0; j < 25; j++)
        {
            gameBoard.tiles[j][i].mine = false;
        }
    }

    while (numOfMines < desiredMines) // Adding random mines
    {
        int randomX = Random::Int(0, 24);
        int randomY = Random::Int(0, 15);

        if (!gameBoard.tiles[randomX][randomY].mine) // randomMine == 1 &&...
        {
            gameBoard.tiles[randomX][randomY].mine = true;
            numOfMines++;
        }
    }
    gameBoard.NeighborChecker();
}