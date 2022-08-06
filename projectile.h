#pragma once

#include "common.h"
#include "entity.h"

class Projectile :public Entity {
public:
	Projectile(Image& image, Level& lev, float Damage, float GunAngle, float X, float Y, int W, int H, bool Team, String Name);

	void CollisionWithEntity() override;

	void CollisionWithArmor(Vector2f P1, Vector2f P2, float damage, SoundClass&) override;

	void GunControl(Vector2f v) override;

	void update(float time) override;
private:
	int currTime = 0;
	int timeToLive = 40000;
};