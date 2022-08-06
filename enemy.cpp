#include "enemy.h"

Enemy::Enemy(Vehicle& vehicle, Image& image, int W, int H, Image& GunImage, int GunW, int GunH, Level& lev, float X, float Y, String Name) :
	Entity(image, GunImage, X, Y, W, H, Name) 
{
	enemyVehicle = vehicle;
	obj = lev.GetObjects("wall");

	if (name == "enemy1") {
		sprite.setTextureRect(IntRect(0, 0, w, h));
		speed = enemyVehicle.getSpeed();
		XP = enemyVehicle.getXP();
		maxHealth = enemyVehicle.getHealth();
		health = enemyVehicle.getHealth();
		damage = enemyVehicle.getDamage();
		ReloadingProcess = enemyVehicle.getReloadingTime();
	}
}

void Enemy::GunControl(Vector2f Object)
{
	float dX = GunSprite.getPosition().x - Object.x;
	float dY = GunSprite.getPosition().y - Object.y;
	float rotation = (atan2(dY, dX)) * GRADS;
	GunSprite.setRotation(rotation - 90);
	if (ReloadingProcess < 1) { isShoot = true; ReloadingProcess = enemyVehicle.getReloadingTime(); }
}

void Enemy::CollisionWithEntity()
{
	if (collision)
	{
		speed = -speed; 
		x = x0;
		y = y0;
		collision = false;
	}
}

void Enemy::CollisionWithArmor(Vector2f P1, Vector2f P2, float damage, SoundClass& sound)
{
	if (P1 == TopLeftCorner && P2 == TopRightCorner ||
		P1 == TopRightCorner && P2 == TopLeftCorner) {
		health -= damage * enemyVehicle.getFrontShootMultiplier();

		if (damage > 1) sound.shell_frontArmor_explosion();
	}

	else if ((P1 == TopLeftCorner && P2 == LowLeftCorner ||
		P1 == TopRightCorner && P2 == LowRightCorner) ||
		(P1 == LowLeftCorner && P2 == TopLeftCorner ||
			P1 == LowRightCorner && P2 == TopRightCorner)) {
		health -= damage * enemyVehicle.getSideShootMultiplier();

		if (damage > 1) sound.shell_sideArmor_explosion();
	}

	else if (P1 == LowLeftCorner && P2 == LowRightCorner ||
		P1 == LowRightCorner && P2 == LowLeftCorner) {
		health -= damage * enemyVehicle.getRearShootMultiplier();

		if (damage > 1) sound.shell_rearArmor_explosion();
	}
}

void Enemy::update(float time)
{
	if (name == "enemy1") {
		x0 = x;
		y0 = y;
		angle0 = angle;
		spriteAngle0 = sprite.getRotation();
		ReloadingProcess -= time;

		angle = angle - enemyVehicle.getRotationSpeed() * RADS;
		dx = speed * cos(angle); dy = speed * sin(angle);
		x += dx * time; y += dy * time;
		sprite.setOrigin(w / 2, h / 2);
		sprite.setRotation(sprite.getRotation() - enemyVehicle.getRotationSpeed());
		sprite.setPosition(x, y);
		GunSprite.setOrigin(GunSprite.getTextureRect().width / 2, GunSprite.getTextureRect().height / 2);
		GunSprite.setPosition(sprite.getPosition());
		CollisionWithMap();
		VertexesOfRect();

		if (health <= 0) { life = false; }
	}
}