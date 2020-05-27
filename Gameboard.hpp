#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>

const int boardSize = 10;

enum squareState {	// mo¿liwe stany pól planszy
	empty,			// puste pole
	whiteM,			// bia³y pionek
	blackM,			// czarny pionek
	whiteK,			// bia³a damka
	blackK			// czarna damka
};

struct Move				// struktura reprezentuj¹ca pojedyñczy ruch
{
	int x1, y1, x2, y2;	// (x1,y1) - pole pocz¹tkowe, (x2,y2) - pole koñcowe

	Move(int _x1, int _y1, int _x2, int _y2);	// konstruktor
	bool operator!= (const Move& _move);		// przeci¹¿enie operatora nierównoœci
};

struct Sequence				// struktura reprezentuj¹ca sekwencjê ruchów
{
	std::vector<Move> moves;// wektor ruchów

	Sequence() {};			// konstruktor bezparametryczny
	Sequence(Move _move);	// konstruktory parametryczne
	Sequence(int x1, int y1, int x2, int y2);

	void addMove(Move _move);				// metoda dodaj¹ca dany ruch do sekwencji

	bool operator==(const Sequence& seq);	// przeci¹¿enie operatora porównania
};

class Gameboard {
public:
	int tieCheck;			// licznik tur wykoszystywany do decydowania o remisie
	int whiteMen, blackMen, whiteKings, blackKings;	// liczba pionków ka¿dego rodzaju wykorzystywana pr
	squareState board[boardSize][boardSize];		// reprezentacja planszy 

	Gameboard();		// konstruktor bezparametryczny

	bool checkSquareForCapture(int _x, int _y);					// metoda sprawdzaj¹ca, czy pionek na danym polu mo¿e wykonaæ bicie
	bool checkBoardForCapture(bool _black);						// metoda sprawdzaj¹ca ka¿de pole pod wzglêdem,
																// czy znajduje siê na nim pionek, który mo¿e wykonaæ bicie
	std::vector<Sequence> getSequences(Gameboard _gb, Sequence _seq);	// metoda wyszukuje mo¿liwe sekwencje ruchów i je zwraca
	std::vector<Sequence> getPossibleMoves(int _x, int _y);		// metoda wyszukuje i zwraca mo¿liwe do wykonania ruchy dla danego pionka
	std::vector<Sequence> checkBoardForMoves(bool _black);		// metoda sprawdza ka¿de pole, pod wzglêdem tego,
																// jakie pionek, który siê na nim znajduje, mo¿e wykonaæ ruchy 
	void capturePiece(int _x1, int _y1, int _x2, int _y2);	// metoda zbijaj¹ca pionek, znajduj¹cy siê na drodze danego ruchu
	int movePiece(Sequence _seq);	// metoda wykonuj¹ca dan¹ sekwencjê ruchów
	int movePiece(Move _move);		// metody wykonuj¹ca dany ruch 
	int movePiece(int x1, int y1, int x2, int y2, bool _black);
	
	int getSquareValue(int _x, int _y);			// metoda obliczaj¹ca wartoœæ punktow¹ dla danego pola
	int checkGameState(bool _black);			// metoda sprawdzaj¹ca aktualny stan gry
	int getBoardValue(bool _black, bool _side);	// metoda obliczaj¹ca ca³kowit¹ wartoœæ punktow¹ dla danego koloru

	void resetBoard();							// metoda resetuj¹ca planszê do stanu pocz¹tkowego
};