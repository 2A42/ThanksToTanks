#pragma once

#include "common.h"

struct Vehicle {
public:
	Vehicle();
	Vehicle(String name);

	float getHealth();
	float getDamage();
	float getRotationSpeed();
	float getSpeed();
	float getXP();
	float getFrontShootMultiplier();
	float getSideShootMultiplier();
	float getRearShootMultiplier();
	float getReloadingTime();

private:
	float health = 0, damage = 0, rotationSpeed = 0, maxSpeedUp = 0, reloadingTime = 0,
	XP = 0,	frontShootMultiplier = 0, sideShootMultiplier = 0, rearShootMultiplier = 0;
};