#pragma once
#include "Clicker.h"
#include "Tile.h"
#include "Defines.h"
#include "TextButton.h"
#include "Player.h"

struct User
{
	Vector2 mapPos = {0, 0};
	Vector2 mouse = GetMousePosition();
	float mouseWheelPos = 0.f;
	float actualScrollPosition = 0.f;
	bool erasing = false;
};

//Add in camera, player sprite, player sprite placing in edit mode, and a way to swap between play mode and edit mode
class Game
{
	inline static vector<vector<Tile*>> level = {};
	inline static vector<vector<vector<Rectangle>>> TileMapHitBoxes{};
	inline static User u{};
	inline static unordered_map<string, TextButton> textButtons = {};
	inline static Player p{};
	inline static Vector2 cameraBounds = { -TILE_DIMENSION, screenWidth + TILE_DIMENSION};

	static void TileSelectAndDraw();
	static bool LevelSaving();
	static void TileMapGUI();
	static void MouseAdjusting();
	static bool Erasing();
	static void LevelSelectLogic(GameScreen& cs);
	static void LevelSelectDraw();
	static void LeaveMenuDraw();
	static void LeaveMenuLogic(GameScreen& cs);
	
	

public:
	static void EditingLogic(GameScreen& cs);
	static void EditingDraw();
	static void TitleLogic(GameScreen& cs);
	static void GamePlayLogic(GameScreen& cs);
	static void GamePlayDraw();
	static void TitleDraw();
	static void Initialize();
	static void Deconstruct();
	static void InstrctionsLogic(GameScreen& cs);
	static void InstrctionsDraw();
};

