#include "Game.h"

game_t* game = NULL;
const char* settingsFileName[] = { "Map1.txt", "Map2.txt", "Map3.txt" };
LPCWSTR mapFileNames[] = { L"Map1.bmp", L"Map2.bmp", L"Map3.bmp" };
int curMap = 0;

game_t* InitGame(HWND hwnd, const char* settingsFileName, LPCWSTR mapFileName) {
	srand((UINT)time(NULL));
	game_t* game = (game_t*)malloc(sizeof(game_t));
	if (!game)
		abort();
	game->level = InitLevel(settingsFileName, mapFileName);
	game->scores = 0;
	game->isOver = FALSE;
	game->hwnd = hwnd;
	game->isLevelComplete = FALSE;
	SetTimer(hwnd, 1, game->level->freq, (TIMERPROC)NULL);

	return game;
}

void FreeGame(game_t* game) {
	if(game->level!=NULL)
		FreeLevel(game->level);
	KillTimer(game->hwnd, 1);
	free(game);
	game = NULL;
}

void ChangeLevel(game_t* game, const char* settingsFileName, LPCWSTR mapFileName) {
	game->isLevelComplete = 0;
	FreeLevel(game->level);
	game->level = InitLevel(settingsFileName, mapFileName);
	KillTimer(game->hwnd, 1);
	SetTimer(game->hwnd, 1, game->level->freq, (TIMERPROC)NULL);
}

void GameOver(game_t* game) {
	game->isOver = TRUE;
	OnGameOver(game);
}

void OnGameOver(game_t* game) {
	FreeLevel(game->level);
}


void CompleteLevel(game_t* game) {
	game->isLevelComplete = TRUE;
	OnCompleteLevel(game);
}

void OnCompleteLevel(game_t* game) {
	game->scores = CountPython(game->level->python->head)*game->level->pythonSets->scoreCoef;
}

BOOL IsLevelComplete(game_t* game) {
	if (game->level->frogSets->amountSpawned == game->level->frogSets->maxAmount
		&& game->level->frog->head == game->level->frog->tail
		&& game->level->frog->head == NULL)
		return TRUE;
	else
		return FALSE;
}

void OnTick(game_t* game) {
	if (game->isOver == FALSE && game->isLevelComplete==FALSE) {
		static UINT s_FrogSpawn = 0;
		static UINT s_FrogLiveTime = 0;
		static UINT s_StoneSpawn = 0;
		static UINT s_StarvationTime = 0;
		s_FrogSpawn++;
		s_FrogLiveTime++;
		s_StoneSpawn++;
		s_StarvationTime++;

		if (s_FrogSpawn == game->level->frogSets->spawnFreq) {
			SpawnFrog(game->level);
			s_FrogSpawn = 0;
		}

		if (s_FrogLiveTime == game->level->frogSets->liveTime) {
			DespawnFrogOnTimer(game->level);
			s_FrogLiveTime = 0;
			if (IsLevelComplete(game))
				CompleteLevel(game);
		}

		if (s_StoneSpawn == game->level->stoneSets->spawnFreq) {
			SpawnStone(game->level);
			s_StoneSpawn = 0;
		}

		if (s_StarvationTime == game->level->pythonSets->starvationTime) {
			Starvation(game->level);
			s_StarvationTime = 0;
		}

		CheckCollision(game);
		if (game->isOver == TRUE || game->isLevelComplete == TRUE) {
			s_FrogSpawn = 0;
			s_FrogLiveTime = 0;
			s_StoneSpawn = 0;
			s_StarvationTime = 0;
		}

	}
}

void CheckCollision(game_t* game) {
	map_t map = game->level->map;
	COORD expectedCoord = ExpectedCoord(game->level->python->head->coord, game->level->python->headDirection);
	int x = expectedCoord.X;
	int y = expectedCoord.Y;

	switch (map[x][y]) {
	case OBSTACLE:
		GameOver(game);
		break;
	case PYTHON:
		GameOver(game);
		break;
	case STONE:
		DespawnStoneOnHit(game->level, expectedCoord);
		MovePython(game->level->python, expectedCoord);
		break;
	case FROG:
		DespawnFrogOnHit(game->level, expectedCoord);
		if (IsLevelComplete(game))
			CompleteLevel(game);
		break;
	case EMPTY:
		PutObjectOnMap(game->level->map, game->level->python->tail->coord, EMPTY);
		MovePython(game->level->python, expectedCoord);
		PutObjectOnMap(map, expectedCoord, PYTHON);
		break;
	}
}

BOOL HandleGame(HWND hWnd, WPARAM wParam) {
	switch (wParam) {
	case VK_SPACE:
		if (game->isLevelComplete == TRUE) {
			curMap++;
			curMap %= 3;
			ChangeLevel(game, settingsFileName[curMap], mapFileNames[curMap]);
		}
		break;
	case VK_RETURN:
		game = InitGame(hWnd, settingsFileName[curMap], mapFileNames[curMap]);
		break;
	case VK_ESCAPE:
		FreeGame(game);
		return TRUE;
		break;
	case VK_UP:
		if (game)
			if (game->isOver == FALSE
				&& game->isLevelComplete == FALSE)
				ChangeDirection(game->level->python, UP);
		break;
	case VK_LEFT:
		if (game)
			if (game->isOver == FALSE
				&& game->isLevelComplete == FALSE)
				ChangeDirection(game->level->python, LEFT);
		break;
	case VK_DOWN:
		if (game)
			if (game->isOver == FALSE
				&& game->isLevelComplete == FALSE)
				ChangeDirection(game->level->python, DOWN);
		break;
	case VK_RIGHT:
		if (game)
			if (game->isOver == FALSE
				&& game->isLevelComplete == FALSE)
				ChangeDirection(game->level->python, RIGHT);
		break;
	}
	return FALSE;
}

void GameOnTimer() {
	if (game)
		if (game->isOver == FALSE
			&& game->isLevelComplete == FALSE) {
			OnTick(game);
		}
}

game_t* GetGamePtr() {
	return game;
}