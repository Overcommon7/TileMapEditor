#include "pch.h"
#include "Game.h"
#include "LoadAssests.h"
#include "Utils.h"


extern unordered_map<string, Texture2D> textures;
extern unordered_map<string, Texture2D> gameTextures;
extern short page;

inline static short currentTileMap = 1;
inline static Rectangle TileWindow = { screenWidth - (TILE_DIMENSION * TILE_MAP_WINDOW_WIDTH_IN_TILES), 0, (TILE_DIMENSION * TILE_MAP_WINDOW_WIDTH_IN_TILES), screenHeight };
inline static bool levelSelect = false, fileExists = true, menu = false, error = false, autoSave = false;
inline static string levelNumber = "1";

void Game::TileSelectAndDraw()
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		menu = true;
		return;
	}

	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_DELETE))
	{
		ResetLevel(level);
		return;
	}

	if (!CheckCollisionPointRec(u.mouse, TileWindow))
	{
		if (IsKeyPressed(KEY_RIGHT))
		{
			if (page < MAX_NUM_OF_PAGES)
			{
				++page;
				return;
			}

		}
		else if (IsKeyPressed(KEY_LEFT))
		{
			if (page > 1)
			{
				--page;
				return;
			}
		}
	}

	if (CheckCollisionPointRec(u.mouse, TileWindow))
	{
		if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;
		bool cliked = false;
		for (short i = 0; i < TileMapHitBoxes[currentTileMap - 1].size(); i++)
		{
			for (short j = 0; j < TileMapHitBoxes[currentTileMap - 1][i].size(); j++)
			{
				if (CheckCollisionPointRec(u.mouse, TileMapHitBoxes[currentTileMap - 1][i][j]))
				{
					cliked = true;
					goto NextStep;
				}
					
			}
		}
	NextStep:
		if (!cliked) 
			return;
		u.mapPos.x = abs(u.mouse.x - TileWindow.x);
		u.mapPos.y = u.mouse.y + abs(u.mouseWheelPos);
	}
	else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		if (u.mouse.x < 0 || u.mouse.x >= screenWidth) return;
		if (u.mouse.y < 0 || u.mouse.y >= screenHeight) return;

		Vector2 pos = { u.mouse.x + (screenWidth * (page - 1)), u.mouse.y};
		float xindex = u.mouse.x + (screenWidth - (TILE_MAP_WINDOW_WIDTH_IN_TILES * TILE_DIMENSION)) * (page - 1);
		level[int(pos.y / TILE_DIMENSION)][int(xindex / TILE_DIMENSION)]->Assign(currentTileMap, pos, u.mapPos);
	}
}
bool Game::LevelSaving()
{
	if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
	{
		SaveLevel(level, IsKeyDown(KEY_LEFT_SHIFT));
		return true;
	}
	return false;
}
void Game::TileMapGUI()
{
	if (!CheckCollisionPointRec(u.mouse, TileWindow)) return;

	if (IsKeyPressed(KEY_DOWN) && u.mouseWheelPos > -(float)textures[T_PREFIX + to_string(currentTileMap)].height + screenHeight)
		u.mouseWheelPos -= 50.f;
	else if (IsKeyPressed(KEY_UP) && u.mouseWheelPos != 0)
		u.mouseWheelPos += 50.f;

	if (IsKeyPressed(KEY_RIGHT) && currentTileMap < textures.size() - 1)
	{
		++currentTileMap;
		u.mouseWheelPos = 0;
	}		
	else if (IsKeyPressed(KEY_LEFT) && currentTileMap > 1)
	{
		--currentTileMap;
		u.mouseWheelPos = 0;
	}
		
}
void Game::MouseAdjusting()
{
	u.mouse = GetMousePosition();
	if (menu) return;

	if (u.mouse.x > TILE_DIMENSION)
		u.mouse.x -= (float)((int)u.mouse.x % TILE_DIMENSION);
	else u.mouse.x = 0.f;
	if (u.mouse.y > TILE_DIMENSION)
		u.mouse.y -= (float)((int)u.mouse.y % TILE_DIMENSION);
	else u.mouse.y = 0.f;
}
bool Game::Erasing()
{
	if (!(u.erasing = IsMouseButtonDown(MOUSE_RIGHT_BUTTON)))
	{
		if (!((u.erasing = IsKeyDown(KEY_E)) && !CheckCollisionPointRec(u.mouse, TileWindow))) return false;
		if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) return false;
	}

	if (u.mouse.x < 0 || u.mouse.x >= screenWidth) return false;
	if (u.mouse.y < 0 || u.mouse.y >= screenHeight) return false;

	Vector2 pos = { u.mouse.x + (screenWidth * (page - 1)), u.mouse.y };
	float xindex = u.mouse.x + (screenWidth - (TILE_MAP_WINDOW_WIDTH_IN_TILES * TILE_DIMENSION)) * (page - 1);
	level[int(pos.y / TILE_DIMENSION)][int(xindex / TILE_DIMENSION)]->Assign(NULL, pos, { 0 });
	return true;
}
void Game::LeaveMenuLogic(GameScreen& cs)
{
	if (IsKeyPressed(KEY_ESCAPE))
	{
		menu = false;
		return;
	}

	u.mouse = GetMousePosition();
	if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;

	bool doSave = CheckCollisionPointRec(u.mouse, textButtons.at(KB_YES).getCollider());
	if (doSave || CheckCollisionPointRec(u.mouse, textButtons.at(KB_NO).getCollider()))
	{
		if (doSave) SaveLevel(level);
		ResetLevel(level);
		cs = GameScreen::Title;
		autoSave = false;
		menu = false;
		currentLevelNumber = 1;
		SetWindowTitle(BASE_TITLE_NAME);
		return;
	}
}
void Game::EditingLogic(GameScreen& cs)
{
	if (IsKeyPressed(KEY_ENTER))
	{
		cs = GameScreen::Gameplay;
		page = 1;
		p.SetPosition(TILE_DIMENSION, TILE_DIMENSION * 5.f);
		return;
	}

	if (menu)
	{
		LeaveMenuLogic(cs);
		return;
	}

	if (LevelSaving()) return;
	MouseAdjusting();
	TileMapGUI();	
	if (!Erasing())
		TileSelectAndDraw();

}


void Game::LevelSelectLogic(GameScreen& cs)
{
	if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
	{
		
		if (!std::filesystem::exists(T_PREFIX + levelNumber + ".txt"))
		{
			fileExists = false;
			return;
		}

		currentLevelNumber = std::stoi(levelNumber);
		if (!LoadLevel(level, (short)TileMapHitBoxes.size()))
		{
			error = true;
			return;
		}
		levelSelect = false;
		SetWindowTitle((BASE_TITLE_NAME + string(" - LEVEL: ") + to_string(currentLevelNumber)).c_str());
		cs = GameScreen::Editing;
		autoSave = true;
		return;
	}

	if (IsKeyPressed(KEY_ESCAPE))
	{
		levelSelect = false;
		levelNumber = "1";
		fileExists = true;
		error = false;
	}

	if (IsKeyPressed(KEY_BACKSPACE) && !levelNumber.empty())
	{
		levelNumber.pop_back();
		return;
	}
	
	switch (GetKeyPressed())
	{
	case KEY_ONE:
	case KEY_KP_1:
		levelNumber += '1';
		break;
	case KEY_TWO:
	case KEY_KP_2:
		levelNumber += '2';
		break;
	case KEY_THREE:
	case KEY_KP_3:
		levelNumber += '3';
		break;
	case KEY_ZERO:
	case KEY_KP_0:
		levelNumber += '0';
		break;
	case KEY_FOUR:
	case KEY_KP_4:
		levelNumber += '4';
		break;
	case KEY_FIVE:
	case KEY_KP_5:
		levelNumber += '5';
		break;
	case KEY_SIX:
	case KEY_KP_6:
		levelNumber += '6';
		break;
	case KEY_SEVEN:
	case KEY_KP_7:
		levelNumber += '7';
		break;
	case KEY_EIGHT:
	case KEY_KP_8:
		levelNumber += '8';
		break;
	case KEY_NINE:
	case KEY_KP_9:
		levelNumber += '9';
		break;
	default:
		break;
	}
}


void Game::TitleLogic(GameScreen& cs)
{	
	if (levelSelect)
	{
		LevelSelectLogic(cs);
		return;
	}

	u.mouse = GetMousePosition();
	if (IsMouseButtonUp(MOUSE_BUTTON_LEFT)) return;

	if (CheckCollisionPointRec(u.mouse, textButtons.at(KB_EDITOR).getCollider()))
	{
		ResetLevel(level);		
		while (std::filesystem::exists(T_PREFIX + to_string(currentLevelNumber) + ".txt"))
			++currentLevelNumber;

		SetWindowTitle((BASE_TITLE_NAME + string(" - LEVEL: ") + to_string(currentLevelNumber)).c_str());
		autoSave = true;
		cs = GameScreen::Editing;
	}
			
	else if (CheckCollisionPointRec(u.mouse, textButtons.at(KB_OPEN_LEVEL).getCollider()))
	{
		fileExists = true;
		levelSelect = true;
		levelNumber = "";
	}

	else if (CheckCollisionPointRec(u.mouse, textButtons.at(KB_INSTRUCTIONS).getCollider()))
	{
		cs = GameScreen::Instrucitons;
	}
}

void Game::GamePlayLogic(GameScreen& cs)
{
	if (IsKeyPressed(KEY_ESCAPE) || !p.Update())
	{
		cs = GameScreen::Editing;
		p.SetPosition(TILE_DIMENSION, TILE_DIMENSION * 5.f);
		return;
	}
	CheckCollsion(p, level);
	if (p.GetCollider().x > screenWidth - TILE_DIMENSION && page < MAX_NUM_OF_PAGES)
	{
		++page;
		p.SetX(0);
	}
	else if (p.GetCollider().x < 0 && page > 1)
	{
		--page;
		p.SetX(1450.f);
	}
		
}

void Game::GamePlayDraw()
{
	p.Draw();

	
	for (short j = 0; j < level.size(); j++)
	{
		for (short k = (screenWidth / TILE_DIMENSION) * (page - 1); k < (screenWidth / TILE_DIMENSION) * (page); k++)
		{
			level[j][k]->GameplayDraw();
		}
	}	
}

void Game::LevelSelectDraw()
{
	DrawRectangle({ (screenWidth * 0.5 - 250) , 350, 500, 350 });
	DrawRectangleLines({ (screenWidth * 0.5 - 250) , 350, 500, 350 }, BLACK);
	DrawRectangle((screenWidth * 0.5 - 250) + 5, 500, 200, 50, WHITE);
	DrawTextEx(fonts[K_FONT], levelNumber.c_str(), { (screenWidth * 0.5 - 250) + 10, 510 }, 32.f, 1.f, BLACK);
	DrawTextEx(fonts[K_FONT], "Type The Level Number You Like To Open", {(screenWidth * 0.5 - 250) + 10, 365}, 22.f, 1.f, BLACK);
	if (!fileExists) DrawTextEx(fonts[K_FONT], "Level Does Not Exist!", { (screenWidth * 0.5 - 250) + 15.f, 650.f }, 45.f, 1.f, RED);
	if (error) DrawTextEx(fonts[K_FONT], "Error Loading Level", { (screenWidth * 0.5 - 250) + 15.f, 565.f }, 35.f, 1.f, RED);
	if (error) DrawTextEx(fonts[K_FONT], "Not Enough Tile Maps Loaded", { (screenWidth * 0.5 - 250) + 15.f, 610.f }, 30.f, 1.f, RED); 
}

void Game::TitleDraw()
{
	for (short i = 0; i < level.size(); i++)
		for (short j = 0; j < level[i].size(); j++)
			level[i][j]->Draw(screenWidth - 500);

	DrawTexture(gameTextures[K_LOGO], ((screenWidth - 500) * 0.5) - (gameTextures[K_LOGO].width * 0.5), 10, WHITE);
	textButtons.at(KB_EDITOR).Draw(u.mouse, true);
	textButtons.at(KB_INSTRUCTIONS).Draw(u.mouse, true);
	textButtons.at(KB_OPEN_LEVEL).Draw(u.mouse, true);

	if (levelSelect)
		LevelSelectDraw();
}

void Game::LeaveMenuDraw()
{
	DrawRectangle(500, 250, 500, 100, LIGHTGRAY);
	DrawRectangleLines(500, 250, 500, 100, RED);
	DrawTextEx(fonts[K_FONT], "Would You Like To Save Before Exiting?", { 555.f, 260.f }, 20.f, 1.f, BLACK);
	textButtons.at(KB_YES).Draw(u.mouse, true, false);
	textButtons.at(KB_NO).Draw(u.mouse, true, false);
}
void Game::EditingDraw()
{
	DrawTexture(textures[T_PREFIX + to_string(currentTileMap)], screenWidth - (TILE_DIMENSION * 4), u.mouseWheelPos, WHITE);
	/*for (short i = 0; i < TileMapHitBoxes[currentTileMap - 1].size(); i++)
	{
		for (short j = 0; j < TileMapHitBoxes[currentTileMap - 1][i].size(); j++)
		{			
				DrawRectangleLines(TileMapHitBoxes[currentTileMap - 1][i][j]);
		}
	}*/
	for (short i = 0; i < level.size(); i++)
		for (short j = 0; j < level[i].size(); j++)
			level[i][j]->Draw();

	if (menu)
	{
		LeaveMenuDraw();
		return;
	}

	Color c = { 70, 70, 70, 160 };
	if (u.erasing) c = { 255, 109, 194, 160 };
	DrawRectangle(u.mouse.x, u.mouse.y, TILE_DIMENSION, TILE_DIMENSION, c);
	DrawTextEx(fonts[K_FONT], ("Page: " + to_string(page)).c_str(), { screenWidth - 180, screenHeight - 20 }, 20.f, 1.2f, BLACK);

	if (CheckCollisionPointRec(u.mouse, TileWindow)) return;

	if (!u.erasing)
	{
		Rectangle source = { u.mapPos.x, u.mapPos.y, TILE_DIMENSION, TILE_DIMENSION };
		Rectangle dest = { (float)GetMouseX(), GetMouseY() - 25.f, abs(source.width * 0.5f), abs(source.height * 0.5f) };
		Vector2 origin = { 0.0f, 0.0f };
		DrawTexturePro(textures[T_PREFIX + to_string(currentTileMap)], source, dest, origin, 0.0f, WHITE);
	}
	else DrawTexture(gameTextures[K_ERASER], GetMouseX(), GetMouseY() - 25, WHITE);
}


void Game::Initialize()
{
	LoadGameTextures(textButtons);
	
	textures.insert({ "NULL", LoadTexture("TileMaps/NULL.png") });	
	string fileName = "TileMaps/TileMap1";
	int counter = 1;
	while (std::filesystem::exists(fileName + ".png"))
	{
		textures.insert({ fileName, LoadTexture((fileName + ".png").c_str()) });
		++counter;
		fileName = "TileMaps/TileMap" + to_string(counter);
	}
	TileMapHitBoxes.resize(textures.size() - 1);
	
	for (short i = 1; i < textures.size(); i++)
	{
		float y = 0;
		vector<Rectangle> temp;
		int rows = textures[T_PREFIX + to_string(i)].height / TILE_DIMENSION;
		for (; rows > 0; rows--)
		{
			
			for (float x = screenWidth - (TILE_DIMENSION * TILE_MAP_WINDOW_WIDTH_IN_TILES); x < screenWidth;  x += TILE_DIMENSION)
				temp.push_back({ x, y, TILE_DIMENSION - 1, TILE_DIMENSION - 1 });

			TileMapHitBoxes[i - 1].push_back(temp);
			temp.clear();
			y += TILE_DIMENSION;
		}		
	}

	ResetLevel(level);
}
void Game::Deconstruct()
{
	if (autoSave) SaveLevel(level);
	for (auto& it : textures)
		UnloadTexture(it.second);

	for (auto& it : gameTextures)
		UnloadTexture(it.second);

	for (short i = 0; i < level.size(); i++)
		for (short j = 0; j < level[i].size(); j++)	
			delete level[i][j];

}

void Game::InstrctionsLogic(GameScreen& cs)
{
	if (IsKeyPressed(KEY_ESCAPE)) 
		cs = GameScreen::Title;
}

void Game::InstrctionsDraw()
{
	DrawTexture(gameTextures[KB_INSTRUCTIONS], 0, 0, WHITE);
}
