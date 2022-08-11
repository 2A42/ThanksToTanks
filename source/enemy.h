#pragma once

#include "common.h"
#include "entity.h"
#include "vehicles.h"

class Enemy :public Entity {
public:
	Enemy(Vehicle& vehicle, Image& image, int W, int H, Image& GunImage, int GunW, int GunH, Level& lev, float X, float Y, String Name);

	void GunControl(Vector2f Object) override;

	void CollisionWithEntity() override;

	void CollisionWithArmor(Vector2f P1, Vector2f P2, float damage, SoundClass&) override;

	void update(float time) override;

private:
	int ReloadingProcess;
	Vehicle enemyVehicle;
};