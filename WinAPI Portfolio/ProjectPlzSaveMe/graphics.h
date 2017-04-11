#pragma once

class C_GRAPHICS
{
private:
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
	C_RENDER*		m_pRender;

public:
	C_GRAPHICS();
	bool		init			(HINSTANCE hInstance, HWND hWnd);
	void		release			();

	void		updateSCR		(C_OBJMGR* pObjMGR, HDC hdcMain);

private:
	void		renderObjectMap	(objmap_citer Beginiter, objmap_citer Enditer);
	void		presentSCR		(HDC hdcMain);
};