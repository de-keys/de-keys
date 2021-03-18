#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

HWND hwndMain;
const char g_szClassName[] = "myWindowClass";

bool mousedown = false;

POINT lastPos;

// lessen sie die docs: https://docs.microsoft.com/en-us/windows/win32/winmsg/lowlevelmouseproc
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode<0){
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	switch (wParam)
	{
	case WM_LBUTTONDOWN:
		mousedown = true;
		GetCursorPos(&lastPos);
		break;
	case WM_LBUTTONUP:
		mousedown = false;
		break;
	case WM_MOUSEMOVE:
		if (mousedown) {
			POINT currentpos;
			GetCursorPos(&currentpos);
			int x = currentpos.x - lastPos.x;
			int y = currentpos.y - lastPos.y;
			RECT rc;
			GetWindowRect(hwndMain,&rc);

			MoveWindow(hwndMain, rc.left + x,rc.top + y, 500, 0, false);

			lastPos = currentpos;
		}
		break;
	default:
		break;
	}
	printf("tap");

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	hwndMain = hwnd;
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
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//Step 1: Registering the Window Class
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);	// TODO: change this to be the Eszett
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName 	= NULL;
	wc.lpszClassName	= g_szClassName;
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);	// TODO: change this to be the Eszett

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

	SetWindowPos(hwnd,HWND_TOPMOST, 0, 0, 500, 0, SWP_SHOWWINDOW);
	
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	

	HOOKPROC mouseLL = &LowLevelMouseProc;
	HHOOK mouseLLHook;
	mouseLLHook = SetWindowsHookEx(WH_MOUSE_LL, mouseLL, hInstance, 0);

	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
