#include "Globals.h"

bool DEBUG = false;
int GAME_STATE = -1;

float GetDistance(float x1, float y1, float x2, float y2) {
	 return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}

float GetAngle(float x1, float y1, float x2, float y2) {
	return atan2(y1 - y2, x1 - x2);
}