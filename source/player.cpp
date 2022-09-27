#include "player.h"

Player::Player(Vehicle& vehicle, Image& image, int W, int H, Image& GunImage, int GunW, int GunH, Level& lev, float X, float Y, String Name) :
	Entity(image, GunImage, X, Y, W, H, Name) 
{
	playerVehicle = vehicle;
	obj = lev.GetAllObjects();

	if (name == "Player1") {
		playerSpawn = lev.GetObject("Player");
		sprite.setTextureRect(IntRect(0, 0, w, h));
		GunSprite.setTextureRect(IntRect(0, 0, GunW, GunH));
		maxHealth = playerVehicle.getHealth();
		health = playerVehicle.getHealth();
		damage = playerVehicle.getDamage();
		ReloadingProcess = playerVehicle.getReloadingTime();
	}
}

void Player::Control(SoundClass& sound)
{
	int reverse = 1;
	angle0 = angle;
	spriteAngle0 = sprite.getRotation();
	if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
		if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) reverse = -1;
		sprite.setRotation(sprite.getRotation() - reverse * playerVehicle.getRotationSpeed());
		angle = angle - reverse * playerVehicle.getRotationSpeed() * RADS;
		sound.player_moving();
	}
	if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
		if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) reverse = -1;
		sprite.setRotation(sprite.getRotation() + reverse * playerVehicle.getRotationSpeed());
		angle = angle + reverse * playerVehicle.getRotationSpeed() * RADS;
		sound.player_moving();
	}
	if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
		direction = UP;
		speed = playerVehicle.getSpeed();
		sound.player_moving();
	}
	if ((Keyboard::isKeyPressed(Keyboard::Down) || (Keyboard::isKeyPressed(Keyboard::S)))) {
		direction = DOWN;
		speed = playerVehicle.getSpeed();
		sound.player_moving();
	}
}

void Player::GunControl(Vector2f MousePos)
{
	float dX = MousePos.x - sprite.getPosition().x;
	float dY = MousePos.y - sprite.getPosition().y;
	float rotation = (atan2(dY, dX)) * GRADS;

	GunSprite.setOrigin(GunSprite.getTextureRect().width / 2, GunSprite.getTextureRect().height / 2);
	GunSprite.setRotation(rotation + 90);
	GunSprite.setPosition(sprite.getPosition().x, sprite.getPosition().y);
	if (Mouse::isButtonPressed(Mouse::Left) && ReloadingProcess < 1) { isShoot = true; ReloadingProcess = playerVehicle.getReloadingTime(); }
}

void Player::CollisionWithEntity()
{
	if (collision)	{
		x = x0;
		y = y0;
		angle = angle0;
		sprite.setRotation(spriteAngle0);
	}
}

void Player::CollisionWithArmor(Vector2f P1, Vector2f P2, float damage, SoundClass& sound)
{
	if (P1 == TopLeftCorner && P2 == TopRightCorner ||
		P1 == TopRightCorner && P2 == TopLeftCorner) {

		health -= damage * playerVehicle.getFrontShootMultiplier();
		
		if (damage > 1) sound.shell_frontArmor_explosion();
		else			sound.repair_pickup();
	}

	else if ((P1 == TopLeftCorner && P2 == LowLeftCorner ||
			  P1 == TopRightCorner && P2 == LowRightCorner) ||
			 (P1 == LowLeftCorner && P2 == TopLeftCorner ||
			  P1 == LowRightCorner && P2 == TopRightCorner)) {

		health -= damage * playerVehicle.getSideShootMultiplier();
		
		if (damage > 1) sound.shell_sideArmor_explosion();
		else			sound.repair_pickup();
	}

	else if (P1 == LowLeftCorner && P2 == LowRightCorner ||
			 P1 == LowRightCorner && P2 == LowLeftCorner) {

		health -= damage * playerVehicle.getRearShootMultiplier();
		
		if (damage > 1) sound.shell_rearArmor_explosion();
		else			sound.repair_pickup();
	}
}

void Player::update(float time)
{
	switch (direction)
	{
		case Direction::DOWN: dy = -speed * sin(angle); dx = -speed * cos(angle); break;
		case Direction::UP:	  dy = speed * sin(angle); dx = speed * cos(angle);	  break;
	}
	x0 = x;
	y0 = y;
	collision = false;
	ReloadingProcess -= time;
	x += dx * time;
	y += dy * time;
	speed = 0;
	sprite.setPosition(x, y);
	sprite.setOrigin(w / 2, h / 2);
	CollisionWithMap();
	VertexesOfRect();

	if (health > maxHealth)	health = maxHealth;
	if (!life) { life = true; timerToSpawn = 15000; isSpawn = true; }
	else if (isSpawn) { timerToSpawn -= time; if (timerToSpawn < 0) { isSpawn = false; sprite.setColor(Color::White); GunSprite.setColor(Color::White); health = playerVehicle.getHealth(); x = playerSpawn.rect.left; y = playerSpawn.rect.top; } }
}

void Player::LVLprogression(SoundClass& sound)
{
	if (playerXP >= LVL_UP) {
		float multiplier = 0.05;
		playerXP = 0;	playerLVL++;
		LVL_UP = LVL_UP + 2 * LVL_UP * multiplier;
		maxHealth = maxHealth + maxHealth * multiplier; health = health + health * multiplier * 2;
		damage = damage + damage * multiplier;
		sound.level_up();
	}
}
