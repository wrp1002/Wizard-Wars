#include "PathFinder.h"

PathFinder::PathFinder() {
	start.x = 0;
	start.y = 0;
	end.x = 0;
	end.y = 0;
	mapWidth = 0;
	mapHeight = 0;
	maxTries = 40;
}

void PathFinder::SetCollisionMap(vector<vector<Map::Tile>> collisionMap, int chance) {
	int ID = 0;
	map.clear();
	for (unsigned y = 0; y < collisionMap.size(); y++) {
		vector<PathPoint> mapY;
		for (unsigned x = 0; x < collisionMap[0].size(); x++) {
			PathPoint temp;
			temp.x = x;
			temp.y = y;
			if (collisionMap[y][x].solid && rand() % chance == 0)
				temp.solid = true;
			else
				temp.solid = false;
			temp.enemy = collisionMap[y][x].enemy;
			temp.ID = ID;
			ID++;
			mapY.push_back(temp);
		}
		map.push_back(mapY);
	}

	mapHeight = map.size();
	mapWidth = map[0].size();
}

void PathFinder::SetStart(POINT start_) {
	start.x = start_.x;
	start.y = start_.y;
}
void PathFinder::SetStart(int x, int y) {
	start.x = x;
	start.y = y;
}
void PathFinder::SetEnd(POINT start_) {
	end.x = start_.x;
	end.y = start_.y;
}
void PathFinder::SetEnd(int x, int y) {
	end.x = x;
	end.y = y;
}
void PathFinder::Clear() {
	openSet.clear();
	closedSet.clear();
	finalPath.clear();
}
vector<PathFinder::PathPoint> PathFinder::GetOpenSet() {
	return openSet;
}
vector<PathFinder::PathPoint> PathFinder::GetClosedSet() {
	return closedSet;
}
vector<PathFinder::PathPoint> PathFinder::GetFinalPath() {
	return finalPath;
}

vector<vector<PathFinder::PathPoint>> PathFinder::GetCollisionMap() {
	return map;
}

vector<PathFinder::PathPoint> PathFinder::FindPath(int startX, int startY, int endX, int endY) {
	Clear();

	SetStart(startX, startY);
	SetEnd(endX, endY);

	PathPoint temp;
	temp.x = start.x;
	temp.y = start.y;
	openSet.push_back(temp);
	int count = 0;

	while (openSet.size() > 0 && finalPath.size() == 0) {
		PathPoint current;
		int lowest = 0;
		for (unsigned i = 0; i < openSet.size(); i++) {
			if (openSet[i].f < openSet[lowest].f)
				lowest = i;
		}
		current = openSet[lowest];

		if (current.x == end.x && current.y == end.y || count >= maxTries) {
			PathPoint temp = current;
			finalPath.push_back(temp);
			while (temp.parentID != -1) {
				for (unsigned i = 0; i < closedSet.size(); i++) {
					if (closedSet[i].ID == temp.parentID) {
						temp = closedSet[i];
						finalPath.push_back(temp);
						break;
					}
				}
			}
			return finalPath;
		}
		closedSet.push_back(current);
		openSet.erase(openSet.begin() + lowest);

		vector<PathPoint> neighbors;
		if (current.x > 0 && !map[current.y][current.x - 1].solid) neighbors.push_back(map[current.y][current.x - 1]);
		if (current.x < mapWidth - 1 && !map[current.y][current.x + 1].solid) neighbors.push_back(map[current.y][current.x + 1]);
		if (current.y > 0 && !map[current.y - 1][current.x].solid) neighbors.push_back(map[current.y - 1][current.x]);
		if (current.y < mapHeight - 1 && !map[current.y + 1][current.x].solid) neighbors.push_back(map[current.y + 1][current.x]);

		if (current.x > 0 && current.y > 0 && !map[current.y - 1][current.x - 1].solid && !map[current.y - 1][current.x].solid && !map[current.y][current.x - 1].solid) neighbors.push_back(map[current.y - 1][current.x - 1]);
		if (current.x < mapWidth - 1 && current.y > 0 && !map[current.y - 1][current.x + 1].solid && !map[current.y][current.x + 1].solid && !map[current.y - 1][current.x].solid) neighbors.push_back(map[current.y - 1][current.x + 1]);
		if (current.x < mapWidth - 1 && current.y < mapHeight - 1 && !map[current.y + 1][current.x + 1].solid && !map[current.y][current.x + 1].solid && !map[current.y + 1][current.x].solid) neighbors.push_back(map[current.y + 1][current.x + 1]);
		if (current.x > 0 && current.y < mapHeight - 1 && !map[current.y + 1][current.x - 1].solid && !map[current.y + 1][current.x].solid && !map[current.y][current.x - 1].solid) neighbors.push_back(map[current.y + 1][current.x - 1]);

		for (unsigned i = 0; i < neighbors.size(); i++) {
			bool found = false;
			for (unsigned j = 0; j < closedSet.size(); j++) {
				if (neighbors[i].x == closedSet[j].x && neighbors[i].y == closedSet[j].y) {
					found = true;
					break;
				}
			}
			if (found)
				continue;

			float tempG = current.g + 1 + neighbors[i].enemy * 3;
			found = false;
			int index = -1;
			for (unsigned j = 0; j < openSet.size(); j++) {
				if (neighbors[i].x == openSet[j].x && neighbors[i].y == openSet[j].y) {
					found = true;
					index = j;
					break;
				}
			}

			if (found) {
				openSet[index].g = tempG;
			}
			else {
				neighbors[i].g = tempG;
			}

			neighbors[i].h = GetDistance(neighbors[i].x, neighbors[i].y, end.x, end.y);
			neighbors[i].f = neighbors[i].h + neighbors[i].g;
			neighbors[i].parentID = current.ID;

			if (!found)
				openSet.push_back(neighbors[i]);
		}
		count++;
	}
	return finalPath;
}

void PathFinder::RemoveTile(int x, int y) {
	map[y][x].solid = false;
}

void PathFinder::ClearCollisionMap() {
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			map[y][x].solid = false;
		}
	}
}