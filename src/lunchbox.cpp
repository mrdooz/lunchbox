// lunchbox.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "app.hpp"


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	App &app = App::instance();

	if (!app.init())
		return 1;

	if (!app.run())
		return 1;

	if (!app.close())
		return 1;

	return 0;
}
