#pragma once
#include <Windows.h>

typedef enum direction_t {
	LEFT = -2,
	UP = -1,
	RIGHT = 2,
	DOWN = 1
} direction_t;

typedef struct python_elem_t {
	COORD coord;
	python_elem_t* next;
	python_elem_t* prev;
}python_elem_t;

typedef struct python_t {
	direction_t headDirection;
	python_elem_t* head;
	python_elem_t* tail;
}python_t;

//malloc first elem
python_t* InitPython(COORD coord);
void FreePython(python_t* python);

python_elem_t* MallocPythonElem(COORD coord);
void FreePythonElem(python_elem_t* elem);

//new head
void PushPython(python_t* python, python_elem_t* elem); 
//free old tail, get new one
void PopPython(python_t* python);
//count num of elements starting from elem to tail
int CountPython(python_elem_t* elem);
void ChangeDirection(python_t* python, direction_t dir);
COORD ExpectedCoord(COORD c0, direction_t dir);

void MovePython(python_t* python, COORD coord);
void IncreasePython(python_t* python, COORD coord);
void DecreasePython(python_t* python);