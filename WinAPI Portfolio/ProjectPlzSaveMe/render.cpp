#include "stdafx.h"

C_RENDER::C_RENDER()
	: m_hInstance(nullptr)
	, m_hWnd(nullptr)
	, m_hdcBackBuffer(nullptr)
	, m_pBitmapMGR(nullptr)
	, m_hBitmap(nullptr)
{
}


// initialize Instance, Window Handle, load Bitmap Image
bool C_RENDER::init(HINSTANCE hInstance, HWND hWnd)
{
	if (!hInstance || !hWnd)
		return false;
	
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	m_hdcBackBuffer = CreateCompatibleDC(GetDC(m_hWnd));
	m_hBitmap = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_DEFAULTSCR));
	SelectObject(m_hdcBackBuffer, m_hBitmap);

	m_pBitmapMGR = new C_BITMAPMGR;
	
	if (!m_pBitmapMGR->init(m_hInstance, m_hWnd))
		return false;
	m_pBitmapMGR->loadBitmapImage();
	
	return true;
}

//render object (All contents should be included in pObj)
void C_RENDER::renderObject(C_OBJECT* pObj)
{
	HDC ObjHDC = *(m_pBitmapMGR->getHDC(pObj->getBitmapID()));
	fpoint DCStartPos = *(pObj->getDCStartPos());
	fpoint ObjPos = *(pObj->getObjCenPos());
	fpoint ObjSize = *(pObj->getImageSize());
	float fObjScale = pObj->getScale();

	ObjPos.fX -= (ObjSize.fX * fObjScale) / 2;
	ObjPos.fY -= (ObjSize.fY * fObjScale) / 2;
	
	TransparentBlt(m_hdcBackBuffer, (int)ObjPos.fX, (int)ObjPos.fY, (int)(ObjSize.fX*fObjScale), (int)(ObjSize.fY*fObjScale),
		ObjHDC, (int)DCStartPos.fX, (int)DCStartPos.fY, (int)ObjSize.fX, (int)ObjSize.fY, RGB(255, 0, 255));
}

//copy backbuffer DC to hdcMain
void C_RENDER::presentSCR(HDC hdcMain)
{
	RECT rect;
	GetClientRect(m_hWnd, &rect);
	StretchBlt(hdcMain, 0, 0, rect.right, rect.bottom, m_hdcBackBuffer, 0, 0, (int)EC_ConstNUM::E_BACKGROUND_WIDTH, (int)EC_ConstNUM::E_BACKGROUND_HEIGHT, SRCCOPY);
}

//delete bitmap, release Dynamic Memories
void C_RENDER::release()
{
	m_pBitmapMGR->release();
	delete m_pBitmapMGR;
	m_pBitmapMGR = nullptr;
	
	DeleteBitmap(m_hBitmap);
	m_hBitmap = nullptr;

	DeleteDC(m_hdcBackBuffer);
	m_hdcBackBuffer = nullptr;
}