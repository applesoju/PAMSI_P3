#pragma once

#include "Gameboard.hpp"

const int maxInt = INT32_MAX;

int min(int _x, int _y);	// funkcja por�wnuj�ca dwie liczby i zwracaj�ca mniejsz� z nich

int max(int _x, int _y);	// funkcja por�wnuj�ca dwie liczby i zwracaj�ca wi�ksz� z nich

class GameStateNode {		// w�ze� przechowuj�cy kolejne mo�liwe stany gry
public:
	bool black;
	bool side;
	int score;

	Gameboard board;
	Sequence moveSequence;

	std::vector <GameStateNode> nextNodes;

	GameStateNode(int _depth, bool _side, bool _black, Gameboard _b, Sequence _seq = Sequence(0, 0, 0, 0));
};

int minimaxWithAlphaBeta(GameStateNode& _node, int _depth, int _alpha, int _beta, bool _maximizingPlayer);