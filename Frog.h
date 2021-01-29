#pragma once
#include <Windows.h>

typedef struct frog_elem_t {
	COORD coord;
	frog_elem_t* next;
	frog_elem_t* prev;
}frog_elem_t;

typedef struct frog_t {
	frog_elem_t* head;
	frog_elem_t* tail;
}frog_t;

frog_t* InitFrog();
void FreeFrog(frog_t* frog);

frog_elem_t* MallocFrogElem(COORD coord);
void FreeFrogElem(frog_elem_t* elem);

//new head
void PushFrog(frog_t* frog, frog_elem_t* elem);
//free old tail, get new one
void PopFrog(frog_t* frog);
