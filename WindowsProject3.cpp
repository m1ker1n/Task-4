// WindowsProject3.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "WindowsProject3.h"
#include "Game_Graphics.h"	
#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT3, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT3));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT3));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL; //MAKEINTRESOURCEW(IDC_WINDOWSPROJECT3);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, L"Python Game", WS_MINIMIZE,
      CW_USEDEFAULT, 0, 515, 700, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
game_t* game = NULL;
const char* settingsFileName[] = { "Map1.txt", "Map2.txt", "Map3.txt" };
LPCWSTR mapFileNames[] = { L"Map1.bmp", L"Map2.bmp", L"Map3.bmp" };
int curMap = 0;


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_SPACE:
			if (game->isLevelComplete==TRUE) {
				curMap++;
				curMap %= 3;
				ChangeLevel(game, settingsFileName[curMap], mapFileNames[curMap]);
			}
			break;
		case VK_RETURN:
			game = InitGame(hWnd,settingsFileName[curMap], mapFileNames[curMap]);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_ESCAPE:
			if (game)
				FreeGame(game);
			break;
		case VK_UP:
			if (game)
				if (game->isOver == FALSE
					&& game->isLevelComplete == FALSE) 
					ChangeDirection(game->level->python, UP);
			break;
		case VK_LEFT:
			if (game)
				if (game->isOver == FALSE
					&& game->isLevelComplete == FALSE)
					ChangeDirection(game->level->python, LEFT);
			break;
		case VK_DOWN:
			if (game)
				if (game->isOver == FALSE
					&& game->isLevelComplete == FALSE)
					ChangeDirection(game->level->python, DOWN);
			break;
		case VK_RIGHT:
			if (game)
				if (game->isOver == FALSE
					&& game->isLevelComplete == FALSE)	
					ChangeDirection(game->level->python, RIGHT);
			break;
		}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
		if (game) {
			if (game->isOver == FALSE
				&& game->isLevelComplete == FALSE) {
				COORD coords0 = { 0,0 };
				DrawMap(hdc, game->level->map, coords0, 50);
			}
			else {
				COORD coords = { 10, 10 };
				PrintResults(hdc, game, coords);
			}

		}
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_TIMER:
	{
		if (game)
			if (game->isOver == FALSE
				&& game->isLevelComplete == FALSE) {
				OnTick(game);
				InvalidateRect(hWnd, NULL, TRUE);
			}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}