#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "components.h"
#include "my_graphics.h"
#include "Game.h"
#include "Game_Graphics.h"

typedef enum curHandler_t {
	SYSTEM,
	GAME
}curHandler_t;

curHandler_t GetMyHandler();
void ChangeHandler(curHandler_t newHandler);
void MyHandle(HWND hWnd,curHandler_t curHandler, WPARAM wParam);
void HandleSystem(HWND hWnd, WPARAM wParam);


typedef enum {
	SYS_ENTER,
	SYS_LEFT,
	SYS_UP,
	SYS_RIGHT,
	SYS_DOWN
} messages_t;

void SystemOpen(HWND hwnd);
void SystemInit();
void SystemRelease();
void SystemUpdate(HDC hdc);
void SystemClose();

char* HandleMessages(messages_t msg);

void ReadFile(FILE* streamPtr);
void ParseFile(FILE* streamPtr, char* str);

/* Format:
MenuBegin
	WindowBegin
		[WindowSettings]
		Rectangle = (left,top,right,bottom)
		Color = (R,G,B)
		ButtonBegin
			[ButtonSettings]
			Rectangle = (left,top,right,bottom)
			Color = (R,G,B)
			Name = name
			HighlightColor = (R,G,B)
		ButtonEnd
	WindowEnd
MenuEnd
*/