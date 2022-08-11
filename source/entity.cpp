#include "entity.h"

Entity::Entity(Image& image, Image& GunImage, float X, float Y, int W, int H, String Name)
{
	x = X; y = Y; w = W; h = H; name = Name;
	speed = 0; dx = 0; dy = 0;
	texture.loadFromImage(image);
	GunTexture.loadFromImage(GunImage);
	sprite.setTexture(texture);
	sprite.setOrigin(w / 2, h / 2);
	GunSprite.setTexture(GunTexture);
	life = true, collision = false, isShoot = false;
}

float Entity::getXPcost()
{
	return int(XP); //bruh
}

float Entity::getDamage()
{
	return damage;
}

float Entity::getCurrentHealth()
{
	return health;
}

float Entity::getMaxHealth()
{
	return maxHealth;
}

Sprite& Entity::getSprite()
{
	return sprite;
}

Sprite& Entity::getGunSprite()
{
	return GunSprite;
}

String Entity::getName()
{
	return name;
}

std::vector<Vector2f>& Entity::getVertex()
{
	return Vertexes;
}

void Entity::CollisionWithMap()
{
	pointsOfRect.clear();
	for (int j = 0; j < h; j += 25/*<-Accurcy. BIG IMPACT TO PERFOMANCE*/) {
		collisionPointX = ((w - j) / 2 * sin(angle) + h / 2 * cos(angle)); 
		collisionPointY = (-(w - j) / 2 * cos(angle) + h / 2 * sin(angle));
		pointsOfRect.push_back(getRect());
		collisionPointX = ((w - j) / 2 * sin(angle) - h / 2 * cos(angle));  
		collisionPointY = (-(w - j) / 2 * cos(angle) - h / 2 * sin(angle));
		pointsOfRect.push_back(getRect());
		collisionPointX = (w / 2 * sin(angle) + (h - 2 * j) / 2 * cos(angle));  
		collisionPointY = (-w / 2 * cos(angle) + (h - 2 * j) / 2 * sin(angle));
		pointsOfRect.push_back(getRect());
		collisionPointX = (-w / 2 * sin(angle) + (h - 2 * j) / 2 * cos(angle));	
		collisionPointY = (w / 2 * cos(angle) + (h - 2 * j) / 2 * sin(angle));
		pointsOfRect.push_back(getRect());
	}
	for (int i = 0; i < obj.size(); i++) {
		for (auto j = pointsOfRect.begin(); j != pointsOfRect.end(); j++)
			if (j->intersects(obj[i].rect)) { collision = true; break; }
		if(collision) break;
	}
	CollisionWithEntity();
}

void Entity::VertexesOfRect()
{
	Vertexes.clear();
	collisionPointX = (w / 2 * sin(angle) + h / 2 * cos(angle));
	collisionPointY = (-w / 2 * cos(angle) + h / 2 * sin(angle));
	TopLeftCorner = Vector2f(x + collisionPointX, y + collisionPointY); Vertexes.push_back(TopLeftCorner);
	collisionPointX = (w / 2 * sin(angle) - h / 2 * cos(angle));
	collisionPointY = (-w / 2 * cos(angle) - h / 2 * sin(angle));
	LowLeftCorner = Vector2f(x + collisionPointX, y + collisionPointY); Vertexes.push_back(LowLeftCorner);
	collisionPointX = (w / 2 * sin(angle) + h / 2 * cos(angle));
	collisionPointY = (-w / 2 * cos(angle) + h / 2 * sin(angle));
	LowRightCorner = Vector2f(x - collisionPointX, y - collisionPointY); Vertexes.push_back(LowRightCorner);
	collisionPointX = (-w / 2 * sin(angle) + h / 2 * cos(angle));
	collisionPointY = (w / 2 * cos(angle) + h / 2 * sin(angle));
	TopRightCorner = Vector2f(x + collisionPointX, y + collisionPointY); Vertexes.push_back(TopRightCorner);
}

FloatRect Entity::getRect()
{
	return FloatRect(x + collisionPointX, y + collisionPointY, 1, 1);
}