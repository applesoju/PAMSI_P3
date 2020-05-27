#include "checkers.hpp"

Checkers::Checkers() {

	side = false;
	player = !side;

	window.create(sf::VideoMode(windowSize, windowSize), "Warcaby");

	textures[0].loadFromFile("board.png");				// wczytaj tekstury
	textures[1].loadFromFile("white_man.png");
	textures[2].loadFromFile("black_man.png");
	textures[3].loadFromFile("white_king.png");
	textures[4].loadFromFile("black_king.png");
	textures[5].loadFromFile("game_start_white.png");
	textures[6].loadFromFile("game_start_black.png");
	textures[7].loadFromFile("game_over_white.png");
	textures[8].loadFromFile("game_over_black.png");
	textures[9].loadFromFile("game_over_tie.png");

	for (int i = 0; i < sizeof(textures) / sizeof(textures[0]); ++i) {
		sprites[i].setTexture(textures[i]);			// ustaw tekstury dla sprite'�w
	}
}

void Checkers::drawGameContent() {
	sf::Color defaultColor(114, 137, 218, 255);		// domy�lny kolor 

	window.clear(defaultColor);						// wyczy�� okno

	window.draw(sprites[0]);						// narysuj plansz�

	for (int i = 0; i < boardSize; ++i) {
		for (int j = 0; j < boardSize; ++j) {
			switch (board.board[i][j]) {			// narysuj wszystkie pionki
			
			case whiteM:
				sprites[1].setPosition(j * 100 + pieceOffset, i * 100 + pieceOffset);
				window.draw(sprites[1]);
				break;

			case blackM:
				sprites[2].setPosition(j * 100 + pieceOffset, i * 100 + pieceOffset);
				window.draw(sprites[2]);
				break;

			case whiteK:
				sprites[3].setPosition(j * 100 + pieceOffset, i * 100 + pieceOffset);
				window.draw(sprites[3]);
				break;

			case blackK:
				sprites[4].setPosition(j * 100 + pieceOffset, i * 100 + pieceOffset);
				window.draw(sprites[4]);
				break;
			}
		}
	}
}

bool Checkers::gameStart() {
	sf::Event event;
	sf::Sprite displayedSprite;

	if (side) displayedSprite = sprites[6];
	else displayedSprite = sprites[5];

	drawGameContent();		// narysuj elementy gry

	displayedSprite.setPosition(0, 440);	// ustaw pozycj� sprite'a na �rodek
	window.draw(displayedSprite);
	window.display();

	while (true) {
		if (window.pollEvent(event)) {								// pobierz zdarzenie
			if (event.type == sf::Event::Closed) return false;		// je�li okno zosta�o zamkni�te zwr�� "false"
			else if (event.type == sf::Event::KeyPressed) {			// je�li zosta� naci�ni�ty przycisk
				if (event.key.code == sf::Keyboard::U) return true;	// je�li przyciskiem jest "u" zwr�� "true"
				else if (event.key.code == sf::Keyboard::C) {		// je�li przyciskiem jest "c" 
					player = !player;								// zamie� warto�ci "player" i "side"
					side = !side;									// na przeciwne

					sf::Sprite dSprite;

					if (side) dSprite = sprites[6];					// zdecyduj, kt�ry sprite ma si� wy�wietli�
					else dSprite = sprites[5];						
					
					drawGameContent();								// rysuj elementy gry
					dSprite.setPosition(0, 440);					// ustal pozycj� sprite'a
					window.draw(dSprite);
					window.display();
					
				}
			}
		}
	}
}

int Checkers::gameOver(int _gameState) {
	sf::Sprite displayedSprite;
	sf::Event event;

	switch (_gameState) {				// w zale�no�ci od stanu gry
	case 0:								// je�li gra trwa dalej - zwr�� 0
		return 0;

	case -1:							// je�li gra zako�czy�� si� remisem, wy�wietl odpowiedni� wiadomo��
		displayedSprite = sprites[9];
		break;

	case 1:
		displayedSprite = sprites[7];	// je�li wygra bia�y, wy�wietl odpowiedni� wiadomo��
		break;

	case 2:
		displayedSprite = sprites[8];	// je�li wygra czarny, wy�wietl odpowiedni� wiadomo��
		break;
	}

	drawGameContent();					// rysuj elementy gry

	displayedSprite.setPosition(0, 405);// ustal pozycj� wiadomo�ci o ko�cu gry na �rodek
	window.draw(displayedSprite);
	window.display();

	while (true) {
		if (window.pollEvent(event)) {						// pobierz zdarzenie
			if (event.type == sf::Event::Closed) return 1;	// je�li okno zosta�o zamkni�te, zwr�� 1
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {	// je�li przycisk "u" zosta� wci�ni�ty
				board.resetBoard();		// przywr�� plansz� do stanu pocz�tkowego
				player = !side;			
				return !gameStart();
			}
		}
	}
}

void Checkers::gameLoop() {
	if (gameStart()) {				// je�li  gra zostanie zacz�ta poprzez wci�ni�cie "u"
		while (window.isOpen()) {	// dop�ki okno jest otwarte
			drawGameContent();		// rysuj elementy gry
			window.display();

			if (player) {			// je�li jest teraz ruch gracza
				movePlayer();		// wykonaj ruch
			}
			else moveAi();			// je�li jest ruch ai, wykonaj ruch ai

			player = !player;		// zmie� to, czyja jest tura

			if (gameOver(board.checkGameState(player))) return;	// sprawd�, czy gra si� zako�czy�a
		}
	}
}

bool Checkers::closeWindow() {
	sf::Event event;

	while (window.pollEvent(event)) {			// pobierz zdarzenie
		if (event.type == sf::Event::Closed) {	// je�li okno zosta�o zamkni�te
			window.close();						// zamknij okno
			return true;						// zwr�� "true"
		}
	}
	return false;
}

int Checkers::mouseHandler() {
	sf::Vector2i coords;				// wektor przechowuj�cy informacje na temat pozycji kursora

	while (true) {
		if (closeWindow()) return -1;	// je�li okno zostaje zamkni�te zwr�� -1

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {	// je�li zostaje wci�ni�ty lewy przycisk myszy
			coords = sf::Mouse::getPosition(window);		// pobierz pozycj� kursora

			if (coords.x > 20 && coords.y > 20 && coords.x < 1000 && coords.y < 1000) {		// je�li pozycja kursora mie�ci si� na planszy
				coords.x -= 20;		// odejmij od wsp�rz�dnych przesuni�ci� spowodowane obram�wk�
				coords.y -= 20;
				
				return(coords.y - coords.y % 100) / 10 + (coords.x / 100);	// zwr�� zakodowane wsp�rz�dne - cyfra dziesi�tek jest wierszem polea wybranego przez kursor
			}																// cyfra jedno�ci jest kolumn� pola wybranego przez kursor
		}
	}
}

void Checkers::movePlayer() {
	int selectedPiece, selectedSquare;
	int result = 0;

	do {							// dop�ki nie zostanie wykonany poprawny ruch lub sekwencja ruch�w
		if (result == 0) {			// je�li ruch jest nieprawid�owy pobieraj kolejne wej�cia z myszy
			if ((selectedPiece = mouseHandler()) == -1) return;	// sprawdzaj, czy okno nie zostaje zamkni�te
		}
		do {
			if ((selectedSquare = mouseHandler()) == -1) return;	// dop�ki klikany jest ten sam pionek, pobieraj wej�cie z myszy
		} while (selectedPiece == selectedSquare);

		result = board.movePiece(selectedPiece / 10, selectedPiece % 10, selectedSquare / 10, selectedSquare % 10, side);	// wykonaj ruch
		
		if (result == -1) {					// je�li istnieje mo�liwo�� kolejnego bicia
			selectedPiece = selectedSquare;	// zmie� wybrane pole na to, na kt�rym wyl�dowa� pionek
			drawGameContent();				// rysuj elementy gry
			window.display();
		}
	
	} while (result != 1);
}

void Checkers::moveAi(bool _other) {		
	Sequence bestSequence = Ai::getNextMove(board, sDepth, _other ? side : !side);

	for (Move m : bestSequence.moves) {
		board.movePiece(m);
		drawGameContent();
		window.display();
	}
}