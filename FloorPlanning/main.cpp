#include "FloorPlanning.h"
#include <windows.h>
using namespace FloorPlan;


int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	Application::Run(gcnew FloorPlanning());

	return 0;
}