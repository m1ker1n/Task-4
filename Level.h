#pragma once
#include "Frog.h"
#include "Python.h"
#include <Windows.h>
#include <stdio.h>

typedef enum mapObject_t {
	EMPTY,
	OBSTACLE,
	PYTHON,
	FROG,
	STONE
} mapObject_t;

typedef mapObject_t** map_t;

typedef struct frogSettings_t {
	UINT maxAmount;
	UINT amountSpawned;
	UINT liveTime; //in ticks
	UINT spawnFreq; //in ticks
}frogSettings_t;

typedef struct stoneSettings_t {
	UINT spawnFreq; //in ticks
}stoneSettings_t;

typedef struct pythonSettings_t {
	UINT scoreCoef;
	UINT starvationTime;
}pythonSettings_t;

typedef struct map_head_t {
	map_t map;
	COORD pythonHead;
}map_head_t;

typedef struct level_t {
	map_t map;
	frogSettings_t* frogSets;
	frog_t* frog;
	stoneSettings_t* stoneSets;
	pythonSettings_t* pythonSets;
	python_t* python;
	UINT freq; // in ms
}level_t;

level_t* InitLevel(const char* settingsFileName, LPCWSTR mapFileName);
void FreeLevel(level_t* level);

map_head_t InitMap(LPCWSTR filename);
mapObject_t ReadPixel(COLORREF color);
void FreeMap(map_t map);

frogSettings_t* MallocFrogSettings(UINT maxAmount, UINT amountSpawned, UINT liveTime, UINT spawnFreq);
stoneSettings_t* MallocStoneSettings(UINT spawnFreq);
pythonSettings_t* MallocPythonSettings(UINT scoreCoef, UINT starvationTime);
void FreeSettings(void* settings);

COORD RandomCoord();
void PutObjectOnMap(map_t map, COORD coord, mapObject_t obj);
BOOL IsAbleToPut(map_t map, COORD coord, mapObject_t obj);

BOOL SpawnFrog(level_t* level);
void DespawnFrogOnTimer(level_t* level);
BOOL DespawnFrogOnHit(level_t* level, COORD coord);

void SpawnStone(level_t* level);
void DespawnStoneOnHit(level_t* level, COORD coord);

void Starvation(level_t* level);