#include <windows.h>

LRESULT CallWindowProcA(WNDPROC lpPrevWndFunc, HWND    hWnd, UINT    Msg, WPARAM  wParam, LPARAM  lParam)
{

}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = "Game Window Class";
	window_class.lpfnWndProc = 
}