#include "sounds.h"

SoundClass::SoundClass(float volume)
{
	PlayershootBuffer.loadFromFile("Sounds/player_shoot.wav");	player_shoot.setBuffer(PlayershootBuffer);	player_shoot.setVolume(30 * volume);
	EnemyshootBuffer.loadFromFile("Sounds/enemy_shoot.wav");	enemy_shoot.setBuffer(EnemyshootBuffer);	enemy_shoot.setVolume(enemy_shoot.getVolume() * volume);
	
	ShellexpBuffer.loadFromFile("Sounds/shell_exp.wav");		shell_exp.setBuffer(ShellexpBuffer);		shell_exp.setVolume(30 * volume);				shell_exp.setAttenuation(50);
	FrontexpBuffer.loadFromFile("Sounds/shell_frontArmor_exp.wav");		front_exp.setBuffer(FrontexpBuffer);	front_exp.setVolume(35 * volume);
	SideexpBuffer.loadFromFile("Sounds/shell_sideArmor_exp.ogg");		side_exp.setBuffer(SideexpBuffer);		side_exp.setVolume(35 * volume);
	RearexpBuffer.loadFromFile("Sounds/shell_rearArmor_exp.ogg");		rear_exp.setBuffer(RearexpBuffer);		rear_exp.setVolume(35 * volume);

	XpBuffer.loadFromFile("Sounds/xp_pick.ogg");			xp.setBuffer(XpBuffer);							xp.setVolume(xp.getVolume() * volume);
	RepairBuffer.loadFromFile("Sounds/repair_pick.ogg");	repair.setBuffer(RepairBuffer);					repair.setVolume(repair.getVolume() * volume);
	LvlupBuffer.loadFromFile("Sounds/level_up.ogg");		lvl_up.setBuffer(LvlupBuffer);					lvl_up.setVolume(lvl_up.getVolume() * volume);

	TanktracksBuffer.loadFromFile("Sounds/player_moving.wav"); tank_tracks.setBuffer(TanktracksBuffer);		tank_tracks.setVolume(20 * volume);
	TankengineBuffer.loadFromFile("Sounds/tank_engine.wav");   tank_engine.setBuffer(TankengineBuffer);		tank_engine.setVolume(10 * volume);

	amb.openFromFile("Sounds/exp_ambient.ogg");				   amb.setLoop(true);							amb.setVolume(amb.getVolume() * volume);
}

void SoundClass::enemy_fire()
{
	enemy_shoot.play();
}

void SoundClass::player_fire()
{
	player_shoot.play();
}

void SoundClass::shell_explosion(Vector2f pos)
{
	shell_exp.setPosition(pos.x / 1000, pos.y / 1000, 0.0);
	shell_exp.play();
}

void SoundClass::shell_frontArmor_explosion()
{
	front_exp.play();
}

void SoundClass::shell_sideArmor_explosion()
{
	side_exp.play();
}

void SoundClass::shell_rearArmor_explosion()
{
	rear_exp.play();
}

void SoundClass::xp_pickup()
{
	xp.play();
}

void SoundClass::repair_pickup()
{
	repair.play();
}

void SoundClass::level_up()
{
	lvl_up.play();
}

void SoundClass::player_moving()
{
	if (tank_tracks.getStatus() == Sound::Status::Stopped)	tank_tracks.play();
}

void SoundClass::tank_engine_work()
{
	if (tank_engine.getStatus() == Sound::Status::Stopped)	tank_engine.play();
}

void SoundClass::ambient(bool isPlay)
{
	if (isPlay && amb.getStatus() == Sound::Status::Stopped) amb.play();
}