#pragma once
#include "pch.h"
#include "Defines.h"
#include "Tile.h"
#include "IO.h"
#include "TextButton.h"

extern unordered_map<string, Texture2D> textures;
extern unordered_map<string, Texture2D> gameTextures;
extern unordered_map<string, Font> fonts;
extern short currentLevelNumber;

static void ResetLevel(vector<vector<Tile*>>& level)
{
	level.clear();
	level.reserve(screenHeight / TILE_DIMENSION);
	level.resize(screenHeight / TILE_DIMENSION);
	for (short i = 0; i < level.size(); i++)
	{
		short counter = 0;
		level[i].reserve((((screenWidth / TILE_DIMENSION) - (TILE_MAP_WINDOW_WIDTH_IN_TILES)) * MAX_NUM_OF_PAGES));
		level[i].resize((((screenWidth / TILE_DIMENSION) - (TILE_MAP_WINDOW_WIDTH_IN_TILES)) * MAX_NUM_OF_PAGES));
		for (short j = 0; j < level[i].size() + (4 * (MAX_NUM_OF_PAGES - 1)); j++)
		{			
			for (short k = 0; k < (((screenWidth * MAX_NUM_OF_PAGES) / TILE_DIMENSION) - (TILE_MAP_WINDOW_WIDTH_IN_TILES + 1)); k += (screenWidth / TILE_DIMENSION))
			{
				if (j >= k && j <= (k + ((screenWidth / TILE_DIMENSION) - (TILE_MAP_WINDOW_WIDTH_IN_TILES + 1))))
				{
					level[i][counter] = (new Tile(0, { (float)(j * TILE_DIMENSION), (float)(i * TILE_DIMENSION) }, { 0 }, counter));
					++counter;
					break;
				}
			}		
		}
	}
}

static bool LoadLevel(vector<vector<Tile*>>& level, const short& numOfTileMaps)
{
	ResetLevel(level);
	const string fileName = "TileMaps/TileMap" + to_string(currentLevelNumber) + ".txt";
	if (!std::filesystem::exists(fileName)) return false;
	short i = 0;
	
	string temp = "";
	fstream inFile(fileName);	
	while (!inFile.eof())
	{	
		string buffer = "";
		std::getline(inFile, temp);
		short j = 0;
		while (!temp.empty())
		{
			size_t offset = temp.find(',');
			if (offset != string::npos)
			{
				buffer = temp.substr(0, offset);
				temp.erase(temp.begin(), temp.begin() + offset + 1);
			}
			else
			{
				buffer = temp;
				temp = "";
			}

			short counter = 0;
			Vector2 mapPos = { 0 };
			Vector2 lvlPos = { 0 };
			short tileMapNum = 0;
			std::stringstream ss(buffer);
			while (ss >> buffer)
			{
				switch (counter)
				{
				case 0:
				{
					tileMapNum = std::stoi(buffer);
					if (tileMapNum > numOfTileMaps)
					{
						ResetLevel(level);
						return false;
					}
						
				} break;
				case 1:
				{
					lvlPos.x = std::stof(buffer);
				} break;
				case 2:
				{
					lvlPos.y = std::stof(buffer);
				} break;
				case 3:
				{
					 mapPos.x = (float)(std::stoi(buffer) * 50);
				} break;
				case 4:
				{
					mapPos.y = (float)(std::stoi(buffer) * 50);
				} break;				
				}
				++counter;
			}
			level[i][j]->Assign(tileMapNum, lvlPos, mapPos);
			++j;
		}
		++i;
	}
	inFile.close();
	return true;
}

static void SaveLevel(vector<vector<Tile*>>& level, const bool newFile = false)
{
	short counter = 1;
	string fileName = "TileMaps/TileMap1.txt";
	if (newFile)
	{
		while (std::filesystem::exists(fileName))
		{
			++counter;
			fileName = T_PREFIX + to_string(counter) + ".txt";
		}
		currentLevelNumber = counter;
	}
	else fileName = T_PREFIX + to_string(currentLevelNumber) + ".txt";
	
	SetWindowTitle((BASE_TITLE_NAME + string(" - LEVEL: ") + to_string(currentLevelNumber)).c_str());

	vector<string> serialized;	
	serialized.reserve(level.size());
	for (short i = 0; i < level.size(); i++)
	{
		string temp = "";
		temp.reserve(16 * level[i].size());
		short j = 0;
		for (; j < level[i].size() - 1; j++)
		{		
			temp += level[i][j]->Serialize() + ',';
		}
		temp += level[i][j]->Serialize() + '\n';
		serialized.push_back(temp);
	}
	serialized[serialized.size() -1].pop_back();

	if (!newFile)
		std::remove(fileName.c_str());	

	ofstream create(fileName);
	create.close();

	fstream inFile(fileName);
	for (short i = 0; i < serialized.size(); i++)
		inFile << serialized[i];

	inFile.close();
}

static void LoadGameTextures(unordered_map<string, TextButton>& t)
{
	gameTextures.insert({ K_ERASER, LoadTexture("Assests/Eraser.png") });
	gameTextures.insert({ K_LOGO, LoadTexture("Assests/Logo.png") });
	gameTextures.insert({ KP_PLAYER, LoadTexture("Assests/Player.png") });
	gameTextures.insert({ KB_INSTRUCTIONS, LoadTexture("Assests/Instructions.png") });

	fonts.insert({ K_FONT, LoadFont("Assests/CandaraBold.ttf") });

	t.insert({KB_EDITOR, TextButton(188.f, K_FONT, 0, 35.f, "START NEW LEVEL", "Open A Blank Canvas")});
	t.insert({KB_OPEN_LEVEL, TextButton(188.f * 2.f, K_FONT, 0, 35.f, "OPEN EXISTING LEVEL", "Select a Specific Level To Continue Editing From")});
	t.insert({KB_INSTRUCTIONS, TextButton(188.f * 3.f, K_FONT, 0, 35.f, "INSTRUCTIONS", "Learn The Basics OF The Editor")});
	t.insert({KB_YES, TextButton(550.f, 300.f, K_FONT, 0, 50.f, "YES", BLACK)});
	t.insert({KB_NO, TextButton(850.f, 300.f, K_FONT, 0, 50.f, "NO", BLACK)});
}

