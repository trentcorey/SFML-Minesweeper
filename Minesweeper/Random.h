#pragma once
#include <random>
class Random
{
	// One instance of the x variable and all instances SHARE this variable
	// Static vars/fxns exist only for lifetime of your program
	static std::mt19937 random;
public:
	static int x;
	// Only one of these functions
	static int Int(int min, int max);
	static float Float(float min, float max);
};