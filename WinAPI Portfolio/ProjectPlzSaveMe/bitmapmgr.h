#pragma once

class C_BITMAPMGR
{
public:
	enum E_BITMAP_TYPE
	{
		E_BACKGROUND,
		E_PLAYER,
		E_BULLET,
		E_MAX
	};

private:
	struct S_BITMAP
	{
		HDC			hdc;
		HBITMAP		hBitmap;
	};

private:
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
	std::unordered_map<E_BITMAP_TYPE, S_BITMAP>		m_Map_Bitmap;

public:
	C_BITMAPMGR();
	bool			init				(HINSTANCE hInstance, HWND hWnd);
	void			release				();
	
	void			loadBitmapImage		();
	HDC*			getHDC				(E_BITMAP_TYPE eBitmapType);
};