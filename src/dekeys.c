#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

HWND hwndMain;
HINSTANCE hinstMain;
const char g_szClassName[] = "myWindowClass";

bool mousedown = false;

POINT lastPos;

const int winSizeX = 900;
const int winSizeY = 0;

// lessen sie die docs: https://docs.microsoft.com/en-us/windows/win32/winmsg/lowlevelmouseproc
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// It said to do this in the docs, and we don't want to annoy micro-soft
	if(nCode<0){
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	switch (wParam)
	{
	// Handle pressing down
	case WM_LBUTTONDOWN:
		mousedown = true;
		GetCursorPos(&lastPos);
		break;
	// Handle releasing mouse
	case WM_LBUTTONUP:
		mousedown = false;
		break;
	// Handle dragging mouse
	// TODO: stop dragging if mouse far away from window
	case WM_MOUSEMOVE:
		if (mousedown) {
			POINT currentpos;
			GetCursorPos(&currentpos);
			int x = currentpos.x - lastPos.x;
			int y = currentpos.y - lastPos.y;
			RECT rc;
			GetWindowRect(hwndMain,&rc);

			MoveWindow(hwndMain, rc.left + x,rc.top + y, winSizeX, winSizeY, false);

			lastPos = currentpos;
		}
		break;
	default:
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void MakeThoseButtons(HINSTANCE hInst, HWND hWnd){
	// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw

	static HWND title;
	title = CreateWindow( "static", "de-keys",
				WS_CHILD | WS_VISIBLE | BS_TEXT | SS_CENTER | DS_SETFONT,
				0, 0, 
				80, 40,
				hWnd, (HMENU) 0,
				hInst, NULL );

	static HWND hButton;
	hButton = CreateWindow( "button", "Label",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				80, 0, 
				80, 40,
				hWnd, (HMENU) 5,
				hInst, NULL );

	static HWND closeButton;
	closeButton = CreateWindow( "button", "X",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				winSizeX-40, 0, 
				40, 40,
				hWnd, (HMENU) 99,
				hInst, NULL );

	printf("Made those buttons!!");
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	hwndMain = hwnd;
	switch(msg)
	{
		// Handle buttons
		case WM_COMMAND:
			switch (wParam)
			{
			// Handle exit button
			case 99:
				PostMessage(hwndMain,WM_CLOSE,0,0);
				break;
			case 5:
				MessageBox(NULL, TEXT("Label button pressed!"), TEXT("Title"), 0);
			
			default:
				break;
			}
			break;
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

	hinstMain = hInstance;

	// Registering the Window Class
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

	// Create the window
	hwnd = CreateWindowEx(
		WS_EX_TRANSPARENT,
		g_szClassName,
		"de-keys",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, winSizeX, winSizeY,
		NULL, NULL, hInstance, NULL);

	SetWindowLong(hwnd, GWL_STYLE, WS_BORDER );

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Initalise the window
	hwndMain = hwnd;
	MakeThoseButtons(hInstance, hwndMain);
	SetWindowPos(hwnd,HWND_TOPMOST, 0, 0, winSizeX, winSizeY, SWP_SHOWWINDOW);

	// Show the window
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	
	// Add a mouse hook
	HOOKPROC mouseLL = &LowLevelMouseProc;
	HHOOK mouseLLHook;
	mouseLLHook = SetWindowsHookEx(WH_MOUSE_LL, mouseLL, hInstance, 0);

	// Handle the event loop
	while(GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
