#pragma once
#include "Defines.h"

class Tile
{
	int tileMapNum;
	string tileMap;	
	Vector2 levelPos;
	Vector2 mapPos;
	Vector2 gameplayCoordinate;

public:
	
	Tile(const int& TileMapNum, const Vector2& LevelPos, const Vector2& MapPos, const float& GamePlayX) : levelPos(LevelPos), mapPos(MapPos), tileMapNum(TileMapNum), gameplayCoordinate({GamePlayX, LevelPos.y / TILE_DIMENSION})
	{
		tileMap = T_PREFIX + to_string(TileMapNum);
	}
	Tile(const Tile& t)
	{
		tileMap = t.tileMap;
		levelPos = t.levelPos;
		mapPos = t.mapPos;
		tileMapNum = t.tileMapNum;
		gameplayCoordinate = t.gameplayCoordinate;
	}
	Tile& operator= (const Tile& t)
	{
		tileMap = t.tileMap;
		levelPos = t.levelPos;
		mapPos = t.mapPos;
		tileMapNum = t.tileMapNum;
		gameplayCoordinate = t.gameplayCoordinate;

		return *this;
	}
	~Tile() {}

	void Draw(const float& constraint = screenWidth - (TILE_DIMENSION * (TILE_MAP_WINDOW_WIDTH_IN_TILES + 1))) const;
	void GameplayDraw() const;
	const string Serialize() const;
	void Assign(const int& TileMapNum, const Vector2& LevelPos, const Vector2& MapPos);
	inline const int& GetTileMapNumber() const { return tileMapNum; }
	inline const Vector2& GetGameplayCoord() const { return gameplayCoordinate; }
	inline const Vector2 GetGameplayPosInPixels() const { return { gameplayCoordinate.x * TILE_DIMENSION, levelPos.y }; }
	inline const Rectangle GetCollider() const { return { gameplayCoordinate.x, gameplayCoordinate.y, TILE_DIMENSION, TILE_DIMENSION }; }
};

