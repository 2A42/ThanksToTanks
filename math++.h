#pragma once

#include "common.h"

namespace math {
	bool isPointAtLine(Vector2f P1, Vector2f P2, Vector2f P3);

	int getDistance(Vector2f P1, Vector2f P2);

	Vector2f getVectorDistance(Vector2f P1, Vector2f P2);

	Vector2f getTop(Sprite sp);
}
