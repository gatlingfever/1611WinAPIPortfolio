#pragma once

class C_WINDOW
{
private:
	static C_WINDOW*		m_pInstance;
	
	HINSTANCE				m_hInstance;
	HWND					m_hWnd;
	C_GAME*					m_pGame;

	UINT					m_nTickCount;
	UINT					m_nFrame;
	float					m_nElapsedTime;

private:
	C_WINDOW();
	static LRESULT CALLBACK StaticWndProc		(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK		WndProc				(HWND& hWnd, UINT& message, WPARAM& wParam, LPARAM& lParam);

public:
	static bool				initInstance		();
	static bool				releaseInstance		();
	static C_WINDOW*		getInstance			();

	bool					initWnd				(const HINSTANCE& hInstance);
	bool					initGame			();
	bool					releaseGame			();
	void					updateMSG			();

private:
	void					setFrame			(UINT nFrame);
};