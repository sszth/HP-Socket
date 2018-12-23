#pragma once

#include <intrin.h>


//���ƿ��Կ���õ����������ڴ���ʲ����ı������Ż���
//_ReadBarrier��_WriteBarrier��_ReadWriteBarrier�������ڲ������Լ�MemoryBarrier�궼�ǲ��޳�ʹ�õġ�
//�����̼߳�ͨ�ţ�����ʹ��atomic_thread_fence��std::atomic�Ȼ��ƣ���Щ��������c++��׼���ж���ġ�
//����Ӳ�����ʣ�ʹ�� / volatile:iso������ѡ���Լ�volatile�ؼ��֡� by 2016.11
#pragma intrinsic(_ReadBarrier)
#pragma intrinsic(_WriteBarrier)
#pragma intrinsic(_ReadWriteBarrier)

#define DEFAULT_CRISEC_SPIN_COUNT	0
#define THREAD_YIELD_CYCLE			63
#define THREAD_SWITCH_CYCLE			4095

#ifndef YieldProcessor
#pragma intrinsic(_mm_pause)
#define YieldProcessor _mm_pause
#endif

inline void YieldThread(UINT i = THREAD_YIELD_CYCLE)
{
	if ((i & THREAD_SWITCH_CYCLE) == THREAD_SWITCH_CYCLE)
		::SwitchToThread();
	else if ((i & THREAD_YIELD_CYCLE) == THREAD_YIELD_CYCLE)
		::YieldProcessor();
}

class CInterCriSec
{
public:
	CInterCriSec(DWORD dwSpinCount = DEFAULT_CRISEC_SPIN_COUNT)
	{
		//ENSURE�˹���������֤�����ݵĲ����ĺ�����
		ENSURE(::InitializeCriticalSectionAndSpinCount(&m_crisec, dwSpinCount));
	}
	~CInterCriSec()
	{
		::DeleteCriticalSection(&m_crisec);
	}
	void Lock() { ::EnterCriticalSection(&m_crisec); }
	void UnLock() { ::LeaveCriticalSection(&m_crisec); }
	BOOL TryLock() { return ::TryEnterCriticalSection(&m_crisec); }
	DWORD SetSpinCount(DWORD dwSpinCount) { return ::SetCriticalSectionSpinCount(&m_crisec, dwSpinCount); }

	CRITICAL_SECTION * GetObject() { return &m_crisec; }

	DECLARE_PRIVATE_COPY_CONSTRUCTOR(CInterCriSec)
private:
	CRITICAL_SECTION m_crisec;
};

class CInterCriSec2
{
public:
	CInterCriSec2(DWORD dwSpinCount = DEFAULT_CRISEC_SPIN_COUNT, BOOL bInitialize = TRUE)
	{
		if (bInitialize)
		{
			m_pcrisec = new CRITICAL_SECTION();
			ENSURE(::InitializeCriticalSectionAndSpinCount(m_pcrisec, dwSpinCount));
		}
		else
		{
			m_pcrisec = nullptr;
		}
	}

	void Attach(CRITICAL_SECTION * pcrisec)
	{
		Reset();
		m_pcrisec = pcrisec;
	}

	void Lock() { ::EnterCriticalSection(m_pcrisec); }
	void Unlock() { ::LeaveCriticalSection(m_pcrisec); }
	BOOL TryLock() { return ::TryEnterCriticalSection(m_pcrisec); }
	DWORD SetSpinCount(DWORD dwSpinCount) { return ::SetCriticalSectionSpinCount(m_pcrisec, dwSpinCount); }

	CRITICAL_SECTION * GetObject() { return m_pcrisec; }

	DECLARE_PRIVATE_COPY_CONSTRUCTOR(CInterCriSec2)
private:
	void Reset()
	{
		if (m_pcrisec)
		{
			::DeleteCriticalSection(m_pcrisec);
			delete m_pcrisec;
			m_pcrisec = nullptr;
		}
	}
private:
	CRITICAL_SECTION * m_pcrisec;
};

class CMTX
{
public:
	CMTX(BOOL bInitialOwner = FALSE, LPCTSTR pszName = nullptr, LPSECURITY_ATTRIBUTES pSecurity = nullptr)
	{
		m_hMutex = ::CreateMutex(pSecurity, bInitialOwner, pszName);
		ASSERT(IsValid());
	}

	~CMTX()
	{
		if (IsValid())
		{
			::CloseHandle(m_hMutex);

		}
	}

	BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
	{
		if (IsValid())
		{
			ENSURE(::CloseHandle(m_hMutex));
		}

		m_hMutex = ::OpenMutex(dwAccess, bInheritHandle, pszName);
		return IsValid();
	}

	void Lock(DWORD dwMilliseconds = INFINITE) { ::WaitForSingleObject(m_hMutex, dwMilliseconds); }
	void UnLock() { ::ReleaseMutex(m_hMutex); }

	HANDLE & GetHandle() { return m_hMutex; }
	operator HANDLE() { return m_hMutex; }
	BOOL IsValid() { return m_hMutex != nullptr; }

	DECLARE_PRIVATE_COPY_CONSTRUCTOR(CMTX)
private:
	HANDLE m_hMutex;
};

class CSpinGuard
{
public:
	CSpinGuard() : m_lFlag(0)
	{}

	~CSpinGuard()
	{
		ASSERT(m_lFlag == 0);
	}

	void Lock()
	{
		for (UINT i = 0; !TryLock(); ++i)
		{
			YieldThread(i);
		}
	}

	BOOL TryLock()
	{
		if (::_InterlockedCompareExchange(&m_lFlag, 1, 0) == 0)
		{
			::_ReadWriteBarrier();
			return TRUE;
		}

		return FALSE;
	}

	void Unlock()
	{
		ASSERT(m_lFlag == 1);
		m_lFlag = 0;
	}


	DECLARE_PRIVATE_COPY_CONSTRUCTOR(CSpinGuard)

private:
	volatile LONG m_lFlag;
};

class CReentrantSpinGuard
{
public:
	CReentrantSpinGuard()
		: m_dwThreadID(0)
		, m_iCount(0)
	{

	}

	~CReentrantSpinGuard()
	{
		ASSERT(m_dwThreadID == 0);
		ASSERT(m_iCount == 0);
	}

	void Lock()
	{
		for (UINT i = 0; _TryLock(i == 0); ++i)
		{
			YieldThread(i);
		}
	}

	BOOL TryLock()
	{
		return _TryLock(TRUE);
	}

	void Unlock()
	{
		ASSERT(m_dwThreadID == ::GetCurrentThreadId());

		if ((--m_iCount) == 0 )
		{
			m_dwThreadID = 0;
		}
	}

	DECLARE_PRIVATE_COPY_CONSTRUCTOR(CReentrantSpinGuard)
private:
	BOOL _TryLock(BOOL bFirst)
	{
		DWORD dwCurrentThreadID = ::GetCurrentThreadId();

		if (bFirst && m_dwThreadID == dwCurrentThreadID)
		{
			++m_iCount;
			return TRUE;
		}

		if (::InterlockedCompareExchange(&m_dwThreadID, dwCurrentThreadID, 0) == 0 )
		{
			::_ReadWriteBarrier();
			ASSERT(m_iCount == 0);

			m_iCount = 1;

			return TRUE;
		}

		return FALSE;
	}


private:
	volatile DWORD	m_dwThreadID;
	int				m_iCount;
};

class CFakeGuard
{
public:
	void Lock(){}
	void Unlock(){}
	BOOL TryLock() { return TRUE; }
};

template<class CLockObj>
class CLocalLock
{
public:
	CLocalLock(CLockObj & obj)
		: m_lock(obj)
	{
		m_lock.lock();
	}

	~CLocalLock()
	{
		m_lock.Unlock();
	}

private:
	CLockObj & m_lock;
};

template<class CLockObj>
class CLocalTryLock
{
public:
	CLocalTryLock(CLockObj & obj)
		: m_lock(obj)
	{
		m_bValid = m_lock.TryLock();
	}

	~CLocalTryLock()
	{
		if (m_bValid)
		{
			m_lock.Unlock();
		}
	}

	BOOL IsValid()
	{
		return m_bValid;
	}
private:
	CLockObj &	m_lock;
	BOOL		m_bValid;
};

typedef CInterCriSec						CCriSec;

typedef CLocalLock<CCriSec>					CCriSecLock;
typedef CLocalLock<CInterCriSec>			CInterCriSecLock;
typedef CLocalLock<CInterCriSec2>			CInterCriSecLock2;
typedef CLocalLock<CMTX>					CMutexLock;
typedef CLocalLock<CSpinGuard>				CSpinLock;
typedef CLocalLock<CReentrantSpinGuard>		CReentrantSpinLock;
typedef CLocalLock<CFakeGuard>				CFakeLock;

typedef CLocalTryLock<CCriSec>				CCriSecTryLock;
typedef CLocalTryLock<CInterCriSec>			CInterCriSecTryLock;
typedef CLocalTryLock<CInterCriSec2>		CInterCriSecTryLock2;
typedef CLocalTryLock<CMTX>					CMutexTryLock;
typedef CLocalTryLock<CSpinGuard>			CSpinTryLock;
typedef CLocalTryLock<CReentrantSpinGuard>	CReentrantSpinTryLock;
typedef CLocalTryLock<CFakeGuard>			CFackeTryLock;

