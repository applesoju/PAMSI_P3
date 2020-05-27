#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>

const int boardSize = 10;

enum squareState {	// mo�liwe stany p�l planszy
	empty,			// puste pole
	whiteM,			// bia�y pionek
	blackM,			// czarny pionek
	whiteK,			// bia�a damka
	blackK			// czarna damka
};

struct Move				// struktura reprezentuj�ca pojedy�czy ruch
{
	int x1, y1, x2, y2;	// (x1,y1) - pole pocz�tkowe, (x2,y2) - pole ko�cowe

	Move(int _x1, int _y1, int _x2, int _y2);	// konstruktor
	bool operator!= (const Move& _move);		// przeci��enie operatora nier�wno�ci
};

struct Sequence				// struktura reprezentuj�ca sekwencj� ruch�w
{
	std::vector<Move> moves;// wektor ruch�w

	Sequence() {};			// konstruktor bezparametryczny
	Sequence(Move _move);	// konstruktory parametryczne
	Sequence(int x1, int y1, int x2, int y2);

	void addMove(Move _move);				// metoda dodaj�ca dany ruch do sekwencji

	bool operator==(const Sequence& seq);	// przeci��enie operatora por�wnania
};

class Gameboard {
public:
	int tieCheck;			// licznik tur wykoszystywany do decydowania o remisie
	int whiteMen, blackMen, whiteKings, blackKings;	// liczba pionk�w ka�dego rodzaju wykorzystywana pr
	squareState board[boardSize][boardSize];		// reprezentacja planszy 

	Gameboard();		// konstruktor bezparametryczny

	bool checkSquareForCapture(int _x, int _y);					// metoda sprawdzaj�ca, czy pionek na danym polu mo�e wykona� bicie
	bool checkBoardForCapture(bool _black);						// metoda sprawdzaj�ca ka�de pole pod wzgl�dem,
																// czy znajduje si� na nim pionek, kt�ry mo�e wykona� bicie
	std::vector<Sequence> getSequences(Gameboard _gb, Sequence _seq);	// metoda wyszukuje mo�liwe sekwencje ruch�w i je zwraca
	std::vector<Sequence> getPossibleMoves(int _x, int _y);		// metoda wyszukuje i zwraca mo�liwe do wykonania ruchy dla danego pionka
	std::vector<Sequence> checkBoardForMoves(bool _black);		// metoda sprawdza ka�de pole, pod wzgl�dem tego,
																// jakie pionek, kt�ry si� na nim znajduje, mo�e wykona� ruchy 
	void capturePiece(int _x1, int _y1, int _x2, int _y2);	// metoda zbijaj�ca pionek, znajduj�cy si� na drodze danego ruchu
	int movePiece(Sequence _seq);	// metoda wykonuj�ca dan� sekwencj� ruch�w
	int movePiece(Move _move);		// metody wykonuj�ca dany ruch 
	int movePiece(int x1, int y1, int x2, int y2, bool _black);
	
	int getSquareValue(int _x, int _y);			// metoda obliczaj�ca warto�� punktow� dla danego pola
	int checkGameState(bool _black);			// metoda sprawdzaj�ca aktualny stan gry
	int getBoardValue(bool _black, bool _side);	// metoda obliczaj�ca ca�kowit� warto�� punktow� dla danego koloru

	void resetBoard();							// metoda resetuj�ca plansz� do stanu pocz�tkowego
};