#include "gui.h"
#include <iostream>

namespace Gui
{
	Window wnd;
}

void Gui::Display()
{
#ifdef IMGUI_HAS_VIEWPORT
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::DockSpaceOverViewport(1, viewport, 0, 0);
#endif

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Graph")		    ) {}
			if (ImGui::MenuItem("Trainer")		    ) {}
			if (ImGui::MenuItem("Logger")		    ) {}
			if (ImGui::MenuItem("Profile Selector") ) {}

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// Views
	if (ImGui::Begin("Salut", nullptr))
	{
		ImGui::Text("%f", ImGui::GetIO().Framerate);
		ImGui::Text("Hello ImGui !");
	}
	ImGui::End();

	if (ImGui::Begin("Hi Guys", nullptr))
	{
		ImGui::Text("Hello ImGui !");
	}
	ImGui::End();
}

void Gui::Loop(const std::function<void() >& func) 
{
	bool isRunning = false;
	int clearColor[4] = {0, 0, 0, 0};

	while (!isRunning)
	{
		MSG msg{};

		// Polls event
		while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessageA(&msg);

			if (msg.message == WM_QUIT)
				isRunning = true;
		}

		if (isRunning)
			break;

		// Resize Window
		if (*g_pWidth != 0 && *g_pHeight != 0)
		{
			wnd.d3dParameters.BackBufferWidth  = *g_pWidth;
			wnd.d3dParameters.BackBufferHeight = *g_pHeight;
			*g_pWidth = *g_pHeight = 0;
			wnd.ResetDevice();
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		func();

		ImGui::EndFrame();

		// Render
		wnd.pD3d9Device->SetRenderState(D3DRS_ZENABLE,			 FALSE);
		wnd.pD3d9Device->SetRenderState(D3DRS_ALPHABLENDENABLE,  FALSE);
		wnd.pD3d9Device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		D3DCOLOR clearColorD3d9 = D3DCOLOR_RGBA(0, 0, 0, 255);
		wnd.pD3d9Device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColorD3d9, 1.0f, 0);

		if (wnd.pD3d9Device->BeginScene() >= 0) 
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			wnd.pD3d9Device->EndScene();
		}

		wnd.pD3d9Device->Present(nullptr, nullptr, nullptr, nullptr);
	}
	// Cleaning up
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	::DestroyWindow(wnd.hWnd);
}