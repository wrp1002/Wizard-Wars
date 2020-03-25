#include "Enemy.h"

Enemy::Enemy(Map map, int type_, float playerX, float playerY, bool randomPos) {
	imageSize = 48;
	if (randomPos) {
		bool good = false;
		while (!good) {
			x = rand() % map.GetMapWidth();
			y = rand() % map.GetMapHeight();

			if (!map.GetMap()[y][x].solid && GetDistance(x * TILE_SIZE, y * TILE_SIZE, playerX, playerY) > (500 + imageSize) * (500 + imageSize)) {
				good = true;

				x = x * TILE_SIZE + TILE_SIZE / 2;
				y = y * TILE_SIZE + TILE_SIZE / 2;
			}
			if (DEBUG) {
				al_draw_filled_circle(x * TILE_SIZE + TILE_SIZE / 2, y * TILE_SIZE + TILE_SIZE / 2, 8, al_map_rgb(255, 0, 255));
				al_flip_display();
			}
		}
	}
	else {
		x = playerX;
		y = playerY;
	}
	type = type_;

	velX = 0;
	velY = 0;
	velPadding = .97;
	
	prevX = x;
	prevY = y;
	xTile = x / TILE_SIZE;
	yTile = y / TILE_SIZE;
	dir = 0;
	startFrame = 2;
	frame = startFrame;
	frameDir = 1;
	minFrame = 1;
	maxFrame = 4;
	frameTimerStart = 5;
	frameTimer = frameTimerStart;
	accSpeedStart = .1;
	accSpeed = accSpeedStart;
	following = false;
	active = true;
	effectable = true;
	tileCollidable = true;
	randomPath.x = xTile;
	randomPath.y = yTile;
	tileCollideChance = 1;
	slowness = 0;
	activationRadius = 0;
	effect = 0;
	effectTime = 0;
	invincibleTime = 0;
	image = 0;
	data = 0;
	timer = 0;
	handX = 0;
	handY = 0;
	gunOffX = 14;
	gunOffY = 10;

	if (type == 0) {
		maxVel = 5 + rand() % 2000 * .001;
		startHealth = 3;
		health = startHealth;
		knockback = 3;
		damage = 2.5;
		radius = 16;
		searchRadius = 300;
		ignoreRadius = 500;
		activationRadius = 150;
		randomPathRadius = 6;
		randomPathChance = 100;
		image = 0;
	}
	if (type == 1) {
		maxVel = 2 + rand() % 2000 * .001;
		startHealth = 4;
		health = startHealth;
		knockback = 3;
		damage = 3;
		radius = 16;
		searchRadius = 100;
		ignoreRadius = 300;
		randomPathRadius = 12;
		randomPathChance = 1000;
		image = 1;
	}
	else if (type == 2) {
		maxVel = 2 + rand() % 2000 * .001;
		randomPathRadius = 5;
		startHealth = 3;
		health = startHealth;
		knockback = 2;
		damage = 2;
		radius = 8;
		activationRadius = 150;
		searchRadius = 400;
		ignoreRadius = 600;
		randomPathRadius = 8;
		randomPathChance = 100;
		tileCollideChance = 3;
		data = 0;
		image = 2;
	}
	else if (type == 3) {
		maxVel = 0;
		startHealth = 10000;
		health = startHealth;
		knockback = 0;
		damage = 0;
		radius = 5;
		searchRadius = 0;
		ignoreRadius = 0;
		activationRadius = 100;
		randomPathRadius = 0;
		randomPathChance = 1;
		image = -1;
		data = 15;
		effectable = false;
		tileCollidable = false;
	}
	else if (type == 4) {
		maxVel = 5 + rand() % 2000 * .001;
		startHealth = 5;
		health = startHealth;
		knockback = 4;
		damage = 3.5;
		radius = 16;
		searchRadius = 300;
		ignoreRadius = 1000;
		activationRadius = 200;
		randomPathRadius = 6;
		randomPathChance = 200;
		image = 3;
		maxFrame = 5;
		startFrame = 3;
	}
	else if (type == 5) {
		maxVel = 5 + rand() % 2000 * .001;
		startHealth = 2;
		health = startHealth;
		knockback = 2;
		damage = 1.5;
		radius = 16;
		searchRadius = 200;
		ignoreRadius = 500;
		activationRadius = 0;
		randomPathRadius = 8;
		randomPathChance = 50;
		image = 4;
		minFrame = 0;
		maxFrame = 3;
		startFrame = 0;
	}
	else if (type == 6) {
		maxVel = 4 + rand() % 2000 * .001;
		startHealth = 3;
		health = startHealth;
		knockback = 5;
		damage = 1;
		radius = 16;
		searchRadius = 300;
		ignoreRadius = 500;
		activationRadius = 280;
		randomPathRadius = 10;
		randomPathChance = 100;
		image = -1;
		effectable = false;
	}
	else if (type == 7) {
		maxVel = 10;
		startHealth = 20;
		health = startHealth;
		knockback = 8;
		damage = 3;
		radius = 16;
		searchRadius = 1000;
		ignoreRadius = 1000;
		activationRadius = 450;
		randomPathRadius = 10;
		randomPathChance = 100;
		image = 5;
		maxFrame = 5;
		startFrame = 3;
		//effectable = false;
	}

	pathFinder.SetCollisionMap(map.GetMap(), tileCollideChance);
}

Enemy::~Enemy() {

}

void Enemy::Update(float playerX, float playerY, Map map, vector<Bullet> &bullets, vector<Particle> &particles) {
	prevX = x;
	prevY = y;
	accSpeed = accSpeedStart;
	float distance;

	handX = x + (gunOffX * cos(dir)) - (gunOffY * sin(dir));
	handY = y + (gunOffY * cos(dir)) + (gunOffX * sin(dir));

	if (type != 3) {
		distance = GetDistance(x, y, playerX, playerY);

		if (!following && distance < searchRadius * searchRadius)
			following = true;
		else if (following && distance > ignoreRadius * ignoreRadius)
			following = false;

		if (following) {
			path = pathFinder.FindPath(xTile, yTile, playerX / TILE_SIZE, playerY / TILE_SIZE);

			if (xTile == playerX / TILE_SIZE && yTile == playerY / TILE_SIZE) {
				targetX = playerX;
				targetY = playerY;
			}
			else {
				targetX = path[path.size() - 2].x * TILE_SIZE + TILE_SIZE / 2;
				targetY = path[path.size() - 2].y * TILE_SIZE + TILE_SIZE / 2;
			}
			randomPath.x = xTile;
			randomPath.y = yTile;
		}
		else  {
			if (path.size() > 0 && xTile == path[0].x && yTile == path[0].y) {
				bool good = false;
				while (!good) {
					randomPath.x = xTile + rand() % (randomPathRadius + 1) - randomPathRadius / 2;
					randomPath.y = yTile + rand() % (randomPathRadius + 1) - randomPathRadius / 2;

					if (randomPath.x < 0) randomPath.x = 0;
					else if (randomPath.x > map.GetMapWidth() - 2) randomPath.x = map.GetMapWidth() - 2;
					if (randomPath.y < 0) randomPath.y = 0;
					else if (randomPath.y > map.GetMapHeight() - 2) randomPath.y = map.GetMapHeight() - 2;

					if (!map.GetMap()[randomPath.y][randomPath.x].solid) {
						good = true;
					}
				}
			}

			path = pathFinder.FindPath(xTile, yTile, randomPath.x, randomPath.y);

			targetX = path[path.size() - 2].x * TILE_SIZE + TILE_SIZE / 2;
			targetY = path[path.size() - 2].y * TILE_SIZE + TILE_SIZE / 2;
		}

		if (path.size() > 0) {
			if (type != 4 && type != 7 || ((type == 4 || type == 7) && distance > (activationRadius * activationRadius) / 2)) {
				float tempDir = GetAngle(targetX, targetY, x, y);
				velX += accSpeed * cos(tempDir);
				velY += accSpeed * sin(tempDir);
				if (type == 7) {
					if (GetDistance(x + cos(dir + .1), y + sin(dir + .1), playerX, playerY) < GetDistance(x + cos(dir), y + sin(dir), playerX, playerY))
						dir += .1;
					else
						dir -= .1;
				}
				else if (health > 0)
					dir = GetAngle(x + velX, y + velY, x, y);
			}
			else {
				if (GetDistance(x + cos(dir + .1), y + sin(dir + .1), playerX, playerY) < GetDistance(x + cos(dir), y + sin(dir), playerX, playerY))
					dir += .1;
				else
					dir -= .1;
				//cout << dir << endl;
			}

			if (velX > maxVel) velX = maxVel;
			else if (velX < -maxVel) velX = -maxVel;
			if (velY > maxVel) velY = maxVel;
			else if (velY < -maxVel) velY = -maxVel;

			velX *= velPadding * slowness;
			velY *= velPadding * slowness;

			x += velX;
			y += velY;

			//cout << path[1].x << " " << path[1].y << endl;
		}
	}
	xTile = x / TILE_SIZE;
	yTile = y / TILE_SIZE;

	frameTimer--;
	if (frameTimer <= 0) {
		frameTimer = frameTimerStart;
		if (data == 0)
			startFrame = 3;
		else
			startFrame = 1;
		if (type != 4 && type != 7 || ((type == 4 || type == 7) && (abs(velX) + abs(velY)) / 2 > .5 && (type == 7 && distance > (activationRadius * activationRadius) / 2 || type == 4 && distance > (activationRadius * activationRadius)) || frame != startFrame))
			frame += frameDir;
		if (frame > maxFrame) frame = maxFrame;
		else if (frame < minFrame) frame = minFrame;
		if (frame == maxFrame || frame == minFrame)
			frameDir *= -1;
		if ((type == 4 || type == 7) && frame == 1 && startFrame != 1)
			frame += frameDir;
	}

	if (health <= 0 && active)
		active = false;

	if (invincibleTime > 0)
		invincibleTime--;

	if (type == 1) {
		if (rand() % 20 == 0) {
			bullets.push_back(Bullet(x, y, 0, 6, 300, 1));
		}
	}
	else if (type == 2) {
		if (data > 0) {
			data++;
			if (rand() % 2 == 0)
				particles.push_back(Particle(x + rand() % radius - radius / 2, y + rand() % radius - radius / 2, 0, 8, rand() % 20 + 10, NULL, NULL, NULL, NULL, NULL, 0, 0));
			if (rand() % int(500 - data) == 0)
				active = false;
		}
	}
	else if (type == 3) {
		data += rand() % 500 * .001;

		radius += rand() % 11 - 5;
		if (radius < data - 20)
			radius = data - 20;
		else if (radius > data)
			radius = data;

		activationRadius = radius + 100;

		float tempDir = rand() % 360 * (M_PI / 180);
		float speed = rand() % (int)data + data;
		float tempX = x + speed * cos(tempDir);
		float tempY = y + speed * sin(tempDir);
		particles.push_back(Particle(tempX, tempY, 3, 3, rand() % (int)data / 3 + data / 3, GetAngle(x, y, tempX, tempY), rand() % 3, 0, 0, 0, 0, 0));
	}
	else if (type == 4) {
		if (following && rand() % 500 == 0) {
			data = 3;
			timer = 20 + rand() % 100;
		}
		//if (distance < activationRadius * activationRadius) {
			if (data == 0 && distance < activationRadius * activationRadius) {
				data = rand() % 3 + 1;
				//data = 3;
				if (data == 1)
					timer = 100 + rand() % 20;
				else if (data == 2)
					timer = 150 + rand() % 20;
				else if (data == 3)
					timer = 20 + rand() % 100;
			}
			if (timer > 0) {
				timer--;
				if (timer == 0) {
					if (data == 1) {
						int divider = rand() % 3 + 1;
						float amount = .4;
						for (float i = -amount; i <= amount; i += amount / divider)
							bullets.push_back(Bullet(handX, handY, dir + i, 10, 30, 5));
						AddVelocityAngle(dir + M_PI, 3);
					}
					else if (data == 2) {
						bullets.push_back(Bullet(x, y, dir, 0, 1, 6));
						AddVelocityAngle(dir + M_PI, 4);
					}
					else if (data == 3) {
						bool good = false;
						velX = 0;
						velY = 0;

						for (int i = 0; i < 15; i++) {
							particles.push_back(Particle(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, 3, rand() % 5000 * .001, rand() % 10 + 10, rand() % 360 * (M_PI / 180), rand() % 7 + 1, 0, 155 + rand() % 100, 0, 0, 0));
						}
						int tempX = xTile;
						int tempY = yTile;
						while (!good) {
							tempX += rand() % 31 - 15;
							tempY += rand() % 31 - 15;
							if (tempX < 0) tempX = 0;
							else if (tempX > map.GetMapWidth() - 2) tempX = map.GetMapWidth() - 2;
							if (tempY < 0) tempY = 0;
							else if (tempY > map.GetMapHeight() - 2) tempY = map.GetMapHeight() - 2;
							if (!map.GetMap()[tempY][tempX].solid) {
								good = true;
								x = tempX * TILE_SIZE + TILE_SIZE / 2;
								y = tempY * TILE_SIZE + TILE_SIZE / 2;
								for (int i = 0; i < 15; i++) {
									particles.push_back(Particle(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, 3, rand() % 5000 * .001, rand() % 10 + 10, rand() % 360 * (M_PI / 180), rand() % 7 + 1, 0, 155 + rand() % 100, 0, 0, 0));
								}
							}
						}
					}
					data = 0;
				}
			}


		/*}
		else {
			timer = 0;
			data = 0;
		}*/
		if (timer > 0 && rand() % 3 == 0) {
			float tempDir = rand() % 360 * (M_PI / 180);
			float speed = rand() % (int)data + data;
			float tempX = x + speed * cos(tempDir);
			float tempY = y + speed * sin(tempDir);
			particles.push_back(Particle(tempX, tempY, 4, 6, 20, GetAngle(x, y, tempX, tempY), NULL, 0, 255, 0, 0, 0));
		}
	}
	else if (type == 5) {
		particles.push_back(Particle(x + rand() % radius - radius / 2, y + rand() % radius - radius / 2, 4, 6, 20, rand() % 360 * (M_PI / 180), NULL, 0, 0, 0, 0, 0));
	}
	else if (type == 6) {
		for (int i = 0; i < 10; i++)
			particles.push_back(Particle(x + rand() % radius - radius / 2, y + rand() % radius - radius / 2, 5, rand() % 3000 * .001, rand() % 20, NULL, rand() % 3, 0, 0, 0, 0, 0));

		if (data > 0) {
			data--;
			if (data == 0) {
				for (int i = 0; i < rand() % 4 + 3; i++)
				bullets.push_back(Bullet(x, y, dir + rand() % 5000 * .0001 - .25, rand() % 5000 * .001 + 7, 30, 7));
				AddVelocityAngle(dir + M_PI, 3);
			}
		}
	}
	else if (type == 7) {
		if (following && rand() % 500 == 0) {
			data = 3;
			timer = 20 + rand() % 100;
		}
		//if (distance < activationRadius * activationRadius) {
		if (data == 0 && distance < activationRadius * activationRadius) {
			data = rand() % 4 + 1;
			//data = 3;
			if (data == 1)
				timer = 150 + rand() % 20;
			else if (data == 2)
				timer = 200 + rand() % 20;
			else if (data == 3)
				timer = 20 + rand() % 100;
			else if (data == 4)
				timer = 150 + rand() % 20;
		}
		if (effect == 2 && effectTime > 0) {
			if (rand() % 20 == 0) {
				if (data != 1 && data != 4)
					timer = rand() % 10 + 5;
				if (data == 1 && timer > 50)
					timer = 50;
				else if (data == 4 && timer > 70)
					timer = 70;
			}
		}
		if (timer > 0) {
			timer--;
			if (timer < 50 && data == 1) {
				bullets.push_back(Bullet(handX, handY, dir + (rand() % 101 - 50) * .001 + sin(timer / 100.0 + M_PI / 4) - cos(timer / 100.0 + M_PI / 4), 8, 30, 0));
				bullets.push_back(Bullet(handX, handY, dir + (rand() % 101 - 50) * .001 + cos(timer / 100.0 + M_PI / 4) - sin(timer / 100.0 + M_PI / 4), 8, 30, 0));
			}
			else if (timer < 70 && data == 4) {
				AddVelocityAngle(dir, 1);
				invincibleTime = 2;
				for (int i = 0; i < 5; i++) {
					particles.push_back(Particle(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, 0, 6, rand() % 20 + 10, NULL, 4, 0, 0, 0, 0, 0));
					bullets.push_back(Bullet(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, rand() % 360 * (M_PI / 180), rand() % 3000 * .001 + 2, rand() % 20 + 5, 3));
				}
				bullets.push_back(Bullet(handX, handY, dir + (rand() % 101 - 50) * .001, 8, 30, 0));
			}
			if (timer == 0) {
				if (data == 2) {
					for (int i = 0; i < rand() % 3 + 1; i++)
						bullets.push_back(Bullet(x, y, rand() % 360 * (M_PI / 180), 10, rand() % 20 + 50, 10));
					AddVelocityAngle(dir + M_PI, 4);
				}
				else if (data == 3) {
					bool good = false;
					velX = 0;
					velY = 0;
					for (int i = 0; i < 15; i++) {
						particles.push_back(Particle(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, 3, rand() % 5000 * .001, rand() % 10 + 10, rand() % 360 * (M_PI / 180), rand() % 7 + 1, 155 + rand() % 100, 0, 0, 0, 0));
					}
					int tempX = xTile;
					int tempY = yTile;
					while (!good) {
						tempX += rand() % 31 - 15;
						tempY += rand() % 31 - 15;
						if (tempX < 1) tempX = 1;
						else if (tempX > map.GetMapWidth() - 2) tempX = map.GetMapWidth() - 2;
						if (tempY < 1) tempY = 1;
						else if (tempY > map.GetMapHeight() - 2) tempY = map.GetMapHeight() - 2;
						if (!map.GetMap()[tempY][tempX].solid) {
							good = true;
							x = tempX * TILE_SIZE + TILE_SIZE / 2;
							y = tempY * TILE_SIZE + TILE_SIZE / 2;
							prevX = x;
							prevY = y;
							for (int i = 0; i < 15; i++) {
								particles.push_back(Particle(x + rand() % imageSize - imageSize / 2, y + rand() % imageSize - imageSize / 2, 3, rand() % 5000 * .001, rand() % 10 + 10, rand() % 360 * (M_PI / 180), rand() % 7 + 1, 155 + rand() % 100, 0, 0, 0, 0));
							}
						}
					}
				}
				data = 0;
			}
		}


		/*}
		else {
		timer = 0;
		data = 0;
		}*/
		if (timer > 0 && rand() % 3 == 0) {
			float tempDir = rand() % 360 * (M_PI / 180);
			float speed = 40 + rand() % 10;
			float tempX = x + speed * cos(tempDir);
			float tempY = y + speed * sin(tempDir);
			particles.push_back(Particle(tempX, tempY, 3, 5, 10, GetAngle(x, y, tempX, tempY), rand() % 5, 255, 0, 0, 0, 0));
		}
	}
	
	if (effectTime > 0) {
		effectTime--;

		if (effect == 1) {
			particles.push_back(Particle(x + rand() % radius - radius / 2, y + rand() % radius - radius / 2, 0, 8, rand() % 20 + 10, NULL, NULL, NULL, NULL, NULL, 0, 0));
			health -= .001;
		}
		else if (effect == 2) {
			x += rand() % 3 - 1;
			y += rand() % 3 - 1;
			velX *= .97;
			velY *= .97;
			dir += (rand() % 501 - 250) * .001;
			frame += rand() % 3 - 1;
			if (frame < minFrame) frame = minFrame;
			else if (frame > maxFrame) frame = maxFrame;
			if (type != 7) {
				data = 0;
				timer = 0;
			}
			//bullets.push_back(Bullet(x, y, rand() % 360 * (M_PI / 180), 3, 5, 4));
		}
	}

	slowness = 1;
}

void Enemy::Draw(ALLEGRO_BITMAP *enemyImages, ALLEGRO_DISPLAY *display, float cameraPos[2]) {
	if (image != -1) {
		if (x + imageSize > cameraPos[0] && x - imageSize < cameraPos[0] + SCREEN_WIDTH && y + imageSize > cameraPos[1] && y - imageSize < cameraPos[1] + SCREEN_HEIGHT) {
			ALLEGRO_TRANSFORM t;
			ALLEGRO_TRANSFORM prevT;
			al_copy_transform(&prevT, al_get_current_transform());
			al_identity_transform(&t);
			al_translate_transform(&t, -imageSize / 2, -imageSize / 2);
			al_rotate_transform(&t, dir);
			al_translate_transform(&t, x, y);
			al_compose_transform(&t, &prevT);
			al_use_transform(&t);
			al_draw_bitmap_region(enemyImages, imageSize * frame, imageSize * image, imageSize, imageSize, 0, 0, NULL);
			al_use_transform(&prevT);
		}
	}
	else {
		al_hold_bitmap_drawing(false);
		
		if (type == 3) {
			al_draw_filled_circle(x, y, radius, al_map_rgb(0, 0, 0));
		}

		al_hold_bitmap_drawing(true);
	}
}

void Enemy::DrawDebug() {
	al_draw_circle(x, y, radius, al_map_rgb(255, 0, 255), 2);
	if (path.size() > 2) {
		al_draw_line(x, y, path[path.size() - 2].x * TILE_SIZE + TILE_SIZE / 2, path[path.size() - 2].y * TILE_SIZE + TILE_SIZE / 2, al_map_rgb(255, 0, 255), 2);
		for (unsigned i = 0; i < path.size() - 2; i++)
			al_draw_line(path[i].x * TILE_SIZE + TILE_SIZE / 2, path[i].y * TILE_SIZE + TILE_SIZE / 2, path[i + 1].x * TILE_SIZE + TILE_SIZE / 2, path[i + 1].y * TILE_SIZE + TILE_SIZE / 2, al_map_rgb(255, 0, 255), 2);

		al_draw_filled_circle(targetX, targetY, 3, al_map_rgb(0, 255, 0));
	}
	if (following)
		al_draw_circle(x, y, ignoreRadius, al_map_rgb(255, 0, 255), 2);
	else
		al_draw_circle(x, y, searchRadius, al_map_rgb(255, 0, 255), 2);
	al_draw_circle(x, y, activationRadius, al_map_rgb(255, 0, 0), 2);

	al_draw_filled_rectangle(x - 10, y - 20, x + 10, y - 18, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(x - 10, y - 20, x - 10 + 20 * health / startHealth, y - 18, al_map_rgb(0, 255, 0));
}

float Enemy::GetX() { return x; }
float Enemy::GetY() { return y; }
float Enemy::GetVelX() { return velX; }
float Enemy::GetVelY() { return velY; }
float Enemy::GetPrevX() { return prevX; }
float Enemy::GetPrevY() { return prevY; }
float Enemy::GetDamage() { return damage; }
float Enemy::GetHealth() { return health; }
float Enemy::GetStartHealth() { return startHealth; }
float Enemy::GetData() { return data; }
float Enemy::GetDir() { return dir; }
int Enemy::GetXTile() { return xTile; }
int Enemy::GetYTile() { return yTile; }
int Enemy::GetRadius() { return radius; }
int Enemy::GetActivationRadius() { return activationRadius; }
int Enemy::GetKnockback() { return knockback; }
int Enemy::GetType() { return type; }
int Enemy::GetFrame() { return frame; }
int Enemy::GetImage() { return image; }
int Enemy::GetImageSize() { return imageSize; }
bool Enemy::GetActive() { return active; }
bool Enemy::GetInvincible() { return invincibleTime > 0; }
bool Enemy::GetTileCollidable() { return tileCollidable; }
bool Enemy::GetFollowing() { return following; }
PathFinder Enemy::GetPathFinder() { return pathFinder; }

void Enemy::SetPos(float x_, float y_) {
	x = x_;
	y = y_;
}

void Enemy::AddVelocityAngle(float dir_, float speed_) {
	velX += speed_ * cos(dir_);
	velY += speed_ * sin(dir_);
}

void Enemy::AddVelocity(float velX_, float velY_) {
	velX += velX_;
	velY += velY;
}

void Enemy::SetVelocityAngle(float dir_, float speed_) {
	velX = speed_ * cos(dir_);
	velY = speed_ * sin(dir_);
}

void Enemy::SetVelocity(float velX_, float velY_) {
	velX = velX_;
	velY = velY;
}

void Enemy::SetHealth(float health_) {
	health = health_;
}

void Enemy::RemoveTile(int x, int y) {
	pathFinder.RemoveTile(x, y);
}

void Enemy::SetSlowness(float slowness_) {
	slowness = slowness_;
}

void Enemy::SetData(float data_) {
	data = data_;
}

void Enemy::SetEffect(int effect_, int effectTime_) {
	if (effectable) {
		effect = effect_;
		effectTime = effectTime_;
	}
}

void Enemy::SetInvincibleTime(int invincibleTime_) {
	invincibleTime = invincibleTime_;
}

void Enemy::SetActive(bool active_) {
	active = active_;
}

void Enemy::Anger() {
	following = true;
}

void Enemy::Damage(float damage) {
	if (!GetInvincible()) {
		health -= damage;
		invincibleTime = 2;
	}
}