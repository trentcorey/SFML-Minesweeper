#pragma once
#include <unordered_map>
#include <string>
#include "SFML/Graphics.hpp"
using std::unordered_map;
using std::string;

class TextureManager
{
	static unordered_map<string, sf::Texture> textures;
public:
	static void LoadTexture(const char* fileName);
	static sf::Texture& GetTexture(const char* textureKey);
	static void Clear(); // Clean up static textures from map
};