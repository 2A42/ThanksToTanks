#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <math.h>

#include "common.h"
#include "entity.h"
#include "player.h"
#include "enemy.h"
#include "projectile.h"
#include "effects.h"
#include "sounds.h"
#include "vehicles.h"
#include "gui.h"
#include "level.h"
#include "math++.h"
#include "camera.h"
#include "TinyXML/tinyxml.h"

using namespace sf;

void CollisionBetweenEntitiesAndPlayer(Player& PLAYER, std::list<Entity*>& entities, SoundClass& sound) {
	//////////////////////////////////////////WARNING! BAD CODE HERE///////////////////////////////////////////////////
	std::list<Entity*>::iterator it;
	std::list<Entity*>::iterator it2;
	for (it = entities.begin(); it != entities.end(); it++) {
		auto i = (*it)->getVertex().begin();
		auto j = PLAYER.getVertex().begin();
		////////////////////////////////////////Collision between player & enemy/////////////////////////////////////////////////////
		for (; i != (*it)->getVertex().end(); i++)
			for (; j != PLAYER.getVertex().end(); j++)
				if ((*it)->getName() != "repair" && (((i + 1) != (*it)->getVertex().end() && math::isPointAtLine(*i, *(i + 1), *j)) ||
					math::isPointAtLine((*it)->getVertex().back(), (*it)->getVertex().front(), *j))) {
					if ((*it)->getName() == "shell") {	//if entity is enemy shell
						if ((*it)->team == 1) {
							(*it)->collision = true; (*it)->CollisionWithEntity();
						}
					}
					else if ((*it)->getName() == "xp") {
						(*it)->collision = true; (*it)->CollisionWithEntity();
						PLAYER.playerXP += (*it)->getXPcost();
						sound.xp_pickup();
					}
					else {
						(*it)->collision = true; (*it)->CollisionWithEntity();
						PLAYER.collision = true; PLAYER.CollisionWithEntity();
					}
					return;
				}

		for (i = PLAYER.getVertex().begin(); i != PLAYER.getVertex().end(); i++)
			for (j = (*it)->getVertex().begin(); j != (*it)->getVertex().end(); j++)
				if ((*it)->getName() != "xp" && (((i + 1) != PLAYER.getVertex().end() && math::isPointAtLine(*i, *(i + 1), *j)) ||
					math::isPointAtLine(PLAYER.getVertex().front(), PLAYER.getVertex().back(), *j) ||
					(abs((*it)->getSprite().getPosition().x - PLAYER.getSprite().getPosition().x) <= 10) &&
					(abs((*it)->getSprite().getPosition().y - PLAYER.getSprite().getPosition().y) <= 10))) {
					if ((*it)->getName() == "shell") {
						if ((*it)->team == 1) {
							(*it)->collision = true; (*it)->CollisionWithEntity();
							if(math::isPointAtLine(PLAYER.getVertex().front(), PLAYER.getVertex().back(), *j)) PLAYER.CollisionWithArmor(PLAYER.getVertex().front(), PLAYER.getVertex().back(), (*it)->getDamage(), sound);
							else if (abs((*it)->getSprite().getPosition().x - PLAYER.getSprite().getPosition().x) <= 10) PLAYER.CollisionWithArmor(PLAYER.getVertex()[1], PLAYER.getVertex()[2], (*it)->getDamage(), sound);
							else PLAYER.CollisionWithArmor(*i, *(i + 1), (*it)->getDamage(), sound);
						}
					}
					else if ((*it)->getName() == "repair") {
						if (PLAYER.getCurrentHealth() - (*it)->getDamage() <= PLAYER.getMaxHealth()) {
							(*it)->collision = true; (*it)->CollisionWithEntity();
							PLAYER.CollisionWithArmor(*i, *(i + 1), (*it)->getDamage(), sound);
						}
					}
					else {
						(*it)->collision = true; (*it)->CollisionWithEntity();
						PLAYER.collision = true; PLAYER.CollisionWithEntity();
					}
					return;
				}
		//////////////////////////////////////////Collision between enemies/////////////////////////////////////////////////////
		for (it2 = entities.begin(); it2 != entities.end(); it2++) {
			if ((*it)->getVertex() != (*it2)->getVertex())	//not the same entity
			{
				i = (*it)->getVertex().begin();
				j = (*it2)->getVertex().begin();
				for (; i != (*it)->getVertex().end(); i++)
					for (; j != (*it2)->getVertex().end(); j++)
						if ((*it2)->getName() != "xp" && (*it)->getName() != "xp" && (*it)->getName() != "repair" && (((i + 1) != (*it)->getVertex().end() && math::isPointAtLine(*i, *(i + 1), *j)) ||
							math::isPointAtLine((*it)->getVertex().front(), (*it)->getVertex().back(), *j) ||
							(abs((*it)->getSprite().getPosition().x - (*it2)->getSprite().getPosition().x) <= 10) &&
							(abs((*it)->getSprite().getPosition().y - (*it2)->getSprite().getPosition().y) <= 10))) {
							if ((*it2)->getName() == "shell") {
								if ((*it2)->team == 0) {
									if (math::isPointAtLine((*it)->getVertex().front(), (*it)->getVertex().back(), *j)) (*it)->CollisionWithArmor((*it)->getVertex().front(), (*it)->getVertex().back(), (*it2)->getDamage(), sound);
									else if (abs((*it)->getSprite().getPosition().x - (*it2)->getSprite().getPosition().x) <= 10) (*it)->CollisionWithArmor((*it)->getVertex()[1], (*it)->getVertex()[2], (*it2)->getDamage(), sound);
									else (*it)->CollisionWithArmor(*i, *(i + 1), (*it2)->getDamage(), sound);
									(*it2)->collision = true; (*it2)->CollisionWithEntity();
								}
							}
							else if ((*it2)->getName() == "repair") {
								if ((*it)->getCurrentHealth() - (*it2)->getDamage() <= (*it)->getMaxHealth()) {
									(*it2)->collision = true; (*it2)->CollisionWithEntity();
									(*it)->CollisionWithArmor(*i, *(i + 1), (*it2)->getDamage(), sound);
								}
							}
							else {
								(*it)->collision = true; (*it)->CollisionWithEntity();
								(*it2)->collision = true; (*it2)->CollisionWithEntity();
							}
							return;
						}
			}
		}
	}
		//////////////////////////////////////////			///////////////////////////////////////////////////
}

bool isGameStart() {
	/////////////////////////////Vehicles initialization/////////////////////////////
	std::list<Entity*>  entities;
	Vehicle PlayerVehicle; Image PlayerGun, PlayerBody;
	Vehicle Panzer4("Panzer4"); Image Panzer4Gun, Panzer4Body;
	Vehicle Tiger1("Tiger1"); Image Tiger1Gun, Tiger1Body;
	Vehicle Panther("Panther"); Image PantherGun, PantherBody;
	Vehicle Tiger2("Tiger2"); Image Tiger2Gun, Tiger2Body;
	Vehicle Greyhound("Greyhound"); Image GreyhoundGun, GreyhoundBody;
	Vehicle Valentine("Valentine"); Image ValentineGun, ValentineBody;
	Vehicle Firefly("Firefly"); Image FireflyGun, FireflyBody;
	Vehicle Churchil("Churchil"); Image ChurchilGun, ChurchilBody;
	Panzer4Gun.loadFromFile("Images/vehicles/panzer4_gun.png");
	Panzer4Body.loadFromFile("Images/vehicles/panzer4_body.png");
	Tiger1Gun.loadFromFile("Images/vehicles/tiger1_gun.png");
	Tiger1Body.loadFromFile("Images/vehicles/tiger1_body.png");
	PantherGun.loadFromFile("Images/vehicles/panther_gun.png");
	PantherBody.loadFromFile("Images/vehicles/panther_body.png");
	Tiger2Gun.loadFromFile("Images/vehicles/tiger2_gun.png");
	Tiger2Body.loadFromFile("Images/vehicles/tiger2_body.png");
	GreyhoundGun.loadFromFile("Images/vehicles/greyhound_gun.png");
	GreyhoundBody.loadFromFile("Images/vehicles/greyhound_body.png");
	ValentineGun.loadFromFile("Images/vehicles/valentine_gun.png");
	ValentineBody.loadFromFile("Images/vehicles/valentine_body.png");
	FireflyGun.loadFromFile("Images/vehicles/sherman_gun.png");
	FireflyBody.loadFromFile("Images/vehicles/sherman_body.png");
	ChurchilGun.loadFromFile("Images/vehicles/churchil_gun.png");
	ChurchilBody.loadFromFile("Images/vehicles/churchil_body.png");
	/////////////////////////////Effects & Projectiles initialization/////////////////////////////
	std::list<Effect*>  effects;
	Image Projectile_image, Repair_image, Xp_image, SmokeShell_image, 
	ExplosionShell_image, ExplosionTank_image, MuzzleFlash_image, Smoke_image;
	Projectile_image.loadFromFile("Images/AP_shell.png");
	Repair_image.loadFromFile("Images/repair.png");
	Xp_image.loadFromFile("Images/xp.png");
	SmokeShell_image.loadFromFile("Images/effects/smoke_trail.png");
	ExplosionShell_image.loadFromFile("Images/effects/shell_exp.png");
	ExplosionTank_image.loadFromFile("Images/effects/tank_exp.png");
	MuzzleFlash_image.loadFromFile("Images/effects/muzzle_flash.png");
	Smoke_image.loadFromFile("Images/effects/Smoke.png");
	/////////////////////////////difficulty params////////////////////////////////
	int maxShootDist;
	int accurcy;
	int PLAYERlifesCount;
	int vehicle;
	float XPmultiplier; // >= 1
	/////////////////////////////Level initialization/////////////////////////////
	Level lvl;
	lvl.LoadFromFile("maps/map.tmx");
	Object playerFromLvl = lvl.GetObject("Player");
	std::vector<Object> enemiesFromLvl;
	int wave = 1;
	//////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////Loading Settings/////////////////////////////////////
	std::fstream fileBuffer;
	int W, H;	float volume;	bool isInGame = true;
	fileBuffer.open("settings.txt", std::ios::in);
	fileBuffer >> W;	fileBuffer >> H;	fileBuffer >> volume;	fileBuffer.close();
	RenderWindow window(VideoMode(W, H), "Game", Style::None);	camera.reset(FloatRect(0, 0, W, H));	window.setView(camera);

	Gui GUI(window);
	GUI.menu(W, H, volume, XPmultiplier, maxShootDist, accurcy, PLAYERlifesCount, vehicle, isInGame);	camera.reset(FloatRect(0, 0, W, H));
	window.setMouseCursorVisible(false);	window.setFramerateLimit(100);
	if (vehicle == 1) { PlayerVehicle = Panzer4; PlayerBody = Panzer4Body; PlayerGun = Panzer4Gun; }
	else if (vehicle == 2) { PlayerVehicle = Tiger1; PlayerBody = Tiger1Body; PlayerGun = Tiger1Gun; }
	else if (vehicle == 3) { PlayerVehicle = Panther; PlayerBody = PantherBody; PlayerGun = PantherGun; }
	else if (vehicle == 4) { PlayerVehicle = Tiger2; PlayerBody = Tiger2Body; PlayerGun = Tiger2Gun; }
	Player PLAYER(PlayerVehicle, PlayerBody, PlayerBody.getSize().x, PlayerBody.getSize().y, PlayerGun, PlayerGun.getSize().x, PlayerGun.getSize().y, lvl, playerFromLvl.rect.left, playerFromLvl.rect.top, "Player1");

	SoundClass sounds(volume/100);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	Clock clock, clock2;
	int timeForLastWave = 60; float timeAfterPause = 0, gameSpeed = 0.0025;  // 0.0025 is default value

	Event event;
	while (window.isOpen() && isInGame)
	{
		float time = clock.getElapsedTime().asMicroseconds();
		float gameTime = clock2.getElapsedTime().asSeconds() - timeAfterPause;
		if (Keyboard::isKeyPressed(Keyboard::Escape))	if (GUI.pause(timeAfterPause)) return GUI.endScreen(gameTime);
		gameTime = clock2.getElapsedTime().asSeconds() - timeAfterPause;

		clock.restart();
		time = time * gameSpeed;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		sounds.ambient(true);

		///////////////////////////////////////////map Gameplay////////////////////////////////////////////////////
		switch (int(gameTime)) {
			int enemiesCount;
		case 5:	//wave 1
			if (wave == 1) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 20:	//wave 2
			if (wave == 2) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 40:	//...
			if (wave == 3) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 80:	//...
			if (wave == 4) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 85:	//...
			if (wave == 5) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Firefly"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(FIREFLY, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 120:	//...
			if (wave == 6) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Firefly"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(FIREFLY, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 180:	//...
			if (wave == 7) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 200:	//...
			if (wave == 8) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Churchil"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(CHURCHIL, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 245:	//...
			if (wave == 9) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Churchil"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(CHURCHIL, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Firefly"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(FIREFLY, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 360:	//...
			if (wave == 10) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 375:	//...
			if (wave == 11) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Firefly"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(FIREFLY, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 400:	//...
			if (wave == 12) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Churchil"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(CHURCHIL, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 450:	//...
			if (wave == 13) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Firefly"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(FIREFLY, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 530:	//...
			if (wave == 14) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 600:	//...
			if (wave == 15) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Churchil"); enemiesCount = 1;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(CHURCHIL, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Firefly"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(FIREFLY, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 4;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		case 715:	//...
			if (wave == 16) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Churchil"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(CHURCHIL, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Firefly"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(FIREFLY, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
			}
			break;
		default:
			if (wave > 16 && int(gameTime) - 715 > timeForLastWave) {
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Churchil"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(CHURCHIL, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Firefly"); enemiesCount = 4;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(FIREFLY, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Valentine"); enemiesCount = 3;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(VALENTINE, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				enemiesFromLvl.clear(); enemiesFromLvl = lvl.GetObjects("Greyhound"); enemiesCount = 2;
				for (int i = 0; i < enemiesCount; i++)
					entities.push_back(new Enemy(GREYHOUND, lvl, enemiesFromLvl[i].rect.left, enemiesFromLvl[i].rect.top, "enemy1"));
				wave++;
				timeForLastWave += 100;
			}
			break;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////

		Vector2i MousePixelPos = Mouse::getPosition(window);
		Vector2f MousePos = window.mapPixelToCoords(MousePixelPos);
		///////////////////////////////////////////Player/////////////////////////////////////////////////
		if (PLAYER.life == true && PLAYER.getCurrentHealth() > 0) PLAYER.Control(sounds);
		PLAYER.update(time);		
		PLAYER.LVLprogression(sounds);
		if (PLAYER.life == true && PLAYER.getCurrentHealth() > 0) {
			PLAYER.GunControl(MousePos);
			if (PLAYER.isShoot == true) {
				PLAYER.isShoot = false;
				entities.push_back(new Projectile(Projectile_image, lvl, PLAYER.getDamage(), PLAYER.getGunSprite().getRotation(), PLAYER.getSprite().getPosition().x, PLAYER.getSprite().getPosition().y, 7, 21, 0, "shell"));
				effects.push_back(new Effect(Smoke_image, PLAYER.getGunSprite().getRotation(), math::getTop(PLAYER.getGunSprite()).x, math::getTop(PLAYER.getGunSprite()).y, 167, 140, 12, 1, "smoke"));
				effects.push_back(new Effect(MuzzleFlash_image, PLAYER.getGunSprite().getRotation(), math::getTop(PLAYER.getGunSprite()).x, math::getTop(PLAYER.getGunSprite()).y, 300, 300, 2, 1, "muzzle_flash"));
				sounds.player_fire();
			}

			sounds.tank_engine_work();

			CollisionBetweenEntitiesAndPlayer(PLAYER, entities, sounds);	//////////////Main collision func

			if (PLAYER.getCurrentHealth() <= 0) { PLAYERlifesCount--; PLAYER.life = false; PLAYER.getSprite().setColor(Color::Black); PLAYER.getGunSprite().setColor(Color::Black); effects.push_back(new Effect(ExplosionTank_image, PLAYER.getSprite().getRotation(), PLAYER.getSprite().getPosition().x, PLAYER.getSprite().getPosition().y, 256, 256, 8, 8, "tank_exp")); }
			if (PLAYERlifesCount < 1) { GUI.drawString("GAME OVER", camera.getCenter(), 35, Color::Black); window.display(); Time t(sf::seconds(1.75)); sleep(t); return GUI.endScreen(gameTime); }
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////Entities///////////////////////////////////////////////////
		for (auto it = entities.begin(); it != entities.end();) {
			Entity* ent = *it;
			ent->update(time);

			if (ent->getName() == "shell") {
				effects.push_back(new Effect(SmokeShell_image, (*it)->getSprite().getRotation(), (*it)->getSprite().getPosition().x, (*it)->getSprite().getPosition().y, 120, 120, 4, 1, "shell_trail"));
			}
			else {
				ent->GunControl(PLAYER.getSprite().getPosition());
				if (ent->isShoot == true && math::getDistance(ent->getSprite().getPosition(), PLAYER.getSprite().getPosition()) <= maxShootDist) {
					ent->isShoot = false;
					entities.push_back(new Projectile(Projectile_image, lvl, (*it)->getDamage(), (*it)->getGunSprite().getRotation() + rand() % accurcy - rand() % accurcy, (*it)->getGunSprite().getPosition().x, (*it)->getGunSprite().getPosition().y, 7, 21, 1, "shell"));
					effects.push_back(new Effect(Smoke_image, (*it)->getGunSprite().getRotation(), math::getTop((*it)->getGunSprite()).x, math::getTop((*it)->getGunSprite()).y, 167, 140, 12, 1, "smoke"));
					effects.push_back(new Effect(MuzzleFlash_image, (*it)->getGunSprite().getRotation(), math::getTop((*it)->getGunSprite()).x, math::getTop((*it)->getGunSprite()).y, 300, 300, 2, 1, "muzzle_flash"));
					sounds.enemy_fire();
				}
				else ent->isShoot = false;
			}

			if (ent->life == false ||
				abs(ent->getSprite().getPosition().x) > MAP_BOUNDS_X ||
				abs(ent->getSprite().getPosition().y) > MAP_BOUNDS_Y)
			{
				if ((*it)->getName() == "shell") {
					sounds.shell_explosion(Vector2f(abs((*it)->getSprite().getPosition().x - PLAYER.getSprite().getPosition().x), abs((*it)->getSprite().getPosition().y - PLAYER.getSprite().getPosition().y)) );
					effects.push_back(new Effect(ExplosionShell_image, (*it)->getSprite().getRotation(), (*it)->getSprite().getPosition().x, (*it)->getSprite().getPosition().y, 256, 254, 8, 6, "shell_exp"));
				}
				else if((*it)->getName() == "enemy1"){
					effects.push_back(new Effect(ExplosionTank_image, (*it)->getSprite().getRotation(), (*it)->getSprite().getPosition().x, (*it)->getSprite().getPosition().y, 256, 256, 8, 8, "tank_exp")); 
					for (int i = 0; i < round((*it)->getMaxHealth() / 100 / 4); i++) {
						entities.push_back(new Projectile(Repair_image, lvl, -200, 0, (*it)->getSprite().getPosition().x + rand() % 80, (*it)->getSprite().getPosition().y + rand() % 80, Repair_image.getSize().x, Repair_image.getSize().y, 2, "repair"));
						entities.push_back(new Projectile(Xp_image, lvl, (*it)->getXPcost() * XPmultiplier, 0, (*it)->getSprite().getPosition().x - rand() % 80, (*it)->getSprite().getPosition().y - rand() % 80, Xp_image.getSize().x, Xp_image.getSize().y, 2, "xp"));
					}
				}
				it = entities.erase(it); delete ent;
			}
			else it++;
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////Effects///////////////////////////////////////////////////////////////
		for (auto it = effects.begin(); it != effects.end();) {
			Effect* ef = *it;
			if (ef->isOutOfFrames) { it = effects.erase(it); delete ef; }
			else
			{
				ef->update();
				it++;
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////Camera & Drawing///////////////////////////////////////////////////
		setPlayerCoordinateForCamera(PLAYER.getSprite().getPosition().x, PLAYER.getSprite().getPosition().y);
		window.setView(camera);

		window.clear(Color(205, 147, 93));

		lvl.Draw(window);
		window.draw(PLAYER.getSprite());
		window.draw(PLAYER.getGunSprite());
		for (auto it = entities.begin(); it != entities.end(); it++) { window.draw((*it)->getSprite()); if ((*it)->getName() == "enemy1") { window.draw((*it)->getGunSprite()); GUI.drawHealth((*it)->getMaxHealth(), (*it)->getCurrentHealth(), (*it)->getSprite().getPosition(), (*it)->getName()); } }
		for (auto ef = effects.begin(); ef != effects.end(); ef++) window.draw((*ef)->getSprite());

		GUI.drawHealth(PLAYER.getMaxHealth(), PLAYER.getCurrentHealth(), PLAYER.getSprite().getPosition(), PLAYER.getName());
		GUI.drawString(std::to_string(gameTime), Vector2f(camera.getCenter().x, camera.getCenter().y - camera.getSize().y / 2), 30, Color::White);
		GUI.drawString("Lifes:  " + std::to_string(PLAYERlifesCount), Vector2f(camera.getCenter().x - 40, camera.getCenter().y - 120), 20, Color::Red);
		GUI.drawString("Lvl   " + std::to_string(PLAYER.playerLVL), Vector2f(camera.getCenter().x - 20, camera.getCenter().y - 140), 15, Color::Yellow);
		GUI.drawCursor(MousePos);

		window.display();
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	return false;
}

void gameRunning(){
	if (isGameStart()) gameRunning();
}

int main()
{
	gameRunning();
	return 0;
}

