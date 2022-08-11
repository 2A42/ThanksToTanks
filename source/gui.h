#pragma once

#include "common.h"
#include <fstream>

class Gui {
public:
	Gui(RenderWindow&);

	void drawCursor(Vector2f);

	void drawString(String, Vector2f, int, Color);

	void drawHealth(float, float, Vector2f, String);
	
	void menu(int&, int&, float&, float&, int&, int&, int&, int&, bool&);

	bool endScreen(float);

	bool pause(float&);

private:
	Font font;
	RenderWindow* Window;
	Texture cursorTexture; Sprite spriteCursor;
	float bestTime;
	int bestDate;
	std::fstream fileBuffer;

	void menu2 (float&, int&, int&, int&, int&, bool&);
	void menu3 (float&, int&, int&, bool&);
};