#include <iostream>
#include "window.h"

#pragma region Globals

std::unique_ptr<UINT> g_pWidth(nullptr);
std::unique_ptr<UINT> g_pHeight(nullptr);
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
		*g_pWidth.get() = (UINT)LOWORD(lparam);
		*g_pHeight.get() = (UINT)HIWORD(lparam);
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
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

	g_pWidth  = std::make_unique<UINT>(m_width);
	g_pHeight = std::make_unique<UINT>(m_height);

	if (!::RegisterClassEx(&wndClass))
		std::printf("[-] Can't register WndClass %d", GetLastError());

	hWnd = ::CreateWindow(wndClass.lpszClassName, nullptr, WS_OVERLAPPEDWINDOW, 0, 0, *g_pWidth, *g_pHeight, nullptr, nullptr, wndClass.hInstance, nullptr);
	
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
	pD3d9 = ::Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3d9) 
	{
		std::printf("[-] Can't create the directx device %d \n", GetLastError());
		std::terminate();
	}

	::ZeroMemory(&d3dParameters, sizeof(d3dParameters));
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

VOID Window::Destroy()
{
	if (pD3d9Device)
		pD3d9Device->Release(); pD3d9Device = nullptr;

	if (pD3d9)
		pD3d9->Release(); pD3d9 = nullptr;
}