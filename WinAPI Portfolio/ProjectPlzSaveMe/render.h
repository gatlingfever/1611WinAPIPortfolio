#pragma once

class C_RENDER
{
private:
	HINSTANCE			m_hInstance;
	HWND				m_hWnd;
	HDC					m_hdcBackBuffer;
	C_BITMAPMGR*		m_pBitmapMGR;
	HBITMAP				m_hBitmap;
	
public:
	C_RENDER();
	bool		init			(HINSTANCE hInstance, HWND hWnd);
	void		release();
	
	void		renderObject	(C_OBJECT* pObj);
	void		presentSCR		(HDC hdcMain);
};