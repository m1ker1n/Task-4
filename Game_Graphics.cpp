#include "Game_Graphics.h"

void DrawCell(HDC hdc, COORD coords, COLORREF color, UINT cellSize) {
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hBrush);
	Rectangle(hdc, coords.X, coords.Y, coords.X + cellSize, coords.Y + cellSize);
	DeleteObject(hBrush);
}

COLORREF GetColor(mapObject_t obj) {
	COLORREF res = 0;
	switch (obj) {
	case EMPTY:
		res = RGB(255, 255, 255);
		break;
	case PYTHON:
		res = RGB(200, 200, 100);
		break;
	case OBSTACLE:
		res = 0;
		break;
	case FROG:
		res = RGB(0, 255, 0);
		break;
	case STONE:
		res = RGB(100, 100, 100);
		break;
	}
	return res;
}

void DrawMap(HDC hdc, map_t map, COORD coords0, SHORT cellSize) {
	for (SHORT y = 0; y < 10; y++)
		for (SHORT x = 0; x < 10; x++) {
			COLORREF color = GetColor(map[x][y]);
			COORD coords = { coords0.X + x * cellSize,coords0.Y + y * cellSize };
			DrawCell(hdc, coords, color, cellSize);
		}
}

#pragma warning(disable:4996)
void PrintResults(HDC hdc, game_t* game, COORD coords) {
	
	if (game->isLevelComplete == TRUE && game->isOver == FALSE) {
		TCHAR text[3][40] = { { L'\0' },{ L'\0' },{ L'\0' } };
		_swprintf_p(text[0], 40, L"Scores of Level: %u", game->scores);
		_swprintf_p(text[1], 40, L"Press Enter to restart level");
		_swprintf_p(text[2], 40, L"Press Space to next level");
		TextOutW(hdc, coords.X, coords.Y, text[0], wcslen(text[0]));
		TextOutW(hdc, coords.X, coords.Y+20, text[1], wcslen(text[1]));
		TextOutW(hdc, coords.X, coords.Y+40, text[2], wcslen(text[2]));
	}
	if (game->isLevelComplete == FALSE && game->isOver == TRUE) {
		TCHAR text[40] = { L'\0' };
		_swprintf_p(text, 40, L"Press Enter to restart\0");
		TextOutW(hdc, coords.X, coords.Y, text, wcslen(text));
	}
}