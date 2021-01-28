#include "Python.h"

python_t* InitPython(COORD coord) {
	python_t* ptr = (python_t*)malloc(sizeof(python_t));
	if (!ptr)
		abort();
	ptr->headDirection = RIGHT;

	python_elem_t* elem = MallocPythonElem(coord);
	ptr->head = ptr->tail = elem;
	return ptr;
}

void FreePython(python_t* python) {
	while (python->tail != python->head) {
		PopPython(python);
	}
	free(python->tail);
	free(python);
}

python_elem_t* MallocPythonElem(COORD coord) {
	python_elem_t* ptr = (python_elem_t*)malloc(sizeof(python_elem_t));
	if (!ptr)
		abort();
	ptr->coord = coord;
	ptr->next = NULL;
	ptr->prev = NULL;
	return ptr;
}

void FreePythonElem(python_elem_t* elem) {
	free(elem);
}

void PushPython(python_t* python, python_elem_t* elem) {
	elem->next = python->head;
	python->head->prev = elem;
	python->head = elem;
}

void PopPython(python_t* python) {
	if (python->head != python->tail) {
		python_elem_t* old = python->tail;
		python->tail->prev->next = NULL;
		python->tail = python->tail->prev;
		FreePythonElem(old);
	}
}

int CountPython(python_elem_t* elem) {
	if (!elem)
		return -1;
	
	int i = 0;
	python_elem_t* temp = elem;
	while (temp->next != NULL) {
		i++;
		temp = temp->next;
	}
	i++;
	return i;
}

void ChangeDirection(python_t* python, direction_t dir) {
	if (python->headDirection + dir != 0) 
		python->headDirection = dir;	
}

COORD ExpectedCoord(COORD old, direction_t dir) {
	COORD exp = old;
	switch (dir) {
	case LEFT:
		exp.X--;
		break;
	case RIGHT:
		exp.X++;
		break;
	case UP:
		exp.Y--;
		break;
	case DOWN:
		exp.Y++;
		break;
	}
	return exp;
}

void MovePython(python_t* python, COORD coord) {
	IncreasePython(python, coord);
	DecreasePython(python);
}

void IncreasePython(python_t* python, COORD coord) {
	python_elem_t* elem = MallocPythonElem(coord);
	PushPython(python, elem);
}

void DecreasePython(python_t* python) {
	PopPython(python);
}

