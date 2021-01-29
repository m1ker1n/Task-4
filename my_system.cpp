#include "my_system.h"

menu_t* menu;
int isFileRead=0;
window_t* curWindow=NULL;
button_t* curButton=NULL;
activeComponent* actComp;

curHandler_t curHandler = SYSTEM;

curHandler_t GetMyHandler() {
	return curHandler;
}

void ChangeHandler(curHandler_t newHandler) {
	curHandler = newHandler;
}

void HandleSystem(HWND hWnd, WPARAM wParam) {
	switch (wParam) {
	case VK_LEFT:
		HandleMessages(SYS_LEFT);
		break;
	case VK_UP:
		HandleMessages(SYS_UP);
		break;
	case VK_RIGHT:
		HandleMessages(SYS_RIGHT);
		break;
	case VK_DOWN:
		HandleMessages(SYS_DOWN);
		break;
	case VK_RETURN:
		{
			char* output = HandleMessages(SYS_ENTER);
			if (strcmp(output, "StartGame") == 0)
			{
				ChangeHandler(GAME);
				HandleGame(hWnd, VK_RETURN);
			}
			if (strcmp(output, "Exit") == 0) {
				SystemRelease();
				SystemClose();
				PostQuitMessage(0);
			}
		}
		break;
	}
}




void MyHandle(HWND hWnd, curHandler_t curHandler, WPARAM wParam) {
	switch (curHandler) {
	case SYSTEM:
		HandleSystem(hWnd, wParam);
		break;
	case GAME:
		if (HandleGame(hWnd, wParam)==TRUE)
			ChangeHandler(SYSTEM);
		break;
	}
	InvalidateRect(hWnd, NULL, TRUE);
}

#pragma warning(push)
#pragma warning(disable:4996)
// code with C4996

void SystemOpen(HWND hwnd) {
	InitWindow(hwnd);
}

void SystemInit() {
	FILE* streamPtr;
	streamPtr = fopen("menuFileName.txt", "r");
	if (!streamPtr)
		SystemClose();
	ReadFile(streamPtr);

	actComp = (activeComponent*)malloc(sizeof(activeComponent));
	
	actComp->window = menu->firstWindow;
	actComp->button = actComp->window->firstButton;
	fclose(streamPtr);
}

void SystemRelease() {
	if (menu != NULL) {
		ReleaseWindows(menu);
		free(menu);
	}
	isFileRead = 0;
	curButton = NULL;
	curWindow = NULL;
	free(actComp);
	menu = NULL;
	actComp = NULL;
}

void SystemUpdate(HDC hdc) {
	DrawMenu(menu, actComp,hdc);
}

void SystemClose() {
	PostQuitMessage(0);
}

char* HandleMessages(messages_t msg) {
	switch (msg) {
	case SYS_ENTER:
		break;
	case SYS_LEFT:
		if (actComp->button->prev != NULL)
			actComp->button = actComp->button->prev;
		break;
	case SYS_RIGHT:
		if (actComp->button->next != NULL)
			actComp->button = actComp->button->next;
		break;
	case SYS_UP:
		if (actComp->window->prev != NULL) {
			actComp->window = actComp->window->prev;
			actComp->button = actComp->window->firstButton;
		}
		break;
	case SYS_DOWN:
		if (actComp->window->next != NULL) {
			actComp->window = actComp->window->next;
			actComp->button = actComp->window->firstButton;
		}
		break;
	}
	return actComp->button->sets->name;
}


void ReadFile(FILE* streamPtr) {
	char buf[1024] = { '\0' };
	while (fgets(buf, 1024, streamPtr) && !isFileRead) {
		ParseFile(streamPtr, buf);
	}
}

void ParseFile(FILE* streamPtr,char* str) {
	if (strstr(str, "MenuBegin")) {
		menu = (menu_t*)malloc(sizeof(menu_t));
		menu->firstWindow = NULL;
		menu->lastWindow = NULL;
		return;
	}
	
	if (strstr(str, "WindowBegin")) {
		AddWindow(menu);
		curWindow = menu->lastWindow;
		return;
	}

	if (strstr(str, "[WindowSettings]")) {
		int left, top, right, bottom, r, g, b;
		COLORREF color;
		fscanf(streamPtr, " Rectangle = (%d,%d,%d,%d)\n Color = (%d,%d,%d)\n", &left, &top, &right, &bottom, &r, &g, &b);
		color = RGB(r, g, b);
		WSets* sets = GetWindowSets(left, top, right, bottom, color);
		LoadWindowSets(curWindow, sets);	
		return;
	}

	if (strstr(str, "ButtonBegin")) {
		AddButton(curWindow);
		curButton = curWindow->lastButton;
		return;
	}
	
	if (strstr(str, "[ButtonSettings]")) {
		int left, top, right, bottom, r1, g1, b1, r2, g2, b2;
		char* name = (char*)malloc(sizeof(char) * 128);
		COLORREF color, highlightColor;
		fscanf(streamPtr, " Rectangle = (%d,%d,%d,%d)\n Color = (%d,%d,%d)\n Name = %s\n HighlightColor = (%d,%d,%d)\n", &left, &top, &right, &bottom, &r1, &g1, &b1, name, &r2, &g2, &b2);
		color = RGB(r1, g1, b1);
		highlightColor = RGB(r2, g2, b2);
		BSets* sets = GetButtonSets(left, top, right, bottom, color, name, highlightColor);
		LoadButtonSets(curButton, sets);
		return;
	}

	if (strstr(str, "ButtonEnd")) {
		curButton = NULL;
		return;
	}

	if (strstr(str, "WindowEnd")) {
		curWindow = NULL;
		return;
	}

	if (strstr(str, "MenuEnd")) {
		isFileRead = 1;
		return;
	}
}

#pragma warning(pop)