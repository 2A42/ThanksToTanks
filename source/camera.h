#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

View camera;
void setPlayerCoordinateForCamera(float x, float y) {
	camera.setCenter(x, y);
}