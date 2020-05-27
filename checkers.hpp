#pragma once

#include "ai.hpp"
#include "SFML/Graphics.hpp"

const int windowSize = 1040;

const int pieceOffset = 34;

class Checkers {
	
	bool player;
	bool side;

	Gameboard board;

	sf::RenderWindow window;
	sf::Texture textures[10];
	sf::Sprite sprites[10];

public:
	Checkers();							// konstruktor

	void drawGameContent();				// metoda wyœwietlaj¹ca elementy gry - planszê i pionki

	bool gameStart();					// rozpoczêcie gry
	int gameOver(int _gameState);		// koniec gry, wyœwietl kto wygra³
	void gameLoop();					// pêtla gry
	void gameLoopForDriver();			// pêtla gry dla drivera

	bool closeWindow();					// metoda zamykaj¹ca okno
	int mouseHandler();					// metoda obs³uguj¹ca mysz
	
	void movePlayer();					// metoda obs³uguj¹ca ruch gracza
	void moveAi(bool _other = false);	// metoda obs³uguj¹ca ruch sztucznej inteligencji
};