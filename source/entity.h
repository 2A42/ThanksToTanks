#pragma once

#include <vector>

#include "common.h"
#include "level.h"
#include "sounds.h"

using namespace sf;

class Entity {
public:
	bool life, team, collision, isShoot;

	Entity(Image& image, Image& GunImage, float X, float Y, int W, int H, String Name);

	virtual void update(float time) = 0;
	virtual void CollisionWithEntity() = 0;
	virtual void CollisionWithArmor(Vector2f, Vector2f, float, SoundClass&) = 0;
	virtual void GunControl(Vector2f) = 0;

	float getXPcost();
	float getDamage();
	float getCurrentHealth();
	float getMaxHealth();

	Sprite& getSprite();
	Sprite& getGunSprite();

	String getName();

	std::vector<Vector2f>& getVertex();

protected:
		int direction = 0, lifesCount;
		float dx, dy, x, y, x0, y0, w, h, health, maxHealth, damage, XP, speed = 0, angle = -PI / 2, angle0 = -PI / 2, spriteAngle0;
		float collisionPointX, collisionPointY;
		Texture texture, GunTexture;
		String name;
		Sprite sprite, GunSprite;
		Vector2f TopLeftCorner, TopRightCorner, LowLeftCorner, LowRightCorner;	//<-Rectangle Vertexes
		std::vector<Vector2f> Vertexes;
		std::vector<Object> obj;
		std::vector<FloatRect> pointsOfRect;

		void CollisionWithMap();

		void VertexesOfRect();

		FloatRect getRect();
};