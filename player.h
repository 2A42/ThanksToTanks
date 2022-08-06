#pragma once

#include "entity.h"
#include "vehicles.h"

enum Direction
{
	DOWN,
	UP
};

using namespace sf;

class Player : public Entity {
public:
	Player(Vehicle& vehicle, Image& image, int W, int H, Image& GunImage, int GunW, int GunH, Level& lev, float X, float Y, String Name);

	void Control(SoundClass&);

	void GunControl(Vector2f MousePos) override;

	void CollisionWithEntity() override;

	void CollisionWithArmor(Vector2f P1, Vector2f P2, float damage, SoundClass&) override;

	void update(float time) override;

	void LVLprogression(SoundClass&);

	int playerXP = 0, playerLVL = 1;
private:
	bool isSpawn = false;
	int LVL_UP = 600, ReloadingProcess, timerToSpawn = 0;
	Vehicle playerVehicle;
	Object playerSpawn;
};