//#include "Engine.h"
//
//Engine::Engine() {
//
//}
//
//void Engine::Init() {
//	cout << "Initializing Engine..." << endl;
//
//	done = false;
//	redraw = true;
//	FPS = 1.0 / 60.0;
//	keys[UP] = false;
//	keys[DOWN] = false;
//	keys[LEFT] = false;
//	keys[RIGHT] = false;
//	cameraPos[0] = 0;
//	cameraPos[1] = 0;
//
//	al_init();
//	al_init_primitives_addon();
//	al_init_image_addon();
//	al_init_font_addon();
//	al_init_ttf_addon();
//	al_install_keyboard();
//	al_install_mouse();
//
//	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
//	event_queue = al_create_event_queue();
//	timer = al_create_timer(FPS);
//	font10 = al_load_font("arial.ttf", 10, NULL);
//	font18 = al_load_font("arial.ttf", 18, NULL);
//	image = al_load_bitmap("bigAsteroid.png");
//
//	al_register_event_source(event_queue, al_get_display_event_source(display));
//	al_register_event_source(event_queue, al_get_timer_event_source(timer));
//	al_register_event_source(event_queue, al_get_mouse_event_source());
//	al_register_event_source(event_queue, al_get_keyboard_event_source());
//
//	map.Init();
//	map.GenerateMap();
//	player.Init(map.GetMapWidth() * TILE_SIZE / 2, map.GetMapHeight() * TILE_SIZE / 2);
//	UpdateCamera();
//
//	//enemies.push_back(Enemy(map, cameraPos, NULL, NULL));
//
//	//Enemy enemy = Enemy(map, cameraPos, NULL, NULL);
//
//	cout << "Starting Game..." << endl;
//
//	al_start_timer(timer);
//}
//
//void Engine::Collide() {
//	vector<vector<Map::Tile>> tiles = map.GetMap();
//	vector<Map::Tile> neighbors;
//	if (player.GetYTile() > 0 && player.GetYTile() < map.GetMapHeight() - 1 && player.GetXTile() > 0 && player.GetXTile() < map.GetMapWidth() - 1) {
//		neighbors.push_back(tiles[player.GetYTile() + 1][player.GetXTile()]);
//		neighbors.push_back(tiles[player.GetYTile() - 1][player.GetXTile()]);
//		neighbors.push_back(tiles[player.GetYTile()][player.GetXTile() + 1]);
//		neighbors.push_back(tiles[player.GetYTile()][player.GetXTile() - 1]);
//		neighbors.push_back(tiles[player.GetYTile() + 1][player.GetXTile() + 1]);
//		neighbors.push_back(tiles[player.GetYTile() + 1][player.GetXTile() - 1]);
//		neighbors.push_back(tiles[player.GetYTile() - 1][player.GetXTile() + 1]);
//		neighbors.push_back(tiles[player.GetYTile() - 1][player.GetXTile() - 1]);
//	}
//
//	for (unsigned i = 0; i < neighbors.size(); i++) {
//		if (neighbors[i].solid) {
//			if (player.GetY() - player.GetRadius() + 3 < neighbors[i].y * TILE_SIZE + TILE_SIZE && player.GetY() + player.GetRadius() - 3 > neighbors[i].y * TILE_SIZE && player.GetX() + player.GetRadius() >= neighbors[i].x * TILE_SIZE && player.GetPrevX() <= neighbors[i].x * TILE_SIZE) player.SetPos(neighbors[i].x * TILE_SIZE - player.GetRadius(), player.GetY());
//			else if (player.GetY() - player.GetRadius() < neighbors[i].y * TILE_SIZE + TILE_SIZE && player.GetY() + player.GetRadius() > neighbors[i].y * TILE_SIZE && player.GetX() - player.GetRadius() <= neighbors[i].x * TILE_SIZE + TILE_SIZE && player.GetPrevX() - player.GetRadius() >= neighbors[i].x * TILE_SIZE + TILE_SIZE) player.SetPos(neighbors[i].x * TILE_SIZE + TILE_SIZE + player.GetRadius(), player.GetY());
//			if (player.GetX() - player.GetRadius() < neighbors[i].x * TILE_SIZE + TILE_SIZE && player.GetX() + player.GetRadius() > neighbors[i].x * TILE_SIZE && player.GetY() + player.GetRadius() >= neighbors[i].y * TILE_SIZE && player.GetPrevY() <= neighbors[i].y * TILE_SIZE) player.SetPos(player.GetX(), neighbors[i].y * TILE_SIZE - player.GetRadius());
//			else if (player.GetX() - player.GetRadius() < neighbors[i].x * TILE_SIZE + TILE_SIZE && player.GetX() + player.GetRadius() > neighbors[i].x * TILE_SIZE && player.GetY() - player.GetRadius() <= neighbors[i].y * TILE_SIZE + TILE_SIZE && player.GetPrevY() - player.GetRadius() >= neighbors[i].y * TILE_SIZE + TILE_SIZE) player.SetPos(player.GetX(), neighbors[i].y * TILE_SIZE + TILE_SIZE + player.GetRadius());
//		}
//	}
//
//	/*if (player.GetXTile() > 0 && (player.GetX() + player.GetRadius() >= tiles[player.GetYTile()][player.GetXTile() + 1].x * TILE_SIZE) && tiles[player.GetYTile()][player.GetXTile() + 1].solid) player.SetPos(tiles[player.GetYTile()][player.GetXTile() + 1].x * TILE_SIZE - player.GetRadius(), player.GetY());
//	else if (player.GetYTile() < map.GetMapWidth() && player.GetX() - player.GetRadius() <= tiles[player.GetYTile()][player.GetXTile() - 1].x * TILE_SIZE + TILE_SIZE && tiles[player.GetYTile()][player.GetXTile() - 1].solid) player.SetPos(tiles[player.GetYTile()][player.GetXTile() - 1].x * TILE_SIZE + TILE_SIZE + player.GetRadius(), player.GetY());
//	if (player.GetYTile() > 0 && player.GetY() + player.GetRadius() >= tiles[player.GetYTile() + 1][player.GetXTile()].y * TILE_SIZE && tiles[player.GetYTile() + 1][player.GetXTile()].solid) player.SetPos(player.GetX(), tiles[player.GetYTile() + 1][player.GetYTile()].y * TILE_SIZE - player.GetRadius());
//	else if (player.GetYTile() < map.GetMapHeight() && player.GetY() - player.GetRadius() <= tiles[player.GetYTile() - 1][player.GetXTile()].y * TILE_SIZE + TILE_SIZE && tiles[player.GetYTile() - 1][player.GetXTile()].solid) player.SetPos(player.GetX(), tiles[player.GetYTile() - 1][player.GetYTile()].y * TILE_SIZE + TILE_SIZE + player.GetRadius());
//	if (tiles[player.GetYTile()][player.GetXTile()].solid) {
//
//	}*/
//	
//	for (unsigned i = 0; i < enemies.size(); i++) {
//		float distance = GetDistance(enemies[i].GetX(), enemies[i].GetY(), player.GetX(), player.GetY());
//		while (distance < (enemies[i].GetRadius() + player.GetRadius()) * (enemies[i].GetRadius() + player.GetRadius())) {
//			float angle1 = GetAngle(enemies[i].GetX(), enemies[i].GetY(), player.GetX(), player.GetY());
//			enemies[i].SetPos(enemies[i].GetX() + cos(angle1), enemies[i].GetY() + sin(angle1));
//			distance = GetDistance(enemies[i].GetX(), enemies[i].GetY(), player.GetX(), player.GetY());
//		}
//
//		for (unsigned j = 0; j < enemies.size(); j++) {
//			if (i != j) {
//				float distance = GetDistance(enemies[i].GetX(), enemies[i].GetY(), enemies[j].GetX(), enemies[j].GetY());
//				if (distance < (enemies[i].GetRadius() + enemies[j].GetRadius()) * (enemies[i].GetRadius() + enemies[j].GetRadius())) {
//					float angle1 = GetAngle(enemies[i].GetX(), enemies[i].GetY(), enemies[j].GetX(), enemies[j].GetY());
//					float angle2 = GetAngle(enemies[j].GetX(), enemies[j].GetY(), enemies[i].GetX(), enemies[i].GetY());
//					enemies[i].SetPos(enemies[i].GetX() + cos(angle1), enemies[i].GetY() + sin(angle1));
//					enemies[j].SetPos(enemies[j].GetX() + cos(angle2), enemies[j].GetY() + sin(angle2));
//					distance = GetDistance(enemies[i].GetX(), enemies[i].GetY(), enemies[j].GetX(), enemies[j].GetY());
//				}
//			}
//		}
//		neighbors.clear();
//		if (enemies[i].GetYTile() > 0 && enemies[i].GetYTile() < map.GetMapHeight() - 1 && enemies[i].GetXTile() > 0 && enemies[i].GetXTile() < map.GetMapWidth() - 1) {
//			neighbors.push_back(tiles[enemies[i].GetYTile() + 1][enemies[i].GetXTile()]);
//			neighbors.push_back(tiles[enemies[i].GetYTile() - 1][enemies[i].GetXTile()]);
//			neighbors.push_back(tiles[enemies[i].GetYTile()][enemies[i].GetXTile() + 1]);
//			neighbors.push_back(tiles[enemies[i].GetYTile()][enemies[i].GetXTile() - 1]);
//			neighbors.push_back(tiles[enemies[i].GetYTile() + 1][enemies[i].GetXTile() + 1]);
//			neighbors.push_back(tiles[enemies[i].GetYTile() + 1][enemies[i].GetXTile() - 1]);
//			neighbors.push_back(tiles[enemies[i].GetYTile() - 1][enemies[i].GetXTile() + 1]);
//			neighbors.push_back(tiles[enemies[i].GetYTile() - 1][enemies[i].GetXTile() - 1]);
//		}
//		for (unsigned j = 0; j < neighbors.size(); j++) {
//			if (neighbors[j].solid) {
//				if (enemies[i].GetY() - enemies[i].GetRadius() + 3 < neighbors[j].y * TILE_SIZE + TILE_SIZE && enemies[i].GetY() + enemies[i].GetRadius() - 3 > neighbors[j].y * TILE_SIZE && enemies[i].GetX() + enemies[i].GetRadius() >= neighbors[j].x * TILE_SIZE && enemies[i].GetPrevX() <= neighbors[j].x * TILE_SIZE) enemies[i].SetPos(neighbors[j].x * TILE_SIZE - enemies[i].GetRadius(), enemies[i].GetY());
//				else if (enemies[i].GetY() - enemies[i].GetRadius() < neighbors[j].y * TILE_SIZE + TILE_SIZE && enemies[i].GetY() + enemies[i].GetRadius() > neighbors[j].y * TILE_SIZE && enemies[i].GetX() - enemies[i].GetRadius() <= neighbors[j].x * TILE_SIZE + TILE_SIZE && enemies[i].GetPrevX() - enemies[i].GetRadius() + 3 >= neighbors[j].x * TILE_SIZE + TILE_SIZE)  enemies[i].SetPos(neighbors[j].x * TILE_SIZE + TILE_SIZE + enemies[i].GetRadius() + 2, enemies[i].GetY()); 
//				if (enemies[i].GetX() - enemies[i].GetRadius() < neighbors[j].x * TILE_SIZE + TILE_SIZE && enemies[i].GetX() + enemies[i].GetRadius() > neighbors[j].x * TILE_SIZE && enemies[i].GetY() + enemies[i].GetRadius() >= neighbors[j].y * TILE_SIZE && enemies[i].GetPrevY() <= neighbors[j].y * TILE_SIZE) enemies[i].SetPos(enemies[i].GetX(), neighbors[j].y * TILE_SIZE - enemies[i].GetRadius());
//				else if (enemies[i].GetX() - enemies[i].GetRadius() < neighbors[j].x * TILE_SIZE + TILE_SIZE && enemies[i].GetX() + enemies[i].GetRadius() > neighbors[j].x * TILE_SIZE && enemies[i].GetY() - enemies[i].GetRadius() <= neighbors[j].y * TILE_SIZE + TILE_SIZE && enemies[i].GetPrevY() - enemies[i].GetRadius() + 3 >= neighbors[j].y * TILE_SIZE + TILE_SIZE) enemies[i].SetPos(enemies[i].GetX(), neighbors[j].y * TILE_SIZE + TILE_SIZE + enemies[i].GetRadius() + 2);
//			}
//		}
//	}
//}
//void Engine::UpdateCamera() {
//	cameraPos[0] = player.GetX() - SCREEN_WIDTH / 2;
//	cameraPos[1] = player.GetY() - SCREEN_HEIGHT / 2;
//
//	if (cameraPos[0] < 0) cameraPos[0] = 0;
//	else if (cameraPos[0] + SCREEN_WIDTH > map.GetMapWidth() * TILE_SIZE) cameraPos[0] = map.GetMapWidth() * TILE_SIZE - SCREEN_WIDTH;
//	if (cameraPos[1] < 0) cameraPos[1] = 0;
//	else if (cameraPos[1] + SCREEN_HEIGHT > map.GetMapHeight() * TILE_SIZE) cameraPos[1] = map.GetMapHeight() * TILE_SIZE - SCREEN_HEIGHT;
//}
//
//void Engine::Run() {
//	while (!done) {
//		ALLEGRO_EVENT ev;
//		al_wait_for_event(event_queue, &ev);
//
//		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
//			done = true;
//		}
//		else if (ev.type == ALLEGRO_EVENT_TIMER) {
//			redraw = true;
//
//			//if (rand() % 50 == 0 && enemies.size() < 10) {
//				//enemies.push_back(Enemy(map, cameraPos));
//				//cout << "Added Enemy! size: " << enemies.size() << endl;
//			//}
//
//			player.Update(keys);
//			for (auto &enemy : enemies)
//				enemy.Update(player, map, enemies);
//			Collide();
//			
//			UpdateCamera();
//		}
//		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
//			switch (ev.keyboard.keycode) {
//			case ALLEGRO_KEY_ESCAPE:
//				done = true;
//				break;
//			case ALLEGRO_KEY_W:
//				keys[UP] = true;
//				break;
//			case ALLEGRO_KEY_S:
//				keys[DOWN] = true;
//				break;
//			case ALLEGRO_KEY_A:
//				keys[LEFT] = true;
//				break;
//			case ALLEGRO_KEY_D:
//				keys[RIGHT] = true;
//				break;
//			case ALLEGRO_KEY_F3:
//				DEBUG = !DEBUG;
//				break;
//			case ALLEGRO_KEY_SPACE:
//				map.GenerateMap();
//				break;
//			case ALLEGRO_KEY_Z:
//				enemies.push_back(Enemy(map, cameraPos, NULL, NULL));
//				cout << "Added Enemy! size: " << enemies.size() << endl;
//				break;
//			case ALLEGRO_KEY_X:
//				enemies.push_back(Enemy(map, cameraPos, player.GetX(), player.GetY()));
//				cout << "Added Enemy! size: " << enemies.size() << endl;
//				break;
//			case ALLEGRO_KEY_F1:
//				enemies.clear();
//				break;
//			}
//		}
//		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
//			switch (ev.keyboard.keycode) {
//			case ALLEGRO_KEY_W:
//				keys[UP] = false;
//				break;
//			case ALLEGRO_KEY_S:
//				keys[DOWN] = false;
//				break;
//			case ALLEGRO_KEY_A:
//				keys[LEFT] = false;
//				break;
//			case ALLEGRO_KEY_D:
//				keys[RIGHT] = false;
//				break;
//			}
//		}
//
//		if (redraw && al_is_event_queue_empty(event_queue)) {
//			redraw = false;
//
//			al_clear_to_color(al_map_rgb(0, 0, 0));
//
//			al_identity_transform(&camera);
//			al_translate_transform(&camera, -cameraPos[0], -cameraPos[1]);
//			al_use_transform(&camera);
//			
//			if (DEBUG)
//				al_draw_filled_rectangle(player.GetXTile() * TILE_SIZE, player.GetYTile() * TILE_SIZE, player.GetXTile() * TILE_SIZE + TILE_SIZE, player.GetYTile() * TILE_SIZE + TILE_SIZE, al_map_rgb(255, 0, 255));
//
//			map.Draw(cameraPos);
//			for (auto enemy : enemies)
//				enemy.Draw();
//			player.Draw();
//			//al_draw_textf(font18, al_map_rgb(255, 0, 255), 2, 2, NULL, "X:%f Y:%f", player.GetX(), player.GetY());
//			if (DEBUG) {
//				al_draw_text(font10, al_map_rgb(255, 0, 255), cameraPos[0], cameraPos[1], NULL, "Debug Mode");
//				al_draw_textf(font10, al_map_rgb(255, 0, 255), cameraPos[0], cameraPos[1] + 10, NULL, "PlayerX:%f PlayerY:%f", player.GetX(), player.GetY());
//				al_draw_textf(font10, al_map_rgb(255, 0, 255), cameraPos[0], cameraPos[1] + 20, NULL, "CameraX:%f CameraY:%f", cameraPos[0], cameraPos[1]);
//			}
//			al_flip_display();
//		}
//	}
//
//	al_destroy_font(font18);
//	al_destroy_event_queue(event_queue);
//	al_destroy_timer(timer);
//	al_destroy_display(display);
//}