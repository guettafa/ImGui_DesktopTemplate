#include <iostream>
#include "window.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return 0;
}

VOID Window::CreateWnd()
{
	WNDCLASSEXW wndClass =
	{
		sizeof(wndClass),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(nullptr),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		L"MyWindow",
		nullptr
	};

	DWORD styles = WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE;
	hWnd = CreateWindowEx(styles, L"MyWindow", nullptr, WS_POPUP, 0, 0, m_width, m_height, NULL, NULL, wndClass.hInstance, nullptr);

	SetLayeredWindowAttributes(hWnd, 0, 0, ULW_COLORKEY);
	
	if (!RegisterClassEx(&wndClass))
		std::printf("[-] Can't register WndClass - EC : %d", GetLastError);
}

VOID Window::InitDevice()
{
	pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3d9) {
		std::printf("[-] Can't create the directx device\n");
		std::terminate();
	}

	ZeroMemory(&m_d3dParameters, sizeof(m_d3dParameters));
	m_d3dParameters.Windowed = TRUE;
	m_d3dParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dParameters.BackBufferFormat = D3DFMT_A8R8G8B8;  // Format 
	m_d3dParameters.EnableAutoDepthStencil = TRUE;
	m_d3dParameters.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3dParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	m_d3dParameters.MultiSampleType = D3DMULTISAMPLE_NONE;

	// Create the D3D device
	HRESULT hr = pD3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dParameters,
		&pD3d9Device
	);

	// Double check
	if (FAILED(hr)) {
		hr = pD3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dParameters,
			&pD3d9Device
		);

		if (FAILED(hr)) {
			std::printf("[-] Failed to create DirectX device\n");
			std::terminate();
		}
	}
}

VOID Window::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	if (!ImGui_ImplWin32_Init(hWnd)) {
		std::printf("[-] Can't initialize the ImGui Win32 API \n");
		std::terminate();
	}

	if (!ImGui_ImplDX9_Init(pD3d9Device)) {
		std::printf("[-] Can't initialize the ImGui Dx9 API \n");
		std::terminate();
	}

	ImGui::StyleColorsDark();
}

/// <summary>
/// Not implemented
/// </summary>
VOID Window::Cleanup()
{
	// NOTHING
}