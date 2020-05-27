#pragma once

#include "mini_max.hpp"

const int sDepth = 4;

class Ai {
	
	Ai() {};

public:
	static Sequence getNextMove(Gameboard _b, int _maxdepth, bool _side);
};