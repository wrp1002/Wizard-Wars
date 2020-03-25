#include "Player.h"

Player::Player() {
	startHealth = 0;
	health = 0;
	x = 0;
	y = 0;
	prevX = 0;
	prevY = 0;
	velX = 0;
	velY = 0;
	maxVel = 0;
	accSpeed = 0;
	dir = 0;
	xTile = 0, yTile = 0;
	radius = 0;
	shootTimer = 0;
	imageSize = 0;
	frame = 0;
	maxFrame = 0;
	frameDir = 0;
	frameTimerStart = 0;
	frameTimer = 0;
	startFrame = 0;
	gunOffX = 0;
	gunOffY = 0;
	invincibleTime = 0;
	invincibleTimerStart = 0;
	energy = 0;
	energyStart = 0;
	shooting = false;
}

Player::~Player() {
	//al_destroy_bitmap(image);
}

void Player::Init(float x_, float y_) {
	cout << "Initializing Player..." << endl;

	InitWeapons();

	x = x_;
	y = y_;
	handX = 0;
	handY = 0;
	prevX = x;
	prevY = y;
	xTile = x / TILE_SIZE;
	yTile = y / TILE_SIZE;
	velX = 0;
	velY = 0;
	maxVel = 15;
	accSpeed = .3;
	radius = 16;
	startHealth = 10;
	health = startHealth;
	startFrame = 3;
	frame = startFrame;
	frameDir = 1;
	maxFrame = 5;
	frameTimerStart = 5;
	frameTimer = frameTimerStart;
	gunOffX = 14;
	gunOffY = 10;
	invincibleTime = 0;
	invincibleTimerStart = 50;
	invincibilityVisible = 0;
	energyStart = 1000;
	energy = energyStart;
	darkness = 0;
	darknessTime = 0;
	weapon = 0;
	shootTimer = weapons[weapon].shootTimerStart;

	image = al_load_bitmap("Resources/Images/wizard sprite sheet.png");
	imageSize = al_get_bitmap_height(image);
	AddWeapon(0);
}

void Player::InitWeapons() {
	weapons.clear();
	Weapon tempWeapon;
	tempWeapon.energyUse = 50;
	tempWeapon.shootTimerStart = 20;
	tempWeapon.bulletType = 0;
	tempWeapon.speed = 10;
	tempWeapon.life = 50;
	tempWeapon.knockback = 3;
	tempWeapon.weaponID = 0;
	weapons.push_back(tempWeapon);

	tempWeapon.energyUse = 75;
	tempWeapon.shootTimerStart = 5;
	tempWeapon.bulletType = 4;
	tempWeapon.speed = 8;
	tempWeapon.life = 20;
	tempWeapon.knockback = 0;
	tempWeapon.weaponID = 1;
	weapons.push_back(tempWeapon);

	tempWeapon.energyUse = 700;
	tempWeapon.shootTimerStart = 100;
	tempWeapon.bulletType = 8;
	tempWeapon.speed = 10;
	tempWeapon.life = -1;
	tempWeapon.knockback = 8;
	tempWeapon.weaponID = 2;
	weapons.push_back(tempWeapon);

	tempWeapon.energyUse = 30;
	tempWeapon.shootTimerStart = 1;
	tempWeapon.bulletType = -1;
	tempWeapon.speed = 0;
	tempWeapon.life = 0;
	tempWeapon.knockback = 0;
	tempWeapon.weaponID = 3;
	weapons.push_back(tempWeapon);

	tempWeapon.energyUse = 10;
	tempWeapon.shootTimerStart = 1;
	tempWeapon.bulletType = -1;
	tempWeapon.speed = 8;
	tempWeapon.life = 20;
	tempWeapon.knockback = 0;
	tempWeapon.weaponID = 4;
	weapons.push_back(tempWeapon);
}

void Player::AddWeapon(int weapon_) {
	if (weapons.size() > 0) {
		if (weapon_ == -1) {
			int type = rand() % weapons.size();
			availableWeapons.push_back(weapons[type]);
			cout << "Added Weapon " << type << endl;
			weapons.erase(weapons.begin() + type);
		}
		else {
			availableWeapons.push_back(weapons[weapon_]);
			cout << "Added Weapon " << weapon_ << endl;
			weapons.erase(weapons.begin() + weapon_);
		}
	}
}

void Player::Spawn(Map map) {
	bool good = false;
	while (!good) {
		int tryX = rand() % map.GetMapWidth();
		int tryY = rand() % map.GetMapHeight();
		if (!map.GetMap()[tryY][tryX].solid) {
			good = true;
			x = tryX * TILE_SIZE + TILE_SIZE / 2;
			y = tryY * TILE_SIZE + TILE_SIZE / 2;
		}
	}
}

void Player::Reset(Map map) {
	Spawn(map);
	health = startHealth;
	energy = energyStart;
	invincibleTime = 0;
	frame = startFrame;
	darkness = 0;
	darknessTime = 0;
	weapon = 0;
	InitWeapons();
	availableWeapons.clear();
	AddWeapon(0);
}

void Player::Update(bool keys[4], POINT mousePos, float cameraPos[], bool mouseDown, vector<Bullet> &bullets, vector<Particle> &particles) {
	if (health > 0 && (velX != 0 || velY != 0 || (frame != startFrame && x == prevX && y == prevY))) {
		frameTimer--;
		if (frameTimer <= 0) {
			frameTimer = frameTimerStart;
			if (keys[RIGHT] + keys[LEFT] + keys[UP] + keys[DOWN] > 0 || frame != startFrame) {
				frame += frameDir;
				if (frame == maxFrame || frame == 1)
					frameDir *= -1;
			}
		}
	}

	if (health == 0) {
		if (energy > 0) {
			energy -= 10;
			if (energy < 0)
				energy = 0;
			for (int i = 0; i < 5; i++)
				bullets.push_back(Bullet(x, y, rand() % 360 * (M_PI / 180), rand() % 5 + 5, rand() % 30 + 10, rand() % 2 == 0 ? 0 : 3));
			bullets.push_back(Bullet(x + rand() % 101 - 50, y + rand() % 101 - 50, rand() % 360 * (M_PI / 180), rand() % 5 + 5, -1, 2));
		}
		velX = 0;
		velY = 0;
	}

	prevX = x;
	prevY = y;

	handX = x + (gunOffX * cos(dir)) - (gunOffY * sin(dir));
	handY = y + (gunOffY * cos(dir)) + (gunOffX * sin(dir));

	if (GAME_STATE != 2) {
		velX += keys[RIGHT] * accSpeed;
		velX -= keys[LEFT] * accSpeed;
		velY += keys[DOWN] * accSpeed;
		velY -= keys[UP] * accSpeed;
		dir = GetAngle(mousePos.x + cameraPos[0], mousePos.y + cameraPos[1], x, y);

		//cout << "Weapon:" << weapon << " weaponID:" << availableWeapons[weapon].weaponID << endl;

		shooting = false;
		fire = false;
		if (mouseDown && (availableWeapons[weapon].weaponID != 4 || availableWeapons[weapon].weaponID == 4 && health + .035 < startHealth)) {
			if (shootTimer <= 0 && energy > availableWeapons[weapon].energyUse) {
				shooting = true;
				shootTimer = availableWeapons[weapon].shootTimerStart;
				if (availableWeapons[weapon].weaponID != 4 || availableWeapons[weapon].weaponID == 4 && health + .035 < startHealth)
					energy -= availableWeapons[weapon].energyUse;

				if (availableWeapons[weapon].weaponID == 0) {
					for (int i = 0; i < 10; i++)
						particles.push_back(Particle(handX, handY, 0, 6, rand() % 10 + 10, NULL, NULL, NULL, NULL, NULL, 0, 0));
					bullets.push_back(Bullet(handX, handY, dir, availableWeapons[weapon].speed, availableWeapons[weapon].life, availableWeapons[weapon].bulletType));
				}
				else if (availableWeapons[weapon].weaponID == 1)
					for (int i = 0; i < 3; i++)
						bullets.push_back(Bullet(handX, handY, dir + (rand() % 501 - 250) * .001, availableWeapons[weapon].speed, rand() % availableWeapons[weapon].life + availableWeapons[weapon].life / 2, availableWeapons[weapon].bulletType));
				else if (availableWeapons[weapon].weaponID == 4) {
					if (health + .035 < startHealth) {
						health += .035;
						if (rand() % 3 == 0)
							particles.push_back(Particle(x + rand() % (imageSize)-imageSize / 2, y + rand() % (imageSize)-imageSize / 2, 2, rand() % 3000 * .001, rand() % 20 + 10, NULL, NULL, NULL, NULL, NULL, 0, 0));
					}
				}
				else if (availableWeapons[weapon].weaponID == 3) {
					fire = true;
					AddVelocityAngle(dir, 2);
					invincibleTime = 10;
					for (int i = 0; i < 5; i++) {
						particles.push_back(Particle(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, 0, 6, rand() % 20 + 10, NULL, 4, 0, 0, 0, 0, 0));
						bullets.push_back(Bullet(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, rand() % 360 * (M_PI / 180), rand() % 3000 * .001 + 2, rand() % 20 + 5, 3));
					}
				}
				else if (availableWeapons[weapon].weaponID == 2)
					bullets.push_back(Bullet(handX, handY, dir, availableWeapons[weapon].speed, availableWeapons[weapon].life, availableWeapons[weapon].bulletType));
				AddVelocityAngle(dir + M_PI, availableWeapons[weapon].knockback);
			}
		}
		else if (energy < energyStart) {
			energy += 3;
			if (energy > energyStart)
				energy = energyStart;
		}
	}

	if (abs(velX) <= .3) velX = 0;
	if (abs(velY) <= .3) velY = 0;

	if (velX > maxVel) velX = maxVel;
	else if (velX < -maxVel) velX = -maxVel;
	if (velY > maxVel) velY = maxVel;
	else if (velY < -maxVel) velY = -maxVel;

	x += velX;
	y += velY;

	xTile = x / TILE_SIZE;
	yTile = y / TILE_SIZE;

	velX *= .9;
	velY *= .9;

	if (shootTimer > 0)
		shootTimer--;

	if (health <= 0)
		health = 0;
	else if (shootTimer == 0) {
		if (availableWeapons[weapon].weaponID == 0) {
			particles.push_back(Particle(handX, handY, 0, 6, rand() % 10, NULL, NULL, NULL, NULL, NULL, 0, 0));
		}
		else if (availableWeapons[weapon].weaponID == 1) {
			if (!mouseDown)
				particles.push_back(Particle(handX, handY, 1, 6, rand() % 5, NULL, NULL, NULL, NULL, NULL, 0, 0));
		}
		else if (availableWeapons[weapon].weaponID == 4) {
			particles.push_back(Particle(handX, handY, 3, rand() % 5000 * .001, rand() % 5 + 3, NULL, rand() % 2 + 1, 255, 0, 0, 0, 0));
		}
		else if (availableWeapons[weapon].weaponID == 3) {
			particles.push_back(Particle(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, 0, 6, rand() % 20 + 10, NULL, 4, 0, 0, 0, 0, 0));
		if (rand() % 3 == 0)
			bullets.push_back(Bullet(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, rand() % 360 * (M_PI / 180), rand() % 5000 * .0001, rand() % 10 + 1, 3));
		}
		else if (availableWeapons[weapon].weaponID == 2) {
			float tempDir = rand() % 360 * (M_PI / 180);
			float tempSpeed = 15;
			float tempX = handX + tempSpeed * cos(tempDir);
			float tempY = handY + tempSpeed * sin(tempDir);
			particles.push_back(Particle(tempX, tempY, 3, rand() % 3000 * .001 + 1, rand() % 5 + 2, tempDir + M_PI, rand() % 3, 0, 0, 0, 0, 0));
		}
	}

	if (invincibleTime > 0) {
		invincibleTime--;
		invincibilityVisible++;
		if (invincibilityVisible > 1000)
			invincibilityVisible = 0;
	}

	if (darknessTime > 0) {
		if (darkness < 255)
			darkness += 3;
		if (darkness >= 255) {
			darknessTime--;
		}
	}
	else if (darkness > 0)
		darkness -= 3;
}

void Player::Draw(ALLEGRO_DISPLAY *display) {
	if (health > 0 && (invincibleTime == 0 || invincibleTime > 0 && invincibilityVisible % 4 == 0) && (availableWeapons[weapon].weaponID != 3 || availableWeapons[weapon].weaponID == 3 && !shooting)) {
		ALLEGRO_BITMAP *temp = al_create_bitmap(imageSize, imageSize);
		al_set_target_bitmap(temp);
		al_draw_bitmap_region(image, imageSize * frame, 0, imageSize, imageSize, 0, 0, NULL);
		al_set_target_bitmap(al_get_backbuffer(display));
		al_draw_rotated_bitmap(temp, imageSize / 2, imageSize / 2, x, y, dir, NULL);
		al_destroy_bitmap(temp);
	}

	if (DEBUG) {
		float handX = x + (gunOffX * cos(dir)) - (gunOffY * sin(dir));
		float handY = y + (gunOffY * cos(dir)) + (gunOffX * sin(dir));
		al_draw_filled_circle(handX, handY, 2, al_map_rgb(0, 0, 0));

		al_draw_circle(x, y, radius, al_map_rgb(255, 0, 255), 1);
	}
}

void Player::DrawHealthBar(ALLEGRO_FONT *font) {
	int drawY = 1;
	int drawH = 5;
	al_draw_filled_rectangle(1, drawY, SCREEN_WIDTH / 6, drawY + drawH, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(1, drawY, health / startHealth * SCREEN_WIDTH / 6, drawY + drawH, al_map_rgb(0, 255, 0));
	al_draw_rectangle(1, 1, SCREEN_WIDTH / 6, drawY + drawH, al_map_rgb(0, 0, 0), 1);

	if (GAME_STATE == 0)
		al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 6 / 2, 10, ALLEGRO_ALIGN_CENTER, "Health");

	al_draw_filled_rectangle(SCREEN_WIDTH, drawY, SCREEN_WIDTH - SCREEN_WIDTH / 6, drawY + drawH, al_map_rgb(255, 100, 255));
	al_draw_filled_rectangle(SCREEN_WIDTH, drawY, SCREEN_WIDTH - SCREEN_WIDTH / 6 * energy / energyStart, drawY + drawH, shootTimer == 0 && energy - availableWeapons[weapon].energyUse > 0 ? al_map_rgb(0, 0, 255) : al_map_rgb(100, 0, 100));
	al_draw_rectangle(SCREEN_WIDTH, drawY, SCREEN_WIDTH - SCREEN_WIDTH / 6, drawY + drawH, al_map_rgb(0, 0, 0), 1);

	if (GAME_STATE == 0)
		al_draw_text(font, al_map_rgb(255, 0, 0),  SCREEN_WIDTH - SCREEN_WIDTH / 6 / 2, 10, ALLEGRO_ALIGN_CENTER, "Energy");
}

float Player::GetX() { return x; }
float Player::GetY() { return y; }
float Player::GetVelX() { return velX; }
float Player::GetVelY() { return velY; }
float Player::GetPrevX() { return prevX; }
float Player::GetPrevY() { return prevY; }
float Player::GetHealth() { return health; }
float Player::GetStartHealth() { return startHealth; }
int Player::GetXTile() { return xTile; }
int Player::GetYTile() { return yTile; }
int Player::GetRadius() { return radius; }
int Player::GetWeapon() { return weapon; }
int Player::GetDarkness() { return darkness; }
int Player::GetWeaponID() { return availableWeapons[availableWeapons.size() - 1].weaponID; }
int Player::GetWeaponAmount() { return availableWeapons.size(); }
bool Player::GetInvincible() { return invincibleTime > 0 ? true : false; }
bool Player::GetFire() { return fire; }

void Player::SetPos(float x_, float y_) {
	x = x_;
	y = y_;
}

void Player::AddVelocityAngle(float dir_, float speed_) {
	velX += speed_ * cos(dir_);
	velY += speed_ * sin(dir_);
}

void Player::AddVelocity(float velX_, float velY_) {
	velX += velX_;
	velY += velY;
}

void Player::SetVelocityAngle(float dir_, float speed_) {
	velX = speed_ * cos(dir_);
	velY = speed_ * sin(dir_);
}

void Player::SetVelocity(float velX_, float velY_) {
	velX = velX_;
	velY = velY_;
}

void Player::SetHealth(float health_) {
	health = health_;
}

void Player::Damage(float damage) {
	if (invincibleTime <= 0 && health > 0) {
		health -= damage;
		Invincible();
	}
}

void Player::Invincible() {
	invincibleTime = invincibleTimerStart;
	invincibilityVisible = 0;
}

void Player::SetWeapon(int weapon_) {
	for (unsigned i = 0; i < availableWeapons.size(); i++) {
		if (availableWeapons[i].weaponID == weapon_) {
			weapon = i;
			shootTimer = 5;
		}
	}
}

void Player::SetDarknessTime(int darknessTime_) {
	darknessTime = darknessTime_;
}