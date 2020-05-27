#include "Gameboard.hpp"

/* * * * * * * * * * * * Move * * * * * * * * * * * */

Move::Move(int _x1, int _y1, int _x2, int _y2) {
	x1 = _x1;
	y1 = _y1;
	x2 = _x2;
	y2 = _y2;
}

bool Move::operator!= (const Move& _move) {
	if (x1 != _move.x1 || x2 != _move.x2 || y1 != _move.y1 || y2 != _move.y2) return true;
	
	return false;
}

/* * * * * * * * * * * * Sequence * * * * * * * * * * * */

Sequence::Sequence(Move _move) {
	moves.push_back(_move);
}

Sequence::Sequence(int _x1, int _y1, int _x2, int _y2) {
	moves.push_back(Move(_x1, _y1, _x2, _y2));
}

void Sequence::addMove(Move _move) {
	moves.push_back(_move);
}

bool Sequence::operator== (const Sequence& seq) {
	if (moves.size() != seq.moves.size()) return false;
	else {
		for (int i = 0; i < moves.size(); ++i)
			if (moves[i] != seq.moves[i]) return false;
	}
	return true;
}

/* * * * * * * * * * * * Gameboard * * * * * * * * * * * */

Gameboard::Gameboard() {
	resetBoard();
}

bool Gameboard::checkSquareForCapture(int _x, int _y) {
	bool black = true;
	if (board[_x][_y] == whiteM || board[_x][_y] == whiteK) black = false;

	if (board[_x][_y] == whiteM || board[_x][_y] == blackM) {	// jeœli na danym polu znajduje siê zwyk³y pionek
		for (int i : {-1, 1}) {			// dla pól o jedno w górê i jedno w dó³						
			for (int j : {-1, 1}) {		// i dla pól o jedno w lewi i jedno w prawo
				int ix = _x + i;
				int jy = _y + j;
				if (ix + i < boardSize && ix + i >= 0 && jy + j < boardSize && jy + j >= 0) {	// jeœli dane pole mieœci siê na planszy
					if (black) {
						if (board[ix][jy] == whiteM || board[ix][jy] == whiteK) {				// jeœli pionek na drodze jest bia³y
							if (board[ix + i][jy + j] == empty)									// i pole, na którym wyl¹dowa³ by pionek jest puste	
								return true;													// zwróæ "true"
						}
					}
					else {
						if (board[ix][jy] == blackM || board[ix][jy] == blackK) {				// jeœli pionek na drodze jest czarny
							if (board[ix + i][jy + j] == empty)									// i pole, na którym wyl¹dowa³ by pionek jest puste 
								return true;													// zwróæ "true"
						}					
					}
					
				}
			}
		}
	}

	else if (board[_x][_y] == whiteK || board[_x][_y] == blackK) {		// jeœli pionek jest damk¹
		for (int i : {-1, 1}) {			// dla pól o jedno w górê i jedno w dó³	
			for (int j : {-1, 1}) {		// i dla pól o jedno w lewi i jedno w prawo
				int ix = _x + i;
				int jy = _y + j;

				while (ix + i < boardSize && ix + i >= 0 && jy + j < boardSize && jy + j >= 0) {	// dopóki dane pole mieœci siê na planszy
					if (black) {														// jeœli pionek jest czarny
						if (board[ix][jy] == blackM || board[ix][jy] == blackK) break;	// i ma na swojej drodze pionek sojuszniczy, przerwij
						if (board[ix][jy] == whiteM || board[ix][jy] == whiteK) {		// jeœli ma na drodze przeciwny pionek
							if (board[ix + i][jy + j] == empty) return true;			// i za nim jest puste pole, zwróæ "true"
							else break;													// w przeciwnym wypadku przerwij
						}
					}
					else {																// jeœli pionek jest bia³y
						if (board[ix][jy] == whiteM || board[ix][jy] == whiteK) break;	// i ma na swojej drodze pionek sojuszniczy, przerwij
						if (board[ix][jy] == blackM || board[ix][jy] == blackK) {		// jeœli ma na drodze przeciwny pionek
							if (board[ix + i][jy + j] == empty) return true;			// i za nim jest puste pole, zwróæ "true"
							else break;													// w przeciwnym wypadku przerwij
						}
					}
					ix += i;		// przeszukuj dalsze pola
					jy += j;
				}
			}
		}
	}
	return false;					// jeœli nic nie zostanie wczeœniej zwrócone, zwróæ "false"
}

bool Gameboard::checkBoardForCapture(bool _black) {
	for (int i = 0; i < boardSize; ++i) {		// dla ka¿dego wiersza
		for (int j = 0; j < boardSize; ++j) {	// i dla ka¿dej kolumny
			if (_black) {
				if (board[i][j] == blackM || board[i][j] == blackK) {				// jeœli pionek jest czarny
					if (checkSquareForCapture(i, j)) return true;					// zwróæ wynik sprawdzenia,
				}																	// czy z danego pola mo¿liwe jest bicie
			}
			else {
				if (board[i][j] == whiteM || board[i][j] == whiteK) {				// jeœli pionek jest bia³y
					if (checkSquareForCapture(i, j)) return true;					// zwróæ wynik sprawdzenia,
				}																	// czy z danego pola mo¿liwe jest bicie
			}
		}
	}
	return false;		// jeœli nic nie zostanie wczeœniej zwrócone, zwróæ "false"
}

std::vector<Sequence> Gameboard::getSequences(Gameboard _gb, Sequence _seq) {
	std::vector<Sequence> moveSequences;

	if (_gb.movePiece(_seq.moves[0]) == 1)			// po wykonaniu odpowiedniego ruchu
		moveSequences.emplace_back(_seq.moves[0]);	// wstaw ruch do sekwencji
	else {											// w przeciwnym wypadku
		for (Sequence nextMove : _gb.getPossibleMoves(_seq.moves[0].x2, _seq.moves[0].y2)) {	// dla ka¿dej kolejnej mo¿liwej sekwencji
			std::vector<Sequence> nextSeq = getSequences(_gb, nextMove);						// znajdŸ kolejne sekwencje ruchów
			moveSequences.insert(moveSequences.end(), nextSeq.begin(), nextSeq.end());			// i dopisz je do danej sekwencji
		}
		for (Sequence seq : moveSequences) {	// dla ka¿dej sekwencji ruchów 
			seq.addMove(_seq.moves[0]);			// dodaj ruch z ostatniej sekwencji
		}
	}
	return moveSequences;						// zwróæ sekwencjê
}

std::vector<Sequence> Gameboard::getPossibleMoves(int _x, int _y) {
	std::vector<Sequence> moveSequences;

	bool black = true;
	if (board[_x][_y] == whiteM || board[_x][_y] == whiteK) black = false;

	if (board[_x][_y] == empty) return moveSequences;	// jeœli dane pole jest puste zwróæ pusty wektor	
	
	if (checkBoardForCapture(black) != checkSquareForCapture(_x, _y)) return moveSequences; // jeœli mo¿liwe jest bicie dla innego ruchu zwróæ pusty wektor
	if (board[_x][_y] == whiteM || board[_x][_y] == blackM) {	// jeœli na polu jest zwyk³y pionek
		if (checkSquareForCapture(_x, _y)) {					// jeœli dla danego pola i koloru jest mo¿liwoœæ bicia
			for (int i : {-1, 1}) {								// dla pól o jedno w górê i jedno w dó³	
				for (int j : {-1, 1}) {							// i dla pól o jedno w lewi i jedno w prawo
					int ix = _x + i;
					int jy = _y + j;
					if (ix + i < boardSize && ix + i >= 0 && jy + j < boardSize && jy + j >= 0) {					// jeœli dane pole mieœci siê na planszy

						if (black && (board[ix][jy] == whiteM || board[ix][jy] == whiteK)) {						// jeœli pionek jest bia³y
							if (board[ix + i][jy + j] == empty) moveSequences.emplace_back(_x, _y, ix + i, jy + j);	// i jeœli pole na którym wyl¹dowa³ by pionek jest puste
						}																							// dodaj ruch do sekwencji
						else if (!black && (board[ix][jy] == blackM || board[ix][jy] == blackK)) {					// jeœli pionek jest czarny
							if (board[ix + i][jy + j] == empty) moveSequences.emplace_back(_x, _y, ix + i, jy + j);	// i jeœli pole na którym wyl¹dowa³ by pionek jest puste
						}																							// dodaj ruch do sekwencji
					}
				}
			}
		}
		else {								// jeœli dla danego pola nie ma mo¿liwoœci bicia
			int direction = black ? -1 : 1;	// okreœl kierunek, w zale¿noœci od koloru

			if (_y < boardSize - 1 && board[_x + direction][_y + 1] == empty)	// jeœli pole nie jest polem przy prawej krawêdzi
				moveSequences.emplace_back(_x, _y, _x + direction, _y + 1);		// dodaj ruch w jej kierunku 
			if (_y > 0 && board[_x + direction][_y - 1] == empty)				// jeœli pole nie jest polem przy lewej krawêdzi
				moveSequences.emplace_back(_x, _y, _x + direction, _y - 1);		// dodaj ruch w jej kierunku
		}
	}
	else {		// jeœli dany pionek jest damk¹
		bool capturePossible = checkSquareForCapture(_x, _y);	// sprawdŸ, czy jest mo¿liwe bicie

		for (int i : {-1, 1}) {			// dla pól o jedno w górê i jedno w dó³	
			for (int j : {-1, 1}) {		// i dla pól o jedno w lewi i jedno w prawo
				int ix = _x + i;
				int jy = _y + j;

				while (ix >= 0 && ix <= boardSize - 1 && jy >= 0 && jy <= boardSize - 1) {	// dopóki kolejne pole mieœci siê na planszy
					if (board[ix][jy] == empty) {											// i jest puste
						if (!capturePossible) moveSequences.emplace_back(_x, _y, ix, jy);	// jeœli nie ma mo¿liwoœci bicia,
					}																		// dodaj dany ruch 

					else if (black) {	// jeœli dany pionek jest czarny
						if (board[ix][jy] == blackM || board[ix][jy] == blackK) break;		// i na kolejnym polu jest pionek sojuszniczy, przerwij
						else if (capturePossible && (board[ix][jy] == whiteM || board[ix][jy] == whiteK)) {	// jeœli mo¿liwe jest bicie i na kolejnym polu jest wrogi pionek
							do {
								ix += i;
								jy += j;

								if (ix >= 0 && ix <= boardSize - 1 && jy >= 0 && jy <= boardSize - 1) {		// jeœli kolejne pole mieœci siê na planszy
									if (board[ix][jy] == empty) moveSequences.emplace_back(_x, _y, ix, jy);	// i jeœli jest puste, dodaj dany ruch
									else break;		// jeœli pole nie jest puste przerwij
								}
								else break;			// jeœli kolejne pole nie mieœci siê na planszy, przerwij
							} while (true);			// powtarzaj dopóki pêtla nie zostanie przerwana
							break;					// przerwij po wyjœciu z pêtli
						}
						else break;					// jeœli bicie nie jest mo¿liwe lub na kolejnym polu nie znajduje siê wrogi pionek, przerwij
					}
					else {							// jeœli dany pionek jest bia³y
						if (board[ix][jy] == whiteM || board[ix][jy] == whiteK) break;		// i na kolejnym polu jest pionek sojuszniczy, przerwij
						else if (capturePossible && (board[ix][jy] == blackM || board[ix][jy] == blackK)) {	// jeœli mo¿liwe jest bicie i na kolejnym polu jest wrogi pionek
							do {
								ix += i;
								jy += j;

								if (ix >= 0 && ix <= boardSize - 1 && jy >= 0 && jy <= boardSize - 1) {		// jeœli kolejne pole mieœci siê na planszy
									if (board[ix][jy] == empty) moveSequences.emplace_back(_x, _y, ix, jy);	// i jeœli jest puste, dodaj dany ruch
									else break;		// jeœli pole nie jest puste przerwij
								}
								else break;			// jeœli kolejne pole nie mieœci siê na planszy, przerwij
							} while (true);			// powtarzaj dopóki pêtla nie zostanie przerwana
							break;					// przerwij po wyjœciu z pêtli
						}
						else break;					// jeœli bicie nie jest mo¿liwe lub na kolejnym polu nie znajduje siê wrogi pionek, przerwij
					}
					ix += i;						// przeszukuj dalsze pola
					jy += j;
				}
			}
		}
	}
	return moveSequences;		// zwróæ sekwencjê ruchów				
}

std::vector<Sequence> Gameboard::checkBoardForMoves(bool _black) {
	std::vector<Sequence> moveSequences;

	for (int i = 0; i < boardSize; ++i) {		// dla kolejnych wierszy
		for (int j = 0; j < boardSize; ++j) {	// i kolejnych kolumn
			if (_black && (board[i][j] == blackM || board[i][j] == blackK)) {	// jeœli na danym polu jest czarny pionek i sprawdzamy ruchy dla czarnych pionków
				std::vector<Sequence> moveSeq = getPossibleMoves(i, j);			// znajdŸ mo¿liwe sekwencje ruchów dla danego pola

				for (Sequence seq : moveSeq) {	// dla ka¿dej sekwencji ruchów 
					std::vector<Sequence> nextSeq = getSequences(*this, seq);					// znajdŸ kolejne sekwencje
					moveSequences.insert(moveSequences.end(), nextSeq.begin(), nextSeq.end());	// wstaw kolejne sekwencje
				}
			}
			else if (!_black && (board[i][j] == whiteM || board[i][j] == whiteK)) {	// jeœli na danym polu jest bia³y pionek i sprawdzamy ruchy dla bia³ych pionków
				std::vector<Sequence> moveSeq = getPossibleMoves(i, j);				// znajdŸ mo¿liwe sekwencje ruchów dla danego pola

				for (Sequence seq : moveSeq) {	// dla ka¿dej sekwencji ruchów 
					std::vector<Sequence> nextSeq = getSequences(*this, seq);					// znajdŸ kolejne sekwencje
					moveSequences.insert(moveSequences.end(), nextSeq.begin(), nextSeq.end());	// wstaw kolejne sekwencje
				}
			}
		}
	}
	return moveSequences;		// zwróæ wektor sekwencji
}

void Gameboard::capturePiece(int _x1, int _y1, int _x2, int _y2) {
	int xDif = (_x1 < _x2) ? 1 : -1;	// jeœli indeks poprzeniego wiersza jest mnniejszy od nastêpnego ustaw "xDif"
												// jako 1, w przeciwnym razie jako -1
	int yDif = (_y1 < _y2) ? 1 : -1;	// jeœli indeks poprzedniej kolumny jest mniejszy od nastêpnego ustaw "yDif"
												// jako -1 
	for (int i = _x1 + xDif, j = _y1 + yDif; i != _x2 && j != _y2; i += xDif, j += yDif) {
		board[i][j] = empty;	// ustaw kolejne pola, które s¹ "po drodze" dla danego ruchu na "empty"
	}
}

int Gameboard::movePiece(Sequence _seq) {
	int i = 0;
	while (movePiece(_seq.moves[i]) == -1) ++i;		// wykonuj kolejne ruchy sekwencji biæ
	return 1;
}

int Gameboard::movePiece(Move _move) {
	bool black = true;			// sprawdŸ jakiego koloru jest pionek znajduj¹cy siê na polu
	if (board[_move.x1][_move.y1] == whiteM || board[_move.x1][_move.y1] == whiteK)
		black = false;

	return movePiece(_move.x1, _move.y1, _move.x2, _move.y2, black);	// wykonaj dany ruch
}

int Gameboard::movePiece(int _x1, int _y1, int _x2, int _y2, bool _black) {
	
	if((_black &&										// sprawdŸ, czy zosta³ wybrany odpowiedni kolor
		(board[_x1][_y1] == whiteM || board[_x1][_y1] == whiteK)) ||
		(!_black &&
		(board[_x1][_y1] == blackM || board[_x1][_y1] == blackK))) return 0;
	
	bool capturePossible = false;						// zmienna pomocnicza okreœlaj¹ca, czy bicie jest mo¿liwe
	
	for (Sequence seq : getPossibleMoves(_x1, _y1)) {	// dla ka¿dej kolejnej sekwencji 
		if (seq == Sequence(_x1, _y1, _x2, _y2)) {		// jeœli sekwencja równa siê pojedynczemu, danemu ruchowi
			if (checkBoardForCapture(_black)) {			// jeœli dane pionki maj¹ mo¿liwoœæ bicia
				capturePiece(_x1, _y1, _x2, _y2);		// zbij pionki bêd¹ce "po drodze" danego ruchu
				capturePossible = true;					// ustaw zmienn¹ pomocnicz¹ na "true"
			}

			board[_x2][_y2] = board[_x1][_y1];			// zmieñ stan pola koñcowego na stan pola pocz¹tkowego
			board[_x1][_y1] = empty;					// zmieñ pole pocz¹tkowe na "empty"

			if (capturePossible && checkSquareForCapture(_x2, _y2)) return -1;		// jeœli bicie jest mo¿liwe i mo¿liwe s¹ kolejne bicia zwróæ -1
			else if ((_black && _x2 == 0) || (!_black && _x2 == boardSize - 1)) {	// jeœli ruch koñczy siê na polu w ostatnim rzêdzie
				if (_black) board[_x2][_y2] = blackK;								// zmieñ dany pionek w damkê 
				else board[_x2][_y2] = whiteK;
			}
			return 1;		// jeœli ruch jest ostatnim w sekwencji zwróæ 1
		}
	}
	return 0;				// jeœli w sekwencji nie wystêpuje mo¿liwy ruch, zwróæ 0
}

int Gameboard::getSquareValue(int _x, int _y) {
	int value = 0;			// wartoœæ punktowa

	switch (board[_x][_y]) {
	case whiteM:				
		value += 5;					// jeœli na polu wystêpuje pionek dodaj 5 punktów

		if (_x > 1) {				// jeœli nie wystêpuje w 2 ostatnich rzêdach, dodaj 1 punkt
			++value;

			if (_x > 2) {			// jeœli pionek wystêpuje w rzêdach od 3 wzwy¿, dodaj 1 punkt
				++value;

				if (_x > 3) {		// jeœli pionek wystêpuje w rzêdach od 4 wzwy¿, dodaj 1 punkt
					++value;

					if (_x > 4) {	// jeœli pionek wystêpuje w przeciwnej po³owie planszy dodaj 1 punkt
						++value;
					}
				}
			}
		}

		if (_y == 0 || _y == boardSize - 1) value += 3;		// jeœli pionek wystêpuje przy prawej lub lewej krawêdzi planszy, dodaj 3 punkty
		else if (_y == 1 || _y == boardSize - 2) ++value;	// jeœli pionek wystêpuje w drugiej lub przedostatniej kolumnie, dodaj 1 punkt
		
		if (checkSquareForCapture(_x, _y)) value += 12;		// jeœli pionek ma mo¿liwoœæ bicia, dodaj 12 punktów
		break;

	case blackM:
		value += 5;								// jeœli na polu wystêpuje pionek dodaj 5 punktów

		if (_x < boardSize - 2) {				// jeœli nie wystêpuje w 2 ostatnich rzêdach, dodaj 1 punkt
			++value;

			if (_x < boardSize - 3) {			// jeœli pionek wystêpuje w rzêdach od 6 w dó³, dodaj 1 punkt
				++value;

				if (_x < boardSize - 4) {		// jeœli pionek wystêpuje w rzêdach od 5 w dó³, dodaj 1 punkt
					++value;

					if (_x < boardSize - 5) {	// jeœli pionek wystêpuje w przeciwnej po³owie planszy dodaj 1 punkt
						++value;
					}
				}
			}
		}

		if (_y == 0 || _y == boardSize - 1) value += 3;		// jeœli pionek wystêpuje przy prawej lub lewej krawêdzi planszy, dodaj 3 punkty
		else if (_y == 1 || _y == boardSize - 2) ++value;	// jeœli pionek wystêpuje w drugiej lub przedostatniej kolumnie, dodaj 1 punkt

		if (checkSquareForCapture(_x, _y)) value += 12;		// jeœli pionek ma mo¿liwoœæ bicia, dodaj 12 punktów
		break;

	case whiteK:
		value += 20;										// jeœli pionek jest królem dodaj 20 punktów
		
		if (_y == 0 || _y == boardSize - 1) value += 3;		// jeœli pionek wystêpuje przy prawej lub lewej krawêdzi planszy, dodaj 3 punkty
		else if (_y == 1 || _y == boardSize - 2) ++value;	// jeœli pionek wystêpuje w drugiej lub przedostatniej kolumnie, dodaj 1 punkt

		if (checkSquareForCapture(_x, _y)) value += 12;		// jeœli pionek ma mo¿liwoœæ bicia, dodaj 12 punktów
		break;

	case blackK:
		value += 20;										// jeœli pionek jest królem dodaj 20 punktów

		if (_y == 0 || _y == boardSize - 1) value += 3;		// jeœli pionek wystêpuje przy prawej lub lewej krawêdzi planszy, dodaj 3 punkty
		else if (_y == 1 || _y == boardSize - 2) ++value;	// jeœli pionek wystêpuje w drugiej lub przedostatniej kolumnie, dodaj 1 punkt

		if (checkSquareForCapture(_x, _y)) value += 12;		// jeœli pionek ma mo¿liwoœæ bicia, dodaj 12 punktów
		break;

	case empty:
		break;
	}
	return value;		// zwróæ ³¹czn¹ wartoœæ
}

int Gameboard::checkGameState(bool _black) {
	bool whiteTurn = false;		// zmienna kontrolna, przyjmuj¹ca "true", gdy jest kolej bia³ych
	bool blackTurn = false;		// zmienna kontrolna, przyjmuj¹ca "true", gdy jest kolej czarnych

	int wMen = 0;
	int bMen = 0;
	int wKings = 0;
	int bKings = 0;

	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {
			switch (board[i][j]) {
			case whiteM:
				if (!whiteTurn)		// jeœli nie jest tura bia³ych
					if (getPossibleMoves(i, j).size() != 0) whiteTurn = true;	// jeœli istniej¹ mo¿liwe ruchy zmieñ "whiteTurn" na "true"

				++wMen;				// zliczaj wszystkie bia³e pionki nie bêd¹ce damkami
				break;

			case blackM:
				if (!blackTurn) {	// jeœli nie jest tura czarnych
					if (getPossibleMoves(i, j).size() != 0) blackTurn = true;	// jeœli istniej¹ mo¿liwe ruchy zmieñ "blackTurn" na "true"

				++bMen;				// zliczaj wszystkie czarne pionki nie bêd¹ce damkami
				break;

			case whiteK:
				if (!whiteTurn)		// jeœli nie jest tura bia³ych
					if (getPossibleMoves(i, j).size() != 0) whiteTurn = true;	// jeœli istniej¹ mo¿liwe ruchy zmieñ "whiteTurn" na "true"
				}

				++wKings;			// zliczaj wszystkie bia³e damki
				break;

			case blackK:
				if (!blackTurn)		// jeœli nie jest tura czarnych
					if (getPossibleMoves(i, j).size() != 0)blackTurn = true;	// jeœli istniej¹ mo¿liwe ruchy zmieñ "blackTurn" na "true"

				++bKings;			// zliczaj wszystkie czarne damki
				break;

			case empty:
				break;
			}
		}
	}
	
	if ((wMen || wKings) &&
		(bMen || bKings) &&
		((_black && blackTurn) || (!_black && whiteTurn))) {	// jeœli istniej¹ bia³e i czarne pionki oraz 
																// jeœli sprawdzamy dla czarnych i jest ich tura lub sprawdzamy dla bia³ych i jest ich tura 
		if (bKings &&
			wKings &&
			bMen == blackMen &&
			bKings == blackKings &&
			wMen == whiteMen &&
			wKings == whiteKings) {				// jeœli liczba pionków siê nie zmienia i istniej¹ damki 
			if (++tieCheck == 40) return -1;	// zwróæ -1 - gra koñczy siê remisem
			else tieCheck = 0;
		}

		whiteMen = wMen;		// podmieñ wartoœci 
		whiteKings = wKings;
		blackMen = bMen;
		blackKings = bKings;
		return 0;				// zwróæ 0 - gra toczy siê dalej
	}
	else if ((!bMen && !bKings) || (_black && !blackTurn)) return 1;	// jeœli nie ma ju¿ czarnych pionków lub czarne nie mo¿e wykonaæ ruchu zwróæ 1 - bia³y wygra³
	else if ((!wMen && !wKings) || (!_black && !whiteTurn)) return 2;	// jeœli nie ma ju¿ bia³ych pionków lub bia³e nie mog¹ wykonaæ ruchu zwróæ 2 - czarny wygra³

}

int Gameboard::getBoardValue(bool _black, bool _side) {
	switch (checkGameState(_black)) {
	case -1: 
		return 0;	// jeœli gra jest zremisowana zwróæ 0

	case 0: 
		break;

	case 1: 
		if (_side) return -999;	// jeœli sprawdzamy dla czarnych i bia³y wygrywa, zwróæ -999
		else return 999;		// jeœli sprawdzamy dla bia³ych i bia³y wygrywa, zwróæ 999

	case 2:
		if (_side) return 999;	// jeœli sprawdzamy dla czarnych i czarny wygrywa, zwróæ 999
		else return -999;		// jeœli sprawdzamy dla bia³ych i czarny wygrywa, zwróæ -999
	}

	int whiteScore = 0;
	int blackScore = 0;

	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {		// dla ka¿dego pola 
			if (board[i][j] == blackM || board[i][j] == blackK) blackScore += getSquareValue(i, j);	// jeœli na polu jest pionek czarny, dodaj jego wartoœæ punktow¹ do puntów czarnych
			else if (board[i][j] != empty) whiteScore += getSquareValue(i, j);	// jeœli na polu jest pionek bia³y, dodaje jego wartoœæ punktow¹ do punktów bia³ych
		}
	}

	if (_side) return blackScore - whiteScore;	// jeœli sprawdzamy dla czarnych zwróæ punkty czarnych odj¹æ punkty bia³ych
	else return whiteScore - blackScore;		// jeœli sprawdzamy dla bia³ych zwróæ punkty bia³ych odj¹æ punkty czarnych
}

void Gameboard::resetBoard() {
	tieCheck = 0;

	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {
			if (i % 2 != j % 2) {						// jeœli "i" i "j" tworz¹ parê liczb parzystej i nieparzystej
				if (i < 4) board[i][j] = whiteM;		// jeœli pole znajduje siê w pierwszych czterech rzêdach wstaw bia³y pionek
				else if (i > 5) board[i][j] = blackM;	// jeœli pole znajduje siê w ostatnich czterech rzêdach wstaw czarny pionek
				else board[i][j] = empty;				// ustaw resztê pól z tej puli jako puste
			}
			else board[i][j] = empty;					// ustaw resztê pól jako puste
		}
	}
}