#pragma once
#include "Globals.h"
#include "Player.h"
#include "Map.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Particle.h"

using namespace std;

void RemoveTile(Map &map, vector<Enemy> &enemies, int x, int y);
void Collide(Map &map, Player &player, vector<Enemy> &enemies, vector<Particle> &particles);
void CollideBullets(Map &map, Player &player, vector<Bullet> &playerBullets, vector<Bullet> &enemyBullets, vector<Enemy> &enemies, vector<Particle> &particles);
void UpdateCamera(Player player, Map map, float cameraPos[2]);
void CleanUp(vector<Enemy> &enemies, vector<Bullet> &playerBullets, vector<Bullet> &enemyBullets, vector<Particle> &particles);
void UpdateWave(int wave, int &enemyCount, int &maxEnemies);
void Draw(float cameraPos[], Player player, Map map, vector<Bullet> playerBullets, vector<Bullet> enemyBullets, vector<Enemy> enemies, vector<Particle> particles, ALLEGRO_BITMAP *bulletImages, ALLEGRO_BITMAP *enemyImages, ALLEGRO_BITMAP *particleImages, ALLEGRO_BITMAP *mouseImage, ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font40, ALLEGRO_FONT *font10, ALLEGRO_FONT *font12, POINT mousePos, int startTimer, int waveTimer, int wave, int enemyCount);