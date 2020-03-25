#pragma once
#include "Globals.h"
#include "Particle.h"
#include "Map.h"

class Enemy;

//void Collide(Map &map, Player &player, vector<Enemy> &enemies, vector<Bullet> &playerBullets, vector<Bullet> &enemyBullets, vector<Particle> &particles);

class Bullet
{
private:
	int xTile, yTile;
	float x, y, dir, speed, knockback, damage, data;
	int type, radius, life, image, imageSize;
	int frame, frameTimerStart, frameTimer;
	bool active, tileCollidable, entityCollidable, dodgable, dieFromEntity;
	int effect, effectTime;
	vector<POINT> pathPoints;

public:
	Bullet(float x_, float y_, float dir_, float speed_, int life_, int type_);
	~Bullet();

	void Update(Map map, vector<Particle> &particles, vector<Enemy> &enemies);
	void Draw(ALLEGRO_BITMAP *bulletImages, ALLEGRO_DISPLAY *display, float cameraPos[2]);
	void DrawDebug();

	float GetX();
	float GetY();
	float GetDir();
	float GetDamage();
	float GetSpeed();
	float GetData();
	int GetXTile();
	int GetYTile();
	int GetType();
	int GetRadius();
	int GetKnockback();
	int GetEffect();
	int GetEffectTime();
	int GetFrame();
	bool GetActive();
	bool GetTileCollidable();
	bool GetEntityCollidable();
	bool GetDodgable();
	bool GetDieFromEntity();

	void Kill(vector<Particle> &particles);
	void SetData(float data_);
};

