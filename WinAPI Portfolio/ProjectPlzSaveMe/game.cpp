#include "stdafx.h"

C_GAME::C_GAME()
	: m_hInstance(nullptr)
	, m_hWnd(nullptr)
	, m_arKeyState{}
	, m_pGraphics(nullptr)
	, m_pObjMGR(nullptr)
	, m_pPlayer(nullptr)
	, m_bGameStarted(false)
	, m_nBullet1Num(NULL)
	, m_nBullet2Num(NULL)
	, m_nBullet3Num(NULL)
	, m_nBullet4Num(NULL)
	, m_fBullet4Index(0.f)
	, m_fElapsedTime(0.f)
	, m_nBullet5Num(NULL)
	, m_nNowTime(NULL)
	, m_fBullet5Index(0.f)
{
}

bool C_GAME::initGame(HINSTANCE& hInstance, HWND& hWnd)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	m_pObjMGR = new C_OBJMGR;
	setBackGround();
	setPlayer();
	
	m_pGraphics = new C_GRAPHICS;
	m_pGraphics->init(hInstance, hWnd);
	
	srand((UINT)time(NULL));

	m_pDurationTimer = new C_TIMER;

	return true;
}

bool C_GAME::releaseGame()
{
	for (int i = 0; i < C_OBJMGR::E_MAX; ++i)
	{
		m_pObjMGR->destroyAllObj((C_OBJMGR::E_ObjTYPE)i);
	}
	delete m_pObjMGR;
	m_pObjMGR = nullptr;

	m_pGraphics->release();
	delete m_pGraphics;
	m_pGraphics = nullptr;

	return true;
}

void C_GAME::update()
{
	if (m_bGameStarted)
	{
		int level = m_nNowTime / 10000;
		m_fElapsedTime = (float)m_pDurationTimer->getNowTime() - (float)m_nNowTime;
		m_nNowTime = m_pDurationTimer->getNowTime();
		m_fBullet4Index += m_fElapsedTime;
		m_fBullet5Index += m_fElapsedTime;

		for (int i = m_pObjMGR->getSize(C_OBJMGR::E_BULLET_TYPE1); i < level + 5; ++i)
			createBulletNum1();
		for (int i = m_pObjMGR->getSize(C_OBJMGR::E_BULLET_TYPE2); i < level + 1; ++i)
			createBulletNum2();
		for (int i = m_pObjMGR->getSize(C_OBJMGR::E_BULLET_TYPE3); i < level - 1; ++i)
		{
			createBulletNum3();
			for (objmap_citer iter = m_pObjMGR->getBeginIter(C_OBJMGR::E_BULLET_TYPE3);
				iter != m_pObjMGR->getEndIter(C_OBJMGR::E_BULLET_TYPE3); ++iter)
				(*iter)->setDirection(m_pPlayer->getObjCenPos()->fX - (*iter)->getObjCenPos()->fX,
					m_pPlayer->getObjCenPos()->fY - (*iter)->getObjCenPos()->fY);
		}
		if (m_fBullet4Index > 1000.f)
		{
			if (level >= 5)
				createBulletNum41();
			if (level >= 10)
				createBulletNum42();
			if (level >= 15)
				createBulletNum43();
			if (level >= 20)
				createBulletNum44();
			m_fBullet4Index -= 500.f;
		}
		for (int i = m_pObjMGR->getSize(C_OBJMGR::E_BULLET_TYPE5); i < level - 7; ++i)
			createBulletNum5();
		if (m_fBullet5Index > 1000.f)
		{
			for (objmap_citer iter = m_pObjMGR->getBeginIter(C_OBJMGR::E_BULLET_TYPE5);
				iter != m_pObjMGR->getEndIter(C_OBJMGR::E_BULLET_TYPE5); ++iter)
				(*iter)->setDirection((float)(rand() % 10) - 5.f, (float)(rand() % 10) - 5.f);
			m_fBullet5Index -= 1000.f;
		}
	}
	if (m_pPlayer->isOutofSCR())
	{
		m_pPlayer->setDirection(-m_pPlayer->getDirection()->fX, -m_pPlayer->getDirection()->fY);
		setPlayerHDC();
	}
	for (int i = 2; i < C_OBJMGR::E_MAX; ++i)
	{
		for (objmap_citer iter = m_pObjMGR->getBeginIter((C_OBJMGR::E_ObjTYPE)i);
			iter != m_pObjMGR->getEndIter((C_OBJMGR::E_ObjTYPE)i);)
		{
			if (m_pPlayer->isCollided(*iter))
				showDieSCR();
			if ((*iter)->isOutofSCRBullet())
				iter = m_pObjMGR->destroyObj((C_OBJMGR::E_ObjTYPE)i, iter);
			else
				++iter;
		}
	}
	m_pObjMGR->updateAllObj();
}

void C_GAME::render()
{
	HDC hdc = GetDC(m_hWnd);
	m_pGraphics->updateSCR(m_pObjMGR, hdc);

	WCHAR DurationTime[20] = {};
	_stprintf_s(DurationTime, L"Live for %9.1fs", m_nNowTime / 1000.f);
	TextOut(hdc, 800, 50, DurationTime, 19);

	ReleaseDC(m_hWnd, hdc);
}

bool & C_GAME::getKeyStateRef(UINT nKeyIndex)
{
	return m_arKeyState[nKeyIndex];
}

void C_GAME::setBackGround()
{
	C_OBJECT* pBackGroundOBJ = m_pObjMGR->createObj(C_OBJMGR::E_BACKGROUND);
	
	pBackGroundOBJ->setBitmapID(C_BITMAPMGR::E_BACKGROUND);
	pBackGroundOBJ->setCollideRad(0.f);
	pBackGroundOBJ->setDCStartPos(0.f, 0.f);
	pBackGroundOBJ->setImageSize((float)EC_ConstNUM::E_BACKGROUND_WIDTH, (float)EC_ConstNUM::E_BACKGROUND_HEIGHT);
	pBackGroundOBJ->setObjCenPos((float)EC_ConstNUM::E_BACKGROUND_WIDTH / 2.f, (float)EC_ConstNUM::E_BACKGROUND_HEIGHT / 2.f);
	pBackGroundOBJ->setDirection(0.f, 0.f);
	pBackGroundOBJ->setScale(1.f);
	pBackGroundOBJ->setVelocity(0.f);
}

void C_GAME::setPlayer()
{
	m_pPlayer = m_pObjMGR->createObj(C_OBJMGR::E_PLAYER);

	m_pPlayer->setBitmapID(C_BITMAPMGR::E_PLAYER);
	m_pPlayer->setCollideRad(25.f);
	m_pPlayer->setDCStartPos((float)EC_ConstNUM::E_PLAYER_DEFAULT_XPOS, (float)EC_ConstNUM::E_PLAYER_DEFAULT_YPOS);
	m_pPlayer->setImageSize((float)EC_ConstNUM::E_PLAYER_WIDTH, (float)EC_ConstNUM::E_PLAYER_HEIGHT);
	m_pPlayer->setObjCenPos((float)EC_ConstNUM::E_BACKGROUND_WIDTH / 2.f, (float)EC_ConstNUM::E_BACKGROUND_HEIGHT / 2.f);
	m_pPlayer->setDirection(0.f, 0.f);
	m_pPlayer->setScale(0.5f);
	m_pPlayer->setVelocity(1.5f);
}

void C_GAME::setPlayerHDC()
{
	fpoint arPlayerDC[3][3] = { 
		{
			{ (float)EC_ConstNUM::E_PLAYER_UPLEFT_XPOS, (float)EC_ConstNUM::E_PLAYER_UPLEFT_YPOS},
			{ (float)EC_ConstNUM::E_PLAYER_UP_XPOS, (float)EC_ConstNUM::E_PLAYER_UP_YPOS },
			{ (float)EC_ConstNUM::E_PLAYER_UPRIGHT_XPOS, (float)EC_ConstNUM::E_PLAYER_UPRIGHT_YPOS },
		},
		{
			{ (float)EC_ConstNUM::E_PLAYER_LEFT_XPOS, (float)EC_ConstNUM::E_PLAYER_LEFT_YPOS },
			{ (float)EC_ConstNUM::E_PLAYER_DEFAULT_XPOS, (float)EC_ConstNUM::E_PLAYER_DEFAULT_YPOS },
			{ (float)EC_ConstNUM::E_PLAYER_RIGHT_XPOS, (float)EC_ConstNUM::E_PLAYER_RIGHT_YPOS },
		},
		{
			{ (float)EC_ConstNUM::E_PLAYER_DOWNLEFT_XPOS, (float)EC_ConstNUM::E_PLAYER_DOWNLEFT_YPOS },
			{ (float)EC_ConstNUM::E_PLAYER_DOWN_XPOS, (float)EC_ConstNUM::E_PLAYER_DOWN_YPOS },
			{ (float)EC_ConstNUM::E_PLAYER_DOWNRIGHT_XPOS, (float)EC_ConstNUM::E_PLAYER_DOWNRIGHT_YPOS },
		}	};
	int Xindex = 0;
	int Yindex = 0;
	
	if (m_pPlayer->getDirection()->fX > 0.55f)
		Xindex = 2;
	else if (m_pPlayer->getDirection()->fX < -0.55f)
		Xindex = 0;
	else
		Xindex = 1;
	if (m_pPlayer->getDirection()->fY > 0.55f)
		Yindex = 2;
	else if (m_pPlayer->getDirection()->fY < -0.55f)
		Yindex = 0;
	else
		Yindex = 1;

	m_pPlayer->setDCStartPos(arPlayerDC[Yindex][Xindex].fX, arPlayerDC[Yindex][Xindex].fY);
}

void C_GAME::keydownUp()
{
	fpoint fDir = *(m_pPlayer->getDirection());
	m_pPlayer->setDirection(fDir.fX, - 1.f);
	if (!m_bGameStarted)
	{
		m_bGameStarted = true;
		m_pDurationTimer->setBeginTime();
	}
	setPlayerHDC();
}

void C_GAME::keydownRight()
{
	fpoint fDir = *(m_pPlayer->getDirection());
	m_pPlayer->setDirection(1.f, fDir.fY);
	if (!m_bGameStarted)
	{
		m_bGameStarted = true;
		m_pDurationTimer->setBeginTime();
	}
	setPlayerHDC();
}

void C_GAME::keydownDown()
{
	fpoint fDir = *(m_pPlayer->getDirection());
	m_pPlayer->setDirection(fDir.fX, + 1.f);
	if (!m_bGameStarted)
	{
		m_bGameStarted = true;
		m_pDurationTimer->setBeginTime();
	}
	setPlayerHDC();
}

void C_GAME::keydownLeft()
{
	fpoint fDir = *(m_pPlayer->getDirection());
	m_pPlayer->setDirection(- 1.f, fDir.fY);
	if (!m_bGameStarted)
	{
		m_bGameStarted = true;
		m_pDurationTimer->setBeginTime();
	}
	setPlayerHDC();
}

void C_GAME::keydownSpace()
{
	destoyAllBullet();

	m_pPlayer->setBitmapID(C_BITMAPMGR::E_PLAYER);
	m_pPlayer->setCollideRad((float)EC_ConstNUM::E_PLAYER_WIDTH / 2.f);
	m_pPlayer->setDCStartPos((float)EC_ConstNUM::E_PLAYER_DEFAULT_XPOS, (float)EC_ConstNUM::E_PLAYER_DEFAULT_YPOS);
	m_pPlayer->setImageSize((float)EC_ConstNUM::E_PLAYER_WIDTH, (float)EC_ConstNUM::E_PLAYER_HEIGHT);
	m_pPlayer->setObjCenPos((float)EC_ConstNUM::E_BACKGROUND_WIDTH / 2.f, (float)EC_ConstNUM::E_BACKGROUND_HEIGHT / 2.f);
	m_pPlayer->setDirection(0.f, 0.f);
	m_pPlayer->setScale(0.5f);
	m_pPlayer->setVelocity(1.5f);

	m_nNowTime = 0;
	m_bGameStarted = false;
}

void C_GAME::destoyAllBullet()
{
	for (int i = 2; i < C_OBJMGR::E_MAX; ++i)
		m_pObjMGR->destroyAllObj((C_OBJMGR::E_ObjTYPE)i);
}

void C_GAME::createBulletNum1()
{
	int nRandIndex = (rand() * m_pObjMGR->getSize(C_OBJMGR::E_BULLET_TYPE1)) % 4;
	int nXpos = 0;
	int nYpos = 0;
	switch (nRandIndex)
	{
	case 0:
		nXpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = 0;
		break;
	case 1:
		nXpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	case 2:
		nXpos = 0;
		nYpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	case 3:
		nXpos = (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	}
	C_OBJECT* pBullet = m_pObjMGR->createObj(C_OBJMGR::E_BULLET_TYPE1);
	pBullet->setBitmapID(C_BITMAPMGR::E_BULLET);
	pBullet->setCollideRad((float)EC_ConstNUM::E_BULLET_WIDTH / 2.f);
	pBullet->setDCStartPos(0.f, 0.f);
	pBullet->setDirection(m_pPlayer->getObjCenPos()->fX - (float)nXpos, m_pPlayer->getObjCenPos()->fY - (float)nYpos);
	pBullet->setImageSize((float)EC_ConstNUM::E_BULLET_WIDTH, (float)EC_ConstNUM::E_BULLET_HEIGHT);
	pBullet->setObjCenPos((float)nXpos, (float)nYpos);
	pBullet->setScale(0.5f);
	pBullet->setVelocity(1.f);
}

void C_GAME::createBulletNum2()
{
	int nRandIndex = (rand() * m_pObjMGR->getSize(C_OBJMGR::E_BULLET_TYPE2)) % 4;
	int nXpos = 0;
	int nYpos = 0;
	switch (nRandIndex)
	{
	case 0:
		nXpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = 0;
		break;
	case 1:
		nXpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	case 2:
		nXpos = 0;
		nYpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	case 3:
		nXpos = (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	}
	C_OBJECT* pBullet = m_pObjMGR->createObj(C_OBJMGR::E_BULLET_TYPE2);
	pBullet->setBitmapID(C_BITMAPMGR::E_BULLET);
	pBullet->setCollideRad((float)EC_ConstNUM::E_BULLET_WIDTH / 2.f);
	pBullet->setDCStartPos(20.f, 0.f);
	pBullet->setDirection(m_pPlayer->getObjCenPos()->fX - (float)nXpos, m_pPlayer->getObjCenPos()->fY - (float)nYpos);
	pBullet->setImageSize((float)EC_ConstNUM::E_BULLET_WIDTH, (float)EC_ConstNUM::E_BULLET_HEIGHT);
	pBullet->setObjCenPos((float)nXpos, (float)nYpos);
	pBullet->setScale(1.f);
	pBullet->setVelocity(2.f);
}

void C_GAME::createBulletNum3()
{
	int nRandIndex = (rand() * m_pObjMGR->getSize(C_OBJMGR::E_BULLET_TYPE3)) % 4;
	int nXpos = 0;
	int nYpos = 0;
	switch (nRandIndex)
	{
	case 0:
		nXpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = 0;
		break;
	case 1:
		nXpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	case 2:
		nXpos = 0;
		nYpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	case 3:
		nXpos = (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	}
	C_OBJECT* pBullet = m_pObjMGR->createObj(C_OBJMGR::E_BULLET_TYPE3);
	pBullet->setBitmapID(C_BITMAPMGR::E_BULLET);
	pBullet->setCollideRad((float)EC_ConstNUM::E_BULLET_WIDTH / 2.f);
	pBullet->setDCStartPos(40.f, 0.f);
	pBullet->setDirection(m_pPlayer->getObjCenPos()->fX - (float)nXpos, m_pPlayer->getObjCenPos()->fY - (float)nYpos);
	pBullet->setImageSize((float)EC_ConstNUM::E_BULLET_WIDTH, (float)EC_ConstNUM::E_BULLET_HEIGHT);
	pBullet->setObjCenPos((float)nXpos, (float)nYpos);
	pBullet->setScale(0.5f);
	pBullet->setVelocity(1.5f);
}

void C_GAME::createBulletNum41()
{
	int nXpos = 0;
	int nYpos = 0;
	
	C_OBJECT* pBullet = m_pObjMGR->createObj(C_OBJMGR::E_BULLET_TYPE3);
	pBullet->setBitmapID(C_BITMAPMGR::E_BULLET);
	pBullet->setCollideRad((float)EC_ConstNUM::E_BULLET_WIDTH / 2.f);
	pBullet->setDCStartPos(60.f, 0.f);
	pBullet->setDirection(m_pPlayer->getObjCenPos()->fX - (float)nXpos, m_pPlayer->getObjCenPos()->fY - (float)nYpos);
	pBullet->setImageSize((float)EC_ConstNUM::E_BULLET_WIDTH, (float)EC_ConstNUM::E_BULLET_HEIGHT);
	pBullet->setObjCenPos((float)nXpos, (float)nYpos);
	pBullet->setScale(0.5f);
	pBullet->setVelocity(1.f);
}

void C_GAME::createBulletNum42()
{
	int nXpos = (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
	int nYpos = 0;

	C_OBJECT* pBullet = m_pObjMGR->createObj(C_OBJMGR::E_BULLET_TYPE3);
	pBullet->setBitmapID(C_BITMAPMGR::E_BULLET);
	pBullet->setCollideRad((float)EC_ConstNUM::E_BULLET_WIDTH / 2.f);
	pBullet->setDCStartPos(60.f, 0.f);
	pBullet->setDirection(m_pPlayer->getObjCenPos()->fX - (float)nXpos, m_pPlayer->getObjCenPos()->fY - (float)nYpos);
	pBullet->setImageSize((float)EC_ConstNUM::E_BULLET_WIDTH, (float)EC_ConstNUM::E_BULLET_HEIGHT);
	pBullet->setObjCenPos((float)nXpos, (float)nYpos);
	pBullet->setScale(0.5f);
	pBullet->setVelocity(1.f);
}

void C_GAME::createBulletNum43()
{
	int nXpos = 0;
	int nYpos = (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;

	C_OBJECT* pBullet = m_pObjMGR->createObj(C_OBJMGR::E_BULLET_TYPE3);
	pBullet->setBitmapID(C_BITMAPMGR::E_BULLET);
	pBullet->setCollideRad((float)EC_ConstNUM::E_BULLET_WIDTH / 2.f);
	pBullet->setDCStartPos(60.f, 0.f);
	pBullet->setDirection(m_pPlayer->getObjCenPos()->fX - (float)nXpos, m_pPlayer->getObjCenPos()->fY - (float)nYpos);
	pBullet->setImageSize((float)EC_ConstNUM::E_BULLET_WIDTH, (float)EC_ConstNUM::E_BULLET_HEIGHT);
	pBullet->setObjCenPos((float)nXpos, (float)nYpos);
	pBullet->setScale(0.5f);
	pBullet->setVelocity(1.f);
}

void C_GAME::createBulletNum44()
{
	int nXpos = (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
	int nYpos = (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;

	C_OBJECT* pBullet = m_pObjMGR->createObj(C_OBJMGR::E_BULLET_TYPE3);
	pBullet->setBitmapID(C_BITMAPMGR::E_BULLET);
	pBullet->setCollideRad((float)EC_ConstNUM::E_BULLET_WIDTH / 2.f);
	pBullet->setDCStartPos(60.f, 0.f);
	pBullet->setDirection(m_pPlayer->getObjCenPos()->fX - (float)nXpos, m_pPlayer->getObjCenPos()->fY - (float)nYpos);
	pBullet->setImageSize((float)EC_ConstNUM::E_BULLET_WIDTH, (float)EC_ConstNUM::E_BULLET_HEIGHT);
	pBullet->setObjCenPos((float)nXpos, (float)nYpos);
	pBullet->setScale(0.5f);
	pBullet->setVelocity(1.f);
}

void C_GAME::createBulletNum5()
{
	int nRandIndex = (rand() * m_pObjMGR->getSize(C_OBJMGR::E_BULLET_TYPE5)) % 4;
	int nXpos = 0;
	int nYpos = 0;
	switch (nRandIndex)
	{
	case 0:
		nXpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = 0;
		break;
	case 1:
		nXpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	case 2:
		nXpos = 0;
		nYpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	case 3:
		nXpos = (int)EC_ConstNUM::E_BACKGROUND_WIDTH;
		nYpos = rand() % (int)EC_ConstNUM::E_BACKGROUND_HEIGHT;
		break;
	}
	C_OBJECT* pBullet = m_pObjMGR->createObj(C_OBJMGR::E_BULLET_TYPE5);
	pBullet->setBitmapID(C_BITMAPMGR::E_BULLET);
	pBullet->setCollideRad((float)EC_ConstNUM::E_BULLET_WIDTH / 2.f);
	pBullet->setDCStartPos(80.f, 0.f);
	pBullet->setDirection(m_pPlayer->getObjCenPos()->fX - (float)nXpos, m_pPlayer->getObjCenPos()->fY - (float)nYpos);
	pBullet->setImageSize((float)EC_ConstNUM::E_BULLET_WIDTH, (float)EC_ConstNUM::E_BULLET_HEIGHT);
	pBullet->setObjCenPos((float)nXpos, (float)nYpos);
	pBullet->setScale(1.5f);
	pBullet->setVelocity(1.5f);
}

void C_GAME::showDieSCR()
{
	for (int i = 1; i < C_OBJMGR::E_MAX; ++i)
	{
		for (auto iter = m_pObjMGR->getBeginIter((C_OBJMGR::E_ObjTYPE)i);
			iter != m_pObjMGR->getEndIter((C_OBJMGR::E_ObjTYPE)i); ++iter)
			(*iter)->setVelocity(0.f);
	}
	m_bGameStarted = false;
	m_pDurationTimer->stopTimer();
}

void C_GAME::controlMSG()
{
	if (m_arKeyState[VK_UP])
		keydownUp();
	if (m_arKeyState[VK_DOWN])
		keydownDown();
	if (m_arKeyState[VK_LEFT])
		keydownLeft();
	if (m_arKeyState[VK_RIGHT])
		keydownRight();
	if (m_arKeyState[VK_SPACE])
		keydownSpace();
}
