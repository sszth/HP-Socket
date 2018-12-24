#pragma once


/******************************************************************************
Module:  PrivateHeap.h
Notices: Copyright (c) 2006 Bruce Liang
Purpose: �������˽�ж�.
Desc:
		 1. CPrivateHeap:		�Զ����������ٽ���˽�ж�
								ÿһ������Ķ��󶼴���һ��˽�ж�, ����
								���������ص���: һ���������ڶ��Ƚϳ�
								ͨ����Ϊȫ�ֶ���, ������ľ�̬��Ա����
								����һЩ���������������ĳ�Ա����
		 2. CPrivateHeapBuffer: ��˽�ж����Զ�������ͷ�ָ����С���ڴ�
								һ�������ں������ڷ�����ͷžֲ�������Ķ��ڴ�
								�Ӷ������ CPrivateHeap::Alloc() ��
								CPrivateHeap::Free() �ĵ���

Examples:
			CPrivateHeap g_hpPrivate;

			int _tmain(int argc, _TCHAR* argv[])
			{
				CPrivateHeapStrBuffer buff(g_hpPrivate, 32);
				lstrcpy(buff, _T("ʧ���˳ɹ�֮ĸ"));
				SIZE_T size = buff.Size();
				buff.ReAlloc(40);
				size = buff.Size();
				std::cout << (TCHAR*)buff << '\n';
				// OR
				// ASSERT(g_hpPrivate.IsValid());
				// TCHAR* pch	= (TCHAR*)g_hpPrivate.Alloc(32 * sizeof(TCHAR));
				// lstrcpy(pch, _T("ʧ���˳ɹ�֮ĸ"));
				// SIZE_T size = g_hpPrivate.Size(pch);
				// g_hpPrivate.ReAlloc(pch, 40 * sizeof(TCHAR));
				// size = g_hpPrivate.Size(pch);
				// std::cout << pch << '\n';
				// g_hpPrivate.Free(pch);
				//
				return 0;
			}

******************************************************************************/

class CPrivateHeapImpl
{
public:
	PVOID Alloc(SIZE_T dwSize, DWORD dwFlags = 0) { return ::HeapAlloc(m_hHeap, dwFlags, dwSize); }
	PVOID ReAlloc(PVOID pvMemory, SIZE_T dwSize, DWORD dwFlags = 0) { return ::HeapReAlloc(m_hHeap, dwFlags, pvMemory, dwSize); }
	SIZE_T Size(PVOID pvMemory, DWORD dwFlags = 0) { return ::HeapSize(m_hHeap, dwFlags, pvMemory); }
	BOOL Free(PVOID pvMemory, DWORD dwFlags = 0) { return ::HeapFree(m_hHeap, dwFlags, pvMemory); }
	SIZE_T Compact(DWORD dwFlags = 0) { return ::HeapCompact(m_hHeap, dwFlags); }
	BOOL IsValid() { return m_hHeap != nullptr; }
	BOOL Reset()
	{
		if (IsValid())
		{
			::HeapDestroy(m_hHeap);
		}
		m_hHeap = ::HeapCreate(m_dwOptions, m_dwInitSize, m_dwMaxSize);
		return IsValid();
	}
public:
	CPrivateHeapImpl(DWORD dwOptions = 0, SIZE_T dwInitSize = 0, SIZE_T dwMaxSize = 0)
		: m_dwOptions(dwOptions), m_dwInitSize(dwInitSize), m_dwMaxSize(dwMaxSize)
	{
		m_hHeap = ::HeapCreate(m_dwOptions, m_dwInitSize, m_dwMaxSize);
	}
	~CPrivateHeapImpl()
	{
		if (IsValid())
		{
			::HeapDestroy(m_hHeap);
		}
	}

	operator HANDLE() { return m_hHeap; }
	DECLARE_NO_COPY_CLASS(CPrivateHeapImpl)
private:
	HANDLE	m_hHeap;
	DWORD	m_dwOptions;
	SIZE_T	m_dwInitSize;
	SIZE_T	m_dwMaxSize;
};

class CGlobalHeapImpl
{
public:
	PVOID Alloc(SIZE_T dwSize, DWORD dwFlags = 0)
	{
		PVOID pv = malloc(dwSize);

		if (pv && (dwFlags & HEAP_ZERO_MEMORY))
		{
			ZeroMemory(pv, dwSize);
		}

		return pv;
	}

	SIZE_T Compact(DWORD dwFlags = 0) { return 0; }
	SIZE_T Size(PVOID pvMemory, DWORD dwFlags = 0) { return _msize(pvMemory); }

	BOOL IsValid() { return TRUE; }
	BOOL Reset() { return TRUE; }
public:
	CGlobalHeapImpl(DWORD dwOptions = 0, SIZE_T dwInitSize = 0, SIZE_T dwMaxSize = 0){}
	~CGlobalHeapImpl(){}

	operator HANDLE() { return nullptr; }
	DECLARE_NO_COPY_CLASS(CGlobalHeapImpl)
};

#ifndef _NOT_USE_PRIVATE_HEAP
	typedef CPrivateHeapImpl CPrivateHeap;
#else
	typedef CGlobalHeapImpl CPrivateHeap;
#endif // !_NOT_USE_PRIVATE_HEAP

template<class T>
class CPrivateHeapBuffer
{
public:
	CPrivateHeapBuffer(CPrivateHeap& hpPrivate, SIZE_T dwSize = 0) 
	{}
public:
	T * Alloc(SIZE_T dwSize, DWORD dwFlags = 0)
	{
		if (IsValid())
		{
			Free();
		}

		if (dwSize > 0)
		{
			m_pvMemory = (T*)m_hpPrivate.Alloc(dwSize * sizeof(t), dwFlags);
		}
		//���ÿ���new��0�����·����ڴ�Ҷ�������ô
		return m_pvMemory;
	}

	T * ReAlloc(SIZE_T dwSize, DWORD dwFlags = 0)
	{
		return m_pvMemory = (T*)m_hpPrivate.ReAlloc(m_pvMemory, dwSize * sizeof(T), dwFlags);
	}

	BOOL IsValid() { return m_pvMemory != nullptr; }
	operator T * () const { return m_pvMemory; }
	T & operator[] ( int i ) const { return *(m_pvMemory)+i; }

	DECLARE_NO_COPY_CLASS(CPrivateHeapBuffer)
private:
	CPrivateHeap &	m_hpPrivate;
	T *				m_pvMemory;
};






