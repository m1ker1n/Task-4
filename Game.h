#pragma once
#include "Level.h"
#include <time.h>

typedef struct game_t {
	HWND hwnd;
	level_t* level;
	UINT scores;
	BOOL isLevelComplete;
	BOOL isOver;
}game_t;

game_t* InitGame(HWND hwnd, const char* settingsFileName, LPCWSTR mapFileName);
void FreeGame(game_t* game);
void ChangeLevel(game_t* game, const char* settingsFileName, LPCWSTR mapFileName);

void GameOver(game_t* game);
void OnGameOver(game_t* game);

void CompleteLevel(game_t* game);
void OnCompleteLevel(game_t* game);
BOOL IsLevelComplete(game_t* game);

void OnTick(game_t* game);
void CheckCollision(game_t* game);