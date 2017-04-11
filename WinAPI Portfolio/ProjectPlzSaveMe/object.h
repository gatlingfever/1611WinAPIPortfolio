#pragma once

class C_OBJECT
{
private:
	
private:
	struct S_OBJECT
	{
		C_BITMAPMGR::E_BITMAP_TYPE	eBitmapID;
		
		fpoint fObjCenPos;

		fpoint fDcStartPos;
		fpoint fImageSize;
		
		fpoint fDirection;
		float fVelocity;
		
		float fCollideRad;
		
		float fScale;
	};

private:
	S_OBJECT*	m_sObject;

public:
	C_OBJECT();

	bool	createOBJ		();
	void	destroyOBJ		();

	void	setBitmapID						(C_BITMAPMGR::E_BITMAP_TYPE eBitmapID);
	C_BITMAPMGR::E_BITMAP_TYPE getBitmapID	();

	void	setObjCenPos	(float fXpos, float fYpos);
	fpoint*	getObjCenPos	();

	void	setDCStartPos	(float fXpos, float fYpos);
	fpoint*	getDCStartPos	();

	void	setImageSize	(float fWidth, float fHeight);
	fpoint*	getImageSize	();

	void	setDirection	(float fXdir, float fYdir);
	fpoint*	getDirection	();

	void	setVelocity		(float fVelocity);
	float	getVelocity		();

	void	setCollideRad	(float fRad);
	float	getcollideRad	();

	void	setScale		(float fScale);
	float	getScale		();

public:
	bool	isCollided		(C_OBJECT* pTargetObj);
	void	update			();
	bool	isOutofSCR		();
	bool	isOutofSCRBullet();

private:
	fpoint	normalizeFPoint	(fpoint fpoint);
	float	getLentoFPoint	(fpoint fpoint);
	float	getSQLentoFPoint(fpoint fpoint);
};