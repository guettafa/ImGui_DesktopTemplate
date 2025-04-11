#include <iostream>
#include "gui.h"

int main(int argc, char** argv)
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