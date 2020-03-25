#pragma once
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <iostream>

#include "Globals.h"
#include "Player.h"
#include "Map.h"
#include "Enemy.h"

using namespace std;

class Engine {
private:
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE * event_queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font10;
	ALLEGRO_FONT *font18;
	ALLEGRO_BITMAP *image;
	ALLEGRO_TRANSFORM camera;

	float FPS;
	bool done;
	bool redraw;
	bool keys[4];
	float cameraPos[2];

	vector<Enemy> enemies;
	Map map;
	Player player;

	void Collide();
	void UpdateCamera();

public:
	Engine();

	void Init();
	void Run();
};

