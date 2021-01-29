#pragma once
#include <Windows.h>
#include "Game.h"

void DrawCell(HDC hdc, COORD coords, COLORREF color, UINT cellSize);
COLORREF GetColor(mapObject_t obj);
void DrawMap(HDC hdc, map_t map, COORD coords0, SHORT cellSize);
void PrintResults(HDC hdc, game_t* game, COORD coords);
void GameOnPaint(HDC hdc);