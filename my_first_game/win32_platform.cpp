#include <windows.h>

bool running = true;

//WNDCLASS structure documentation
//https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa

LRESULT CALLBACK window_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	switch (Msg)
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		} break;

		default:
		{
			result = DefWindowProc(hWnd, Msg, wParam, lParam);
		}
	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//create a window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;

	//create register class
	RegisterClass(&window_class);
	
	//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowa
	//create window
	HWND window = CreateWindow(window_class.lpszClassName, L"My First Game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

	while (running)
	{
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
	}
}