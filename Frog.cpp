#include "Frog.h"

frog_t* InitFrog() {
	frog_t* ptr = (frog_t*)malloc(sizeof(frog_t));
	if (!ptr)
		abort();
	ptr->head = NULL;
	ptr->tail = NULL;
	return ptr;
}

void FreeFrog(frog_t* frog) {
	while (frog->tail != frog->head) {
		PopFrog(frog);
	}
	free(frog->tail);
	free(frog);
}

frog_elem_t* MallocFrogElem(COORD coord) {
	frog_elem_t* ptr = (frog_elem_t*)malloc(sizeof(frog_elem_t));
	if (!ptr)
		abort();
	ptr->coord = coord;
	ptr->next = NULL;
	ptr->prev = NULL;
	return ptr;
}

void FreeFrogElem(frog_elem_t* elem) {
	free(elem);
}

void PushFrog(frog_t* frog, frog_elem_t* elem) {
	if (frog->head == NULL) {
		frog->head = elem;
		frog->tail = elem;
		elem->next = elem->prev = NULL;
	}
	else {
		elem->next = frog->head;
		frog->head->prev = elem;
		frog->head = elem;
	}
}

void PopFrog(frog_t* frog) {
	if (frog->head != frog->tail) {
		frog_elem_t* old = frog->tail;
		frog->tail->prev->next = NULL;
		frog->tail = old->prev;
		FreeFrogElem(old);
	}
	else {
		FreeFrogElem(frog->head);
		frog->head = frog->tail = NULL;
	}
}