#include "Particle.h"


Particle::Particle(float x_, float y_, int type_, float speed_, int life_, float dir_, int radius_, int r_, int g_, int b_, float velX_, float velY_) {
	active = true;
	imageSize = 8;

	x = x_;
	y = y_;
	velX = velX_;
	velY = velY_;
	type = type_;
	life = life_;
	speed = speed_;
	radius = radius_;
	r = 0;
	g = 0;
	b = 0;
	if (dir_ == NULL) dir = rand() % 360 * (M_PI / 180);
	else dir = dir_;

	if (type == 0) {
		image = 0;
		radius = 4;
	}
	else if (type == 1) {
		image = -1;
		radius = 4;
	}
	else if (type == 2) {
		image = 1;
		radius = 4;
		dir = 0;
	}
	else if (type == 3) {
		image = -1;
		r = r_;
		g = g_;
		b = b_;
		if (radius_ == -1)
			radius = rand() % 3;
		else
			radius = radius_;
	}
	else if (type == 4) {
		image = 2;
		radius = 4;
	}
	else if (type == 5) {
		image = -1;
	}
}


Particle::~Particle() {

}

void Particle::Update() {
	if (type == 0 || type == 4) {
		x += rand() % int(speed + 1) - speed / 2;
		y += rand() % int(speed + 1) - speed / 2;

		life--;
		if (life <= 0)
			active = false;
	}
	else if (type == 1) {
		life--;
		if (life <= 0)
			active = false;
	}
	else if (type == 2) {
		y--;

		life--;
		if (life <= 0)
			active = false;
	}
	else if (type == 3) {
		x += speed * cos(dir);
		y += speed * sin(dir);

		speed *= .98;

		life--;
		if (life < radius)
			radius--;
		if (life <= 0)
			active = false;

		x += velX;
		y += velY;
		velX *= .93;
		velY *= .93;
	}
	else if (type == 5) {
		if (speed > 0) {
			x += speed * cos(dir) + rand() % 3 - 1;
			y += speed * sin(dir) + rand() % 3 - 1;
		}
		else {
			x += rand() % 5 - 2;
			y += rand() % 5 - 2;
		}
		dir += (rand() % 1001 - 500) * .001;

		life--;
		if (life <= 0)
			active = false;
	}
}

void Particle::Draw(ALLEGRO_BITMAP *particleImages, ALLEGRO_DISPLAY *display, float cameraPos[2]) {
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
			al_draw_bitmap_region(particleImages, 0, imageSize * image, imageSize, imageSize, 0, 0, NULL);
			al_use_transform(&prevT);
		}
	}
	else {
		al_hold_bitmap_drawing(false);

		if (type == 1)
			al_draw_line(x + rand() % 3 - 1, y + rand() % 3 - 1, x + rand() % 10 * cos(dir), y + rand() % 10 * sin(dir), al_map_rgb(255, 255, 0), 1);
		else if (type == 3 || type == 5) {
			if (radius > 0)
				al_draw_filled_circle(x, y, radius, al_map_rgb(r, g, b));
			else
				al_draw_pixel(x, y, al_map_rgb(r, g, b));
		}

		al_hold_bitmap_drawing(true);
	}
}

void Particle::DrawDebug() {
	al_draw_circle(x, y, radius, al_map_rgb(255, 0, 255), 1);
}

bool Particle::GetActive() { return active; }