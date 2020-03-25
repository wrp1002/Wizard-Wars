#pragma once
#include "Globals.h"

class Particle
{
private:
	float x, y, velX, velY, dir, speed;
	int life, image, type, radius, imageSize;
	int r, g, b;
	bool active;

public:
	Particle(float x_, float y_, int type_, float speed_, int life_, float dir_, int radius_, int r_, int g_, int b_, float velX_, float velY_);
	~Particle();

	void Update();
	void Draw(ALLEGRO_BITMAP *particleImages, ALLEGRO_DISPLAY *display, float cameraPos[2]);
	void DrawDebug();

	bool GetActive();
};

