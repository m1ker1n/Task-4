#include "components.h"

void AddWindow(menu_t* menu) {
	window_t* newWindow = (window_t*)malloc(sizeof(window_t));
	if (!newWindow)
		abort();
	newWindow->firstButton = NULL;
	newWindow->lastButton = NULL;

	if (menu->lastWindow != NULL && menu->firstWindow != NULL){
		newWindow->prev = menu->lastWindow;
		newWindow->prev->next = newWindow;
		newWindow->next = menu->firstWindow;
		menu->lastWindow = newWindow;
		menu->firstWindow->prev = newWindow;
	}
	else {
		newWindow->prev = newWindow;
		newWindow->next = newWindow;
		menu->firstWindow = newWindow;
		menu->lastWindow = newWindow;
	}
}

void AddButton(window_t* window) {
	button_t* newButton = (button_t*)malloc(sizeof(button_t));
	if (!newButton)
		abort();

	if (window->lastButton != NULL && window->firstButton != NULL) {
		newButton->prev = window->lastButton;
		newButton->prev->next = newButton;
		newButton->next = window->firstButton;
		window->lastButton = newButton;
		window->firstButton->prev = newButton;
	}
	else {
		newButton->prev = newButton;
		newButton->next = newButton;
		window->firstButton = newButton;
		window->lastButton = newButton;
	}
}


void LoadWindowSets(window_t* window, WSets* sets){
	window->sets = sets;
}
void LoadButtonSets(button_t* button, BSets* sets) {
	button->sets = sets;
}


BSets* GetButtonSets(int left, int top, int right, int bottom, COLORREF color, char* name, COLORREF highlightColor ) {
	BSets* sets = (BSets*)malloc(sizeof(BSets));
	if (!sets)
		abort();
	sets->color = color;
	sets->highlightColor = highlightColor;
	sets->name = name;
	sets->rect.left = left;
	sets->rect.top = top;
	sets->rect.right = right;
	sets->rect.bottom = bottom;
	return sets;
}

WSets* GetWindowSets(int left, int top, int right, int bottom, COLORREF color) {
	WSets* sets = (WSets*)malloc(sizeof(WSets));
	if (!sets)
		abort();
	sets->color = color;
	sets->rect.left = left;
	sets->rect.top = top;
	sets->rect.right = right;
	sets->rect.bottom = bottom;
	return sets;
}


void ReleaseWindows(menu_t* menu) {
	if (menu->firstWindow != NULL && menu->lastWindow != NULL) {
		window_t* temp = menu->firstWindow;
		while (temp->next != menu->firstWindow) {
			ReleaseButtons(temp);
			temp = temp->next;
			free(temp->prev);
		}
		ReleaseButtons(temp);
		free(temp);
	}
}

void ReleaseButtons(window_t* window) {
	if (window->firstButton != NULL && window->lastButton != NULL) {
		button_t* temp = window->firstButton;
		while (temp->next != window->firstButton ) {
			free(temp->sets->name);
			free(temp->sets);
			temp = temp->next;
			free(temp->prev);
		}
		free(temp);
	}
}

