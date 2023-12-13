#include <windows.h>
#include "utils.cpp"

global_variable bool running = true;

struct Render_State
{
	void* memory;
	int width;
	int height;
	BITMAPINFO bitmap_info;
};
global_variable Render_State render_state;

#include "renderer.cpp"

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

	case WM_SIZE: {
		RECT rect;
		GetClientRect(hWnd, &rect);
		render_state.width = rect.right - rect.left;
		render_state.height = rect.bottom - rect.top;

		int size = render_state.width * render_state.height * sizeof(u32);

		if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
		render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
		render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
		render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
		render_state.bitmap_info.bmiHeader.biPlanes = 1;
		render_state.bitmap_info.bmiHeader.biBitCount = 32;
		render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
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
	HDC hdc = GetDC(window);

	while (running)
	{
		//input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		//simulate
		clear_screen(0xff5500);
		draw_rect(0, 0, 1, 1, 0x00ff22);
		draw_rect(30, 30, 5, 5, 0x00ff22);
		draw_rect(-20, 20, 8, 3, 0x00ff22);

		

		//render
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
}