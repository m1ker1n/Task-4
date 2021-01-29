#include "Level.h"

#pragma warning(disable:4996)
level_t* InitLevel(const char* settingsFileName, LPCWSTR mapFileName) {
	FILE* stream;
	stream = fopen(settingsFileName, "r");
	if (!stream)
		abort();

	UINT levelFreq;
	UINT frogMaxAmount;
	UINT frogLiveTime;
	UINT frogSpawnFreq;
	UINT stoneSpawnFreq;
	UINT pythonScoreCoef;
	UINT pythonStarvationTime;


	fscanf(stream, "levelFreq=%u\n", &levelFreq);
	fscanf(stream, "frogMaxAmount=%u\n", &frogMaxAmount);
	fscanf(stream, "frogLiveTime=%u\n", &frogLiveTime);
	fscanf(stream, "frogSpawnFreq=%u\n", &frogSpawnFreq);
	fscanf(stream, "stoneSpawnFreq=%u\n", &stoneSpawnFreq);
	fscanf(stream, "pythonScoreCoef=%u\n", &pythonScoreCoef);
	fscanf(stream, "pythonStarvationTime=%u", &pythonStarvationTime);

	map_head_t map_head = InitMap(mapFileName);
	map_t map = map_head.map;
	COORD pythonHead = map_head.pythonHead;

	python_t* python = InitPython(pythonHead);
	frog_t* frog = InitFrog();

	level_t* level = (level_t*)malloc(sizeof(level_t));
	if (!level)
		abort();

	level->map = map;
	level->frogSets = MallocFrogSettings(frogMaxAmount, 0, frogLiveTime, frogSpawnFreq);
	level->frog = frog; 
	level->stoneSets = MallocStoneSettings(stoneSpawnFreq);
	level->pythonSets = MallocPythonSettings(pythonScoreCoef, pythonStarvationTime);
	level->python = python;
	level->freq = levelFreq;

	return level;
}

void FreeLevel(level_t* level) {
	if (level) {
		FreeMap(level->map);
		FreeSettings(level->frogSets);
		FreeFrog(level->frog);
		FreeSettings(level->stoneSets);
		FreeSettings(level->pythonSets);
		FreePython(level->python);
		free(level);
	}
}


map_head_t InitMap(LPCWSTR filename) {
	COLORREF color;

	HDC bmp_hdc = CreateCompatibleDC(NULL);
	HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(bmp_hdc, hBitmap);

	map_t map = (map_t)malloc(10 * sizeof(mapObject_t*));
	if (!map)
		abort();
	for (int i = 0; i < 10; i++) {
		mapObject_t* temp = (mapObject_t*)malloc(10 * sizeof(mapObject_t));
		map[i] = temp;
		if (!temp)
			abort();
	}

	COORD pythonHead;

	for (int y = 0; y < 10; y++)
		for (int x = 0; x < 10; x++)
		{
			color = GetPixel(bmp_hdc, x, y);
			mapObject_t obj = ReadPixel(color);
			if (obj == PYTHON) {
				pythonHead.X = x;
				pythonHead.Y = y;
			}
			map[x][y] = obj;
		}

	DeleteObject(hBitmap);
	DeleteDC(bmp_hdc);

	map_head_t map_head = { map, pythonHead };
	return map_head;
}

mapObject_t ReadPixel(COLORREF color)
{
	switch (color)
	{
	case RGB(0, 0, 0):
		return OBSTACLE;
	case RGB(255, 255, 255):
		return EMPTY;
	default:
		return PYTHON;
	}
}

void FreeMap(map_t map) {
	for (int i = 0; i < 10; i++)
		free(map[i]);
	free(map);
}


frogSettings_t* MallocFrogSettings(UINT maxAmount, UINT amountSpawned, UINT liveTime, UINT spawnFreq) {
	frogSettings_t* res = (frogSettings_t*)malloc(sizeof(frogSettings_t));
	if (!res)
		abort();
	res->maxAmount = maxAmount;
	res->amountSpawned = amountSpawned;
	res->liveTime = liveTime;
	res->spawnFreq = spawnFreq;
	return res;
}

stoneSettings_t* MallocStoneSettings(UINT spawnFreq) {
	stoneSettings_t* res = (stoneSettings_t*)malloc(sizeof(stoneSettings_t));
	if (!res)
		abort();
	res->spawnFreq = spawnFreq;
	return res;
}

pythonSettings_t* MallocPythonSettings(UINT scoreCoef, UINT starvationTime) {
	pythonSettings_t* res = (pythonSettings_t*)malloc(sizeof(pythonSettings_t));
	if (!res)
		abort();
	res->scoreCoef = scoreCoef;
	res->starvationTime = starvationTime;
	return res;
}

void FreeSettings(void* settings) {
	free(settings);
}


COORD RandomCoord() {
	COORD res = { rand() % 10, rand() % 10 };
	return res;
}

void PutObjectOnMap(map_t map, COORD coord, mapObject_t obj) {
	map[coord.X][coord.Y] = obj;
}

BOOL IsAbleToPut(map_t map, COORD coord, mapObject_t obj) {
	switch (obj) {
	case PYTHON:
		if (map[coord.X][coord.Y] == EMPTY
			|| map[coord.X][coord.Y] == FROG
			|| map[coord.X][coord.Y] == STONE)
			return TRUE;
		break;
	case FROG: 
		if (map[coord.X][coord.Y] == EMPTY)
			return TRUE;
		break;
	case STONE:
		if (map[coord.X][coord.Y] == EMPTY)
			return TRUE;
		break;
	}
	return FALSE;
}


BOOL SpawnFrog(level_t* level) {
	COORD coord = RandomCoord();
	if (level->frogSets->amountSpawned < level->frogSets->maxAmount) {
		while (!IsAbleToPut(level->map, coord, FROG)) {
			coord = RandomCoord();
		}
		//can spawn and found place for frog
		//on map
		PutObjectOnMap(level->map, coord, FROG);

		//on queue
		frog_elem_t* elem = MallocFrogElem(coord);
		PushFrog(level->frog, elem);

		//on sets
		level->frogSets->amountSpawned++;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL DespawnFrogOnHit(level_t* level, COORD coord) {
	frog_elem_t* temp = level->frog->head;
	while (!(temp->coord.X == coord.X && temp->coord.Y == coord.Y) && temp->next!=NULL) {
		temp = temp->next;
	}
	if (temp->coord.X == coord.X && temp->coord.Y == coord.Y) {
		PutObjectOnMap(level->map, coord, PYTHON);
		IncreasePython(level->python, coord);

		if (level->frog->head == level->frog->tail) {
			PopFrog(level->frog);
		}
		else {
			if (temp == level->frog->head) {
				level->frog->head = temp->next;
				level->frog->head->prev = NULL;
				FreeFrogElem(temp);
			}
			else {
				if (temp == level->frog->tail)
				{
					level->frog->tail = temp->prev;
					level->frog->tail->next = NULL;
					FreeFrogElem(temp);
				}
				else {
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
					FreeFrogElem(temp);
				}
			}
		}

		return TRUE;
	}
	else
		abort();
	return FALSE;
}

void DespawnFrogOnTimer(level_t* level) {
	//on map
	PutObjectOnMap(level->map, level->frog->tail->coord, EMPTY);

	//on queue
	PopFrog(level->frog);
}


void SpawnStone(level_t* level) {
	COORD coord;
	do {
		coord = RandomCoord();
	} while (!IsAbleToPut(level->map, coord, STONE));

	PutObjectOnMap(level->map, coord, STONE);
}

void DespawnStoneOnHit(level_t* level, COORD expectedCoord) {
	PutObjectOnMap(level->map, expectedCoord, PYTHON);
	int pythonLen = CountPython(level->python->head);
	PutObjectOnMap(level->map, level->python->tail->coord, EMPTY);
	if (pythonLen > 1) {
		PutObjectOnMap(level->map, level->python->tail->prev->coord, EMPTY);
		DecreasePython(level->python);
	}
}

void Starvation(level_t* level) {
	if (CountPython(level->python->head) > 1) {
		PutObjectOnMap(level->map, level->python->tail->coord, EMPTY);
		DecreasePython(level->python);
	}
}
