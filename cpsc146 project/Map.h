#pragma once
#include "Globals.h"

class Map {
public:
	struct Tile {
		int x, y, type;
		int image, orientation;
		bool solid;
		bool enemy;
	};
private:
	vector<vector<Tile>> tiles;
	float emptyPercent;
	int mapWidth, mapHeight;
	int backgroundWidth, backgroundHeight;

	ALLEGRO_BITMAP *backgroundImage;
	ALLEGRO_BITMAP *rockImage;

public:
	Map();
	~Map();

	void Init();
	void Draw(float cameraPos[2]);
	vector<vector<Tile>> GetMap();
	//vector<vector<Tile>> GetMapWithEnemies(vector<Enemy> enemies, Player player, int xTile, int yTile);
	float GetLevelPercent();
	void GenerateMap();
	int GetMapWidth();
	int GetMapHeight();

	void KillTile(int x, int y);
};