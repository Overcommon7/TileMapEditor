#pragma once
#include "pch.h"
#include "Player.h"
#include "Tile.h"

extern float dt;
extern short page;

static void DrawRectangleLines(const Rectangle& r, const Color& color = RED)
{
    DrawRectangleLines(r.x, r.y, r.width, r.height, color);
}

static void DrawRectangle(const Rectangle& r, const Color& color = GRAY)
{
    DrawRectangle(r.x, r.y, r.width, r.height, color);
}

static void CheckCollsion(Player& p, const vector<vector<Tile*>>& level)
{

    if (p.GetCollider().x / TILE_DIMENSION >= level[0].size() - 1 || p.GetCollider().x < 0) return;
    if (p.GetCollider().y / TILE_DIMENSION >= level.size() - 1 || p.GetCollider().y < 0) return;

    vector<Vector2> corners = p.GetAllPoints();
    Vector2 point = {};

    if (p.GetHorizantalVelocity() < 0)
    {
        point = { corners[0].x, corners[0].y + 0.1f };
        Tile t = *level[point.y][point.x];
        if (t.GetTileMapNumber() != 0)
        {
            if (CheckCollisionPointRec(point, t.GetCollider()))
            {
                p.SetX((t.GetGameplayPosInPixels().x - (screenWidth * (page - 1))) + TILE_DIMENSION);
                p.SetHorizantalvelocity(0);
            }
        }
        point = { corners[2].x, corners[2].y - 0.1f };
        t = *level[point.y][point.x];
        if (t.GetTileMapNumber() != 0)
        {
            if (CheckCollisionPointRec(point, t.GetCollider()))
            {
                p.SetX((t.GetGameplayPosInPixels().x - (screenWidth * (page - 1))) + TILE_DIMENSION);
                p.SetHorizantalvelocity(0);
            }
        }          
    }
    else
    {
        point = { corners[1].x, corners[1].y + 0.1f };
        Tile t = *level[point.y][point.x];
        if (t.GetTileMapNumber() != 0)
        {
            if (CheckCollisionPointRec(point, t.GetCollider()))
            {
                p.SetX((t.GetGameplayPosInPixels().x - (screenWidth * (page - 1))) - TILE_DIMENSION);
                p.SetHorizantalvelocity(0);
            }
        }
        point = { corners[3].x, corners[3].y - 0.1f };
        t = *level[point.y][point.x];
        if (t.GetTileMapNumber() != 0)
        {
            if (CheckCollisionPointRec(point, t.GetCollider()))
            {
                p.SetX((t.GetGameplayPosInPixels().x - (screenWidth * (page - 1))) - TILE_DIMENSION);
                p.SetHorizantalvelocity(0);
            }
        }         
    }

    if (p.GetVerticalVelocity() > 0)
    {
        point = { corners[2].x + 0.1f, corners[2].y};
        Tile t = *level[point.y][point.x];
        if (t.GetTileMapNumber() != 0)
        {
            if (CheckCollisionPointRec(point, t.GetCollider()))
            {
                p.SetY(t.GetGameplayPosInPixels().y - TILE_DIMENSION);
                p.SetVerticalvelocity(1);
                p.SetOnplatform(true);
            }
        }
        point = { corners[3].x - 0.1f, corners[3].y };
        t = *level[point.y][point.x];
        if (t.GetTileMapNumber() != 0)
        {
            if (CheckCollisionPointRec(point, t.GetCollider()))
            {
                p.SetY(t.GetGameplayPosInPixels().y - TILE_DIMENSION);
                p.SetVerticalvelocity(1);
                p.SetOnplatform(true);
            }
        }                           
    }
    else
    {
        point = { corners[0].x + 0.1f, corners[0].y };
        Tile t = *level[point.y][point.x];
        if (t.GetTileMapNumber() != 0)
        {
            if (CheckCollisionPointRec(point, t.GetCollider()))
            {
                p.SetY(t.GetGameplayPosInPixels().y + TILE_DIMENSION);
                p.SetVerticalvelocity(-1);
            }
        }        
        point = { corners[1].x - 0.1f, corners[1].y };
        t = *level[point.y][point.x];
        if (t.GetTileMapNumber() != 0)
        {
            if (CheckCollisionPointRec(point, t.GetCollider()))
            {
                p.SetY(t.GetGameplayPosInPixels().y + TILE_DIMENSION);
                p.SetVerticalvelocity(-1);
            }
        }        
    }
}
