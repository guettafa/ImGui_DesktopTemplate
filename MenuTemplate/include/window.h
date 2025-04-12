#ifndef WINDOW_H
#define WINDOW_H

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <windows.h>

class Window
{
public:
	HWND hWnd;
	LPDIRECT3D9 pD3d9;
	LPDIRECT3DDEVICE9 pD3d9Device;

	VOID CreateWnd();
	VOID InitImGui();
	VOID InitDevice();
	VOID Cleanup();

	Window() {};
	Window(UINT wndWidth, UINT wndHeight) 
		: m_width(wndWidth), m_height(wndHeight)
	{
		CreateWnd();
		InitDevice();
		InitImGui();
	}

	~Window()
	{
		Cleanup();
	}

private:
	D3DPRESENT_PARAMETERS m_d3dParameters{};
	UINT m_width;
	UINT m_height;
};

#endif