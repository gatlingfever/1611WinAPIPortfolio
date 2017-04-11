#include "stdafx.h"

C_OBJECT::C_OBJECT()
	: m_sObject(nullptr)
{
}

//create object struct(all contents should be setted before use)
bool C_OBJECT::createOBJ()
{
	if (m_sObject)
		return false;
	m_sObject = new S_OBJECT;
	return true;
}

//destroy object
void C_OBJECT::destroyOBJ()
{
	delete m_sObject;
	m_sObject = nullptr;
}

//set BitmapImageID (ID defined in C_BITMAPMGR::E_BITMAP_TYPE)
void C_OBJECT::setBitmapID(C_BITMAPMGR::E_BITMAP_TYPE eBitmapID)
{
	m_sObject->eBitmapID = eBitmapID;
}

//get BitmapImageID
C_BITMAPMGR::E_BITMAP_TYPE C_OBJECT::getBitmapID()
{
	return m_sObject->eBitmapID;
}

//set Object's center position (Image Center & Collision Center)
void C_OBJECT::setObjCenPos(float fXpos, float fYpos)
{
	m_sObject->fObjCenPos.fX = fXpos;
	m_sObject->fObjCenPos.fY = fYpos;
}

//get Object's Center Position
fpoint * C_OBJECT::getObjCenPos()
{
	return &(m_sObject->fObjCenPos);
}

//set DC Start Position (use for sprite)
void C_OBJECT::setDCStartPos(float fXpos, float fYpos)
{
	m_sObject->fDcStartPos.fX = fXpos;
	m_sObject->fDcStartPos.fY = fYpos;
}

//get DC Start Position
fpoint * C_OBJECT::getDCStartPos()
{
	return &(m_sObject->fDcStartPos);
}

//set Image Size in original DC
void C_OBJECT::setImageSize(float fWidth, float fHeight)
{
	m_sObject->fImageSize.fX = fWidth;
	m_sObject->fImageSize.fY = fHeight;
}

//get Original Image Size (Scale not included)
fpoint * C_OBJECT::getImageSize()
{
	return &(m_sObject->fImageSize);
}

//set Direction (auto normalize)
void C_OBJECT::setDirection(float fXdir, float fYdir)
{
	m_sObject->fDirection = normalizeFPoint({ fXdir, fYdir });
}

//get Direction
fpoint * C_OBJECT::getDirection()
{
	return &(m_sObject->fDirection);
}

//set velocity (pixel per frame)
void C_OBJECT::setVelocity(float fVelocity)
{
	m_sObject->fVelocity = fVelocity;
}

//get velocity
float C_OBJECT::getVelocity()
{
	return m_sObject->fVelocity;
}

//set Collide radius (from Center Position)
void C_OBJECT::setCollideRad(float fRad)
{
	m_sObject->fCollideRad = fRad;
}

//get Collide Radius
float C_OBJECT::getcollideRad()
{
	return m_sObject->fCollideRad;
}

//set Scale (should be 1.f in default value)
void C_OBJECT::setScale(float fScale)
{
	m_sObject->fScale = fScale;
}

//get Scale
float C_OBJECT::getScale()
{
	return m_sObject->fScale;
}

//calc Collision with pTargetObj (return true : collided)
bool C_OBJECT::isCollided(C_OBJECT * pTargetObj)
{
	float fRadSumSQ = (getcollideRad()*getScale() + pTargetObj->getcollideRad()*pTargetObj->getScale()) 
		* (getcollideRad()*getScale() + pTargetObj->getcollideRad()*pTargetObj->getScale());
	float fLenSQ = getSQLentoFPoint(*(pTargetObj->getObjCenPos()));
	
	if (fLenSQ - fRadSumSQ < 0.00001f)
		return true;
	else
		return false;
}

//update Object
void C_OBJECT::update()
{
	m_sObject->fObjCenPos.fX += m_sObject->fDirection.fX * m_sObject->fVelocity;
	m_sObject->fObjCenPos.fY += m_sObject->fDirection.fY * m_sObject->fVelocity;
}

//get normalized vector2
fpoint C_OBJECT::normalizeFPoint(fpoint fpoint)
{
	float fLength = sqrtf(fpoint.fX * fpoint.fX + fpoint.fY * fpoint.fY);
	if (!fLength)
		return{ 0.f, 0.f };
	fpoint.fX /= fLength;
	fpoint.fY /= fLength;
	return fpoint;
}

//get length to point
float C_OBJECT::getLentoFPoint(fpoint fpoint)
{
	return sqrtf(getSQLentoFPoint(fpoint));
}

//get squared length to point
float C_OBJECT::getSQLentoFPoint(fpoint fpoint)
{
	return (fpoint.fX - m_sObject->fObjCenPos.fX) * (fpoint.fX - m_sObject->fObjCenPos.fX)
		+ (fpoint.fY - m_sObject->fObjCenPos.fY) * (fpoint.fY - m_sObject->fObjCenPos.fY);
}

bool C_OBJECT::isOutofSCR()
{
	float fRad = m_sObject->fCollideRad * m_sObject->fScale;
	if (m_sObject->fObjCenPos.fX > (float)EC_ConstNUM::E_BACKGROUND_WIDTH - fRad
		|| m_sObject->fObjCenPos.fX < fRad)
		return true;
	if (m_sObject->fObjCenPos.fY >(float)EC_ConstNUM::E_BACKGROUND_HEIGHT - fRad
		|| m_sObject->fObjCenPos.fY < fRad)
		return true;

	return false;
}

bool C_OBJECT::isOutofSCRBullet()
{
	if (m_sObject->fObjCenPos.fX >(float)EC_ConstNUM::E_BACKGROUND_WIDTH + 100
		|| m_sObject->fObjCenPos.fX < -100)
		return true;
	if (m_sObject->fObjCenPos.fY >(float)EC_ConstNUM::E_BACKGROUND_HEIGHT + 100
		|| m_sObject->fObjCenPos.fY < - 100)
		return true;

	return false;
}
