#include "stdafx.h"

C_WINDOW* C_WINDOW::m_pInstance = nullptr;

C_WINDOW::C_WINDOW()
	: m_hInstance(nullptr)
	, m_hWnd(nullptr)
	, m_pGame(nullptr)
	, m_nTickCount(0)
	, m_nFrame(0)
{
}

bool C_WINDOW::initInstance()
{
	if (m_pInstance)
		return false;

	m_pInstance = new C_WINDOW;
	return true;
}

bool C_WINDOW::releaseInstance()
{
	if (!m_pInstance)
		return false;

	delete m_pInstance;
	m_pInstance = nullptr;
	return true;
}

C_WINDOW * C_WINDOW::getInstance()
{
	return m_pInstance;
}

bool C_WINDOW::initWnd(const HINSTANCE & hInstance)
{
	if (!hInstance)
		return false;

	m_hInstance = hInstance;
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = StaticWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_DEFAULT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Project.PlzSaveMe";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDC_DEFAULT));
	RegisterClassExW(&wcex);
	m_hWnd = CreateWindowW(wcex.lpszClassName, L"PlzSaveMe! by.ÀÛ¼ºÀÚNull", 
		WS_BORDER|WS_BORDER|WS_SYSMENU|WS_CAPTION,
		CW_USEDEFAULT, 0, (int)EC_ConstNUM::E_BACKGROUND_WIDTH, (int)EC_ConstNUM::E_BACKGROUND_HEIGHT,
		nullptr, nullptr, hInstance, nullptr);

	if (!m_hWnd)
		return false;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	return true;
}

bool C_WINDOW::initGame()
{
	if (m_pGame)
		return false;
	
	m_pGame = new C_GAME();
	m_pGame->initGame(m_hInstance, m_hWnd);
	m_nTickCount = GetTickCount();
	return true;
}

bool C_WINDOW::releaseGame()
{
	if (!m_pGame)
		return false;
	
	delete m_pGame;
	m_pGame = nullptr;
	return true;
}

void C_WINDOW::updateMSG()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			setFrame(90);
			for (UINT i = 0; i < m_nFrame; ++i)
			{
				m_pGame->update();
				m_pGame->render();
			}
		}
	}
}

void C_WINDOW::setFrame(UINT nFrame)
{
	UINT nNowTime = GetTickCount();
	m_nElapsedTime += (float)(nNowTime - m_nTickCount);
	m_nTickCount = nNowTime;

	float fFrameTime = (float)nFrame / 1000.f;
	m_nFrame = (int)(m_nElapsedTime * fFrameTime);
	m_nElapsedTime -= (float)(m_nFrame / fFrameTime);
}

LRESULT C_WINDOW::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_pInstance->WndProc(hWnd, message, wParam, lParam);
}

LRESULT C_WINDOW::WndProc(HWND & hWnd, UINT & message, WPARAM & wParam, LPARAM & lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		m_pGame->getKeyStateRef(wParam) = true;
		m_pGame->controlMSG();
		break;
	case WM_KEYUP:
		m_pGame->getKeyStateRef(wParam) = false;
		m_pGame->controlMSG();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
