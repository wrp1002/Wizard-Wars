#pragma once
#include "Globals.h"
#include "Bullet.h"
#include "Map.h"

class Player {
private:
	struct Weapon {
		float energyUse;
		int shootTimerStart;
		int bulletType;
		int speed;
		int life;
		float knockback;
		int weaponID;
	};

	float startHealth, health;
	float x, y, handX, handY, prevX, prevY, velX, velY, maxVel, accSpeed, dir;
	int xTile, yTile;
	int radius, imageSize;
	int shootTimer, gunOffX, gunOffY;
	int frame, frameDir, maxFrame, frameTimerStart, frameTimer, startFrame;
	int invincibleTime, invincibleTimerStart, invincibilityVisible;
	int energy, energyStart, weapon, darkness, darknessTime;
	bool shooting, fire;
	vector<Weapon> weapons;
	ALLEGRO_BITMAP *image;

public:
	vector<Weapon> availableWeapons;

	Player();
	~Player();

	void Init(float x_, float y_);
	void InitWeapons();
	void AddWeapon(int weapon_);
	void Spawn(Map map);
	void Reset(Map map);
	void Update(bool keys[4], POINT mousePos, float cameraPos[], bool mouseDown, vector<Bullet> &bullets, vector<Particle> &particles);
	void Draw(ALLEGRO_DISPLAY *display);
	void DrawHealthBar(ALLEGRO_FONT *font);

	float GetX();
	float GetY();
	float GetVelX();
	float GetVelY();
	float GetPrevX();
	float GetPrevY();
	float GetHealth();
	float GetStartHealth();
	int GetXTile();
	int GetYTile();
	int GetRadius();
	int GetWeapon();
	int GetDarkness();
	int GetWeaponID();
	int GetWeaponAmount();
	bool GetInvincible();
	bool GetFire();

	void SetPos(float x_, float y_);
	void AddVelocityAngle(float dir_, float speed_);
	void AddVelocity(float velX_, float velY_);
	void SetVelocityAngle(float dir_, float speed_);
	void SetVelocity(float velX_, float velY_);
	void SetHealth(float health_);
	void Damage(float damage);
	void Invincible();
	void SetWeapon(int weapon_);
	void SetDarknessTime(int darknessTime_);
};

