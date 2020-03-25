#include "Map.h"

Map::Map() {
	emptyPercent = 0;
	mapWidth = 0;
	mapHeight = 0;
	backgroundWidth = 0;
	backgroundHeight = 0;
}

Map::~Map() {

}

void Map::Init() {
	cout << "Initializing Map..." << endl;
	backgroundImage = al_load_bitmap("Resources/Images/sand_back.png");
	rockImage = al_load_bitmap("Resources/Images/rock_sheet.png");
	emptyPercent = .4;
	mapWidth = SCREEN_WIDTH / TILE_SIZE * 2;
	mapHeight = SCREEN_HEIGHT / TILE_SIZE * 2;

	backgroundWidth = al_get_bitmap_width(backgroundImage);
	backgroundHeight = al_get_bitmap_height(backgroundImage);
}

void Map::Draw(float cameraPos[2]) {
	al_hold_bitmap_drawing(true);
	for (int y = 0; y < mapHeight * TILE_SIZE; y += backgroundHeight) {
		for (int x = 0; x < mapWidth * TILE_SIZE; x += backgroundWidth) {
			if (x + backgroundWidth > cameraPos[0] && x < cameraPos[0] + SCREEN_WIDTH && y + backgroundHeight > cameraPos[1] && y < cameraPos[1] + SCREEN_HEIGHT) {
				al_draw_bitmap(backgroundImage, x, y, NULL);
			}
		}
	}
	al_hold_bitmap_drawing(false);
	int startY = cameraPos[1] / TILE_SIZE ;
	int endY = (cameraPos[1] + SCREEN_HEIGHT) / TILE_SIZE + 1 < tiles.size() ? (cameraPos[1] + SCREEN_HEIGHT) / TILE_SIZE + 1 : tiles.size();
	int startX = cameraPos[0] / TILE_SIZE;
	int endX = (cameraPos[0] + SCREEN_WIDTH) / TILE_SIZE + 1 < tiles[0].size() ? (cameraPos[0] + SCREEN_WIDTH) / TILE_SIZE + 1 : tiles[0].size();

	al_hold_bitmap_drawing(true);
	for (int y = startY; y < endY; y++) {
		for (int x = startX; x < endX; x++) {
			if (tiles[y][x].solid) {
				if (x > 0 && x < mapWidth - 1 && y > 0 && y < mapHeight - 1 &&
					tiles[y - 1][x].solid && tiles[y + 1][x].solid && tiles[y][x - 1].solid && tiles[y][x + 1].solid ||
						x == 0 || y == 0 || x == mapWidth - 1 || y == mapHeight - 1)
						al_draw_bitmap_region(rockImage, 0, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, NULL);
				else if (x > 0 && x < mapWidth - 1 && y > 0 && y < mapHeight - 1) {
						al_draw_bitmap_region(rockImage, TILE_SIZE, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, NULL);
						
						if (tiles[y][x - 1].solid) {
							al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_HORIZONTAL);
							al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);
						}
						if (tiles[y][x + 1].solid) {
							al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, NULL);
							al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_VERTICAL);
						}
						if (tiles[y - 1][x].solid) {
							al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, NULL);
							al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_HORIZONTAL);
						}
						if (tiles[y + 1][x].solid) {
							al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_VERTICAL);
							al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);
						}
						if (!tiles[y][x - 1].solid) {
							if (tiles[y - 1][x - 1].solid)
								al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE - TILE_SIZE, y * TILE_SIZE, NULL);
							if (tiles[y + 1][x - 1].solid)
								al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE - TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_VERTICAL);
						}
						if (!tiles[y][x + 1].solid) {
							if (tiles[y - 1][x + 1].solid)
								al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_HORIZONTAL);
							if (tiles[y + 1][x + 1].solid)
								al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);
						}
						if (!tiles[y - 1][x].solid) {
							if (tiles[y - 1][x - 1].solid)
								al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE - TILE_SIZE, ALLEGRO_FLIP_HORIZONTAL | ALLEGRO_FLIP_VERTICAL);
							if (tiles[y - 1][x + 1].solid)
								al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE - TILE_SIZE, ALLEGRO_FLIP_VERTICAL);
						}
						if (!tiles[y + 1][x].solid) {
							if (tiles[y + 1][x - 1].solid)
								al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE, ALLEGRO_FLIP_HORIZONTAL);
							if (tiles[y + 1][x + 1].solid)
								al_draw_bitmap_region(rockImage, TILE_SIZE * 2, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE, NULL);
						}
				}
				al_draw_bitmap_region(rockImage, 48 * tiles[y][x].image, 0, TILE_SIZE, TILE_SIZE, x * TILE_SIZE, y * TILE_SIZE, tiles[y][x].orientation);
			}
		}
	}
	al_hold_bitmap_drawing(false);
}

float Map::GetLevelPercent() {
	float total = 1;
	float emptyNum = 0;
	
	for (unsigned y = 0; y < tiles.size(); y++) {
		for (unsigned x = 0; x < tiles[0].size(); x++) {
			if (!tiles[y][x].solid)
				emptyNum++;
			total++;
		}
	}

	return emptyNum / total;
}

void Map::GenerateMap() {
	tiles.clear();
	cout << "Generating Level..." << endl;

	for (int y = 0; y < mapHeight; y++) {
		vector<Tile> levelY;
		for (int x = 0; x < mapWidth; x++) {
			Tile temp;
			temp.solid = true;
			temp.enemy = false;
			temp.type = 0;
			temp.x = x;
			temp.y = y;
			temp.image = rand() % 15 + 3;
			temp.orientation = rand() % 3;
			levelY.push_back(temp);
		}
		tiles.push_back(levelY);
	}

	POINT pos;
	pos.x = mapWidth / 2;
	pos.y = mapHeight / 2;
	int count = 0;
	
	while (GetLevelPercent() < emptyPercent) {
		if (rand() % 2 == 0)
			pos.x += rand() % 3 - 1;
		else
			pos.y += rand() % 3 - 1;
		
		if (((pos.x < 1 || pos.x > mapWidth - 2 || pos.y < 1 || pos.y > mapHeight - 2) && rand() % 3 == 0) || count > 10) {
			bool good = false;
			while (!good) {
				POINT test;
				test.x = rand() % (mapWidth - 1);
				test.y = rand() % (mapHeight - 1);
				if (!tiles[test.y][test.x].solid &&
					(tiles[test.y - 1][test.x].solid || tiles[test.y + 1][test.x].solid || tiles[test.y][test.x - 1].solid || tiles[test.y][test.x + 1].solid) &&
					test.x > 2 && test.x < mapWidth - 3 && test.y > 2 && test.y < mapHeight - 3) {
					pos.x = test.x;
					pos.y = test.y;
					good = true;
				}
			}
			count = 0;
		}
		
		if (pos.x < 2) pos.x = 2;
		if (pos.x > mapWidth - 3) pos.x = mapWidth - 3;
		if (pos.y < 2) pos.y = 2;
		if (pos.y > mapHeight - 3) pos.y = mapHeight - 3;

		if (tiles[pos.y][pos.x].solid) {
			tiles[pos.y][pos.x].solid = false;
			if (rand() % 3 == 0 && pos.x > 0) tiles[pos.y][pos.x - 1].solid = false;
			if (rand() % 3 == 0 && (unsigned)pos.x < tiles[0].size() - 2) tiles[pos.y][pos.x + 1].solid = false;
			if (rand() % 3 == 0 && pos.y > 0) tiles[pos.y - 1][pos.x].solid = false;
			if (rand() % 3 == 0 && (unsigned)pos.y < tiles.size() - 2) tiles[pos.y + 1][pos.x].solid = false;
			count = 0;
		}
		else
			count++;
	}
}

vector<vector<Map::Tile>> Map::GetMap() { return tiles; }
/*vector<vector<Map::Tile>> Map::GetMapWithEnemies(vector<Enemy> enemies, Player player, int xTile, int yTile) {
	vector<vector<Tile>> tempMap = tiles;
	for (unsigned i = 0; i < enemies.size(); i++) {
		tempMap[enemies[i].GetYTile()][enemies[i].GetXTile()].enemy = true;
	}
	return tempMap;
}*/
int Map::GetMapWidth() { return mapWidth; }
int Map::GetMapHeight() { return mapHeight; }

void Map::KillTile(int x, int y) {
	tiles[y][x].solid = false;
}