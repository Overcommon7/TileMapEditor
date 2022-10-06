#include "pch.h"
#include "IO.h"
#include "Game.h"
#include "Defines.h"

extern float dt;
#ifndef NDEBUG
int main(void)
#else
int wWinMain()
#endif
{
    InitWindow(screenWidth, screenHeight, BASE_TITLE_NAME);
    InitAudioDevice();
    GameScreen currentScreen = GameScreen::Title;

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    SetExitKey(KEY_NULL);
    Game::Initialize();


    while (!WindowShouldClose())
    {
        dt = GetFrameTime();
        switch (currentScreen)
        {
        case GameScreen::Title:
        {
            Game::TitleLogic(currentScreen);
        } break;
        case GameScreen::Editing:
        {
            Game::EditingLogic(currentScreen);
        } break;
        case GameScreen::Gameplay:
        {           
            Game::GamePlayLogic(currentScreen);
        } break;
        case GameScreen::Instrucitons:
        {
            Game::InstrctionsLogic(currentScreen);
        }  break;
        default: break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case GameScreen::Title:
        {
            Game::TitleDraw();
        } break;
        case GameScreen::Editing:
        {           
            Game::EditingDraw();
        } break;
        case GameScreen::Gameplay:
        {
            Game::GamePlayDraw();
        } break;
        case GameScreen::Instrucitons:
        {
            Game::InstrctionsDraw();
        }  break;
        default: break;
        }

        EndDrawing();
    }

    Game::Deconstruct();
    CloseWindow(); 
    CloseAudioDevice();
    return 0;
}
