#include <iostream>
#include "window.h"

#pragma region Globals
UINT g_width = 1920, g_height = 1080;
#pragma endregion

LRESULT WINAPI Window::WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wparam, lparam))
		return true;

	switch (msg)
	{
	case WM_SIZE: // when resized
		if (wparam == SIZE_MINIMIZED)
			return 0;
		g_width =  (UINT)LOWORD(lparam);
		g_height = (UINT)HIWORD(lparam);
		return 0;
	case WM_QUIT: // clean and close
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wparam, lparam);
}

VOID Window::CreateWnd()
{
	WNDCLASSEX wndClass{};
	wndClass.cbSize			= sizeof(wndClass);
	wndClass.style			= CS_CLASSDC;
	wndClass.lpfnWndProc	= WndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= GetModuleHandle(nullptr);
	wndClass.hIcon			= nullptr;
	wndClass.hCursor		= nullptr;
	wndClass.hbrBackground  = nullptr;
	wndClass.lpszMenuName	= nullptr;
	wndClass.lpszClassName	= L"MyWindow";
	wndClass.hIconSm		= nullptr;

	if (!::RegisterClassEx(&wndClass))
		std::printf("[-] Can't register WndClass %d", GetLastError());

	hWnd = ::CreateWindowW(wndClass.lpszClassName, nullptr, WS_OVERLAPPEDWINDOW, 0, 0, g_width, g_height, nullptr, nullptr, wndClass.hInstance, nullptr);
	
	if (!hWnd)
	{
		std::printf("[-] HWND created by CreateWindowEx is NULL %d", GetLastError());
		std::terminate();
	}

	::SetLayeredWindowAttributes(hWnd, 0, 0, ULW_COLORKEY);

	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);
}

VOID Window::InitDevice()
{
	pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3d9) 
	{
		std::printf("[-] Can't create the directx device %d \n", GetLastError());
		std::terminate();
	}

	ZeroMemory(&d3dParameters, sizeof(d3dParameters));
	d3dParameters.Windowed = TRUE;
	d3dParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dParameters.EnableAutoDepthStencil = TRUE;
	d3dParameters.AutoDepthStencilFormat = D3DFMT_D16;
	d3dParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dParameters.MultiSampleType = D3DMULTISAMPLE_NONE;

	// Create the D3D device
	HRESULT hr = pD3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dParameters,
		&pD3d9Device
	);

	if (FAILED(hr)) 
	{
		std::printf("[-] Failed to create DirectX device %d \n", GetLastError());
		std::terminate();
	}
}

VOID Window::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = pD3d9Device->Reset(&d3dParameters);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

VOID Window::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	if (!ImGui_ImplWin32_Init(hWnd)) 
	{
		std::printf("[-] Can't initialize ImGui Win32 API %d \n", GetLastError());
		std::terminate();
	}
	if (!ImGui_ImplDX9_Init(pD3d9Device)) 
	{
		std::printf("[-] Can't initialize ImGui DirectX9 API %d \n", GetLastError());
		std::terminate();
	}
}

VOID Window::Cleanup()
{
	if (pD3d9Device)
		pD3d9Device->Release(); pD3d9Device = nullptr;

	if (pD3d9)
		pD3d9->Release(); pD3d9 = nullptr;
}