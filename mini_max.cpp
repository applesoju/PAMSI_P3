#include "mini_max.hpp"

int min(int _x, int _y) { return _x < _y ? _x : _y; }

int max(int _x, int _y) { return _x > _y ? _x : _y; }

GameStateNode::GameStateNode(int _depth, bool _side, bool _black, Gameboard _b, Sequence _seq) {
	score = 0;
	black = _black;
	side = _side;
	board = _b;
	moveSequence = _seq;

	if (board.checkGameState(side) != 0 || _depth == 0)			// jeœli gra zosta³a zakoñczona, lub g³êbokoœæ rekurencji osi¹gnê³a zero
		score = board.getBoardValue(side, black);				// zlicz wartoœæ punktow¹ ostatniej planszy
	else {
		for (Sequence s : board.checkBoardForMoves(side)) {		// dla ka¿dej znalezionej sekwencji 
			Gameboard nextBoardState(board);					
			nextBoardState.movePiece(s);						// wykonaj symulacjê ruchu
			nextNodes.emplace_back(_depth - 1, !side, black, nextBoardState, s);	// umieœæ dany stan planszy w drzewie
		}
	}
}

int minimaxWithAlphaBeta(GameStateNode& _node, int _depth, int _alpha, int _beta, bool _maximizingPlayer) {
	if (_node.nextNodes.size() == 0) return _node.score;	// jeœli nie ma kolejnych wêz³ów zwróæ wartoœæ punktow¹ aktualnego wêz³a

	int value, maxValue;

	if (_maximizingPlayer) {		// jeœli algorytm maksymalizuje gracza
		maxValue = -maxInt;			// ustal najmniejsz¹ mo¿liw¹ wartoœæ

		for (GameStateNode next : _node.nextNodes) {	// dla ka¿dego kolejnego wêz³a
			value = minimaxWithAlphaBeta(next, _depth + 1, _alpha, _beta, false);	// wywo³aj rekursywnie algorytm minimax
			maxValue = max(value, maxValue);			// przypisz wiêksz¹ z wartoœci do "maxValue"
			_alpha = max(maxValue, _alpha);				// przypisz wiêksz¹ z wartoœci do "_alpha"

			if (_depth == 0 && maxValue == value) _node.moveSequence = next.moveSequence;	// jeœli g³êbokoœæ rekursji jest równa zero
											// i zwrócona wartoœæ jest taka sama, co najmniejsza mo¿liwa, pobierz sekwencjê nastêpnego wêz³a
			if (_alpha >= _beta) break;		// jeœli wartoœæ "_alpha" jest wiêksza lub równa "_beta", przerwij 
		}
	}

	else {						// jeœli algorytm mksymalizuje ai		
		maxValue = maxInt;		// ustal najwiêksz¹ mo¿liw¹ wartoœæ

		for (GameStateNode next : _node.nextNodes) {	// dla ka¿dego kolejnego wêz³a
			value = minimaxWithAlphaBeta(next, _depth + 1, _alpha, _beta, true);	// wywo³aj rekursywnie algorytm minimax
			maxValue = min(value, maxValue);	// przypisz mniejsz¹ z wartoœci do "maxValue"
			_beta = min(maxValue, _beta);		// przypisz mniejsz¹ z wartoœci do "_beta"

			if (_depth == 0 && maxValue == value) _node.moveSequence = next.moveSequence;	// jeœli g³êbokoœæ rekursji jest równa zero
											// i zwrócona wartoœæ jest taka sama, co najmniejsza mo¿liwa, pobierz sekwencjê nastêpnego wêz³a
			if (_alpha >= _beta) break;		// jeœli wartoœæ "_alpha" jest wiêksza lub równa "_beta", przerwij 
		}
	}
}