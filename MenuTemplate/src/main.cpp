#include <iostream>
#include "gui.h"

#ifdef _DEBUG
int main(int argc, char** argv)
#else // Windows Subsystem
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
#endif
{
	Gui::wnd = Window(1920, 1080);

	Gui::Loop(
		[&]()
		{
			Gui::Menu();
		}
	);
	return 0;
}