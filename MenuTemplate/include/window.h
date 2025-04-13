#ifndef WINDOW_H
#define WINDOW_H

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <windows.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*
	Putting width and height
	as globals so I dont have to make them static. 
	WndProc is a private member of the Window Class.
*/
#pragma region Globals
extern UINT g_width, g_height;
#pragma endregion

class Window
{
public:
	HWND hWnd;
	LPDIRECT3D9 pD3d9;
	LPDIRECT3DDEVICE9 pD3d9Device;
	D3DPRESENT_PARAMETERS d3dParameters{};

	VOID CreateWnd();
	VOID InitImGui();
	VOID InitDevice();
	VOID ResetDevice();
	VOID Cleanup();

	Window() {};
	Window(UINT aWidth, UINT aHeight)
	{
		CreateWnd();
		InitDevice();
		InitImGui();

		g_width = aWidth;
		g_height = aHeight;
	}

	~Window()
	{
		Cleanup();
	}

private:
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif