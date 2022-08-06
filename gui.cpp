#include "gui.h"

Gui::Gui(RenderWindow& window)
{
	Window = &window;
	font.loadFromFile("Images/Pencils.ttf");
	cursorTexture.loadFromFile("Images/menu/cursor.png");
	spriteCursor.setTexture(cursorTexture);
	spriteCursor.setScale(0.5, 0.5);
	fileBuffer.open("results.txt", std::ios::in);
	fileBuffer >> bestTime;
	fileBuffer >> bestDate;
	fileBuffer.close();
}

void Gui::drawCursor(Vector2f mousePos)
{
	spriteCursor.setPosition(mousePos);
	Window->draw(spriteCursor);
}

void Gui::drawString(String s, Vector2f pos, int size, Color color)
{
	Text text("", font, size);
	text.setString(s);
	text.setFillColor(color);
	text.setPosition(pos);
	Window->draw(text);
}

void Gui::drawHealth(float maxHP, float HP, Vector2f pos, String name)
{
	String str = std::to_string(int(HP)) + " / " + std::to_string(int(maxHP));
	Text hp(str, font, 25);
	if (name == "Player1") hp.setFillColor(Color::Blue);
	else hp.setFillColor(Color::Red);
	hp.setPosition(pos.x - 70, pos.y - 100);
	hp.setStyle(Text::Italic | Text::Underlined);
	Window->draw(hp);
}

void Gui::menu(int& W, int& H, float& volume, float& XPmultiplier, int& maxShootDist, int& accurcy, int& PLAYERlifesCount, int& PLAYERvehicle, bool& isInGame)
{
	Texture menuOptionsButtonTexture, menuExitButtonTexture, menuFaqButtonTexture, menuBackground;
	menuOptionsButtonTexture.loadFromFile("Images/menu/options.png");
	menuExitButtonTexture.loadFromFile("Images/menu/exit.png");
	menuFaqButtonTexture.loadFromFile("Images/menu/faq.png");
	menuBackground.loadFromFile("Images/menu/background.png");

	Sprite options(menuOptionsButtonTexture), exit(menuExitButtonTexture), faq(menuFaqButtonTexture), menuBg(menuBackground);
	options.setScale(0.14, 0.14); options.setPosition(Window->getView().getSize().x - 170, 15);
	exit.setScale(0.2, 0.2); exit.setPosition(Window->getView().getSize().x - 100, 0);
	faq.setScale(0.49, 0.49); faq.setPosition(Window->getView().getSize().x - 270, 2);
	float ScaleX = menuBg.getTextureRect().width / Window->getView().getSize().x; float ScaleY = menuBg.getTextureRect().height / Window->getView().getSize().y;
	menuBg.setScale(1 / ScaleX, 1 / ScaleY); menuBg.setPosition(0, 0);

	Event event;
	bool isMenu = true, resetStats = false;
	while (isMenu) {
		while (Window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				Window->close();
		}

		if (IntRect(exit.getPosition().x, exit.getPosition().y, exit.getTextureRect().width * exit.getScale().x, exit.getTextureRect().height * exit.getScale().y).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) {	isMenu = false; isInGame = false;	}
		else if (IntRect(faq.getPosition().x, faq.getPosition().y, faq.getTextureRect().width * faq.getScale().x, faq.getTextureRect().height * faq.getScale().y).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left))	{

			Texture faqScreen;	faqScreen.loadFromFile("Images/menu/faqScreen.png");
			Sprite faqScrn(faqScreen); faqScrn.setPosition(Window->getView().getCenter().x - Window->getView().getSize().x / 2, Window->getView().getCenter().y - Window->getView().getSize().y / 2);
			faqScrn.setScale(1 / ScaleX, 1 / ScaleY); faqScrn.setPosition(0, 0);

			bool isFaq = true;
			while (isFaq)
			{
				while (Window->pollEvent(event))
				{
					if (event.type == Event::Closed)
						Window->close();
				}
				if (Keyboard::isKeyPressed(Keyboard::Escape)) { isFaq = false; }
				Window->clear();
				Window->draw(faqScrn);
				Window->display();
			}
		}
		else if (IntRect(options.getPosition().x, options.getPosition().y, options.getTextureRect().width * options.getScale().x, options.getTextureRect().height * options.getScale().y).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) {
			
			Texture mark; mark.loadFromFile("Images/menu/check_mark.png"); Sprite check_mark(mark); check_mark.setScale(0.6, 0.6);
			if (W == 1366)	check_mark.setPosition(Vector2f(Window->getView().getCenter().x - 550, Window->getView().getCenter().y - 200));
			else if (W == 1600) check_mark.setPosition(Vector2f(Window->getView().getCenter().x - 550, Window->getView().getCenter().y - 150));
			else if (W == 1920) check_mark.setPosition(Vector2f(Window->getView().getCenter().x - 550, Window->getView().getCenter().y - 100));

			RectangleShape statsComponent1(Vector2f(40, 40)), statsComponent2(Vector2f(30, 30)), volumeComponent1(Vector2f(300, 10)), volumeComponent2(Vector2f(30, 80));
			statsComponent1.setPosition(Vector2f(Window->getView().getCenter().x + 50, Window->getView().getCenter().y + 70)); 
			statsComponent2.setPosition(statsComponent1.getPosition().x + 5, statsComponent1.getPosition().y + 5); statsComponent2.setFillColor(Color::Black);
			volumeComponent1.setPosition(Vector2f(Window->getView().getCenter().x + 320, Window->getView().getCenter().y - 200));	
			volumeComponent2.setPosition(Vector2f(Window->getView().getCenter().x + (300 * volume/100) + 300, Window->getView().getCenter().y - 237)); volumeComponent2.setFillColor(Color::Yellow);
		
			bool isOptions = true, isOptionsChanged = false;
			while (isOptions) {
				while (Window->pollEvent(event))
				{
					if (event.type == Event::Closed)
						Window->close();
				}

				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					Time t(sf::seconds(0.15)); sleep(t); 
					if (isOptionsChanged) {
						if (resetStats) {
							fileBuffer.open("results.txt", std::ios::out);
							bestTime = 0;	bestDate = 0;
							fileBuffer << bestTime << std::endl;
							fileBuffer << bestDate;
							fileBuffer.close();
							resetStats = false;
						}
						fileBuffer.open("settings.txt", std::ios::out);
						fileBuffer << W << std::endl;
						fileBuffer << H << std::endl;
						fileBuffer << volume;
						fileBuffer.close();

						Window->create(VideoMode(W, H), "Game", Style::None);
						options.setPosition(Window->getView().getSize().x - 170, 15);
						exit.setPosition(Window->getView().getSize().x - 100, 0);
						faq.setPosition(Window->getView().getSize().x - 270, 2);
						ScaleX = menuBg.getTextureRect().width / Window->getView().getSize().x; ScaleY = menuBg.getTextureRect().height / Window->getView().getSize().y;
						menuBg.setScale(1 / ScaleX, 1 / ScaleY); menuBg.setPosition(0, 0);
					}
					isOptions = false; isOptionsChanged = false; isMenu = true;
				}
				else if (resetStats && IntRect(Window->getView().getCenter().x + 50, Window->getView().getCenter().y + 70, 40, 40).contains(Mouse::getPosition(*Window))
					&& Mouse::isButtonPressed(Mouse::Right)) {
					resetStats = false;
				}
				else if (IntRect(Window->getView().getCenter().x - 500, Window->getView().getCenter().y - 200, 350, 35).contains(Mouse::getPosition(*Window))
					&& Mouse::isButtonPressed(Mouse::Left)) {
					check_mark.setPosition(Vector2f(Window->getView().getCenter().x - 550, Window->getView().getCenter().y - 200));
					W = 1366; H = 768;	isOptionsChanged = true;
				}
				else if (IntRect(Window->getView().getCenter().x - 500, Window->getView().getCenter().y - 150, 350, 35).contains(Mouse::getPosition(*Window))
					&& Mouse::isButtonPressed(Mouse::Left)) {
					check_mark.setPosition(Vector2f(Window->getView().getCenter().x - 550, Window->getView().getCenter().y - 150));
					W = 1600; H = 900;	isOptionsChanged = true;
				}
				else if (IntRect(Window->getView().getCenter().x - 500, Window->getView().getCenter().y - 100, 350, 35).contains(Mouse::getPosition(*Window))
					&& Mouse::isButtonPressed(Mouse::Left)) {
					check_mark.setPosition(Vector2f(Window->getView().getCenter().x - 550, Window->getView().getCenter().y - 100));
					W = 1920; H = 1080;	isOptionsChanged = true;
				}

				if (IntRect(Window->getView().getCenter().x + 50, Window->getView().getCenter().y + 70, 40, 40).contains(Mouse::getPosition(*Window))
					&& Mouse::isButtonPressed(Mouse::Left)) {
					resetStats = true;	isOptionsChanged = true;
				}

				if (event.type == Event::MouseWheelScrolled) {
					if (event.mouseWheelScroll.delta > 0 && !(volume == 100)) {
						volumeComponent2.setPosition(volumeComponent2.getPosition().x + 30, volumeComponent2.getPosition().y);
						volume += 10;	isOptionsChanged = true;
					}
					else if (event.mouseWheelScroll.delta < 0 && !(volume == 0)) {
						volumeComponent2.setPosition(volumeComponent2.getPosition().x - 30, volumeComponent2.getPosition().y);
						volume -= 10;	isOptionsChanged = true;
					}
					event.type = Event::TouchEnded;
				}

				Window->clear();
				Window->draw(menuBg);	Window->draw(check_mark);
				drawString("Change Resolution:", Vector2f(Window->getView().getCenter().x - 600, Window->getView().getCenter().y - 300), 40, Color(200, 100, 50));
				drawString("1366 x 768", Vector2f(Window->getView().getCenter().x - 500, Window->getView().getCenter().y - 200), 35, Color::White);
				drawString("1600 x 900", Vector2f(Window->getView().getCenter().x - 500, Window->getView().getCenter().y - 150), 35, Color::White);
				drawString("1920 x 1080", Vector2f(Window->getView().getCenter().x - 500, Window->getView().getCenter().y - 100), 35, Color::White);

				drawString("(Use the mouse wheel)", Vector2f(Window->getView().getCenter().x + 310, Window->getView().getCenter().y - 330), 25, Color(200, 100, 50));
				drawString("Change Volume: ", Vector2f(Window->getView().getCenter().x + 300, Window->getView().getCenter().y - 300), 40, Color(200, 100, 50));
				drawString(std::to_string(int(volume)) + "%", Vector2f(Window->getView().getCenter().x + 440, Window->getView().getCenter().y - 100), 40, Color::White);
				
				drawString("Reset Stats", Vector2f(Window->getView().getCenter().x - 50, Window->getView().getCenter().y), 40, Color(200, 100, 50));
				
				Window->draw(statsComponent1); if(resetStats) Window->draw(statsComponent2); Window->draw(volumeComponent1);	Window->draw(volumeComponent2);
				Window->display();
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Space)) {	isMenu = false;	menu2(XPmultiplier, maxShootDist, accurcy, PLAYERlifesCount, PLAYERvehicle, isMenu);	}

		Window->clear();
		Window->draw(menuBg);
		Window->draw(options);
		Window->draw(exit);
		Window->draw(faq);
		drawString("Press SPACE to start the game", Vector2f(Window->getView().getCenter().x - 300, Window->getView().getCenter().y + 200), 40, Color::White);
		drawString("Best time: " + std::to_string(bestTime), Vector2f(menuBg.getPosition().x + 10, menuBg.getPosition().y + 10), 25, Color::Black);
		drawString("Best date: " + std::to_string(bestDate), Vector2f(menuBg.getPosition().x + 10, menuBg.getPosition().y + 40), 25, Color::Black);
		Window->display();
	}
}

void Gui::menu2(float& XPmultiplier, int& maxShootDist, int& accurcy, int& PLAYERlifesCount, int& PLAYERvehicle, bool& isMenu)
{
	bool isMenu2 = true;
	Texture Background, tank1_card, tank2_card, tank3_card, tank4_card, mark;
	mark.loadFromFile("Images/menu/check_mark.png");
	Background.loadFromFile("Images/menu/endScreenBg.png");
	tank1_card.loadFromFile("Images/menu/pz4_card.png");
	tank2_card.loadFromFile("Images/menu/tiger1_card.png");
	tank3_card.loadFromFile("Images/menu/panther_card.png");
	tank4_card.loadFromFile("Images/menu/tiger2_card.png");
	Sprite Bg(Background), tank1(tank1_card), tank2(tank2_card), tank3(tank3_card), tank4(tank4_card), check_mark(mark);
	Bg.setPosition(Window->getView().getCenter().x - Window->getView().getSize().x / 2, Window->getView().getCenter().y - Window->getView().getSize().y / 2);
	tank1.setPosition(Window->getView().getCenter().x - 800, Window->getView().getCenter().y - 200);
	tank2.setPosition(Window->getView().getCenter().x - 400, Window->getView().getCenter().y - 200);
	tank3.setPosition(Window->getView().getCenter().x + 0, Window->getView().getCenter().y - 200);
	tank4.setPosition(Window->getView().getCenter().x + 400, Window->getView().getCenter().y - 200);
	check_mark.setPosition(-500, 0);

	Event event;
	while (isMenu2) {
		while (Window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				Window->close();
		}

		if (IntRect(tank1.getPosition().x, tank1.getPosition().y, tank1.getTextureRect().width * tank1.getScale().x, tank1.getTextureRect().height * tank1.getScale().y).contains(Mouse::getPosition(*Window))) check_mark.setPosition(tank1.getPosition().x + tank1.getTextureRect().width / 2 - 30, tank1.getPosition().y - 100);
		else if (IntRect(tank2.getPosition().x, tank2.getPosition().y, tank2.getTextureRect().width * tank2.getScale().x, tank2.getTextureRect().height * tank2.getScale().y).contains(Mouse::getPosition(*Window))) check_mark.setPosition(tank2.getPosition().x + tank2.getTextureRect().width / 2 - 30, tank2.getPosition().y - 100);
		else if (IntRect(tank3.getPosition().x, tank3.getPosition().y, tank3.getTextureRect().width * tank3.getScale().x, tank3.getTextureRect().height * tank3.getScale().y).contains(Mouse::getPosition(*Window))) check_mark.setPosition(tank3.getPosition().x + tank3.getTextureRect().width / 2 - 30, tank3.getPosition().y - 100);
		else if (IntRect(tank4.getPosition().x, tank4.getPosition().y, tank4.getTextureRect().width * tank4.getScale().x, tank4.getTextureRect().height * tank4.getScale().y).contains(Mouse::getPosition(*Window))) check_mark.setPosition(tank4.getPosition().x + tank4.getTextureRect().width / 2 - 30, tank4.getPosition().y - 100);


		if (Keyboard::isKeyPressed(Keyboard::Escape)) { Time t(sf::seconds(0.15)); sleep(t); isMenu2 = false; isMenu = true; }

		else if (IntRect(tank1.getPosition().x, tank1.getPosition().y, tank1.getTextureRect().width * tank1.getScale().x, tank1.getTextureRect().height * tank1.getScale().y).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) {
			Time t(sf::seconds(0.15)); sleep(t);
			isMenu2 = false;
			PLAYERlifesCount = 4;	PLAYERvehicle = 1;
			menu3(XPmultiplier, maxShootDist, accurcy, isMenu2);
		}

		else if (IntRect(tank2.getPosition().x, tank2.getPosition().y, tank2.getTextureRect().width * tank2.getScale().x, tank2.getTextureRect().height * tank2.getScale().y).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) {
			Time t(sf::seconds(0.15)); sleep(t);
			isMenu2 = false;
			PLAYERlifesCount = 3;	PLAYERvehicle = 2;
			menu3(XPmultiplier, maxShootDist, accurcy, isMenu2);
		}

		else if (IntRect(tank3.getPosition().x, tank3.getPosition().y, tank3.getTextureRect().width * tank3.getScale().x, tank3.getTextureRect().height * tank3.getScale().y).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) {
			Time t(sf::seconds(0.15)); sleep(t);
			isMenu2 = false;
			PLAYERlifesCount = 2;	PLAYERvehicle = 3;
			menu3(XPmultiplier, maxShootDist, accurcy, isMenu2);
		}

		else if (IntRect(tank4.getPosition().x, tank4.getPosition().y, tank4.getTextureRect().width * tank4.getScale().x, tank4.getTextureRect().height * tank4.getScale().y).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) {
			Time t(sf::seconds(0.15)); sleep(t);
			isMenu2 = false;
			PLAYERlifesCount = 1;	PLAYERvehicle = 4;
			menu3(XPmultiplier, maxShootDist, accurcy, isMenu2);
		}

		Window->clear();
		Window->draw(Bg);
		Window->draw(check_mark);
		drawString("CHOOSE THE UNIT", Vector2f(Window->getView().getCenter().x - 150, Window->getView().getCenter().y - Window->getSize().y / 2 + 50), 40, Color::White);
		drawString("PANZER 4 (GHOST) DIV.				'TIGER TRIO'				PANTHER BATTLEGROUP				KING TIGER ", Vector2f(Window->getView().getCenter().x - 750, Window->getView().getCenter().y - 240), 30, Color::Yellow);
		Window->draw(tank1);
		Window->draw(tank2);
		Window->draw(tank3);
		Window->draw(tank4);

		Window->display();
	}
}

void Gui::menu3(float& XPmultiplier, int& maxShootDist, int& accurcy, bool& isMenu2)
{
	bool isMenu3 = true;
	Texture Background, mark;
	mark.loadFromFile("Images/menu/check_mark.png");
	Background.loadFromFile("Images/menu/endScreenBg.png");
	Sprite Bg(Background), check_mark(mark);
	Bg.setPosition(Window->getView().getCenter().x - Window->getView().getSize().x / 2, Window->getView().getCenter().y - Window->getView().getSize().y / 2);
	check_mark.setScale(0.6, 0.6);
	check_mark.setPosition(-500, 0);

	Event event;
	while (isMenu3) {
		while (Window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				Window->close();
		}

		if (IntRect(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 350, Window->getSize().x / 2, 30).contains(Mouse::getPosition(*Window))) check_mark.setPosition(Window->getView().getCenter().x - 100, Window->getView().getCenter().y - Window->getSize().y / 2 + 350);
		else if (IntRect(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 400, Window->getSize().x / 2, 30).contains(Mouse::getPosition(*Window))) check_mark.setPosition(Window->getView().getCenter().x - 100, Window->getView().getCenter().y - Window->getSize().y / 2 + 400);
		else if (IntRect(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 450, Window->getSize().x / 2, 30).contains(Mouse::getPosition(*Window))) check_mark.setPosition(Window->getView().getCenter().x - 100, Window->getView().getCenter().y - Window->getSize().y / 2 + 450);

		if (Keyboard::isKeyPressed(Keyboard::Escape)) { Time t(sf::seconds(0.15)); sleep(t); isMenu3 = false; isMenu2 = true; }
		else if (IntRect(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 350, Window->getSize().x / 2, 30).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) { XPmultiplier = 1; maxShootDist = 400; accurcy = 50; isMenu3 = false;	  }
		else if (IntRect(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 400, Window->getSize().x / 2, 30).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) { XPmultiplier = 1.25; maxShootDist = 600; accurcy = 30; isMenu3 = false; }
		else if (IntRect(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 450, Window->getSize().x / 2, 30).contains(Mouse::getPosition(*Window))
			&& Mouse::isButtonPressed(Mouse::Left)) { XPmultiplier = 1.5; maxShootDist = 800; accurcy = 10; isMenu3 = false;  }

		Window->clear();
		Window->draw(Bg);
		Window->draw(check_mark);
		drawString("CHOOSE THE DIFFICULTY", Vector2f(Window->getView().getCenter().x - 200, Window->getView().getCenter().y - Window->getSize().y / 2 + 50), 40, Color::White);
		drawString("EASY", Vector2f(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 350), 30, Color::Green);
		drawString("MEDIUM", Vector2f(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 400), 30, Color::Yellow);
		drawString("HARD", Vector2f(Window->getView().getCenter().x, Window->getView().getCenter().y - Window->getSize().y / 2 + 450), 30, Color::Red);

		Window->display();
	}
}

bool Gui::endScreen(float time)
{
	Texture Background; Background.loadFromFile("Images/menu/endScreenBg.png");
	Sprite Bg(Background); Bg.setPosition(Window->getView().getCenter().x - Window->getView().getSize().x / 2, Window->getView().getCenter().y - Window->getView().getSize().y / 2);

	int date = 1945 + int(time / 20);
	bool isEnd = true;

	if (time > bestTime) {
		bestTime = time;
		bestDate = date;
		fileBuffer.open("results.txt", std::ios::out);
		fileBuffer << bestTime << std::endl;
		fileBuffer << bestDate;
		fileBuffer.close();
	}

	while (isEnd) {
		Event event;
		while (Window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				Window->close();
		}

		Window->clear();
		Window->draw(Bg);

		drawString("Time: " + std::to_string(time), Vector2f(Window->getView().getCenter().x - 100, Window->getView().getCenter().y - Window->getSize().y / 2 + 300), 35, Color(200, 100, 50));
		drawString("Best time: " + std::to_string(bestTime), Vector2f(Window->getView().getCenter().x - 100, Window->getView().getCenter().y - Window->getSize().y / 2 + 340), 35, Color(200, 100, 50));

		drawString("The Fatherland will be proud of you! The time for which you delayed", Vector2f(Window->getView().getCenter().x - 400, Window->getView().getCenter().y + 50), 25, Color::White);
		drawString("the Allied troops led to the fact that the Berlin has fallen in ", Vector2f(Window->getView().getCenter().x - 400, Window->getView().getCenter().y + 100), 25, Color::White);
		drawString(std::to_string(date), Vector2f(Window->getView().getCenter().x + 470, Window->getView().getCenter().y + 100), 30, Color::Green);

		drawString("Press SPACE to return to main menu", Vector2f(Window->getView().getCenter().x - 350, Window->getView().getCenter().y + 300), 40, Color::Yellow);

		if (Keyboard::isKeyPressed(Keyboard::Space)) { isEnd = false;	return true; }

		Window->display();
	}
}

bool Gui::pause(float& timeAfterPause)
{
	Clock clock;	Event event;
	bool isPaused = true, isEnd = false;
	Window->setMouseCursorVisible(true);

	Text pauseText("GAME PAUSED", font, 100);
	pauseText.setStyle(Text::Underlined);
	pauseText.setPosition(Vector2f(Window->getView().getCenter().x - 300, Window->getView().getCenter().y - 300));

	Time t(sf::seconds(0.15)); sleep(t);
	while (isPaused) {
		while (Window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				Window->close();
		}

		Vector2i MousePixelPos = Mouse::getPosition(*Window);
		Vector2f MousePos = Window->mapPixelToCoords(MousePixelPos);

		if (Keyboard::isKeyPressed(Keyboard::Escape) || (IntRect(Window->getView().getCenter().x - 100, Window->getView().getCenter().y + 100, 300, 50).contains(Vector2i(MousePos))
			&& Mouse::isButtonPressed(Mouse::Left))) { sleep(t); timeAfterPause += clock.getElapsedTime().asSeconds();	isPaused = false; }
		else if (IntRect(Window->getView().getCenter().x - 100, Window->getView().getCenter().y + 200, 450, 50).contains(Vector2i(MousePos))
			&& Mouse::isButtonPressed(Mouse::Left)) { isEnd = true; isPaused = false; }
		
		Window->draw(pauseText);
		drawString("RESUME", Vector2f(Window->getView().getCenter().x - 100, Window->getView().getCenter().y + 100), 50, Color::White);
		drawString("MAIN MENU", Vector2f(Window->getView().getCenter().x - 100, Window->getView().getCenter().y + 200), 50, Color::White);
		Window->display();
	}
	if (isEnd) return true;
	else { Window->setMouseCursorVisible(false); return false; }
}
