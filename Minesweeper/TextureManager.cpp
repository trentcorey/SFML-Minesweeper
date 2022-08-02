#include "TextureManager.h"
#include <iostream>
using namespace std;

// Redeclaring static class variable
unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(const char* fileName)
{
	string filePath = "images/"; // "Images/"
	filePath += fileName; // "Images/mine"
	filePath += ".png"; // "Images/mine.png"
	textures[fileName].loadFromFile(filePath);
}

sf::Texture& TextureManager::GetTexture(const char* textureKey)
{
	if (textures.find(textureKey) == textures.end()) // Key didn't exist
	{
		LoadTexture(textureKey); // Key now exists =)
	}
	return textures[textureKey];
}

void TextureManager::Clear()
{
	textures.clear();
}