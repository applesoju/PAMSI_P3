#include "mini_max.hpp"

int min(int _x, int _y) { return _x < _y ? _x : _y; }

int max(int _x, int _y) { return _x > _y ? _x : _y; }

GameStateNode::GameStateNode(int _depth, bool _side, bool _black, Gameboard _b, Sequence _seq) {
	score = 0;
	black = _black;
	side = _side;
	board = _b;
	moveSequence = _seq;

	if (board.checkGameState(side) != 0 || _depth == 0)			// je�li gra zosta�a zako�czona, lub g��boko�� rekurencji osi�gn�a zero
		score = board.getBoardValue(side, black);				// zlicz warto�� punktow� ostatniej planszy
	else {
		for (Sequence s : board.checkBoardForMoves(side)) {		// dla ka�dej znalezionej sekwencji 
			Gameboard nextBoardState(board);					
			nextBoardState.movePiece(s);						// wykonaj symulacj� ruchu
			nextNodes.emplace_back(_depth - 1, !side, black, nextBoardState, s);	// umie�� dany stan planszy w drzewie
		}
	}
}

int minimaxWithAlphaBeta(GameStateNode& _node, int _depth, int _alpha, int _beta, bool _maximizingPlayer) {
	if (_node.nextNodes.size() == 0) return _node.score;	// je�li nie ma kolejnych w�z��w zwr�� warto�� punktow� aktualnego w�z�a

	int value, maxValue;

	if (_maximizingPlayer) {		// je�li algorytm maksymalizuje gracza
		maxValue = -maxInt;			// ustal najmniejsz� mo�liw� warto��

		for (GameStateNode next : _node.nextNodes) {	// dla ka�dego kolejnego w�z�a
			value = minimaxWithAlphaBeta(next, _depth + 1, _alpha, _beta, false);	// wywo�aj rekursywnie algorytm minimax
			maxValue = max(value, maxValue);			// przypisz wi�ksz� z warto�ci do "maxValue"
			_alpha = max(maxValue, _alpha);				// przypisz wi�ksz� z warto�ci do "_alpha"

			if (_depth == 0 && maxValue == value) _node.moveSequence = next.moveSequence;	// je�li g��boko�� rekursji jest r�wna zero
											// i zwr�cona warto�� jest taka sama, co najmniejsza mo�liwa, pobierz sekwencj� nast�pnego w�z�a
			if (_alpha >= _beta) break;		// je�li warto�� "_alpha" jest wi�ksza lub r�wna "_beta", przerwij 
		}
	}

	else {						// je�li algorytm mksymalizuje ai		
		maxValue = maxInt;		// ustal najwi�ksz� mo�liw� warto��

		for (GameStateNode next : _node.nextNodes) {	// dla ka�dego kolejnego w�z�a
			value = minimaxWithAlphaBeta(next, _depth + 1, _alpha, _beta, true);	// wywo�aj rekursywnie algorytm minimax
			maxValue = min(value, maxValue);	// przypisz mniejsz� z warto�ci do "maxValue"
			_beta = min(maxValue, _beta);		// przypisz mniejsz� z warto�ci do "_beta"

			if (_depth == 0 && maxValue == value) _node.moveSequence = next.moveSequence;	// je�li g��boko�� rekursji jest r�wna zero
											// i zwr�cona warto�� jest taka sama, co najmniejsza mo�liwa, pobierz sekwencj� nast�pnego w�z�a
			if (_alpha >= _beta) break;		// je�li warto�� "_alpha" jest wi�ksza lub r�wna "_beta", przerwij 
		}
	}
}