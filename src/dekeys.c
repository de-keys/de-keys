#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

const char g_szClassName[] = "myWindowClass";

bool mousedown = false;

POINT lastPos;

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
   printf("tap");

   return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);

		case WM_LBUTTONDOWN: {
			mousedown = true;
			printf("mouse down");

			GetCursorPos(&lastPos);

			break;
		}
		case WM_LBUTTONUP: {
			mousedown = false;
			printf("mouse up");
			break;
		}
		case WM_MOUSEMOVE: {
			if (mousedown) {
				POINT currentpos;
				GetCursorPos(&currentpos);
				int x = currentpos.x - lastPos.x;
				int y = currentpos.y - lastPos.y;
				RECT rc;
				GetWindowRect(hwnd,&rc);

				MoveWindow(hwnd, rc.left + x,rc.top + y, 500, 0, false);

				lastPos = currentpos;
			}
			break;
		}
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
    HOOKPROC hookproc;
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//Step 1: Registering the Window Class
	wc.cbSize		= sizeof(WNDCLASSEX);
	wc.style		 = 0;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	 = hInstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor	   = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm	   = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"de-keys",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 500, 0,
		NULL, NULL, hInstance, NULL);

	SetWindowLong(hwnd, GWL_STYLE, WS_BORDER );

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	SetWindowPos(hwnd,HWND_TOPMOST, 0, 0, 500, 0, SWP_SHOWWINDOW);
	// SetWindowsHookEx(WH_CALLWNDPROC, hookproc, hInstance, 0);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
