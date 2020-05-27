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
		sprites[i].setTexture(textures[i]);			// ustaw tekstury dla sprite'ów
	}
}

void Checkers::drawGameContent() {
	sf::Color defaultColor(114, 137, 218, 255);		// domyœlny kolor 

	window.clear(defaultColor);						// wyczyœæ okno

	window.draw(sprites[0]);						// narysuj planszê

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

	displayedSprite.setPosition(0, 440);	// ustaw pozycjê sprite'a na œrodek
	window.draw(displayedSprite);
	window.display();

	while (true) {
		if (window.pollEvent(event)) {								// pobierz zdarzenie
			if (event.type == sf::Event::Closed) return false;		// jeœli okno zosta³o zamkniête zwróæ "false"
			else if (event.type == sf::Event::KeyPressed) {			// jeœli zosta³ naciœniêty przycisk
				if (event.key.code == sf::Keyboard::U) return true;	// jeœli przyciskiem jest "u" zwróæ "true"
				else if (event.key.code == sf::Keyboard::C) {		// jeœli przyciskiem jest "c" 
					player = !player;								// zamieñ wartoœci "player" i "side"
					side = !side;									// na przeciwne

					sf::Sprite dSprite;

					if (side) dSprite = sprites[6];					// zdecyduj, który sprite ma siê wyœwietliæ
					else dSprite = sprites[5];						
					
					drawGameContent();								// rysuj elementy gry
					dSprite.setPosition(0, 440);					// ustal pozycjê sprite'a
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

	switch (_gameState) {				// w zale¿noœci od stanu gry
	case 0:								// jeœli gra trwa dalej - zwróæ 0
		return 0;

	case -1:							// jeœli gra zakoñczy³¹ siê remisem, wyœwietl odpowiedni¹ wiadomoœæ
		displayedSprite = sprites[9];
		break;

	case 1:
		displayedSprite = sprites[7];	// jeœli wygra bia³y, wyœwietl odpowiedni¹ wiadomoœæ
		break;

	case 2:
		displayedSprite = sprites[8];	// jeœli wygra czarny, wyœwietl odpowiedni¹ wiadomoœæ
		break;
	}

	drawGameContent();					// rysuj elementy gry

	displayedSprite.setPosition(0, 405);// ustal pozycjê wiadomoœci o koñcu gry na œrodek
	window.draw(displayedSprite);
	window.display();

	while (true) {
		if (window.pollEvent(event)) {						// pobierz zdarzenie
			if (event.type == sf::Event::Closed) return 1;	// jeœli okno zosta³o zamkniête, zwróæ 1
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {	// jeœli przycisk "u" zosta³ wciœniêty
				board.resetBoard();		// przywróæ planszê do stanu pocz¹tkowego
				player = !side;			
				return !gameStart();
			}
		}
	}
}

void Checkers::gameLoop() {
	if (gameStart()) {				// jeœli  gra zostanie zaczêta poprzez wciœniêcie "u"
		while (window.isOpen()) {	// dopóki okno jest otwarte
			drawGameContent();		// rysuj elementy gry
			window.display();

			if (player) {			// jeœli jest teraz ruch gracza
				movePlayer();		// wykonaj ruch
			}
			else moveAi();			// jeœli jest ruch ai, wykonaj ruch ai

			player = !player;		// zmieñ to, czyja jest tura

			if (gameOver(board.checkGameState(player))) return;	// sprawdŸ, czy gra siê zakoñczy³a
		}
	}
}

bool Checkers::closeWindow() {
	sf::Event event;

	while (window.pollEvent(event)) {			// pobierz zdarzenie
		if (event.type == sf::Event::Closed) {	// jeœli okno zosta³o zamkniête
			window.close();						// zamknij okno
			return true;						// zwróæ "true"
		}
	}
	return false;
}

int Checkers::mouseHandler() {
	sf::Vector2i coords;				// wektor przechowuj¹cy informacje na temat pozycji kursora

	while (true) {
		if (closeWindow()) return -1;	// jeœli okno zostaje zamkniête zwróæ -1

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {	// jeœli zostaje wciœniêty lewy przycisk myszy
			coords = sf::Mouse::getPosition(window);		// pobierz pozycjê kursora

			if (coords.x > 20 && coords.y > 20 && coords.x < 1000 && coords.y < 1000) {		// jeœli pozycja kursora mieœci siê na planszy
				coords.x -= 20;		// odejmij od wspó³rzêdnych przesuniêciê spowodowane obramówk¹
				coords.y -= 20;
				
				return(coords.y - coords.y % 100) / 10 + (coords.x / 100);	// zwróæ zakodowane wspó³rzêdne - cyfra dziesi¹tek jest wierszem polea wybranego przez kursor
			}																// cyfra jednoœci jest kolumn¹ pola wybranego przez kursor
		}
	}
}

void Checkers::movePlayer() {
	int selectedPiece, selectedSquare;
	int result = 0;

	do {							// dopóki nie zostanie wykonany poprawny ruch lub sekwencja ruchów
		if (result == 0) {			// jeœli ruch jest nieprawid³owy pobieraj kolejne wejœcia z myszy
			if ((selectedPiece = mouseHandler()) == -1) return;	// sprawdzaj, czy okno nie zostaje zamkniête
		}
		do {
			if ((selectedSquare = mouseHandler()) == -1) return;	// dopóki klikany jest ten sam pionek, pobieraj wejœcie z myszy
		} while (selectedPiece == selectedSquare);

		result = board.movePiece(selectedPiece / 10, selectedPiece % 10, selectedSquare / 10, selectedSquare % 10, side);	// wykonaj ruch
		
		if (result == -1) {					// jeœli istnieje mo¿liwoœæ kolejnego bicia
			selectedPiece = selectedSquare;	// zmieñ wybrane pole na to, na którym wyl¹dowa³ pionek
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