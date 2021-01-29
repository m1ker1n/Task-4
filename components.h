#pragma once
#include <Windows.h>
#include <stdlib.h>


typedef struct buttonSettings_t {
	RECT rect;
	COLORREF color;
	COLORREF highlightColor;
	char* name;
}buttonSettings_t, BSets;

typedef struct button_t {
	BSets* sets;
	button_t* prev;
	button_t* next;
}button_t;

typedef struct windowSettings_t {
	RECT rect;
	COLORREF color;
}windowSettings_t, WSets;

typedef struct window_t {
	WSets* sets;
	window_t* prev;
	window_t* next;
	button_t* firstButton;
	button_t* lastButton;
}window_t;

typedef struct menu_t {
	window_t* firstWindow;
	window_t* lastWindow;
}menu_t;

typedef struct activeComponent {
	window_t* window;
	button_t* button;
} activeComponent;

void AddWindow(menu_t* menu);
void AddButton(window_t* window);

void LoadWindowSets(window_t* window, WSets* sets);
void LoadButtonSets(button_t* button, BSets* sets);

BSets* GetButtonSets(int left, int top, int right, int bot, COLORREF color, char* name, COLORREF highlightColor);
WSets* GetWindowSets(int left, int top, int right, int bot, COLORREF color);

void ReleaseWindows(menu_t* menu);
void ReleaseButtons(window_t* window);

