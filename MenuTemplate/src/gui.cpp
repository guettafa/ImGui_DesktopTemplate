#include "gui.h"

namespace Gui
{
	Window wnd;
}

void Gui::Menu()
{
	ImGui::Begin("Salut", nullptr);
	{
		ImGui::Text("%f",ImGui::GetIO().Framerate);
		ImGui::Text("Hello ImGui !");
	}
	ImGui::End();
}

void Gui::Loop(const std::function<void() >& func) 
{
	MSG msg{};
	bool isDone = false;
	int clearColor[4] = {0, 0, 0, 0};

	while (!isDone)
	{
		// Polls event
		if (PeekMessage(&msg, wnd.hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);

			if (msg.message == WM_QUIT)
				isDone = true;

			if (isDone)
				break;
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

		HRESULT result = wnd.pD3d9Device->Present(nullptr, nullptr, nullptr, nullptr);
	}
}