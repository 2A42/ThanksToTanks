#include "math++.h"

namespace math {
	bool isPointAtLine(Vector2f P1, Vector2f P2, Vector2f P3) {

		if ((int(P2.x - P1.x) == 0 && abs(P3.x - P1.x) <= 20) && SEGMENT_Y ||
			(int(P2.y - P1.y) == 0 && abs(P3.y - P1.y) <= 20) && SEGMENT_X)
			return true;

		else
			return	SEGMENT_X && (int((P3.x - P1.x) / (P2.x - P1.x)) == int((P3.y - P1.y) / (P2.y - P1.y)));
	}

	int getDistance(Vector2f P1, Vector2f P2) {
		return sqrt(pow((P2.x - P1.x), 2) + pow((P2.y - P1.y), 2));
	}

	Vector2f getVectorDistance(Vector2f P1, Vector2f P2) {
		return Vector2f(abs(P2.x - P1.x), abs(P2.y - P1.y));
	}

	Vector2f getTop(Sprite sp) {
		auto rotation = (sp.getRotation() - 90) * RADS;
		return Vector2f(((sp.getTextureRect().width - 50) / 2 * sin(rotation) + (sp.getTextureRect().height + 50) / 2 * cos(rotation)) + sp.getPosition().x,
					   (-(sp.getTextureRect().width - 50) / 2 * cos(rotation) + (sp.getTextureRect().height + 50) / 2 * sin(rotation)) + sp.getPosition().y);
	}
}