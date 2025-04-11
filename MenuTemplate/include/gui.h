#ifndef GUI_H
#define GUI_H

#include <functional>
#include "window.h"

namespace Gui
{
	extern Window wnd;

	void Menu();
	void Loop(const std::function<void()>& func);
}

#endif GUI_H