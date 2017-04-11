#include "stdafx.h"

C_TIMER::C_TIMER()
	: m_nBeginTime(NULL)
	, m_bTimerUse(false)
{
}

void C_TIMER::setBeginTime()
{
	m_bTimerUse = true;
	m_nBeginTime = GetTickCount();
}

UINT C_TIMER::getNowTime()
{
	return GetTickCount() - m_nBeginTime;
}

void C_TIMER::stopTimer()
{
	m_bTimerUse = false;
}

bool C_TIMER::getTimerState()
{
	return m_bTimerUse;
}
