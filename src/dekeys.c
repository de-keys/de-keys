/*
MIT License

Copyright (c) 2021 de-keys

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>

HWND hwndMain;
HINSTANCE hinstMain;
const char g_szClassName[] = "myWindowClass";

bool mousedown = false;

POINT lastPos;

const int winSizeX = 560;
const int winSizeY = 0;

void MakeNewButton(HINSTANCE hInst, HWND hWnd, int xpos, int length, int width, LPCWSTR name, int messageCode) {
    static HWND newButton;

    newButton = CreateWindow( "button", name,
				WS_CHILD | WS_VISIBLE | BS_TEXT, // | BS_OWNERDRAW, This be for the button styling
				xpos, 0,
				length, width,
				hWnd, (HMENU) messageCode,
				hInst, NULL);



}

void DrawButton(){

}
void Drag(){
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
}

// lessen sie die docs: https://docs.microsoft.com/en-us/windows/win32/winmsg/lowlevelmouseproc
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// It said to do this in the docs, and we don't want to annoy micro-soft
	if(nCode<0){
		return CallNextHookEx(NULL, nCode, wParam, lParam);
	}

	switch (wParam)
	{
	// Handle releasing mouse
	case WM_LBUTTONUP:
		mousedown = false;
		break;
	case WM_MOUSEMOVE:
		Drag();
		break;
	default:
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void MakeThoseButtons(HINSTANCE hInst, HWND hWnd){
	// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw

    MakeNewButton(hInst, hWnd, 0, 80, 40, "de-keys", 0);
    MakeNewButton(hInst, hWnd, 80, 80, 40, "About", 98);
    MakeNewButton(hInst, hWnd, winSizeX-40, 40, 40, U"×", 99);

    // Could be a check here with a parameter of "language". Each character has a unique ID.

    MakeNewButton(hInst, hWnd, 200, 40, 40, U"ß", 1);
    MakeNewButton(hInst, hWnd, 240, 40, 40, U"ä", 2);
    MakeNewButton(hInst, hWnd, 280, 40, 40, U"Ä", 3);
    MakeNewButton(hInst, hWnd, 320, 40, 40, U"ö", 4);
    MakeNewButton(hInst, hWnd, 360, 40, 40, U"Ö", 5);
    MakeNewButton(hInst, hWnd, 400, 40, 40, U"ü", 6);
    MakeNewButton(hInst, hWnd, 440, 40, 40, U"Ü", 7);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	hwndMain = hwnd;
	switch(msg)
	{
    case WM_DRAWITEM:
        printf("drawing");
        /*PDRAWITEMSTRUCT Item;
        Item = (LPDRAWITEMSTRUCT)lParam;
        SelectObject(Item->hDC, CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial Black"));
        FillRect(Item->hDC, &Item->rcItem, CreateSolidBrush(0));
        SelectObject(Item->hDC, CreateSolidBrush(0));
        if (Item->itemState & ODS_SELECTED)
        {
            SetTextColor(Item->hDC, 0);
            SelectObject(Item->hDC, CreateSolidBrush(0xFF00));
            SelectObject(Item->hDC, CreatePen(PS_SOLID, 2, 0xFF00));
        }
        else
        {
            SetTextColor(Item->hDC, 0x00FF00);
            SelectObject(Item->hDC, CreatePen(PS_SOLID, 2, 0x00FF00));

        }
        SetBkMode(Item->hDC, TRANSPARENT);
        int len;
        len = GetWindowTextLength(Item->hwndItem);
        LPSTR lpBuff;
        char[len+1] = lpBuff;
        GetWindowTextA(Item->hwndItem, lpBuff, len+1);
        DrawTextA(Item->hDC, lpBuff, len, &Item->rcItem, DT_CENTER);
        break; */
		// Handle clicking on the window
		case WM_LBUTTONDOWN:
			mousedown = true;
			GetCursorPos(&lastPos);
			break;

		// Handle buttons
		case WM_COMMAND:
			switch (wParam)
			{
			// Handle exit button
			case 99:
				PostMessage(hwndMain,WM_CLOSE,0,0);
				break;

			case 98:
				MessageBox(NULL, TEXT("De-Keys is free open-source software that allows you to insert characters from any language directly into your text cursor's position at the press of a button. \n\nIt is created using the C programming language and the win32 api.\n\nIf you wish to contribute or get a copy then go to https://github.com/de-keys/de-keys."), TEXT("About De-Keys"), MB_ICONINFORMATION);
				break;
			case 1:

				
				//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput
				printf("tappt");
				INPUT inputs[2];
				ZeroMemory(inputs, sizeof(inputs));

				inputs[0].type = INPUT_KEYBOARD;
				inputs[0].ki.wVk = 0x56;

				inputs[1].type = INPUT_KEYBOARD;
				inputs[1].ki.wVk = 0x56;
				inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

				


				HWND newFocus = FindWindowA("Notepad", "*Untitled - Notepad");
				if(newFocus){
					SetActiveWindow(newFocus);
					SetForegroundWindow(newFocus);
					SetActiveWindow(newFocus);
					SetFocus(newFocus);

					printf("set focus on notepad");
				}else{
					printf("Notepad no findy...");
				}
				UINT uSent = SendInput(2, inputs, sizeof(INPUT));
				
				if (uSent != 2)
				{
					printf("SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
				}
				break;
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
	// Hide console (for release)
	//FreeConsole();

	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	// LoadIcon(hInstance, "../../../MhmCats.github.io/icon.png", 64, 64, LR_LOADFROMFILE);
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
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+3);
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
