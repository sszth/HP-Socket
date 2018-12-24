#pragma once

#include "Singleton.h"

#include "PrivateHeap.h"

#pragma warning(push)
#pragma warning(disable: 4458)

struct TItem
{
	template<class T> friend struct TSimpleList;
	template<class T> friend class	CNodePoolT;

	friend struct TItemList;
	friend struct TBuffer;

public:
	inline int Cat(const BYTE * pData, int Length);
	inline int Cat(const TItem & other);
	inline int Fetch(BYTE * pData, int length);
	inline int Peek(BYTE * pDatam, int length);
	inline int Reduce(int length);
	inline void Reset(int first = 0, int last = 0);

	BYTE * Ptr() { return begin; }
	const BYTE * Ptr() const { return begin; }
	int Size() const { return (int)(end - begin); }
	int Remain() const { return capacity - (int)(end - head); }
	int Capacity() const { return capacity; }
	bool IsEmpty() const { return Size() == 0; }
	bool IsFull() const { return Remain() == 0; }

public:
	operator BYTE * () { return Ptr(); }
	operator const BYTE * () const { return Ptr(); }

public:
	static TItem * Construct(CPrivateHeap)



private:
	TItem * next;
	TItem * last;

	int		capacity;
	BYTE *	head;
	BYTE *	begin;
	BYTE *	end;
};


class BufferPool
{
public:
	BufferPool();
	virtual ~BufferPool();
};

