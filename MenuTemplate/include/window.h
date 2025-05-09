#ifndef WINDOW_H
#define WINDOW_H

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <windows.h>
#include <memory>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/*
	Putting width and height
	as globals so I dont have to make them static. 
	WndProc is a private member of the Window Class.
*/
#pragma region Globals
extern std::unique_ptr<UINT> g_pWidth;
extern std::unique_ptr<UINT> g_pHeight;
#pragma endregion

class Window
{
public:
	HWND hWnd = nullptr;
	LPDIRECT3D9 pD3d9 = nullptr;
	LPDIRECT3DDEVICE9 pD3d9Device = nullptr;
	D3DPRESENT_PARAMETERS d3dParameters{};

	VOID CreateWnd();
	VOID InitImGui();
	VOID InitDevice();
	VOID ResetDevice();
	VOID Destroy();

	Window() {};
	Window(UINT aWidth, UINT aHeight)
		: m_width(aWidth), m_height(aHeight)
	{
		CreateWnd();
		InitDevice();
		InitImGui();
	}

	~Window()
	{
		Destroy();
	}

private:
	UINT m_width;
	UINT m_height;

	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

#endif