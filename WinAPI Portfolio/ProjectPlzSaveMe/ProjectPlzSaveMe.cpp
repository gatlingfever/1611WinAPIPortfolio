#include "stdafx.h"
#include "ProjectPlzSaveMe.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (!C_WINDOW::initInstance())
		return 0;
	if (!C_WINDOW::getInstance()->initWnd(hInstance))
		return 0;
	if (!C_WINDOW::getInstance()->initGame())
		return 0;
	C_WINDOW::getInstance()->updateMSG();
	if (!C_WINDOW::getInstance()->releaseGame())
		return 0;
	if (!C_WINDOW::releaseInstance())
		return 0;
	


    return 0;
}
