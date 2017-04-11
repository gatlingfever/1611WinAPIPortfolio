#include "stdafx.h"

C_GRAPHICS::C_GRAPHICS()
	: m_hInstance(nullptr)
	, m_hWnd(nullptr)
	, m_pRender(nullptr)
{
}

//initialize Instance, Window Handle, init render
bool C_GRAPHICS::init(HINSTANCE hInstance, HWND hWnd)
{
	if (!hInstance || !hWnd)
		return false;

	m_hInstance = hInstance;
	m_hWnd = hWnd;

	m_pRender = new C_RENDER;
	m_pRender->init(m_hInstance, m_hWnd);

	return true;
}

//delete render
void C_GRAPHICS::release()
{
	delete m_pRender;
	m_pRender = nullptr;

	m_hInstance = nullptr;
	m_hWnd = nullptr;
}

//render all obj, present scr
void C_GRAPHICS::updateSCR(C_OBJMGR* pObjMGR, HDC hdcMain)
{
	for (int i = (int)C_OBJMGR::E_BACKGROUND; i != (int)C_OBJMGR::E_MAX; ++i)
		renderObjectMap(pObjMGR->getBeginIter((C_OBJMGR::E_ObjTYPE)i), pObjMGR->getEndIter((C_OBJMGR::E_ObjTYPE)i));
	presentSCR(hdcMain);
}

//copy backbuffer to hdcmain
void C_GRAPHICS::presentSCR(HDC hdcMain)
{
	m_pRender->presentSCR(hdcMain);
}

//render object set
void C_GRAPHICS::renderObjectMap(objmap_citer Beginiter, objmap_citer Enditer)
{
	for (auto iter = Beginiter; iter != Enditer; ++iter)
		m_pRender->renderObject(*iter);
}
