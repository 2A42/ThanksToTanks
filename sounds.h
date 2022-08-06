#pragma once

#include <SFML/Audio.hpp>
#include "common.h"

class SoundClass {
public:
	SoundClass(float);

	void enemy_fire();

	void player_fire();

	void shell_explosion(Vector2f);

	void shell_frontArmor_explosion();

	void shell_sideArmor_explosion();

	void shell_rearArmor_explosion();

	void xp_pickup();

	void repair_pickup();

	void level_up();

	void player_moving();

	void tank_engine_work();

	void ambient(bool);

private:
	Music amb;
	Sound player_shoot, enemy_shoot, shell_exp, front_exp, side_exp, rear_exp,
		xp, repair, lvl_up, tank_engine, tank_tracks;
	SoundBuffer PlayershootBuffer, EnemyshootBuffer, ShellexpBuffer, FrontexpBuffer, SideexpBuffer, RearexpBuffer,
		XpBuffer, RepairBuffer, LvlupBuffer, TankengineBuffer, TanktracksBuffer;
};