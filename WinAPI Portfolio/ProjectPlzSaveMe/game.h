#pragma once

class C_GAME
{
private:
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
	bool			m_arKeyState[256];

	C_OBJMGR*		m_pObjMGR;
	C_GRAPHICS*		m_pGraphics;
	C_OBJECT*		m_pPlayer;
	C_TIMER*		m_pDurationTimer;

	bool			m_bGameStarted;

	UINT			m_nNowTime;

	UINT			m_nBullet1Num;
	UINT			m_nBullet2Num;
	UINT			m_nBullet3Num;
	UINT			m_nBullet4Num;
	float			m_fBullet4Index;
	float			m_fElapsedTime;
	UINT			m_nBullet5Num;
	float			m_fBullet5Index;

public:
	C_GAME							();
	bool			initGame		(HINSTANCE& hInstance, HWND& hWnd);
	bool			releaseGame		();
	void			update			();
	void			render			();
	void			controlMSG();
	
	bool&			getKeyStateRef	(UINT nKeyIndex);

private:
	void			setBackGround	();
	void			setPlayer		();
	void			setPlayerHDC	();
	
	void			keydownUp		();
	void			keydownRight	();
	void			keydownDown		();
	void			keydownLeft		();
	void			keydownSpace	();

	void			destoyAllBullet ();
	void			createBulletNum1();
	void			createBulletNum2();
	void			createBulletNum3();
	void			createBulletNum41();
	void			createBulletNum42();
	void			createBulletNum43();
	void			createBulletNum44();
	void			createBulletNum5();

	void			showDieSCR();
};