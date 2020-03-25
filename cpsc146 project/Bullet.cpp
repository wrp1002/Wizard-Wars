#include "Bullet.h"
#include "Enemy.h"

Bullet::Bullet(float x_, float y_, float dir_, float speed_, int life_, int type_) {
	x = x_;
	y = y_;
	dir = dir_;
	speed = speed_;
	life = life_;
	type = type_;
	xTile = x / TILE_SIZE;
	yTile = y / TILE_SIZE;
	active = true;
	tileCollidable = true;
	entityCollidable = true;
	dodgable = true;
	dieFromEntity = true;
	imageSize = 128;
	frame = 0;
	data = 0;
	effect = 0;
	effectTime = 0;

	if (type == 0) {
		radius = 4;
		knockback = 5;
		damage = 1;
		image = 0;
		effect = 1;
		effectTime = 50;
	}
	else if (type == 1) {
		radius = rand() % 4 + 5;
		knockback = 5;
		damage = 1;
		life = 300;
		image = -1;
	}
	else if (type == 2) {
		radius = 48;
		knockback = 15;
		damage = 4;
		image = 2;
		frame = -rand() % 5;
		frameTimerStart = 2;
		frameTimer = frameTimerStart;
		tileCollidable = false;
	}
	else if (type == 3) {
		radius = 4;
		knockback = 0;
		damage = 0;
		image = 3;
		tileCollidable = false;
		entityCollidable = false;
	}
	else if (type == 4) {
		radius = 4;
		knockback = 0;
		damage = .1;
		image = -1;
		frame = 1;
		data = 2;
		effect = 2;
		effectTime = 100;
		dodgable = false;
		dieFromEntity = false;
	}
	else if (type == 5) {
		radius = 4;
		knockback = 5;
		damage = 1;
		image = 4;
		effect = 1;
		effectTime = 50;
	}
	else if (type == 6) {
		radius = 1;
		image = -1;
	}
	else if (type == 7) {
		radius = 4;
		knockback = 5;
		damage = .5;
		image = 5;
		effect = 3;
	}
	else if (type == 8) {
		radius = 16;
		knockback = 0;
		damage = 0;
		image = 5;
	}
	else if (type == 9) {
		radius = 8;
		knockback = 0;
		damage = .03;
		image = -1;
		tileCollidable = false;
		dieFromEntity = false;
		dodgable = false;
	}
	else if (type == 10) {
		knockback = 0;
		damage = 0;
		life = -1;
		radius = 24;
		image = -1;
	}
}


Bullet::~Bullet() {

}

void Bullet::Update(Map map, vector<Particle> &particles, vector<Enemy> &enemies) {
	if (type == 0) {
		x += speed * cos(dir);
		y += speed * sin(dir);

		particles.push_back(Particle(x, y, 0, 4, rand() % 10 + 4, NULL, NULL, NULL, NULL, NULL, 0, 0));
	}
	else if (type == 1) {
		x += rand() % int(speed + 1) - speed / 2;
		y += rand() % int(speed + 1) - speed / 2;
		if (life < radius)
			radius--;
	}
	else if (type == 2) {
		frameTimer--;
		if (frameTimer <= 0) {
			frameTimer = frameTimerStart;
			frame++;
			if (frame > 8)
				Kill(particles);
		}
	}
	if (type == 3) {
		x += speed * cos(dir);
		y += speed * sin(dir);

		dir += (rand() % 1001 - 500) * .001;

		particles.push_back(Particle(x, y, 0, 4, rand() % 10 + 4, NULL, NULL, NULL, NULL, NULL, 0, 0));
	}
	else if (type == 4) {
		if (frame == 1) {
			life -= life / (5 * (3 - data));
			if (life <= 0 || !active) {
				life = 5 + data;
				frame = 0;
				active = true;
				tileCollidable = false;
			}
			POINT temp;
			temp.x = x;
			temp.y = y;
			pathPoints.push_back(temp);
			x += speed * cos(dir + (rand() % 1001 - 500) * .001);
			y += speed * sin(dir + (rand() % 1001 - 500) * .001);
			//cout << life << endl;
			temp.x = x;
			temp.y = y;
			pathPoints.push_back(temp);
		}
	}
	else if (type == 5) {
		x += speed * cos(dir);
		y += speed * sin(dir);

		particles.push_back(Particle(x, y, 4, 4, rand() % 10 + 4, NULL, NULL, NULL, NULL, NULL, 0, 0));
	}
	else if (type == 7) {
		x += speed * cos(dir);
		y += speed * sin(dir);

		particles.push_back(Particle(x, y, 3, rand() % 3000 * .001, rand() % 10 + 4, NULL, rand() % 4, 0, 0, 0, 0, 0));
	}
	else if (type == 8) {
		x += speed * cos(dir);
		y += speed * sin(dir);

		particles.push_back(Particle(x, y, 3, rand() % 3000 * .001, rand() % 10 + 4, NULL, rand() % 4, 0, 0, 0, 0, 0));
	}
	else if (type == 9) {
		radius += rand() % 5 - 2;
		if (radius < 8 && life > radius) radius = 8;
		else if (radius > 16) radius = 16;

		float tempDir = rand() % 360 * (M_PI / 180);
		float tempSpeed = 50 + rand() % 20;
		float tempX = x + tempSpeed * cos(tempDir);
		float tempY = y + tempSpeed * sin(tempDir);
		particles.push_back(Particle(tempX, tempY, 3, 5, 10, tempDir + M_PI, rand() % 3, 0, 0, 0, 0, 0));

		if (life < radius)
			radius--;
	}
	else if (type == 10) {
		x += speed * cos(dir);
		y += speed * sin(dir);

		particles.push_back(Particle(x, y, 3, rand() % 2000 * .001, rand() % 5 + 10, NULL, rand() % 5 + 5, 0, rand() % 100 + 155, 0, 0, 0));
	}

	if (x < -radius || x > map.GetMapWidth() * TILE_SIZE + radius || y < -radius || y > map.GetMapHeight() * TILE_SIZE + radius) {
		//active = false;
	}

	xTile = x / TILE_SIZE;
	yTile = y / TILE_SIZE;

	life--;
	if (life == 0)
		Kill(particles);
}

void Bullet::Draw(ALLEGRO_BITMAP *bulletImages, ALLEGRO_DISPLAY *display, float cameraPos[2]) {
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
			al_draw_bitmap_region(bulletImages, imageSize * frame, imageSize * image, imageSize, imageSize, 0, 0, NULL);
			al_use_transform(&prevT);
		}
	}
	else {
		al_hold_bitmap_drawing(false);

		if (type == 1) {
			for (int i = 0; i < 5; i++) 
				al_draw_filled_circle(x + rand() % 5 - 2, y + rand() % 5 - 2, radius, al_map_rgba(0, 255, 0, 50));
		} 
		else if (type == 4) {
			for (unsigned i = 0; i < pathPoints.size() - 1; i++) {
				al_draw_line(pathPoints[i].x, pathPoints[i].y, pathPoints[i + 1].x, pathPoints[i + 1].y, al_map_rgb(255, 255, 0), 1);
			}
		}
		else if (type == 9)
			al_draw_filled_circle(x, y, radius, al_map_rgb(0, 0, 0));

		al_hold_bitmap_drawing(true);
	}
}

void Bullet::DrawDebug() {
	al_draw_circle(x, y, radius, al_map_rgb(255, 0, 255), 1);
}

float Bullet::GetX() { return x; }
float Bullet::GetY() { return y; }
float Bullet::GetDir() { return dir; }
float Bullet::GetDamage() { return damage; }
float Bullet::GetData() { return data; }
float Bullet::GetSpeed() { return speed; }
int Bullet::GetXTile() { return xTile; }
int Bullet::GetYTile() { return yTile; }
int Bullet::GetType() { return type; }
int Bullet::GetRadius() { return radius; }
int Bullet::GetKnockback() { return knockback; }
int Bullet::GetEffect() { return effect; }
int Bullet::GetEffectTime() { return effectTime; }
int Bullet::GetFrame() { return frame; }
bool Bullet::GetActive() { return active; }
bool Bullet::GetTileCollidable() { return tileCollidable; }
bool Bullet::GetEntityCollidable() { return entityCollidable; }
bool Bullet::GetDodgable() { return dodgable; }
bool Bullet::GetDieFromEntity() { return dieFromEntity; }

void Bullet::Kill(vector<Particle> &particles) {
	active = false;

	if (type == 0) {
		for (int i = 0; i < 10; i++)
			particles.push_back(Particle(x, y, 0, 8, rand() % 20 + 10, NULL, NULL, NULL, NULL, NULL, 0, 0));
	}
	if (type == 7 || type == 8 || type == 9) {
		for (int i = 0; i < 10; i++) {
			particles.push_back(Particle(x, y, 3, rand() % 3000 * .001, rand() % 10 + 6, NULL, rand() % 4 + 2, 0, 0, 0, 0, 0));
		}
	}
}
void Bullet::SetData(float data_) {
	data = data_;
}