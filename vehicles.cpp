#include "vehicles.h"

Vehicle::Vehicle()
{
}

Vehicle::Vehicle(String name)
{
	///////////////////////////////////////////////Axis vehicles
	if (name == "Panzer4") {
		XP = 200;
		health = 1100;
		damage = 280;
		rotationSpeed = 0.8;
		maxSpeedUp = 0.175;
		frontShootMultiplier = 0.65;
		sideShootMultiplier = 1.15;
		rearShootMultiplier = 1.75;
	}
	else if (name == "Tiger1") {
		XP = 400;
		health = 1800;
		damage = 400;
		rotationSpeed = 0.55;
		maxSpeedUp = 0.105;
		frontShootMultiplier = 0.4;
		sideShootMultiplier = 1.25;
		rearShootMultiplier = 1.75;
	}
	else if (name == "Panther") {
		XP = 600;
		health = 1750;
		damage = 450;
		rotationSpeed = 0.7;
		maxSpeedUp = 0.155;
		frontShootMultiplier = 0.3;
		sideShootMultiplier = 1.3;
		rearShootMultiplier = 1.7;
	}
	else if (name == "Tiger2") {
		XP = 800;
		health = 3000;
		damage = 600;
		rotationSpeed = 0.45;
		maxSpeedUp = 0.085;
		frontShootMultiplier = 0.15;
		sideShootMultiplier = 1.25;
		rearShootMultiplier = 1.75;
	}
	///////////////////////////////////////////////Allies vehicles
	else if (name == "Greyhound") {
		XP = 100;
		health = 200;
		damage = 100;
		rotationSpeed = 0.85;
		maxSpeedUp = 0.225;
		frontShootMultiplier = 0.9;
		sideShootMultiplier = 1;
		rearShootMultiplier = 1.1;
	}
	else if (name == "Valentine") {
		XP = 120;
		health = 850;
		damage = 200;
		rotationSpeed = 0.65;
		maxSpeedUp = 0.13;
		frontShootMultiplier = 0.75;
		sideShootMultiplier = 1.2;
		rearShootMultiplier = 1.5;
	}
	else if (name == "Firefly") {
		XP = 130;
		health = 1250;
		damage = 500;
		rotationSpeed = 0.6;
		maxSpeedUp = 0.15;
		frontShootMultiplier = 0.7;
		sideShootMultiplier = 1.3;
		rearShootMultiplier = 1.7;
	}
	else if (name == "Churchil") {
		XP = 140;
		health = 2275;
		damage = 275;
		rotationSpeed = 0.5;
		maxSpeedUp = 0.11;
		frontShootMultiplier = 0.4;
		sideShootMultiplier = 1.1;
		rearShootMultiplier = 1.75;
	}
	reloadingTime = 4000;
}

float Vehicle::getHealth()
{
	return health;
}

float Vehicle::getDamage()
{
	return damage;
}

float Vehicle::getRotationSpeed()
{
	return rotationSpeed;
}

float Vehicle::getSpeed()
{
	return maxSpeedUp;
}

float Vehicle::getXP()
{
	return XP;
}

float Vehicle::getReloadingTime()
{
	return reloadingTime;
}

float Vehicle::getFrontShootMultiplier()
{
	return frontShootMultiplier;
}

float Vehicle::getSideShootMultiplier()
{
	return sideShootMultiplier;
}

float Vehicle::getRearShootMultiplier()
{
	return rearShootMultiplier;
}