#pragma once

#define SINGLETON_THIS(ClassName)		ClassName::GetThis()
#define	SINGLETON_INSTANCE(ClassName)	ClassName::GetInstance()
#define SINGLETON_OBJECT(ObjName)		SINGLETON_INSTANCE(C##ObjName)

#define DEFINE_SINGLETON(ClassName)											\
	ClassName* ClassName::m_pThis = nullptr;

#define DEFINE_P_THIS(ClassName)											\
	DEFFINE_SINGLETON(ClassName)

#define DECLARE_SINGLETON_INTERFACE(ClassName)								\
public:																		\
	static ClassName * GetThis()	{return m_pThis;}						\
	static ClassName & GetInstance(){return *m_pThis;}						\
protected:																	\
	staic ClassName * m_pThis;


#define DECLARE_SINGLETON_CREATE_INSTANCE(ClassName)						\
public:																		\
	staic BOOL CreateInstance()												\
	{																		\
		if(!m_pThis)														\
			m_pThis = new ClassName();										\
																			\
		return m_pThis != nullptr;											\
	}																		\
																			\
	static BOOL DeleteInstance()											\
	{																		\
		if (m_pThis)														\
		{																	\
			delete m_pThis;													\
			m_pThis = nullptr												\
		}																	\
																			\
		return m_pThis == nullptr;											\
	}																		\

#define DECLARE_PRIVATE_DEFAULT_CONSTRCOUNTOR(ClassName)					\
private:																	\
	ClassName(){}															\

#define DECLARE_PRIVATE_COPY_CONSTRUCTOR(ClassName)							\
private:																	\
	ClassName(const ClassName &);											\
	ClassName & operator = (const ClassName &);								\

#define DECLARE_NO_COPY_CLASS(ClassName)									\
	DECLARE_PRIVATE_COPY_CONSTRUCTOR(ClassName)								\

#define DECLARE_SINGLETON_IMPLEMENT_NO_CREATE_INSTANCE(ClassName)			\
	DECLARE_SINGLETON_INTERFACE(ClassName)				\
	DECLARE_PRIVATE_DEFAULT_CONSTRCOUNTOR(ClassName)	\
	DECLARE_PRIVATE_COPY_CONSTRUCTOR(ClassName)			\


#define DECLARE_SINGLETON_IMPLEMENT_NO_DEFAULT_CONSTRUCTOR(ClassName)		\
	DECLARE_SINGLETON_CREATE_INSTANCE(ClassName)							\
	DECLARE_PRIVATE_COPY_CONSTRUCTOR(ClassName)

#define DECLARE_SINGLETON_IMPLEMENT(ClassName)								\
	DECLARE_SINGLETON_IMPLEMENT_NO_DEFAULT_CONSTRUCTOR(ClassName)			\
	DECLARE_PRIVATE_DEFAULT_CONSTRUCTOR(ClassName)

#define DECLARE_SINGLETON_NO_DEFAULT_CONSTRUCTOR(ClassName)					\
	DECLARE_SINGLETON_INTERFACE(ClassName)									\
	DECLARE_SINGLETON_IMPLEMENT_NO_DEFAULT_CONSTRUCTOR(ClassName)

#define DECLARE_SINGLETON(ClassName)										\
	DECLARE_SINGLETON_NO_DEFAULT_CONSTRUCTOR(ClassName)						\
	DECLARE_PRIVATE_DEFAULT_CONSTRUCTOR(ClassName)

template<class T>
class CSingleObject
{
public:
	CSingleObject() { T::CreateInstance(); }
	~CSingleObject() { T::DeleteInstance(); }
	T* GetPointer() { return T::GetThis(); }
	T& GetObject() { return T::GetInstance(); }
	BOOL IsValid() { return GetPointer() != nullptr; }
};

#define DECLARE_SINGLE_OBJECT(ClassName) CSingleObject<ClassName> _##ClassName##_Single_Object_;






