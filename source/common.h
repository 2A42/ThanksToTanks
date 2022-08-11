#pragma once

#include <SFML/Graphics.hpp>

#define PI 3.141592
#define RADS PI / 180
#define GRADS 180 / PI
#define SEGMENT_X (std::max(P1.x, P2.x) >= P3.x && std::min(P1.x, P2.x) <= P3.x)
#define SEGMENT_Y (std::max(P1.y, P2.y) >= P3.y && std::min(P1.y, P2.y) <= P3.y)
#define MAP_BOUNDS_X lvl.GetWidth() * lvl.GetTileSize().x
#define MAP_BOUNDS_Y lvl.GetHeight() * lvl.GetTileSize().y
#define GREYHOUND Greyhound, GreyhoundBody, GreyhoundBody.getSize().x, GreyhoundBody.getSize().y, GreyhoundGun, GreyhoundGun.getSize().x, GreyhoundGun.getSize().y
#define VALENTINE Valentine, ValentineBody, ValentineBody.getSize().x, ValentineBody.getSize().y, ValentineGun, ValentineGun.getSize().x, ValentineGun.getSize().y
#define FIREFLY Firefly, FireflyBody, FireflyBody.getSize().x, FireflyBody.getSize().y, FireflyGun, FireflyGun.getSize().x, FireflyGun.getSize().y
#define CHURCHIL Churchil, ChurchilBody, ChurchilBody.getSize().x, ChurchilBody.getSize().y, ChurchilGun, ChurchilGun.getSize().x, ChurchilGun.getSize().y

using namespace sf;