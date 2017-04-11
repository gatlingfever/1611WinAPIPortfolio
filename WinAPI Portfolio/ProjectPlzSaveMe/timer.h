#pragma once

class C_TIMER
{
private:
	UINT m_nBeginTime;
	bool m_bTimerUse;
	
public:
	C_TIMER();
	void	setBeginTime	();
	UINT	getNowTime		();
	void	stopTimer		();
	bool	getTimerState	();
};