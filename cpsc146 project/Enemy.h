#pragma once
#include "Globals.h"
#include "PathFinder.h"
#include "Map.h"
#include "Bullet.h"
#include "Particle.h"

class Enemy
{
private:
	int type, radius, activationRadius, searchRadius, ignoreRadius;
	float x, y, handX, handY, gunOffX, gunOffY, prevX, prevY, velX, velY, dir, velPadding, targetX, targetY, accSpeed, accSpeedStart, maxVel;
	float health, startHealth;
	float damage;
	int xTile, yTile, knockback;
	bool following, active, tileCollidable;
	int imageSize;
	int frame, frameDir, minFrame, maxFrame, frameTimerStart, frameTimer, startFrame;
	int randomPathRadius, randomPathChance;
	int tileCollideChance;
	float slowness;
	float data;
	int effect, effectTime, timer;
	bool effectable;
	int invincibleTime;
	int image;

	POINT randomPath;
	PathFinder pathFinder;
	vector<PathFinder::PathPoint> path;

public:
	Enemy(Map map, int type_, float playerX, float playerY, bool randomPos);
	~Enemy();

	void Update(float playerX, float playerY, Map map, vector<Bullet> &bullets, vector<Particle> &particles);
	void Draw(ALLEGRO_BITMAP *enemyImages, ALLEGRO_DISPLAY *display, float cameraPos[2]);
	void DrawDebug();

	float GetX();
	float GetY();
	float GetVelX();
	float GetVelY();
	float GetPrevX();
	float GetPrevY();
	float GetDamage();
	float GetHealth();
	float GetStartHealth();
	float GetData();
	float GetDir();
	int GetXTile();
	int GetYTile();
	int GetRadius();
	int GetActivationRadius();
	int GetKnockback();
	int GetType();
	int GetFrame();
	int GetImage();
	int GetImageSize();
	bool GetActive();
	bool GetInvincible();
	bool GetTileCollidable();
	bool GetFollowing();
	PathFinder GetPathFinder();

	void SetPos(float x_, float y_);
	void AddVelocityAngle(float dir_, float speed_);
	void AddVelocity(float velX_, float velY_);
	void SetVelocityAngle(float dir_, float speed_);
	void SetVelocity(float velX_, float velY_);
	void SetHealth(float health_);
	void RemoveTile(int x, int y);
	void SetSlowness(float slowness_);
	void SetData(float data_);
	void SetEffect(int effect_, int effectTime_);
	void SetInvincibleTime(int invincibleTime_);
	void SetActive(bool active_);
	void Anger();
	void Damage(float damage);
};

