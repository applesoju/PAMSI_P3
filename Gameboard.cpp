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

	if (board[_x][_y] == whiteM || board[_x][_y] == blackM) {	// je�li na danym polu znajduje si� zwyk�y pionek
		for (int i : {-1, 1}) {			// dla p�l o jedno w g�r� i jedno w d�						
			for (int j : {-1, 1}) {		// i dla p�l o jedno w lewi i jedno w prawo
				int ix = _x + i;
				int jy = _y + j;
				if (ix + i < boardSize && ix + i >= 0 && jy + j < boardSize && jy + j >= 0) {	// je�li dane pole mie�ci si� na planszy
					if (black) {
						if (board[ix][jy] == whiteM || board[ix][jy] == whiteK) {				// je�li pionek na drodze jest bia�y
							if (board[ix + i][jy + j] == empty)									// i pole, na kt�rym wyl�dowa� by pionek jest puste	
								return true;													// zwr�� "true"
						}
					}
					else {
						if (board[ix][jy] == blackM || board[ix][jy] == blackK) {				// je�li pionek na drodze jest czarny
							if (board[ix + i][jy + j] == empty)									// i pole, na kt�rym wyl�dowa� by pionek jest puste 
								return true;													// zwr�� "true"
						}					
					}
					
				}
			}
		}
	}

	else if (board[_x][_y] == whiteK || board[_x][_y] == blackK) {		// je�li pionek jest damk�
		for (int i : {-1, 1}) {			// dla p�l o jedno w g�r� i jedno w d�	
			for (int j : {-1, 1}) {		// i dla p�l o jedno w lewi i jedno w prawo
				int ix = _x + i;
				int jy = _y + j;

				while (ix + i < boardSize && ix + i >= 0 && jy + j < boardSize && jy + j >= 0) {	// dop�ki dane pole mie�ci si� na planszy
					if (black) {														// je�li pionek jest czarny
						if (board[ix][jy] == blackM || board[ix][jy] == blackK) break;	// i ma na swojej drodze pionek sojuszniczy, przerwij
						if (board[ix][jy] == whiteM || board[ix][jy] == whiteK) {		// je�li ma na drodze przeciwny pionek
							if (board[ix + i][jy + j] == empty) return true;			// i za nim jest puste pole, zwr�� "true"
							else break;													// w przeciwnym wypadku przerwij
						}
					}
					else {																// je�li pionek jest bia�y
						if (board[ix][jy] == whiteM || board[ix][jy] == whiteK) break;	// i ma na swojej drodze pionek sojuszniczy, przerwij
						if (board[ix][jy] == blackM || board[ix][jy] == blackK) {		// je�li ma na drodze przeciwny pionek
							if (board[ix + i][jy + j] == empty) return true;			// i za nim jest puste pole, zwr�� "true"
							else break;													// w przeciwnym wypadku przerwij
						}
					}
					ix += i;		// przeszukuj dalsze pola
					jy += j;
				}
			}
		}
	}
	return false;					// je�li nic nie zostanie wcze�niej zwr�cone, zwr�� "false"
}

bool Gameboard::checkBoardForCapture(bool _black) {
	for (int i = 0; i < boardSize; ++i) {		// dla ka�dego wiersza
		for (int j = 0; j < boardSize; ++j) {	// i dla ka�dej kolumny
			if (_black) {
				if (board[i][j] == blackM || board[i][j] == blackK) {				// je�li pionek jest czarny
					if (checkSquareForCapture(i, j)) return true;					// zwr�� wynik sprawdzenia,
				}																	// czy z danego pola mo�liwe jest bicie
			}
			else {
				if (board[i][j] == whiteM || board[i][j] == whiteK) {				// je�li pionek jest bia�y
					if (checkSquareForCapture(i, j)) return true;					// zwr�� wynik sprawdzenia,
				}																	// czy z danego pola mo�liwe jest bicie
			}
		}
	}
	return false;		// je�li nic nie zostanie wcze�niej zwr�cone, zwr�� "false"
}

std::vector<Sequence> Gameboard::getSequences(Gameboard _gb, Sequence _seq) {
	std::vector<Sequence> moveSequences;

	if (_gb.movePiece(_seq.moves[0]) == 1)			// po wykonaniu odpowiedniego ruchu
		moveSequences.emplace_back(_seq.moves[0]);	// wstaw ruch do sekwencji
	else {											// w przeciwnym wypadku
		for (Sequence nextMove : _gb.getPossibleMoves(_seq.moves[0].x2, _seq.moves[0].y2)) {	// dla ka�dej kolejnej mo�liwej sekwencji
			std::vector<Sequence> nextSeq = getSequences(_gb, nextMove);						// znajd� kolejne sekwencje ruch�w
			moveSequences.insert(moveSequences.end(), nextSeq.begin(), nextSeq.end());			// i dopisz je do danej sekwencji
		}
		for (Sequence seq : moveSequences) {	// dla ka�dej sekwencji ruch�w 
			seq.addMove(_seq.moves[0]);			// dodaj ruch z ostatniej sekwencji
		}
	}
	return moveSequences;						// zwr�� sekwencj�
}

std::vector<Sequence> Gameboard::getPossibleMoves(int _x, int _y) {
	std::vector<Sequence> moveSequences;

	bool black = true;
	if (board[_x][_y] == whiteM || board[_x][_y] == whiteK) black = false;

	if (board[_x][_y] == empty) return moveSequences;	// je�li dane pole jest puste zwr�� pusty wektor	
	
	if (checkBoardForCapture(black) != checkSquareForCapture(_x, _y)) return moveSequences; // je�li mo�liwe jest bicie dla innego ruchu zwr�� pusty wektor
	if (board[_x][_y] == whiteM || board[_x][_y] == blackM) {	// je�li na polu jest zwyk�y pionek
		if (checkSquareForCapture(_x, _y)) {					// je�li dla danego pola i koloru jest mo�liwo�� bicia
			for (int i : {-1, 1}) {								// dla p�l o jedno w g�r� i jedno w d�	
				for (int j : {-1, 1}) {							// i dla p�l o jedno w lewi i jedno w prawo
					int ix = _x + i;
					int jy = _y + j;
					if (ix + i < boardSize && ix + i >= 0 && jy + j < boardSize && jy + j >= 0) {					// je�li dane pole mie�ci si� na planszy

						if (black && (board[ix][jy] == whiteM || board[ix][jy] == whiteK)) {						// je�li pionek jest bia�y
							if (board[ix + i][jy + j] == empty) moveSequences.emplace_back(_x, _y, ix + i, jy + j);	// i je�li pole na kt�rym wyl�dowa� by pionek jest puste
						}																							// dodaj ruch do sekwencji
						else if (!black && (board[ix][jy] == blackM || board[ix][jy] == blackK)) {					// je�li pionek jest czarny
							if (board[ix + i][jy + j] == empty) moveSequences.emplace_back(_x, _y, ix + i, jy + j);	// i je�li pole na kt�rym wyl�dowa� by pionek jest puste
						}																							// dodaj ruch do sekwencji
					}
				}
			}
		}
		else {								// je�li dla danego pola nie ma mo�liwo�ci bicia
			int direction = black ? -1 : 1;	// okre�l kierunek, w zale�no�ci od koloru

			if (_y < boardSize - 1 && board[_x + direction][_y + 1] == empty)	// je�li pole nie jest polem przy prawej kraw�dzi
				moveSequences.emplace_back(_x, _y, _x + direction, _y + 1);		// dodaj ruch w jej kierunku 
			if (_y > 0 && board[_x + direction][_y - 1] == empty)				// je�li pole nie jest polem przy lewej kraw�dzi
				moveSequences.emplace_back(_x, _y, _x + direction, _y - 1);		// dodaj ruch w jej kierunku
		}
	}
	else {		// je�li dany pionek jest damk�
		bool capturePossible = checkSquareForCapture(_x, _y);	// sprawd�, czy jest mo�liwe bicie

		for (int i : {-1, 1}) {			// dla p�l o jedno w g�r� i jedno w d�	
			for (int j : {-1, 1}) {		// i dla p�l o jedno w lewi i jedno w prawo
				int ix = _x + i;
				int jy = _y + j;

				while (ix >= 0 && ix <= boardSize - 1 && jy >= 0 && jy <= boardSize - 1) {	// dop�ki kolejne pole mie�ci si� na planszy
					if (board[ix][jy] == empty) {											// i jest puste
						if (!capturePossible) moveSequences.emplace_back(_x, _y, ix, jy);	// je�li nie ma mo�liwo�ci bicia,
					}																		// dodaj dany ruch 

					else if (black) {	// je�li dany pionek jest czarny
						if (board[ix][jy] == blackM || board[ix][jy] == blackK) break;		// i na kolejnym polu jest pionek sojuszniczy, przerwij
						else if (capturePossible && (board[ix][jy] == whiteM || board[ix][jy] == whiteK)) {	// je�li mo�liwe jest bicie i na kolejnym polu jest wrogi pionek
							do {
								ix += i;
								jy += j;

								if (ix >= 0 && ix <= boardSize - 1 && jy >= 0 && jy <= boardSize - 1) {		// je�li kolejne pole mie�ci si� na planszy
									if (board[ix][jy] == empty) moveSequences.emplace_back(_x, _y, ix, jy);	// i je�li jest puste, dodaj dany ruch
									else break;		// je�li pole nie jest puste przerwij
								}
								else break;			// je�li kolejne pole nie mie�ci si� na planszy, przerwij
							} while (true);			// powtarzaj dop�ki p�tla nie zostanie przerwana
							break;					// przerwij po wyj�ciu z p�tli
						}
						else break;					// je�li bicie nie jest mo�liwe lub na kolejnym polu nie znajduje si� wrogi pionek, przerwij
					}
					else {							// je�li dany pionek jest bia�y
						if (board[ix][jy] == whiteM || board[ix][jy] == whiteK) break;		// i na kolejnym polu jest pionek sojuszniczy, przerwij
						else if (capturePossible && (board[ix][jy] == blackM || board[ix][jy] == blackK)) {	// je�li mo�liwe jest bicie i na kolejnym polu jest wrogi pionek
							do {
								ix += i;
								jy += j;

								if (ix >= 0 && ix <= boardSize - 1 && jy >= 0 && jy <= boardSize - 1) {		// je�li kolejne pole mie�ci si� na planszy
									if (board[ix][jy] == empty) moveSequences.emplace_back(_x, _y, ix, jy);	// i je�li jest puste, dodaj dany ruch
									else break;		// je�li pole nie jest puste przerwij
								}
								else break;			// je�li kolejne pole nie mie�ci si� na planszy, przerwij
							} while (true);			// powtarzaj dop�ki p�tla nie zostanie przerwana
							break;					// przerwij po wyj�ciu z p�tli
						}
						else break;					// je�li bicie nie jest mo�liwe lub na kolejnym polu nie znajduje si� wrogi pionek, przerwij
					}
					ix += i;						// przeszukuj dalsze pola
					jy += j;
				}
			}
		}
	}
	return moveSequences;		// zwr�� sekwencj� ruch�w				
}

std::vector<Sequence> Gameboard::checkBoardForMoves(bool _black) {
	std::vector<Sequence> moveSequences;

	for (int i = 0; i < boardSize; ++i) {		// dla kolejnych wierszy
		for (int j = 0; j < boardSize; ++j) {	// i kolejnych kolumn
			if (_black && (board[i][j] == blackM || board[i][j] == blackK)) {	// je�li na danym polu jest czarny pionek i sprawdzamy ruchy dla czarnych pionk�w
				std::vector<Sequence> moveSeq = getPossibleMoves(i, j);			// znajd� mo�liwe sekwencje ruch�w dla danego pola

				for (Sequence seq : moveSeq) {	// dla ka�dej sekwencji ruch�w 
					std::vector<Sequence> nextSeq = getSequences(*this, seq);					// znajd� kolejne sekwencje
					moveSequences.insert(moveSequences.end(), nextSeq.begin(), nextSeq.end());	// wstaw kolejne sekwencje
				}
			}
			else if (!_black && (board[i][j] == whiteM || board[i][j] == whiteK)) {	// je�li na danym polu jest bia�y pionek i sprawdzamy ruchy dla bia�ych pionk�w
				std::vector<Sequence> moveSeq = getPossibleMoves(i, j);				// znajd� mo�liwe sekwencje ruch�w dla danego pola

				for (Sequence seq : moveSeq) {	// dla ka�dej sekwencji ruch�w 
					std::vector<Sequence> nextSeq = getSequences(*this, seq);					// znajd� kolejne sekwencje
					moveSequences.insert(moveSequences.end(), nextSeq.begin(), nextSeq.end());	// wstaw kolejne sekwencje
				}
			}
		}
	}
	return moveSequences;		// zwr�� wektor sekwencji
}

void Gameboard::capturePiece(int _x1, int _y1, int _x2, int _y2) {
	int xDif = (_x1 < _x2) ? 1 : -1;	// je�li indeks poprzeniego wiersza jest mnniejszy od nast�pnego ustaw "xDif"
												// jako 1, w przeciwnym razie jako -1
	int yDif = (_y1 < _y2) ? 1 : -1;	// je�li indeks poprzedniej kolumny jest mniejszy od nast�pnego ustaw "yDif"
												// jako -1 
	for (int i = _x1 + xDif, j = _y1 + yDif; i != _x2 && j != _y2; i += xDif, j += yDif) {
		board[i][j] = empty;	// ustaw kolejne pola, kt�re s� "po drodze" dla danego ruchu na "empty"
	}
}

int Gameboard::movePiece(Sequence _seq) {
	int i = 0;
	while (movePiece(_seq.moves[i]) == -1) ++i;		// wykonuj kolejne ruchy sekwencji bi�
	return 1;
}

int Gameboard::movePiece(Move _move) {
	bool black = true;			// sprawd� jakiego koloru jest pionek znajduj�cy si� na polu
	if (board[_move.x1][_move.y1] == whiteM || board[_move.x1][_move.y1] == whiteK)
		black = false;

	return movePiece(_move.x1, _move.y1, _move.x2, _move.y2, black);	// wykonaj dany ruch
}

int Gameboard::movePiece(int _x1, int _y1, int _x2, int _y2, bool _black) {
	
	if((_black &&										// sprawd�, czy zosta� wybrany odpowiedni kolor
		(board[_x1][_y1] == whiteM || board[_x1][_y1] == whiteK)) ||
		(!_black &&
		(board[_x1][_y1] == blackM || board[_x1][_y1] == blackK))) return 0;
	
	bool capturePossible = false;						// zmienna pomocnicza okre�laj�ca, czy bicie jest mo�liwe
	
	for (Sequence seq : getPossibleMoves(_x1, _y1)) {	// dla ka�dej kolejnej sekwencji 
		if (seq == Sequence(_x1, _y1, _x2, _y2)) {		// je�li sekwencja r�wna si� pojedynczemu, danemu ruchowi
			if (checkBoardForCapture(_black)) {			// je�li dane pionki maj� mo�liwo�� bicia
				capturePiece(_x1, _y1, _x2, _y2);		// zbij pionki b�d�ce "po drodze" danego ruchu
				capturePossible = true;					// ustaw zmienn� pomocnicz� na "true"
			}

			board[_x2][_y2] = board[_x1][_y1];			// zmie� stan pola ko�cowego na stan pola pocz�tkowego
			board[_x1][_y1] = empty;					// zmie� pole pocz�tkowe na "empty"

			if (capturePossible && checkSquareForCapture(_x2, _y2)) return -1;		// je�li bicie jest mo�liwe i mo�liwe s� kolejne bicia zwr�� -1
			else if ((_black && _x2 == 0) || (!_black && _x2 == boardSize - 1)) {	// je�li ruch ko�czy si� na polu w ostatnim rz�dzie
				if (_black) board[_x2][_y2] = blackK;								// zmie� dany pionek w damk� 
				else board[_x2][_y2] = whiteK;
			}
			return 1;		// je�li ruch jest ostatnim w sekwencji zwr�� 1
		}
	}
	return 0;				// je�li w sekwencji nie wyst�puje mo�liwy ruch, zwr�� 0
}

int Gameboard::getSquareValue(int _x, int _y) {
	int value = 0;			// warto�� punktowa

	switch (board[_x][_y]) {
	case whiteM:				
		value += 5;					// je�li na polu wyst�puje pionek dodaj 5 punkt�w

		if (_x > 1) {				// je�li nie wyst�puje w 2 ostatnich rz�dach, dodaj 1 punkt
			++value;

			if (_x > 2) {			// je�li pionek wyst�puje w rz�dach od 3 wzwy�, dodaj 1 punkt
				++value;

				if (_x > 3) {		// je�li pionek wyst�puje w rz�dach od 4 wzwy�, dodaj 1 punkt
					++value;

					if (_x > 4) {	// je�li pionek wyst�puje w przeciwnej po�owie planszy dodaj 1 punkt
						++value;
					}
				}
			}
		}

		if (_y == 0 || _y == boardSize - 1) value += 3;		// je�li pionek wyst�puje przy prawej lub lewej kraw�dzi planszy, dodaj 3 punkty
		else if (_y == 1 || _y == boardSize - 2) ++value;	// je�li pionek wyst�puje w drugiej lub przedostatniej kolumnie, dodaj 1 punkt
		
		if (checkSquareForCapture(_x, _y)) value += 12;		// je�li pionek ma mo�liwo�� bicia, dodaj 12 punkt�w
		break;

	case blackM:
		value += 5;								// je�li na polu wyst�puje pionek dodaj 5 punkt�w

		if (_x < boardSize - 2) {				// je�li nie wyst�puje w 2 ostatnich rz�dach, dodaj 1 punkt
			++value;

			if (_x < boardSize - 3) {			// je�li pionek wyst�puje w rz�dach od 6 w d�, dodaj 1 punkt
				++value;

				if (_x < boardSize - 4) {		// je�li pionek wyst�puje w rz�dach od 5 w d�, dodaj 1 punkt
					++value;

					if (_x < boardSize - 5) {	// je�li pionek wyst�puje w przeciwnej po�owie planszy dodaj 1 punkt
						++value;
					}
				}
			}
		}

		if (_y == 0 || _y == boardSize - 1) value += 3;		// je�li pionek wyst�puje przy prawej lub lewej kraw�dzi planszy, dodaj 3 punkty
		else if (_y == 1 || _y == boardSize - 2) ++value;	// je�li pionek wyst�puje w drugiej lub przedostatniej kolumnie, dodaj 1 punkt

		if (checkSquareForCapture(_x, _y)) value += 12;		// je�li pionek ma mo�liwo�� bicia, dodaj 12 punkt�w
		break;

	case whiteK:
		value += 20;										// je�li pionek jest kr�lem dodaj 20 punkt�w
		
		if (_y == 0 || _y == boardSize - 1) value += 3;		// je�li pionek wyst�puje przy prawej lub lewej kraw�dzi planszy, dodaj 3 punkty
		else if (_y == 1 || _y == boardSize - 2) ++value;	// je�li pionek wyst�puje w drugiej lub przedostatniej kolumnie, dodaj 1 punkt

		if (checkSquareForCapture(_x, _y)) value += 12;		// je�li pionek ma mo�liwo�� bicia, dodaj 12 punkt�w
		break;

	case blackK:
		value += 20;										// je�li pionek jest kr�lem dodaj 20 punkt�w

		if (_y == 0 || _y == boardSize - 1) value += 3;		// je�li pionek wyst�puje przy prawej lub lewej kraw�dzi planszy, dodaj 3 punkty
		else if (_y == 1 || _y == boardSize - 2) ++value;	// je�li pionek wyst�puje w drugiej lub przedostatniej kolumnie, dodaj 1 punkt

		if (checkSquareForCapture(_x, _y)) value += 12;		// je�li pionek ma mo�liwo�� bicia, dodaj 12 punkt�w
		break;

	case empty:
		break;
	}
	return value;		// zwr�� ��czn� warto��
}

int Gameboard::checkGameState(bool _black) {
	bool whiteTurn = false;		// zmienna kontrolna, przyjmuj�ca "true", gdy jest kolej bia�ych
	bool blackTurn = false;		// zmienna kontrolna, przyjmuj�ca "true", gdy jest kolej czarnych

	int wMen = 0;
	int bMen = 0;
	int wKings = 0;
	int bKings = 0;

	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {
			switch (board[i][j]) {
			case whiteM:
				if (!whiteTurn)		// je�li nie jest tura bia�ych
					if (getPossibleMoves(i, j).size() != 0) whiteTurn = true;	// je�li istniej� mo�liwe ruchy zmie� "whiteTurn" na "true"

				++wMen;				// zliczaj wszystkie bia�e pionki nie b�d�ce damkami
				break;

			case blackM:
				if (!blackTurn) {	// je�li nie jest tura czarnych
					if (getPossibleMoves(i, j).size() != 0) blackTurn = true;	// je�li istniej� mo�liwe ruchy zmie� "blackTurn" na "true"

				++bMen;				// zliczaj wszystkie czarne pionki nie b�d�ce damkami
				break;

			case whiteK:
				if (!whiteTurn)		// je�li nie jest tura bia�ych
					if (getPossibleMoves(i, j).size() != 0) whiteTurn = true;	// je�li istniej� mo�liwe ruchy zmie� "whiteTurn" na "true"
				}

				++wKings;			// zliczaj wszystkie bia�e damki
				break;

			case blackK:
				if (!blackTurn)		// je�li nie jest tura czarnych
					if (getPossibleMoves(i, j).size() != 0)blackTurn = true;	// je�li istniej� mo�liwe ruchy zmie� "blackTurn" na "true"

				++bKings;			// zliczaj wszystkie czarne damki
				break;

			case empty:
				break;
			}
		}
	}
	
	if ((wMen || wKings) &&
		(bMen || bKings) &&
		((_black && blackTurn) || (!_black && whiteTurn))) {	// je�li istniej� bia�e i czarne pionki oraz 
																// je�li sprawdzamy dla czarnych i jest ich tura lub sprawdzamy dla bia�ych i jest ich tura 
		if (bKings &&
			wKings &&
			bMen == blackMen &&
			bKings == blackKings &&
			wMen == whiteMen &&
			wKings == whiteKings) {				// je�li liczba pionk�w si� nie zmienia i istniej� damki 
			if (++tieCheck == 40) return -1;	// zwr�� -1 - gra ko�czy si� remisem
			else tieCheck = 0;
		}

		whiteMen = wMen;		// podmie� warto�ci 
		whiteKings = wKings;
		blackMen = bMen;
		blackKings = bKings;
		return 0;				// zwr�� 0 - gra toczy si� dalej
	}
	else if ((!bMen && !bKings) || (_black && !blackTurn)) return 1;	// je�li nie ma ju� czarnych pionk�w lub czarne nie mo�e wykona� ruchu zwr�� 1 - bia�y wygra�
	else if ((!wMen && !wKings) || (!_black && !whiteTurn)) return 2;	// je�li nie ma ju� bia�ych pionk�w lub bia�e nie mog� wykona� ruchu zwr�� 2 - czarny wygra�

}

int Gameboard::getBoardValue(bool _black, bool _side) {
	switch (checkGameState(_black)) {
	case -1: 
		return 0;	// je�li gra jest zremisowana zwr�� 0

	case 0: 
		break;

	case 1: 
		if (_side) return -999;	// je�li sprawdzamy dla czarnych i bia�y wygrywa, zwr�� -999
		else return 999;		// je�li sprawdzamy dla bia�ych i bia�y wygrywa, zwr�� 999

	case 2:
		if (_side) return 999;	// je�li sprawdzamy dla czarnych i czarny wygrywa, zwr�� 999
		else return -999;		// je�li sprawdzamy dla bia�ych i czarny wygrywa, zwr�� -999
	}

	int whiteScore = 0;
	int blackScore = 0;

	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {		// dla ka�dego pola 
			if (board[i][j] == blackM || board[i][j] == blackK) blackScore += getSquareValue(i, j);	// je�li na polu jest pionek czarny, dodaj jego warto�� punktow� do punt�w czarnych
			else if (board[i][j] != empty) whiteScore += getSquareValue(i, j);	// je�li na polu jest pionek bia�y, dodaje jego warto�� punktow� do punkt�w bia�ych
		}
	}

	if (_side) return blackScore - whiteScore;	// je�li sprawdzamy dla czarnych zwr�� punkty czarnych odj�� punkty bia�ych
	else return whiteScore - blackScore;		// je�li sprawdzamy dla bia�ych zwr�� punkty bia�ych odj�� punkty czarnych
}

void Gameboard::resetBoard() {
	tieCheck = 0;

	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {
			if (i % 2 != j % 2) {						// je�li "i" i "j" tworz� par� liczb parzystej i nieparzystej
				if (i < 4) board[i][j] = whiteM;		// je�li pole znajduje si� w pierwszych czterech rz�dach wstaw bia�y pionek
				else if (i > 5) board[i][j] = blackM;	// je�li pole znajduje si� w ostatnich czterech rz�dach wstaw czarny pionek
				else board[i][j] = empty;				// ustaw reszt� p�l z tej puli jako puste
			}
			else board[i][j] = empty;					// ustaw reszt� p�l jako puste
		}
	}
}