#include "projectile.h"

Projectile::Projectile(Image& image, Level& lev, float Value, float GunAngle, float X, float Y, int W, int H, bool Team, String Name) :
	Entity(image, image, X, Y, W, H, Name)	
{
	damage = Value;
	sprite.setTextureRect(IntRect(0, 0, w, h));
	if (name == "shell") {
		obj = lev.GetObjects("wall");
		sprite.setRotation(GunAngle);
		angle = (GunAngle - 90) * RADS;
		speed = 0.7;
		team = Team;	//Team == 0 => player team //Team == 1 => enemy team
	}
	else if (name == "repair") {
		VertexesOfRect();
		sprite.setOrigin(w / 2, h / 2);
		sprite.setPosition(x, y);
		sprite.setScale(0.65, 0.65);
	}
	else if (name == "xp"){
		VertexesOfRect();
		sprite.setOrigin(w / 2, h / 2);
		sprite.setPosition(x, y);
		sprite.setScale(0.4, 0.4);
		XP = Value;
	}
}

void Projectile::CollisionWithEntity()
{
	if (collision) { life = false; }
}

void Projectile::CollisionWithArmor(Vector2f P1, Vector2f P2, float damage, SoundClass& sound)
{
}

void Projectile::GunControl(Vector2f v)
{
}

void Projectile::update(float time)
{
	if (name == "shell") {
		dx = speed * cos(angle); dy = speed * sin(angle);
		x += dx * time; y += dy * time;
		sprite.setOrigin(w / 2, h / 2);
		sprite.setPosition(x, y);
		CollisionWithMap();
		VertexesOfRect();
	}

	else if (name == "repair" || name == "xp") {
		currTime += time;
		if (currTime >= timeToLive)	life = false;
	}
}