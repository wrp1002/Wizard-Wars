#include "Globals.h"
#include "Player.h"
#include "Map.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Particle.h"

using namespace std;

void RemoveTile(Map &map, vector<Enemy> &enemies, int x, int y) {
	for (unsigned i = 0; i < enemies.size(); i++) {
		enemies[i].RemoveTile(x, y);
		map.KillTile(x, y);
	}
}
void BitmapToParticles(vector<Particle> &particles, ALLEGRO_BITMAP *image, ALLEGRO_DISPLAY *display, int xPos, int yPos, int sX, int sY, int eX, int eY, float dir, float velX, float velY, float cameraPos[2]) {
	ALLEGRO_BITMAP *temp1 = al_create_bitmap(eX, eY);
	ALLEGRO_BITMAP *temp = al_create_bitmap(eX, eY);
	al_set_target_bitmap(temp1);
	al_draw_bitmap_region(image, sX * eX, sY * eY, eX, eY, 0, 0, NULL);
	al_set_target_bitmap(temp);
	al_draw_rotated_bitmap(temp1, al_get_bitmap_width(temp) / 2, al_get_bitmap_height(temp) / 2, al_get_bitmap_width(temp) / 2, al_get_bitmap_height(temp) / 2, dir, NULL);
	al_set_target_bitmap(al_get_backbuffer(display));

	al_lock_bitmap(temp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
	for (int y = 0; y < al_get_bitmap_height(temp); y++) {
		for (int x = 0; x < al_get_bitmap_width(temp); x++) {
			ALLEGRO_COLOR color = al_get_pixel(temp, x, y);
			if (color.a > 0 && rand() % (eX * eY / 1000) == 0) {
				particles.push_back(Particle(xPos + x - al_get_bitmap_width(temp) / 2, yPos + y - al_get_bitmap_height(temp) / 2, 3, 3000 * .0001, rand() % 5000 * .01, NULL, 0, color.r * 255, color.g * 255, color.b * 255, velX, velY));
			}
		}
	}
	al_unlock_bitmap(temp);
	al_destroy_bitmap(temp);
	al_destroy_bitmap(temp1);
}
void Collide(Map &map, Player &player, vector<Enemy> &enemies, vector<Particle> &particles, int &transition) {
	vector<vector<Map::Tile>> tiles = map.GetMap();
	vector<Map::Tile> neighbors;
	/*if (player.GetXTile() > 0 && (player.GetX() + player.GetRadius() >= tiles[player.GetYTile()][player.GetXTile() + 1].x * TILE_SIZE) && tiles[player.GetYTile()][player.GetXTile() + 1].solid) player.SetPos(tiles[player.GetYTile()][player.GetXTile() + 1].x * TILE_SIZE - player.GetRadius(), player.GetY());
	else if (player.GetYTile() < map.GetMapWidth() && player.GetX() - player.GetRadius() <= tiles[player.GetYTile()][player.GetXTile() - 1].x * TILE_SIZE + TILE_SIZE && tiles[player.GetYTile()][player.GetXTile() - 1].solid) player.SetPos(tiles[player.GetYTile()][player.GetXTile() - 1].x * TILE_SIZE + TILE_SIZE + player.GetRadius(), player.GetY());
	if (player.GetYTile() > 0 && player.GetY() + player.GetRadius() >= tiles[player.GetYTile() + 1][player.GetXTile()].y * TILE_SIZE && tiles[player.GetYTile() + 1][player.GetXTile()].solid) player.SetPos(player.GetX(), tiles[player.GetYTile() + 1][player.GetYTile()].y * TILE_SIZE - player.GetRadius());
	else if (player.GetYTile() < map.GetMapHeight() && player.GetY() - player.GetRadius() <= tiles[player.GetYTile() - 1][player.GetXTile()].y * TILE_SIZE + TILE_SIZE && tiles[player.GetYTile() - 1][player.GetXTile()].solid) player.SetPos(player.GetX(), tiles[player.GetYTile() - 1][player.GetYTile()].y * TILE_SIZE + TILE_SIZE + player.GetRadius());
	if (tiles[player.GetYTile()][player.GetXTile()].solid) {

	}*/

	for (unsigned i = 0; i < enemies.size(); i++) {
		if (player.GetHealth() > 0) {
			float distance = GetDistance(enemies[i].GetX(), enemies[i].GetY(), player.GetX(), player.GetY());
			if (distance < (enemies[i].GetRadius() + player.GetRadius()) * (enemies[i].GetRadius() + player.GetRadius())) {
				float angle1 = GetAngle(enemies[i].GetX(), enemies[i].GetY(), player.GetX(), player.GetY());
				float angle2 = GetAngle(player.GetX(), player.GetY(), enemies[i].GetX(), enemies[i].GetY());
				if (!player.GetFire()) {
					player.SetVelocityAngle(angle2, enemies[i].GetKnockback());
					enemies[i].SetVelocityAngle(angle1, enemies[i].GetKnockback());
					player.Damage(enemies[i].GetDamage());
					if (enemies[i].GetType() == 6)
						player.SetDarknessTime(200);
				}
				else {
					if (player.GetFire()) {
						player.AddVelocityAngle(angle2, enemies[i].GetKnockback());
						enemies[i].AddVelocityAngle(angle1, 15);
						enemies[i].Damage(1);
						enemies[i].SetEffect(1, 50);
					}
				}
				if (enemies[i].GetType() == 3) {
					transition += 5;
					enemies[i].SetData(enemies[i].GetData() + 5);
					if (player.GetHealth() + .075 < player.GetStartHealth())
						player.SetHealth(player.GetHealth() + .075);
					if (transition >= 255) {
						enemies.clear();
						break;
					}
					player.SetPos(player.GetX() + rand() % 5 - 2, player.GetY() + rand() % 5 - 2);
				}
			}
			if (distance < (enemies[i].GetActivationRadius() + player.GetRadius()) * (enemies[i].GetActivationRadius() + player.GetRadius())) {
				if (enemies[i].GetType() == 0) {
					enemies[i].SetSlowness(1.03);
				}
				else if (enemies[i].GetType() == 2) {
					enemies[i].SetData(1);
				}
				else if (enemies[i].GetType() == 3) {
					player.AddVelocityAngle(GetAngle(enemies[i].GetX(), enemies[i].GetY(), player.GetX(), player.GetY()), .5);
				}
				else if (enemies[i].GetType() == 4) {
					if (rand() % 100 == 0) {

					}
				}
				else if (enemies[i].GetType() == 6) {
					if (enemies[i].GetData() == 0)
						enemies[i].SetData(rand() % 50 + 100);
				}
			}
		}

		for (unsigned j = 0; j < enemies.size(); j++) {
			if (i != j) {
				float distance = GetDistance(enemies[i].GetX(), enemies[i].GetY(), enemies[j].GetX(), enemies[j].GetY());
				if (distance < (enemies[i].GetRadius() + enemies[j].GetRadius()) * (enemies[i].GetRadius() + enemies[j].GetRadius())) {
					float angle1 = GetAngle(enemies[i].GetX(), enemies[i].GetY(), enemies[j].GetX(), enemies[j].GetY());
					float angle2 = GetAngle(enemies[j].GetX(), enemies[j].GetY(), enemies[i].GetX(), enemies[i].GetY());
					enemies[i].SetPos(enemies[i].GetX() + cos(angle1), enemies[i].GetY() + sin(angle1));
					enemies[j].SetPos(enemies[j].GetX() + cos(angle2), enemies[j].GetY() + sin(angle2));
					enemies[i].AddVelocityAngle(angle1, 1);
					enemies[j].AddVelocityAngle(angle2, 1);
					distance = GetDistance(enemies[i].GetX(), enemies[i].GetY(), enemies[j].GetX(), enemies[j].GetY());
				}
			}
		}

		if (enemies[i].GetTileCollidable()) {
			neighbors.clear();
			vector<PathFinder::PathPoint> neighbors2;
			if (enemies[i].GetYTile() > 0 && enemies[i].GetYTile() < map.GetMapHeight() - 1 && enemies[i].GetXTile() > 0 && enemies[i].GetXTile() < map.GetMapWidth() - 1) {
				neighbors.push_back(tiles[enemies[i].GetYTile()][enemies[i].GetXTile()]);
				neighbors.push_back(tiles[enemies[i].GetYTile() + 1][enemies[i].GetXTile()]);
				neighbors.push_back(tiles[enemies[i].GetYTile() - 1][enemies[i].GetXTile()]);
				neighbors.push_back(tiles[enemies[i].GetYTile()][enemies[i].GetXTile() + 1]);
				neighbors.push_back(tiles[enemies[i].GetYTile()][enemies[i].GetXTile() - 1]);
				neighbors.push_back(tiles[enemies[i].GetYTile() + 1][enemies[i].GetXTile() + 1]);
				neighbors.push_back(tiles[enemies[i].GetYTile() + 1][enemies[i].GetXTile() - 1]);
				neighbors.push_back(tiles[enemies[i].GetYTile() - 1][enemies[i].GetXTile() + 1]);
				neighbors.push_back(tiles[enemies[i].GetYTile() - 1][enemies[i].GetXTile() - 1]);
				neighbors2.push_back(enemies[i].GetPathFinder().GetCollisionMap()[enemies[i].GetYTile() + 1][enemies[i].GetXTile()]);
				neighbors2.push_back(enemies[i].GetPathFinder().GetCollisionMap()[enemies[i].GetYTile() - 1][enemies[i].GetXTile()]);
				neighbors2.push_back(enemies[i].GetPathFinder().GetCollisionMap()[enemies[i].GetYTile()][enemies[i].GetXTile() + 1]);
				neighbors2.push_back(enemies[i].GetPathFinder().GetCollisionMap()[enemies[i].GetYTile()][enemies[i].GetXTile() - 1]);
				neighbors2.push_back(enemies[i].GetPathFinder().GetCollisionMap()[enemies[i].GetYTile() + 1][enemies[i].GetXTile() + 1]);
				neighbors2.push_back(enemies[i].GetPathFinder().GetCollisionMap()[enemies[i].GetYTile() + 1][enemies[i].GetXTile() - 1]);
				neighbors2.push_back(enemies[i].GetPathFinder().GetCollisionMap()[enemies[i].GetYTile() - 1][enemies[i].GetXTile() + 1]);
				neighbors2.push_back(enemies[i].GetPathFinder().GetCollisionMap()[enemies[i].GetYTile() - 1][enemies[i].GetXTile() - 1]);
			}
			for (unsigned j = 0; j < neighbors2.size(); j++) {
				if (neighbors2[j].solid) {
					if (enemies[i].GetY() - enemies[i].GetRadius() + 3 < neighbors2[j].y * TILE_SIZE + TILE_SIZE && enemies[i].GetY() + enemies[i].GetRadius() - 3 > neighbors2[j].y * TILE_SIZE && enemies[i].GetX() + enemies[i].GetRadius() >= neighbors2[j].x * TILE_SIZE && enemies[i].GetPrevX() <= neighbors2[j].x * TILE_SIZE) enemies[i].SetPos(neighbors2[j].x * TILE_SIZE - enemies[i].GetRadius(), enemies[i].GetY());
					else if (enemies[i].GetY() - enemies[i].GetRadius() < neighbors2[j].y * TILE_SIZE + TILE_SIZE && enemies[i].GetY() + enemies[i].GetRadius() > neighbors2[j].y * TILE_SIZE && enemies[i].GetX() - enemies[i].GetRadius() <= neighbors2[j].x * TILE_SIZE + TILE_SIZE && enemies[i].GetPrevX() - enemies[i].GetRadius() + 3 >= neighbors2[j].x * TILE_SIZE + TILE_SIZE)  enemies[i].SetPos(neighbors2[j].x * TILE_SIZE + TILE_SIZE + enemies[i].GetRadius() + 2, enemies[i].GetY());
					if (enemies[i].GetX() - enemies[i].GetRadius() < neighbors2[j].x * TILE_SIZE + TILE_SIZE && enemies[i].GetX() + enemies[i].GetRadius() > neighbors2[j].x * TILE_SIZE && enemies[i].GetY() + enemies[i].GetRadius() >= neighbors2[j].y * TILE_SIZE && enemies[i].GetPrevY() <= neighbors2[j].y * TILE_SIZE) enemies[i].SetPos(enemies[i].GetX(), neighbors2[j].y * TILE_SIZE - enemies[i].GetRadius());
					else if (enemies[i].GetX() - enemies[i].GetRadius() < neighbors2[j].x * TILE_SIZE + TILE_SIZE && enemies[i].GetX() + enemies[i].GetRadius() > neighbors2[j].x * TILE_SIZE && enemies[i].GetY() - enemies[i].GetRadius() <= neighbors2[j].y * TILE_SIZE + TILE_SIZE && enemies[i].GetPrevY() - enemies[i].GetRadius() + 3 >= neighbors2[j].y * TILE_SIZE + TILE_SIZE) enemies[i].SetPos(enemies[i].GetX(), neighbors2[j].y * TILE_SIZE + TILE_SIZE + enemies[i].GetRadius() + 2);
				}
			}
			for (unsigned j = 0; j < neighbors.size(); j++) {
				if (neighbors[j].solid) {
					if (enemies[i].GetX() + enemies[i].GetRadius() > neighbors[j].x * TILE_SIZE && enemies[i].GetX() - enemies[i].GetRadius() < neighbors[j].x * TILE_SIZE + TILE_SIZE &&
						enemies[i].GetY() + enemies[i].GetRadius() > neighbors[j].y * TILE_SIZE && enemies[i].GetY() - enemies[i].GetRadius() < neighbors[j].y * TILE_SIZE + TILE_SIZE) {
						enemies[i].SetSlowness(.9);
					}
				}
			}
		}
	}

	neighbors.clear();
	if (player.GetYTile() > 0 && player.GetYTile() < map.GetMapHeight() - 1 && player.GetXTile() > 0 && player.GetXTile() < map.GetMapWidth() - 1) {
		neighbors.push_back(tiles[player.GetYTile() + 1][player.GetXTile()]);
		neighbors.push_back(tiles[player.GetYTile() - 1][player.GetXTile()]);
		neighbors.push_back(tiles[player.GetYTile()][player.GetXTile() + 1]);
		neighbors.push_back(tiles[player.GetYTile()][player.GetXTile() - 1]);
		neighbors.push_back(tiles[player.GetYTile() + 1][player.GetXTile() + 1]);
		neighbors.push_back(tiles[player.GetYTile() + 1][player.GetXTile() - 1]);
		neighbors.push_back(tiles[player.GetYTile() - 1][player.GetXTile() + 1]);
		neighbors.push_back(tiles[player.GetYTile() - 1][player.GetXTile() - 1]);
	}

	for (unsigned i = 0; i < neighbors.size(); i++) {
		if (neighbors[i].solid) {
			if (player.GetY() - player.GetRadius() + 3 < neighbors[i].y * TILE_SIZE + TILE_SIZE && player.GetY() + player.GetRadius() - 3 > neighbors[i].y * TILE_SIZE && player.GetX() + player.GetRadius() >= neighbors[i].x * TILE_SIZE && player.GetPrevX() <= neighbors[i].x * TILE_SIZE) {
				player.SetPos(neighbors[i].x * TILE_SIZE - player.GetRadius(), player.GetY());
				player.SetVelocity(0, player.GetVelY());
			}
			else if (player.GetY() - player.GetRadius() < neighbors[i].y * TILE_SIZE + TILE_SIZE && player.GetY() + player.GetRadius() > neighbors[i].y * TILE_SIZE && player.GetX() - player.GetRadius() <= neighbors[i].x * TILE_SIZE + TILE_SIZE && player.GetPrevX() - player.GetRadius() >= neighbors[i].x * TILE_SIZE + TILE_SIZE) {
				player.SetPos(neighbors[i].x * TILE_SIZE + TILE_SIZE + player.GetRadius(), player.GetY());
				player.SetVelocity(0, player.GetVelY());
			}
			if (player.GetX() - player.GetRadius() < neighbors[i].x * TILE_SIZE + TILE_SIZE && player.GetX() + player.GetRadius() > neighbors[i].x * TILE_SIZE && player.GetY() + player.GetRadius() >= neighbors[i].y * TILE_SIZE && player.GetPrevY() <= neighbors[i].y * TILE_SIZE) {
				player.SetPos(player.GetX(), neighbors[i].y * TILE_SIZE - player.GetRadius());
				player.SetVelocity(player.GetVelX(), 0);
			}
			else if (player.GetX() - player.GetRadius() < neighbors[i].x * TILE_SIZE + TILE_SIZE && player.GetX() + player.GetRadius() > neighbors[i].x * TILE_SIZE && player.GetY() - player.GetRadius() <= neighbors[i].y * TILE_SIZE + TILE_SIZE && player.GetPrevY() - player.GetRadius() >= neighbors[i].y * TILE_SIZE + TILE_SIZE) {
				player.SetPos(player.GetX(), neighbors[i].y * TILE_SIZE + TILE_SIZE + player.GetRadius());
				player.SetVelocity(player.GetVelX(), 0);
			}
		}
	}
}
void CollideBullets(Map &map, Player &player, vector<Bullet> &playerBullets, vector<Bullet> &enemyBullets, vector<Enemy> &enemies, vector<Particle> &particles) {
	vector<Map::Tile> neighbors;
	vector<vector<Map::Tile>> tiles = map.GetMap();

	for (unsigned i = 0; i < playerBullets.size(); i++) {
		if (playerBullets[i].GetTileCollidable()) {
			neighbors.clear();
			if (playerBullets[i].GetXTile() >= 0 && playerBullets[i].GetXTile() < map.GetMapWidth() && playerBullets[i].GetYTile() >= 0 && playerBullets[i].GetYTile() < map.GetMapHeight())
				neighbors.push_back(tiles[playerBullets[i].GetYTile()][playerBullets[i].GetXTile()]);
			if (playerBullets[i].GetXTile() > 0 && playerBullets[i].GetXTile() < map.GetMapWidth() - 1 && playerBullets[i].GetYTile() > 0 && playerBullets[i].GetYTile() < map.GetMapHeight() - 1) {
					neighbors.push_back(tiles[playerBullets[i].GetYTile() + 1][playerBullets[i].GetXTile()]);
					neighbors.push_back(tiles[playerBullets[i].GetYTile() - 1][playerBullets[i].GetXTile()]);
					neighbors.push_back(tiles[playerBullets[i].GetYTile()][playerBullets[i].GetXTile() + 1]);
					neighbors.push_back(tiles[playerBullets[i].GetYTile()][playerBullets[i].GetXTile() - 1]);
				}
				for (unsigned j = 0; j < neighbors.size(); j++) {
					if (neighbors[j].solid) {
						if (playerBullets[i].GetX() + playerBullets[i].GetRadius() > neighbors[j].x * TILE_SIZE && playerBullets[i].GetX() - playerBullets[i].GetRadius() < neighbors[j].x * TILE_SIZE + TILE_SIZE &&
							playerBullets[i].GetY() + playerBullets[i].GetRadius() > neighbors[j].y * TILE_SIZE && playerBullets[i].GetY() - playerBullets[i].GetRadius() < neighbors[j].y * TILE_SIZE + TILE_SIZE) {
							playerBullets[i].Kill(particles);
							//RemoveTile(map, enemies, neighbors[j].x, neighbors[j].y);
						}
					
				}
			}
		}

		for (unsigned j = 0; j < enemies.size(); j++) {
			float distance = GetDistance(enemies[j].GetX(), enemies[j].GetY(), playerBullets[i].GetX(), playerBullets[i].GetY());
			if (playerBullets[i].GetEntityCollidable() && distance < (enemies[j].GetRadius() + playerBullets[i].GetRadius()) * (enemies[j].GetRadius() + playerBullets[i].GetRadius())) {
				enemies[j].SetVelocityAngle(playerBullets[i].GetDir(), playerBullets[i].GetKnockback());
				enemies[j].Damage(playerBullets[i].GetDamage());
				enemies[j].SetEffect(playerBullets[i].GetEffect(), playerBullets[i].GetEffectTime());
				enemies[j].Anger();
				if (playerBullets[i].GetDieFromEntity())
					playerBullets[i].Kill(particles);
				if (playerBullets[i].GetType() == 9)
					enemies[j].SetPos(enemies[j].GetX() + rand() % 11 - 5, enemies[j].GetY() + rand() % 11 - 5);
			}
			if (playerBullets[i].GetEntityCollidable() && distance < (enemies[j].GetRadius() + playerBullets[i].GetRadius() + 20) * (enemies[j].GetRadius() + playerBullets[i].GetRadius() + 20)) {
				if (enemies[j].GetFollowing() && playerBullets[i].GetDodgable() && rand() % 5 == 0)
					enemies[j].AddVelocityAngle(GetAngle(enemies[j].GetX(), enemies[j].GetY(), playerBullets[i].GetX(), playerBullets[i].GetY()) + (rand() % 1501 - 7500) * .001, 1);
			}
			if (playerBullets[i].GetType() == 9) {
				if (distance < 25000) {
					enemies[j].AddVelocityAngle(GetAngle(playerBullets[i].GetX(), playerBullets[i].GetY(), enemies[j].GetX(), enemies[j].GetY()), 10000 / distance);
				}
			}
			if (playerBullets[i].GetType() == 9 && enemies[j].GetType() == 3 && distance < (enemies[j].GetRadius() + playerBullets[i].GetRadius()) * (enemies[j].GetRadius() + playerBullets[i].GetRadius()))
				playerBullets[i].Kill(particles);
		}
	}
	
	for (unsigned i = 0; i < enemyBullets.size(); i++) {
		if (!player.GetFire() && enemyBullets[i].GetEntityCollidable() && GetDistance(enemyBullets[i].GetX(), enemyBullets[i].GetY(), player.GetX(), player.GetY()) < (player.GetRadius() + enemyBullets[i].GetRadius()) * (player.GetRadius() + enemyBullets[i].GetRadius())) {
			if (enemyBullets[i].GetType() != 2)
				player.SetVelocityAngle(enemyBullets[i].GetDir(), enemyBullets[i].GetKnockback());
			else
				player.SetVelocityAngle(GetAngle(player.GetX(), player.GetY(), enemyBullets[i].GetX(), enemyBullets[i].GetY()), enemyBullets[i].GetKnockback());
			player.Damage(enemyBullets[i].GetDamage());
			if (enemyBullets[i].GetEffect() == 3) {
				player.SetDarknessTime(100);
			}
			enemyBullets[i].Kill(particles);
		}

		if (enemyBullets[i].GetX() > 0 && enemyBullets[i].GetX() < map.GetMapWidth() * TILE_SIZE && enemyBullets[i].GetY() > 0 && enemyBullets[i].GetY() < map.GetMapHeight() * TILE_SIZE) {
			if (enemyBullets[i].GetTileCollidable()) {
				neighbors.clear();
				neighbors.push_back(tiles[enemyBullets[i].GetYTile()][enemyBullets[i].GetXTile()]);
				if (enemyBullets[i].GetYTile() > 0 && enemyBullets[i].GetYTile() < map.GetMapHeight() - 1 && enemyBullets[i].GetXTile() > 0 && enemyBullets[i].GetXTile() < map.GetMapWidth() - 1) {
					neighbors.push_back(tiles[enemyBullets[i].GetYTile() + 1][enemyBullets[i].GetXTile()]);
					neighbors.push_back(tiles[enemyBullets[i].GetYTile() - 1][enemyBullets[i].GetXTile()]);
					neighbors.push_back(tiles[enemyBullets[i].GetYTile()][enemyBullets[i].GetXTile() + 1]);
					neighbors.push_back(tiles[enemyBullets[i].GetYTile()][enemyBullets[i].GetXTile() - 1]);
				}
				for (unsigned j = 0; j < neighbors.size(); j++) {
					if (neighbors[j].solid) {
						if (enemyBullets[i].GetX() + enemyBullets[i].GetRadius() > neighbors[j].x * TILE_SIZE && enemyBullets[i].GetX() - enemyBullets[i].GetRadius() < neighbors[j].x * TILE_SIZE + TILE_SIZE &&
							enemyBullets[i].GetY() + enemyBullets[i].GetRadius() > neighbors[j].y * TILE_SIZE && enemyBullets[i].GetY() - enemyBullets[i].GetRadius() < neighbors[j].y * TILE_SIZE + TILE_SIZE) {
							enemyBullets[i].Kill(particles);
							//RemoveTile(map, enemies, neighbors[j].x, neighbors[j].y);
						}
					}
				}
			}
		}
	}
}
void UpdateCamera(Player player, Map map, float cameraPos[2]) {
	cameraPos[0] = player.GetX() - SCREEN_WIDTH / 2;
	cameraPos[1] = player.GetY() - SCREEN_HEIGHT / 2;

	if (cameraPos[0] < 0) cameraPos[0] = 0;
	else if (cameraPos[0] + SCREEN_WIDTH > map.GetMapWidth() * TILE_SIZE) cameraPos[0] = map.GetMapWidth() * TILE_SIZE - SCREEN_WIDTH;
	if (cameraPos[1] < 0) cameraPos[1] = 0;
	else if (cameraPos[1] + SCREEN_HEIGHT > map.GetMapHeight() * TILE_SIZE) cameraPos[1] = map.GetMapHeight() * TILE_SIZE - SCREEN_HEIGHT;
}
void CleanUp(Map map, Player player, vector<Enemy> &enemies, vector<Bullet> &playerBullets, vector<Bullet> &enemyBullets, vector<Particle> &particles, int enemyCount, ALLEGRO_DISPLAY *display, ALLEGRO_BITMAP *enemyImages, float cameraPos[2], int wave) {
	float lastEnemyX = -1, lastEnemyY = -1;
	for (unsigned i = 0; i < enemies.size();) {
		if (!enemies[i].GetActive()) {
			if (enemies[i].GetType() == 2) {
				for (int j = 0; j < 5; j++)
					enemyBullets.push_back(Bullet(enemies[i].GetX() + rand() % 201 - 100, enemies[i].GetY() + rand() % 201 - 100, 0, 0, -1, 2));
				for (int j = 0; j < 10; j++)
					enemyBullets.push_back(Bullet(enemies[i].GetX(), enemies[i].GetY(), rand() % 360 * (M_PI / 180), rand() % 5000 * .001 + 3, rand() % 30 + 10, 3));
			}
			else if (enemies[i].GetType() == 7) {
				for (int j = 0; j < 5; j++)
					playerBullets.push_back(Bullet(enemies[i].GetX() + rand() % 201 - 100, enemies[i].GetY() + rand() % 201 - 100, 0, 0, -1, 2));
				for (int j = 0; j < 30; j++)
					playerBullets.push_back(Bullet(enemies[i].GetX(), enemies[i].GetY(), rand() % 360 * (M_PI / 180), rand() % 5000 * .001 + 3, rand() % 30 + 10, 3));
				for (unsigned j = 0; j < enemies.size(); j++)
					enemies[j].SetHealth(0);
			}

			if (enemies.size() + enemyCount == 1  && enemies[i].GetType() != 3 && wave != 10) {
				lastEnemyX = enemies[i].GetX();
				lastEnemyY = enemies[i].GetY();
			}
			else if (enemies[i].GetType() == 7) {
				GAME_STATE = 3;
				for (unsigned j = 0; j < enemyBullets.size(); j++)
					enemyBullets[j].Kill(particles);
			}
			
			if (enemies[i].GetImage() != -1)
				BitmapToParticles(particles, enemyImages, display, enemies[i].GetX(), enemies[i].GetY(), enemies[i].GetFrame(), enemies[i].GetImage(), enemies[i].GetImageSize(), enemies[i].GetImageSize(), enemies[i].GetDir(), enemies[i].GetVelX(), enemies[i].GetVelY(), cameraPos);
			enemies.erase(enemies.begin() + i);

		}
		else
			i++;
	}
	for (unsigned i = 0; i < playerBullets.size();) {
		if (!playerBullets[i].GetActive()) {
			if (playerBullets[i].GetType() == 8)
				playerBullets.push_back(Bullet(playerBullets[i].GetX(), playerBullets[i].GetY(), 0, 0, 300, 9));
			playerBullets.erase(playerBullets.begin() + i);
		}
		else
			i++;
	}
	for (unsigned i = 0; i < enemyBullets.size();) {
		if (!enemyBullets[i].GetActive()) {
			if (enemyBullets[i].GetType() == 6) {
				enemies.push_back(Enemy(map, 5, enemyBullets[i].GetX(), enemyBullets[i].GetY(), false));
				enemies.back().SetVelocityAngle(enemyBullets[i].GetDir(), 10);
			}
			else if (enemyBullets[i].GetType() == 10) {
				if (enemies.size() < 5)
					enemies.push_back(Enemy(map, 4, enemyBullets[i].GetX(), enemyBullets[i].GetY(), false));
			}
			enemyBullets.erase(enemyBullets.begin() + i);
		}
		else
			i++;
	}
	for (unsigned i = 0; i < particles.size();) {
		if (!particles[i].GetActive())
			particles.erase(particles.begin() + i);
		else
			i++;
	}

	if (lastEnemyX != -1 && lastEnemyY != -1) {
		enemies.push_back(Enemy(map, 3, lastEnemyX, lastEnemyY, false));
		cout << "Portal Created" << endl;
	}
}
void UpdateWave(int wave, int &enemyCount, int &maxEnemies) {
	enemyCount = (pow(wave, 2) + 4) / 4 + 3;// +rand() % (wave / 2 + 1);
	maxEnemies = enemyCount * .5 + 1;
	if (wave == 10) {
		enemyCount = 1;
		maxEnemies = 1;
	}
	//maxEnemies = 1;
}
vector<int> InitEnemyTypes() {
	vector<int> types;
	types.push_back(0);
	types.push_back(1);
	types.push_back(2);
	types.push_back(4);
	types.push_back(6);

	//types.clear(); types.push_back(4);
	return types;
}
unordered_map<int, string> InitEnemyTitles() {
	unordered_map<int, string> temp;
	temp[0] = "Wolves";
	temp[1] = "Zombies";
	temp[2] = "Explosive Spiders";
	temp[4] = "Wizards";
	temp[6] = "Wraiths";
	temp[-1] = "All Enemies";
	temp[7] = "Final Boss";
	return temp;
}
unordered_map<int, string> InitWeaponTitles() {
	unordered_map<int, string> temp;
	temp[0] = "Fireball";
	temp[1] = "Lightning";
	temp[2] = "Mini Blackhole";
	temp[3] = "Fire Blast";
	temp[4] = "Heal";
	return temp;
}
void AddAvailableEnemy(vector<int> &enemyTypes, vector<int> &availableEnemimes) {
	int type = rand() % enemyTypes.size();
	availableEnemimes.push_back(enemyTypes[type]);
	cout << "Added enemy " << enemyTypes[type] << endl;
	enemyTypes.erase(enemyTypes.begin() + type);
}
void Draw(float cameraPos[], Player player, Map map, vector<Bullet> playerBullets, vector<Bullet> enemyBullets, vector<Enemy> enemies, vector<Particle> particles, ALLEGRO_BITMAP *bulletImages, ALLEGRO_BITMAP *enemyImages, ALLEGRO_BITMAP *particleImages, ALLEGRO_BITMAP *mouseImage, ALLEGRO_BITMAP *darknessImage, ALLEGRO_DISPLAY *display, ALLEGRO_FONT *font40, ALLEGRO_FONT *font10, ALLEGRO_FONT *font12, ALLEGRO_FONT *font20, ALLEGRO_FONT *font80, POINT mousePos, int startTimer, int waveTimer, int wave, int lastWave, int enemyCount, int transition, int displayEnemy, unordered_map<int, string> enemyTitles, unordered_map<int, string> weaponTitles, int introTime) {
	al_clear_to_color(al_map_rgb(0, 0, 0));

	ALLEGRO_TRANSFORM camera;
	al_identity_transform(&camera);
	al_translate_transform(&camera, -cameraPos[0], -cameraPos[1]);
	al_use_transform(&camera);

	if (DEBUG)
		al_draw_filled_rectangle(player.GetXTile() * TILE_SIZE, player.GetYTile() * TILE_SIZE, player.GetXTile() * TILE_SIZE + TILE_SIZE, player.GetYTile() * TILE_SIZE + TILE_SIZE, al_map_rgb(255, 0, 255));

	map.Draw(cameraPos);

	al_hold_bitmap_drawing(true);
	for (auto enemy : enemies)
		enemy.Draw(enemyImages, display, cameraPos);
	al_hold_bitmap_drawing(false);

	player.Draw(display);

	al_hold_bitmap_drawing(true);
	for (auto particle : particles)
		particle.Draw(particleImages, display, cameraPos);
	al_hold_bitmap_drawing(false);

	al_hold_bitmap_drawing(true);
	for (unsigned i = 0; i < playerBullets.size(); i++)
		playerBullets[i].Draw(bulletImages, display, cameraPos);
	
	for (unsigned i = 0; i < enemyBullets.size(); i++)
		enemyBullets[i].Draw(bulletImages, display, cameraPos);
	al_hold_bitmap_drawing(false);
	

	if (DEBUG) {
		for (auto enemy : enemies)
			enemy.DrawDebug();
		for (auto bullet : playerBullets)
			bullet.DrawDebug();
		for (auto bullet : enemyBullets)
			bullet.DrawDebug();
		for (auto particle : particles)
			particle.DrawDebug();
	}

	if (player.GetDarkness() > 0) {
		//al_draw_bitmap(darknessImage, player.GetX() - al_get_bitmap_width(darknessImage) / 2, player.GetY() - al_get_bitmap_height(darknessImage) / 2, NULL);
		al_draw_tinted_bitmap(darknessImage, al_map_rgba(255 * player.GetDarkness(), 255 * player.GetDarkness(), 255 * player.GetDarkness(), player.GetDarkness()), player.GetX() - al_get_bitmap_width(darknessImage) / 2, player.GetY() - al_get_bitmap_height(darknessImage) / 2, NULL);
	}

	al_translate_transform(&camera, cameraPos[0], cameraPos[1]);
	al_use_transform(&camera);

	player.DrawHealthBar(font12);

	al_draw_bitmap(mouseImage, mousePos.x - al_get_bitmap_width(mouseImage) / 2, mousePos.y - al_get_bitmap_height(mouseImage) / 2, NULL);
	//al_draw_textf(font18, al_map_rgb(255, 0, 255), 2, 2, NULL, "X:%f Y:%f", player.GetX(), player.GetY());

	
	if (GAME_STATE == 0) {
		if (startTimer > 0) {
			if (startTimer > 100)
				al_draw_textf(font40, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "%i", startTimer / 100);
			else
				al_draw_textf(font40, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "GO");
		}
		else {
			al_draw_text(font40, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "Press space to start");
			al_draw_text(font20, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 50, ALLEGRO_ALIGN_CENTER, "WASD to move");
			al_draw_text(font20, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 70, ALLEGRO_ALIGN_CENTER, "Mouse to aim and shoot");
			al_draw_text(font20, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 90, ALLEGRO_ALIGN_CENTER, "1-5 to change spell");
		}
	}
	else if (GAME_STATE == 1) {
		if (waveTimer > 0) {
			al_draw_textf(font40, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "Wave %i/%i", wave, lastWave);
			al_draw_textf(font20, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 50, ALLEGRO_ALIGN_CENTER, "%s", enemyTitles[displayEnemy].c_str());
			if (wave > 1 && wave <= 5)
				al_draw_textf(font20, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 70, ALLEGRO_ALIGN_CENTER, "Unlocked %s on key %i", weaponTitles[player.GetWeaponID()].c_str(), player.GetWeaponID() + 1);
		}
	}
	else if (GAME_STATE == 2) {
		al_draw_text(font40, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "Press space to restart");
		if (wave > 5)
			al_draw_text(font20, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 50, ALLEGRO_ALIGN_CENTER, "Press P to start on wave 6");
	}
	else if (GAME_STATE == 3) {
		al_draw_text(font40, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, ALLEGRO_ALIGN_CENTER, "You Win");
		al_draw_text(font20, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 + 50, ALLEGRO_ALIGN_CENTER, "Press space to restart");
	}
	if (GAME_STATE != 0) {
		if (wave != 10 || wave == 10 && (enemies.size() > 0 && enemies[0].GetType() != 7)) {
			al_draw_textf(font12, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, 3, ALLEGRO_ALIGN_CENTER, "Enemies Left: %i", enemies.size() > 0 && enemies[0].GetType() != 3 ? enemyCount + enemies.size() : 0);
		}
		else if (enemies.size() > 0 && enemies[0].GetType() == 7) {
			float healthPercent = enemies[0].GetHealth() / enemies[0].GetStartHealth();
			int place = 13;
			al_draw_filled_rectangle(SCREEN_WIDTH / 2 - 100, place, SCREEN_WIDTH / 2 + 100, place + 4, al_map_rgb(255, 0, 0));

			al_draw_filled_rectangle(SCREEN_WIDTH / 2, place, SCREEN_WIDTH / 2 + 100 * healthPercent, place + 4, al_map_rgb(0, 255, 0));
			al_draw_filled_rectangle(SCREEN_WIDTH / 2, place, SCREEN_WIDTH / 2 - 100 * healthPercent, place + 4, al_map_rgb(0, 255, 0));

			al_draw_rectangle(SCREEN_WIDTH / 2 - 100, place, SCREEN_WIDTH / 2 + 100, place + 4, al_map_rgb(0, 0, 0), 1);
			al_draw_text(font12, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2, 2, ALLEGRO_ALIGN_CENTER, "Boss Health");
		}
	}

	if (DEBUG) {
		al_draw_text(font10, al_map_rgb(255, 255, 255), 0, 0, NULL, "Debug Mode");
		al_draw_textf(font10, al_map_rgb(255, 255, 255), 0, 10, NULL, "PlayerX:%f PlayerY:%f PlayerVelX:%f PlayerVelY:%f", player.GetX(), player.GetY(), player.GetVelX(), player.GetVelY());
		al_draw_textf(font10, al_map_rgb(255, 255, 255), 0, 20, NULL, "CameraX:%f CameraY:%f", cameraPos[0], cameraPos[1]);
		al_draw_textf(font10, al_map_rgb(255, 255, 255), 0, 30, NULL, "Enemies:%i Bullets:%i", enemies.size(), enemyBullets.size() + playerBullets.size());
		al_draw_filled_circle(mousePos.x, mousePos.y, 4, al_map_rgb(255, 0, 255));
	}

	if (transition > 0)
		al_draw_filled_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, al_map_rgba(0, 0, 0, transition));

	if (GAME_STATE == -1) {
		al_draw_text(font80, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2 - al_get_text_width(font80, "Wizard ") / 2 - (introTime > 400 ? 0 : (400 - introTime) * 1), SCREEN_HEIGHT / 2 - 40, ALLEGRO_ALIGN_CENTER, "Wizard");
		al_draw_text(font80, al_map_rgb(255, 0, 0), SCREEN_WIDTH / 2 + al_get_text_width(font80, "Wizard ") / 2 + (introTime > 400 ? 0 : (400 - introTime) * 1), SCREEN_HEIGHT / 2 - 40, ALLEGRO_ALIGN_CENTER, "Wars");
		//al_draw_filled_circle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, (introTime - 200) / 4, al_map_rgba_f(0, 0, 0, 1 * introTime / 2000));
	}

	al_flip_display();
}
void Reset(Player &player, vector<Enemy> &enemies, vector<Bullet> &playerBullets, vector<Bullet> &enemyBullets, vector<Particle> &particles, Map &map, int &enemyCount, int &maxEnemies, vector<int> &enemyTypes, vector<int> &availableEnemies, int &wave, int &startTimer, int &waveTimer, int &transition) {
	map.GenerateMap();
	player.Reset(map);
	enemies.clear();
	playerBullets.clear();
	enemyBullets.clear();
	particles.clear();
	wave = 1;
	startTimer = 0;
	waveTimer = 0;
	transition = 0;
	UpdateWave(wave, enemyCount, maxEnemies);
	enemyTypes = InitEnemyTypes();
	availableEnemies.clear();
}

int main() {
	srand((unsigned)time(0));
	float FPS;
	bool done, redraw;
	bool keys[4];
	float cameraPos[2];
	POINT mousePos;
	bool mouseDown;
	int enemyCount, maxEnemies;
	int startTimer, startTimerStart;
	int wave, lastWave, waveTimerStart, waveTimer;
	int transition;
	int introTime;

	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE * event_queue;
	ALLEGRO_TIMER *timer;

	vector<Enemy> enemies;
	vector<Bullet> playerBullets;
	vector<Bullet> enemyBullets;
	vector<Particle> particles;
	vector<int> enemyTypes = InitEnemyTypes();
	vector<int> availableEnemies;
	unordered_map<int, string> enemyTitles = InitEnemyTitles();
	unordered_map<int, string> weaponTitles = InitWeaponTitles();
	
	Map map = Map();
	Player player = Player();

	done = false;
	redraw = true;
	FPS = 1.0 / 60.0;
	keys[UP] = false;
	keys[DOWN] = false;
	keys[LEFT] = false;
	keys[RIGHT] = false;
	cameraPos[0] = 0;
	cameraPos[1] = 0;
	mousePos.x = mousePos.y = 0;
	mouseDown = false;
	startTimer = 0;
	startTimerStart = 400;
	wave = 1;
	lastWave = 10;
	waveTimerStart = 200;
	waveTimer = 0;
	enemyCount = 0;
	maxEnemies = 0;
	transition = 230;
	introTime = 2000;
	
	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();

	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	event_queue = al_create_event_queue();
	timer = al_create_timer(FPS);
	ALLEGRO_FONT *font10 = al_load_font("Resources/Fonts/arial.ttf", 10, NULL);
	ALLEGRO_FONT *font12 = al_load_font("Resources/Fonts/Minecraft.ttf", 12, NULL);
	ALLEGRO_FONT *font18 = al_load_font("Resources/Fonts/arial.ttf", 18, NULL);
	ALLEGRO_FONT *font40 = al_load_font("Resources/Fonts/Minecraft.ttf", 40, NULL);
	ALLEGRO_FONT *font80 = al_load_font("Resources/Fonts/Minecraft.ttf", 80, NULL);
	ALLEGRO_FONT *font20 = al_load_font("Resources/Fonts/Minecraft.ttf", 20, NULL);
	ALLEGRO_BITMAP *bulletImages = al_load_bitmap("Resources/Images/bullet sprite sheet.png");
	ALLEGRO_BITMAP *enemyImages = al_load_bitmap("Resources/Images/enemy sprite sheet.png");
	ALLEGRO_BITMAP *particleImages = al_load_bitmap("Resources/Images/particle sprite sheet.png");
	ALLEGRO_BITMAP *mouseImage = al_load_bitmap("Resources/Images/mouse.png");
	ALLEGRO_BITMAP *darknessImage = al_load_bitmap("Resources/Images/darkness.png");

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_hide_mouse_cursor(display);
	map.Init();
	map.GenerateMap();
	player.Init(map.GetMapWidth() * TILE_SIZE / 2, map.GetMapHeight() * TILE_SIZE / 2);
	player.Spawn(map);
	UpdateCamera(player, map, cameraPos);
	UpdateWave(wave, enemyCount, maxEnemies);

	cout << "Starting Game..." << endl;

	al_start_timer(timer);

	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			redraw = true;

			if (GAME_STATE == -1) {
				if (introTime > 0)
					introTime -= 10;
				else if (transition > 0) {
					transition -= 5;
				}
				else
					GAME_STATE = 0;
			}
			if (GAME_STATE == 0) {
				if (startTimer > 0) {
					startTimer -= 3;
					if (startTimer <= 0) {
						GAME_STATE = 1;
						startTimer = 0;
						waveTimer = waveTimerStart;
						if (wave != 10)
							AddAvailableEnemy(enemyTypes, availableEnemies);
						//availableEnemies.clear(); availableEnemies.push_back(7);
					}
				}
			}
			
			if (transition > 0 && enemies.size() == 0 && GAME_STATE != -1) {
				transition -= 5;
				if (transition < 0)
					transition = 0;
			}

			if (GAME_STATE != -1 && waveTimer > 0) {
				waveTimer--;
				if (waveTimer <= 0) {
					waveTimer = 0;
					UpdateWave(wave, enemyCount, maxEnemies);
				}
			}
			
			if (GAME_STATE == 1 && waveTimer == 0 && startTimer == 0 && rand() % 50 == 0 && enemies.size() < (unsigned)maxEnemies && enemyCount > 0) {
				enemies.push_back(Enemy(map, availableEnemies[rand() % availableEnemies.size()], player.GetX(), player.GetY(), true));
				cout << "Added Enemy! size: " << enemies.size() << endl;
				enemyCount--;
			}

			if (enemyCount + enemies.size() == 0 && wave != 10) {
				waveTimer = waveTimerStart;
				wave++;
				UpdateWave(wave, enemyCount, maxEnemies);
				if (wave > 1) {
					enemyBullets.clear();
					playerBullets.clear();
					particles.clear();
					map.GenerateMap();
					player.Spawn(map);
					player.AddWeapon(0);
				}
				if (wave <= 5) {
					availableEnemies.clear();
					AddAvailableEnemy(enemyTypes, availableEnemies);
				}
				else if (wave < 10) {
					enemyTypes = InitEnemyTypes();
					availableEnemies = enemyTypes;
				}
				else if (wave == 10) {
					availableEnemies.clear();
					availableEnemies.push_back(7);
				}
			}

			player.Update(keys, mousePos, cameraPos, mouseDown, playerBullets, particles);
			for (unsigned i = 0; i < enemies.size(); i++)
				enemies[i].Update(player.GetHealth() > 0 ? player.GetX() : -1000, player.GetHealth() > 0 ? player.GetY() : -1000, map, enemyBullets, particles);
			for (unsigned i = 0; i < playerBullets.size(); i++) {
				playerBullets[i].Update(map, particles, enemies);
				if (playerBullets[i].GetType() == 4) {
					while (playerBullets[i].GetFrame() == 1) {
						playerBullets[i].Update(map, particles, enemies);
						CollideBullets(map, player, playerBullets, enemyBullets, enemies, particles);
						CollideBullets(map, player, playerBullets, enemyBullets, enemies, particles);
					}
					playerBullets[i].Update(map, particles, enemies);
					if (playerBullets[i].GetData() > 0) {
						for (int j = 0; j < rand() % 2; j++) {
							playerBullets.push_back(Bullet(playerBullets[i].GetX(), playerBullets[i].GetY(), playerBullets[i].GetDir() + (rand() % 1001 - 500) * .001, playerBullets[i].GetSpeed(), rand() % 20 + 5, 4));
							playerBullets.back().SetData(playerBullets[i].GetData() - 1);
						}
					}
				}
			}
			for (unsigned i = 0; i < enemyBullets.size(); i++)
				enemyBullets[i].Update(map, particles, enemies);
			for (unsigned i = 0; i < particles.size(); i++)
				particles[i].Update();
			CollideBullets(map, player, playerBullets, enemyBullets, enemies, particles);
			Collide(map, player, enemies, particles, transition);
			UpdateCamera(player, map, cameraPos);

			CleanUp(map, player, enemies, playerBullets, enemyBullets, particles, enemyCount, display, enemyImages, cameraPos, wave);

			if (player.GetHealth() <= 0)
				GAME_STATE = 2;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_W:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_1:
				player.SetWeapon(0);
				break;
			case ALLEGRO_KEY_2:
				player.SetWeapon(1);
				break;
			case ALLEGRO_KEY_3:
				player.SetWeapon(2);
				break;
			case ALLEGRO_KEY_4:
				player.SetWeapon(3);
				break;
			case ALLEGRO_KEY_5:
				player.SetWeapon(4);
				break;
			case ALLEGRO_KEY_F1:
				enemies.clear();
				break;
			case ALLEGRO_KEY_F2:
				player.SetHealth(0);
				break;
			case ALLEGRO_KEY_F3:
				DEBUG = !DEBUG;
				break;
			case ALLEGRO_KEY_F4:
				player.AddWeapon(-1);
				break;
			case ALLEGRO_KEY_F5:
				if (GAME_STATE == 2) {
					GAME_STATE = 0;
					Reset(player, enemies, playerBullets, enemyBullets, particles, map, enemyCount, maxEnemies, enemyTypes, availableEnemies, wave, startTimer, waveTimer, transition);
					wave = 10;
					for (int i = 0; i < 4; i++)
						player.AddWeapon(-1);
					availableEnemies.clear();
					availableEnemies.push_back(7);
					UpdateWave(wave, enemyCount, maxEnemies);
				}
				break;
			case ALLEGRO_KEY_F6:
				enemies.push_back(Enemy(map, 3, player.GetX(), player.GetY(), false));
				wave++;
				waveTimer = waveTimerStart;
				break;
			case ALLEGRO_KEY_F7:
				al_save_bitmap(string("Screenshot" + to_string((long)rand() % 9001 + 100) + ".png").c_str(), al_get_target_bitmap());
				cout << "Saved Screenshot" << endl;
				break;
			case ALLEGRO_KEY_SPACE:
				if (GAME_STATE == 0 && startTimer == 0)
					startTimer = startTimerStart;
				else if (GAME_STATE == 2 || GAME_STATE == 3) {
					Reset(player, enemies, playerBullets, enemyBullets, particles, map, enemyCount, maxEnemies, enemyTypes, availableEnemies, wave, startTimer, waveTimer, transition);
					GAME_STATE = 0;
					wave = 1;
					UpdateWave(wave, enemyCount, maxEnemies);
				}
				break;
			case ALLEGRO_KEY_P:
				if (GAME_STATE == 2) {
					GAME_STATE = 0;
					Reset(player, enemies, playerBullets, enemyBullets, particles, map, enemyCount, maxEnemies, enemyTypes, availableEnemies, wave, startTimer, waveTimer, transition);
					wave = 6;
					availableEnemies = enemyTypes;
					for (int i = 0; i < 4; i++)
						player.AddWeapon(-1);
					UpdateWave(wave, enemyCount, maxEnemies);
				}
				break;
			case ALLEGRO_KEY_Z:
				enemies.push_back(Enemy(map, 2, player.GetX(), player.GetY(), true));
				cout << "Added Enemy! size: " << enemies.size() << endl;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_W:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_A:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_D:
				keys[RIGHT] = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
			mousePos.x = ev.mouse.x;
			mousePos.y = ev.mouse.y;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			mouseDown = true;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			mouseDown = false;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			Draw(cameraPos, player, map, playerBullets, enemyBullets, enemies, particles, bulletImages, enemyImages, particleImages, mouseImage, darknessImage, display, font40, font10, font12, font20, font80, mousePos, startTimer, waveTimer, wave, lastWave, enemyCount, transition, availableEnemies.size() == 1 ? availableEnemies[0] : -1, enemyTitles, weaponTitles, introTime);
		}
	}

	al_destroy_font(font18);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display);
}