#pragma once

#include "Globals.h"
#include "Map.h"

using namespace std;

class PathFinder {
public:
	struct PathPoint {
		int x, y, ID;
		int parentID = -1;
		float f = 0;
		float h = 0;
		float g = 0;
		bool solid;
		bool enemy;
	};
private:
	POINT start;
	POINT end;
	vector<vector<PathPoint>> map;
	vector<PathPoint> openSet;
	vector<PathPoint> closedSet;
	vector<PathPoint> finalPath;
	int mapWidth, mapHeight;
	int maxTries;

public:
	PathFinder();

	void SetStart(POINT start_);
	void SetStart(int x, int y);
	void SetEnd(POINT start_);
	void SetEnd(int x, int y);
	void Clear();
	vector<PathPoint> GetOpenSet();
	vector<PathPoint> GetClosedSet();
	vector<PathPoint> GetFinalPath();
	vector<vector<PathPoint>> GetCollisionMap();

	vector<PathPoint> FindPath(int startX, int startY, int endX, int endY);

	void SetCollisionMap(vector<vector<Map::Tile>> collisionMap, int chance);
	void RemoveTile(int x, int y);
	void ClearCollisionMap();
};