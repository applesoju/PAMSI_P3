#include "ai.hpp"

Sequence Ai::getNextMove(Gameboard _b, int _maxdepth, bool _side) {
	
	std::unique_ptr<GameStateNode> next = std::make_unique<GameStateNode>(_maxdepth, _side, _side, _b);

	minimaxWithAlphaBeta(*next, 0, -maxInt, maxInt, true);

	return next->moveSequence;
}