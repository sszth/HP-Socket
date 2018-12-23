#pragma once
#include "SocketInterface.h"
#include "CriticalSection.h"


class CUdpClient :
	public IUdpClient
{
public:
	CUdpClient();
	virtual ~CUdpClient();

private:
	CSpinGuard			m_csState;

	CCriSec				m_csSend;
	TItemList			m_lsSend;
};

