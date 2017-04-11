#include "stdafx.h"

C_BITMAPMGR::C_BITMAPMGR()
	: m_hInstance(nullptr)
	, m_hWnd(nullptr)
	, m_Map_Bitmap()
{
}

//initialize Instance, Window Handle
bool C_BITMAPMGR::init(HINSTANCE hInstance, HWND hWnd)
{
	if (!hInstance | !hWnd)
		return false;

	m_hInstance = hInstance;
	m_hWnd = hWnd;
	return true;
}

//delete Bitmap, Dc, Clear Map
void C_BITMAPMGR::release()
{
	for (auto iter : m_Map_Bitmap)
	{
		DeleteBitmap(iter.second.hBitmap);
		DeleteDC(iter.second.hdc);

		m_Map_Bitmap.erase(iter.first);
	}
	m_Map_Bitmap.clear();
}

//load all Bitmap Image (Build included)
void C_BITMAPMGR::loadBitmapImage()
{
	for (int i = 0; i < E_MAX; ++i)
	{
		HDC hdc = GetDC(m_hWnd);
		S_BITMAP sBitmap;
		sBitmap.hdc = CreateCompatibleDC(hdc);
		sBitmap.hBitmap = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BACKGROUND + i));
		SelectObject(sBitmap.hdc, sBitmap.hBitmap);

		m_Map_Bitmap.insert({ (E_BITMAP_TYPE)i, sBitmap });
		ReleaseDC(m_hWnd, hdc);
	}
}

//get DC handle of BitmapTypeID
HDC * C_BITMAPMGR::getHDC(E_BITMAP_TYPE eBitmapType)
{
	return &(m_Map_Bitmap[eBitmapType].hdc);
}