#include "pch.h"
#include "Tile.h"


extern unordered_map<string, Texture2D> textures;
extern short page;

void Tile::Draw(const float& constraint) const
{
	if (levelPos.x > constraint + (screenWidth * (page - 1))) 
		return; 
	if (levelPos.x < (screenWidth * (page - 1))) 
		return;

	Vector2 drawPos = { levelPos.x - (screenWidth * (page - 1)), levelPos.y };
	if (tileMapNum)
		DrawTextureRec(textures[tileMap], { mapPos.x, mapPos.y, TILE_DIMENSION, TILE_DIMENSION }, drawPos, WHITE);
	else
		DrawTexture(textures["NULL"], drawPos.x, levelPos.y, WHITE);

	//DrawText((to_string(int(levelPos.x / TILE_DIMENSION)) + "-" + to_string(int(levelPos.y / TILE_DIMENSION))).c_str(), drawPos.x, drawPos.y, 10.f, RED);

}

void Tile::GameplayDraw() const
{
	if (tileMapNum == 0) return;

	Vector2 drawPos = { (gameplayCoordinate.x * TILE_DIMENSION) - (screenWidth * (page - 1)), levelPos.y };
	DrawTextureRec(textures[tileMap], { mapPos.x, mapPos.y, TILE_DIMENSION, TILE_DIMENSION }, drawPos, WHITE);

}

const string Tile::Serialize() const
{
	return to_string(tileMapNum) + ' ' +
		to_string((int)levelPos.x) + ' ' + to_string((int)levelPos.y) + ' ' +
		to_string((int)(mapPos.x / TILE_DIMENSION)) + ' ' + to_string((int)(mapPos.y / TILE_DIMENSION));	
}

void Tile::Assign(const int& TileMapNum, const Vector2& LevelPos, const Vector2& MapPos)
{
	tileMap = T_PREFIX + to_string(TileMapNum);
	tileMapNum = TileMapNum;
	levelPos = LevelPos;
	mapPos = MapPos;
}
