#pragma once

#define _USE_MATH_DEFINES
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_font.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <string>
#include <Windows.h>

using namespace std;

const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
const int TILE_SIZE = 48;
extern bool DEBUG;
extern int GAME_STATE;
enum KEYS { UP, DOWN, LEFT, RIGHT };

float GetDistance(float x1, float y1, float x2, float y2);
float GetAngle(float x1, float y1, float x2, float y2);