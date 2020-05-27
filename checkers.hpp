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

	void drawGameContent();				// metoda wy�wietlaj�ca elementy gry - plansz� i pionki

	bool gameStart();					// rozpocz�cie gry
	int gameOver(int _gameState);		// koniec gry, wy�wietl kto wygra�
	void gameLoop();					// p�tla gry 

	bool closeWindow();					// metoda zamykaj�ca okno
	int mouseHandler();					// metoda obs�uguj�ca mysz
	
	void movePlayer();					// metoda obs�uguj�ca ruch gracza
	void moveAi(bool _other = false);	// metoda obs�uguj�ca ruch sztucznej inteligencji
};