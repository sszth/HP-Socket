#pragma once

#include <winsock2.h>

#include "HPTypeDef.h"

/*****************************************************************************************************************************************************/
/***************************************************************** TCP/UDP Interfaces ****************************************************************/
/*****************************************************************************************************************************************************/

/************************************************************************
���ƣ����� Socket ����ӿ�
���������帴�� Socket ��������в������������Է��ʷ��������� Socket ���ͬʱ������ Socket ����
************************************************************************/
class IComplexSocket
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ��ر�ͨ�����
	* �������ر�ͨ��������ر���ɺ�Ͽ��������Ӳ��ͷ�������Դ
	*
	* ������
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Stop() = 0;

	/*
	* ���ƣ���������
	* ��������ָ�����ӷ�������
	*
	* ������		dwConnID	-- ���� ID
	*			pBuffer		-- ���ͻ�����
	*			iLength		-- ���ͻ���������
	*			iOffset		-- ���ͻ�����ָ��ƫ����
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ��ϵͳ API ���� ::GetLastError() ��ȡ�������
	*/
	virtual BOOL Send(CONNID dwConnID, const BYTE* pBuffer, int iLength, int iOffset = 0) = 0;

	/*
	* ���ƣ����Ͷ�������
	* ��������ָ�����ӷ��Ͷ�������
	*		TCP - ˳�����������ݰ�
	*		UDP - ���������ݰ���ϳ�һ�����ݰ����ͣ����ݰ����ܳ��Ȳ��ܴ������õ� UDP ����󳤶ȣ�
	*
	* ������		dwConnID	-- ���� ID
	*			pBuffers	-- ���ͻ���������
	*			iCount		-- ���ͻ�������Ŀ
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ��ϵͳ API ���� ::GetLastError() ��ȡ�������
	*/
	virtual BOOL SendPackets(CONNID dwConnID, const WSABUF pBuffers[], int iCount) = 0;

	/*
	* ���ƣ���ͣ/�ָ�����
	* ��������ͣ/�ָ�ĳ�����ӵ����ݽ��չ���
	*
	* ������		dwConnID	-- ���� ID
	*			bPause		-- TRUE - ��ͣ, FALSE - �ָ�
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ��
	*/
	virtual BOOL PauseReceive(CONNID dwConnID, BOOL bPause = TRUE) = 0;

	/*
	* ���ƣ��Ͽ�����
	* �������Ͽ�ĳ������
	*
	* ������		dwConnID	-- ���� ID
	*			bForce		-- �Ƿ�ǿ�ƶϿ�����
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ��
	*/
	virtual BOOL Disconnect(CONNID dwConnID, BOOL bForce = TRUE) = 0;

	/*
	* ���ƣ��Ͽ���ʱ����
	* �������Ͽ�����ָ��ʱ��������
	*
	* ������		dwPeriod	-- ʱ�������룩
	*			bForce		-- �Ƿ�ǿ�ƶϿ�����
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ��
	*/
	virtual BOOL DisconnectLongConnections(DWORD dwPeriod, BOOL bForce = TRUE) = 0;

	/*
	* ���ƣ��Ͽ���Ĭ����
	* �������Ͽ�����ָ��ʱ���ľ�Ĭ����
	*
	* ������		dwPeriod	-- ʱ�������룩
	*			bForce		-- �Ƿ�ǿ�ƶϿ�����
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ��
	*/
	virtual BOOL DisconnectSilenceConnections(DWORD dwPeriod, BOOL bForce = TRUE) = 0;

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/*
	* ���ƣ��������ӵĸ�������
	* �������Ƿ�Ϊ���Ӱ󶨸������ݻ��߰�ʲô�������ݣ�����Ӧ�ó����������
	*
	* ������		dwConnID	-- ���� ID
	*			pv			-- ����
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���Ч������ ID��
	*/
	virtual BOOL SetConnectionExtra(CONNID dwConnID, PVOID pExtra) = 0;

	/*
	* ���ƣ���ȡ���ӵĸ�������
	* �������Ƿ�Ϊ���Ӱ󶨸������ݻ��߰�ʲô�������ݣ�����Ӧ�ó����������
	*
	* ������		dwConnID	-- ���� ID
	*			ppv			-- ����ָ��
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���Ч������ ID��
	*/
	virtual BOOL GetConnectionExtra(CONNID dwConnID, PVOID* ppExtra) = 0;

	/* ����Ƿ�Ϊ��ȫ���ӣ�SSL/HTTPS�� */
	virtual BOOL IsSecure() = 0;
	/* ���ͨ������Ƿ������� */
	virtual BOOL HasStarted() = 0;
	/* �鿴ͨ�������ǰ״̬ */
	virtual EnServiceState GetState() = 0;
	/* ��ȡ������ */
	virtual DWORD GetConnectionCount() = 0;
	/* ��ȡ�������ӵ� CONNID */
	virtual BOOL GetAllConnectionIDs(CONNID pIDs[], DWORD& dwCount) = 0;
	/* ��ȡĳ������ʱ�������룩 */
	virtual BOOL GetConnectPeriod(CONNID dwConnID, DWORD& dwPeriod) = 0;
	/* ��ȡĳ�����Ӿ�Ĭʱ�䣨���룩 */
	virtual BOOL GetSilencePeriod(CONNID dwConnID, DWORD& dwPeriod) = 0;
	/* ��ȡĳ�����ӵı��ص�ַ��Ϣ */
	virtual BOOL GetLocalAddress(CONNID dwConnID, TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
	/* ��ȡĳ�����ӵ�Զ�̵�ַ��Ϣ */
	virtual BOOL GetRemoteAddress(CONNID dwConnID, TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
	/* ��ȡ���һ��ʧ�ܲ����Ĵ������ */
	virtual EnSocketError GetLastError() = 0;
	/* ��ȡ���һ��ʧ�ܲ����Ĵ������� */
	virtual LPCTSTR GetLastErrorDesc() = 0;
	/* ��ȡ������δ�������ݵĳ��� */
	virtual BOOL GetPendingDataLength(CONNID dwConnID, int& iPending) = 0;
	/* ��ȡ���ӵ����ݽ���״̬ */
	virtual BOOL IsPauseReceive(CONNID dwConnID, BOOL& bPaused) = 0;
	/* ����Ƿ���Ч���� */
	virtual BOOL IsConnected(CONNID dwConnID) = 0;

	/* �������ݷ��Ͳ��� */
	virtual void SetSendPolicy(EnSendPolicy enSendPolicy) = 0;
	/* ���� OnSend �¼�ͬ�����ԣ�Ĭ�ϣ�OSSP_NONE����ͬ���� */
	virtual void SetOnSendSyncPolicy(EnOnSendSyncPolicy enSyncPolicy) = 0;
	/* ���������������������������ֵԤ�����ڴ棬�����Ҫ����ʵ��������ã����˹���*/
	virtual void SetMaxConnectionCount(DWORD dwMaxConnectionCount) = 0;
	/* ���� Socket �����������ʱ�䣨���룬�������ڼ�� Socket ��������ܱ���ȡʹ�ã� */
	virtual void SetFreeSocketObjLockTime(DWORD dwFreeSocketObjLockTime) = 0;
	/* ���� Socket ����ش�С��ͨ������Ϊƽ�������������� 1/3 - 1/2�� */
	virtual void SetFreeSocketObjPool(DWORD dwFreeSocketObjPool) = 0;
	/* �����ڴ�黺��ش�С��ͨ������Ϊ Socket ����ش�С�� 2 - 3 ���� */
	virtual void SetFreeBufferObjPool(DWORD dwFreeBufferObjPool) = 0;
	/* ���� Socket ����ػ��շ�ֵ��ͨ������Ϊ Socket ����ش�С�� 3 ���� */
	virtual void SetFreeSocketObjHold(DWORD dwFreeSocketObjHold) = 0;
	/* �����ڴ�黺��ػ��շ�ֵ��ͨ������Ϊ�ڴ�黺��ش�С�� 3 ���� */
	virtual void SetFreeBufferObjHold(DWORD dwFreeBufferObjHold) = 0;
	/* ���ù����߳�������ͨ������Ϊ 2 * CPU + 2�� */
	virtual void SetWorkerThreadCount(DWORD dwWorkerThreadCount) = 0;
	/* �����Ƿ��Ǿ�Ĭʱ�䣨����Ϊ TRUE ʱ DisconnectSilenceConnections() �� GetSilencePeriod() ����Ч��Ĭ�ϣ�TRUE�� */
	virtual void SetMarkSilence(BOOL bMarkSilence) = 0;

	/* ��ȡ���ݷ��Ͳ��� */
	virtual EnSendPolicy GetSendPolicy() = 0;
	/* ��ȡ OnSend �¼�ͬ������ */
	virtual EnOnSendSyncPolicy GetOnSendSyncPolicy() = 0;
	/* ��ȡ��������� */
	virtual DWORD GetMaxConnectionCount() = 0;
	/* ��ȡ Socket �����������ʱ�� */
	virtual DWORD GetFreeSocketObjLockTime() = 0;
	/* ��ȡ Socket ����ش�С */
	virtual DWORD GetFreeSocketObjPool() = 0;
	/* ��ȡ�ڴ�黺��ش�С */
	virtual DWORD GetFreeBufferObjPool() = 0;
	/* ��ȡ Socket ����ػ��շ�ֵ */
	virtual DWORD GetFreeSocketObjHold() = 0;
	/* ��ȡ�ڴ�黺��ػ��շ�ֵ */
	virtual DWORD GetFreeBufferObjHold() = 0;
	/* ��ȡ�����߳����� */
	virtual DWORD GetWorkerThreadCount() = 0;
	/* ����Ƿ��Ǿ�Ĭʱ�� */
	virtual BOOL IsMarkSilence() = 0;

public:
	virtual ~IComplexSocket() {}
};

/************************************************************************
���ƣ�ͨ�ŷ��������ӿ�
����������ͨ�ŷ������������в������������Է��ʷ���
************************************************************************/
class IServer : public IComplexSocket
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ�����ͨ�����
	* ���������������ͨ�������������ɺ�ɿ�ʼ���տͻ������Ӳ��շ�����
	*
	* ������		lpszBindAddress	-- ������ַ
	*			usPort			-- �����˿�
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Start(LPCTSTR lpszBindAddress, USHORT usPort) = 0;

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* ��ȡ���� Socket �ĵ�ַ��Ϣ */
	virtual BOOL GetListenAddress(TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
};

/************************************************************************
���ƣ�TCP ͨ�ŷ��������ӿ�
���������� TCP ͨ�ŷ������������в������������Է��ʷ���
************************************************************************/
class ITcpServer : public IServer
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ�����С�ļ�
	* ��������ָ�����ӷ��� 4096 KB ���µ�С�ļ�
	*
	* ������		dwConnID		-- ���� ID
	*			lpszFileName	-- �ļ�·��
	*			pHead			-- ͷ����������
	*			pTail			-- β����������
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ��ϵͳ API ���� ::GetLastError() ��ȡ�������
	*/
	virtual BOOL SendSmallFile(CONNID dwConnID, LPCTSTR lpszFileName, const LPWSABUF pHead = nullptr, const LPWSABUF pTail = nullptr) = 0;

#ifdef _SSL_SUPPORT
	/*
	* ���ƣ���ʼ��ͨ����� SSL ��������
	* ������SSL �������������� SSL ͨ���������ǰ��ɳ�ʼ������������ʧ��
	*
	* ������		iVerifyMode				-- SSL ��֤ģʽ���ο� EnSSLVerifyMode��
	*			lpszPemCertFile			-- ֤���ļ�
	*			lpszPemKeyFile			-- ˽Կ�ļ�
	*			lpszKeyPasswod			-- ˽Կ���루û��������Ϊ�գ�
	*			lpszCAPemCertFileOrPath	-- CA ֤���ļ���Ŀ¼��������֤��ͻ��˿�ѡ��
	*			fnServerNameCallback	-- SNI �ص�����ָ�루��ѡ��
	*
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� SYS_GetLastError() ��ȡʧ��ԭ��
	*/
	virtual BOOL SetupSSLContext(int iVerifyMode = SSL_VM_NONE, LPCTSTR lpszPemCertFile = nullptr, LPCTSTR lpszPemKeyFile = nullptr, LPCTSTR lpszKeyPasswod = nullptr, LPCTSTR lpszCAPemCertFileOrPath = nullptr, Fn_SNI_ServerNameCallback fnServerNameCallback = nullptr) = 0;

	/*
	* ���ƣ����� SNI ����֤��
	* ������SSL ������� SetupSSLContext() �ɹ�����Ե��ñ��������Ӷ�� SNI ����֤��
	*
	* ������		iVerifyMode				-- SSL ��֤ģʽ���ο� EnSSLVerifyMode��
	*			lpszPemCertFile			-- ֤���ļ�
	*			lpszPemKeyFile			-- ˽Կ�ļ�
	*			lpszKeyPasswod			-- ˽Կ���루û��������Ϊ�գ�
	*			lpszCAPemCertFileOrPath	-- CA ֤���ļ���Ŀ¼��������֤��ѡ��
	*
	* ����ֵ��	����		-- �ɹ��������� SNI ����֤���Ӧ�������������������� SNI �ص������ж�λ SNI ����
	*			����		-- ʧ�ܣ���ͨ�� SYS_GetLastError() ��ȡʧ��ԭ��
	*/
	virtual BOOL AddSSLContext(int iVerifyMode = SSL_VM_NONE, LPCTSTR lpszPemCertFile = nullptr, LPCTSTR lpszPemKeyFile = nullptr, LPCTSTR lpszKeyPasswod = nullptr, LPCTSTR lpszCAPemCertFileOrPath = nullptr) = 0;

	/*
	* ���ƣ�����ͨ����� SSL ���л���
	* ����������ͨ����� SSL ���л��������� SSL ����ڴ�
	*		1��ͨ���������ʱ���Զ����ñ�����
	*		2����Ҫ��������ͨ����� SSL ��������ʱ����Ҫ�ȵ��ñ���������ԭ�ȵĻ�������
	*
	* ������	��
	*
	* ����ֵ����
	*/
	virtual void CleanupSSLContext() = 0;

	/*
	* ���ƣ����� SSL ����
	* ��������ͨ���������Ϊ���Զ�����ʱ����Ҫ���ñ��������� SSL ����
	*
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� SYS_GetLastError() ��ȡʧ��ԭ��
	*/
	virtual BOOL StartSSLHandShake(CONNID dwConnID) = 0;

	/* ����ͨ��������ַ�ʽ��Ĭ�ϣ�TRUE���Զ����֣� */
	virtual void SetSSLAutoHandShake(BOOL bAutoHandShake) = 0;
	/* ��ȡͨ��������ַ�ʽ */
	virtual BOOL IsSSLAutoHandShake() = 0;
#endif

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* ���� Accept ԤͶ�����������ݸ��ص������ã�Accept ԤͶ������Խ����֧�ֵĲ�����������Խ�ࣩ */
	virtual void SetAcceptSocketCount(DWORD dwAcceptSocketCount) = 0;
	/* ����ͨ�����ݻ�������С������ƽ��ͨ�����ݰ���С�������ã�ͨ������Ϊ 1024 �ı����� */
	virtual void SetSocketBufferSize(DWORD dwSocketBufferSize) = 0;
	/* ���ü��� Socket �ĵȺ���д�С�����ݲ������������������ã� */
	virtual void SetSocketListenQueue(DWORD dwSocketListenQueue) = 0;
	/* ����������������������룬0 �򲻷�����������Ĭ�ϣ�60 * 1000�� */
	virtual void SetKeepAliveTime(DWORD dwKeepAliveTime) = 0;
	/* �����쳣��������������룬0 ����������������Ĭ�ϣ�20 * 1000������������ɴ� [Ĭ�ϣ�WinXP 5 ��, Win7 10 ��] ��ⲻ������ȷ�ϰ�����Ϊ�Ѷ��ߣ� */
	virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval) = 0;

	/* ��ȡ Accept ԤͶ������ */
	virtual DWORD GetAcceptSocketCount() = 0;
	/* ��ȡͨ�����ݻ�������С */
	virtual DWORD GetSocketBufferSize() = 0;
	/* ��ȡ���� Socket �ĵȺ���д�С */
	virtual DWORD GetSocketListenQueue() = 0;
	/* ��ȡ������������� */
	virtual DWORD GetKeepAliveTime() = 0;
	/* ��ȡ�쳣��������� */
	virtual DWORD GetKeepAliveInterval() = 0;
};


/************************************************************************
���ƣ�UDP ͨ�ŷ��������ӿ�
���������� UDP ͨ�ŷ������������в������������Է��ʷ���
************************************************************************/
class IUdpServer : public IServer
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* �������ݱ�����󳤶ȣ������ھ����������²����� 1472 �ֽڣ��ڹ����������²����� 548 �ֽڣ� */
	virtual void SetMaxDatagramSize(DWORD dwMaxDatagramSize) = 0;
	/* ��ȡ���ݱ�����󳤶� */
	virtual DWORD GetMaxDatagramSize() = 0;

	/* ���� Receive ԤͶ�����������ݸ��ص������ã�Receive ԤͶ������Խ���򶪰�����ԽС�� */
	virtual void SetPostReceiveCount(DWORD dwPostReceiveCount) = 0;
	/* ��ȡ Receive ԤͶ������ */
	virtual DWORD GetPostReceiveCount() = 0;

	/* ���ü������Դ�����0 �򲻷��ͼ�������������������Դ�������Ϊ�Ѷ��ߣ� */
	virtual void SetDetectAttempts(DWORD dwDetectAttempts) = 0;
	/* ���ü������ͼ�����룬0 �����ͼ����� */
	virtual void SetDetectInterval(DWORD dwDetectInterval) = 0;
	/* ��ȡ���������� */
	virtual DWORD GetDetectAttempts() = 0;
	/* ��ȡ��������� */
	virtual DWORD GetDetectInterval() = 0;
};

/************************************************************************
���ƣ�ͨ�Ŵ�������ӿ�
����������ͨ�Ŵ�����������в������������Է��ʷ������������������һ��ͬʱ���Ӷ���������Ŀͻ������
************************************************************************/
class IAgent : public IComplexSocket
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ�����ͨ�����
	* ����������ͨ�Ŵ��������������ɺ�ɿ�ʼ����Զ�̷�����
	*
	* ������		lpszBindAddress	-- �󶨵�ַ��Ĭ�ϣ�nullptr���������ַ��
	*			bAsyncConnect	-- �Ƿ�����첽 Connect
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Start(LPCTSTR lpszBindAddress = nullptr, BOOL bAsyncConnect = TRUE) = 0;

	/*
	* ���ƣ����ӷ�����
	* ���������ӷ����������ӳɹ��� IAgentListener ����յ� OnConnect() / OnHandShake() �¼�
	*
	* ������		lpszRemoteAddress	-- ����˵�ַ
	*			usPort				-- ����˶˿�
	*			pdwConnID			-- ���� ID��Ĭ�ϣ�nullptr������ȡ���� ID��
	*			pExtra				-- ���Ӹ������ݣ�Ĭ�ϣ�nullptr��
	*			usLocalPort			-- ���ض˿ڣ�Ĭ�ϣ�0��
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ��ϵͳ API ���� ::GetLastError() ��ȡ�������
	*/
	virtual BOOL Connect(LPCTSTR lpszRemoteAddress, USHORT usPort, CONNID* pdwConnID = nullptr, PVOID pExtra = nullptr, USHORT usLocalPort = 0) = 0;

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* ��ȡĳ�����ӵ�Զ��������Ϣ */
	virtual BOOL GetRemoteHost(CONNID dwConnID, TCHAR lpszHost[], int& iHostLen, USHORT& usPort) = 0;

};


/************************************************************************
���ƣ�TCP ͨ�Ŵ�������ӿ�
���������� TCP ͨ�Ŵ�����������в������������Է��ʷ���
************************************************************************/
class ITcpAgent : public IAgent
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ�����С�ļ�
	* ��������ָ�����ӷ��� 4096 KB ���µ�С�ļ�
	*
	* ������		dwConnID		-- ���� ID
	*			lpszFileName	-- �ļ�·��
	*			pHead			-- ͷ����������
	*			pTail			-- β����������
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ��ϵͳ API ���� ::GetLastError() ��ȡ�������
	*/
	virtual BOOL SendSmallFile(CONNID dwConnID, LPCTSTR lpszFileName, const LPWSABUF pHead = nullptr, const LPWSABUF pTail = nullptr) = 0;

#ifdef _SSL_SUPPORT
	/*
	* ���ƣ���ʼ��ͨ����� SSL ��������
	* ������SSL �������������� SSL ͨ���������ǰ��ɳ�ʼ������������ʧ��
	*
	* ������		iVerifyMode				-- SSL ��֤ģʽ���ο� EnSSLVerifyMode��
	*			lpszPemCertFile			-- ֤���ļ����ͻ��˿�ѡ��
	*			lpszPemKeyFile			-- ˽Կ�ļ����ͻ��˿�ѡ��
	*			lpszKeyPasswod			-- ˽Կ���루û��������Ϊ�գ�
	*			lpszCAPemCertFileOrPath	-- CA ֤���ļ���Ŀ¼��������֤��ͻ��˿�ѡ��
	*
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� SYS_GetLastError() ��ȡʧ��ԭ��
	*/
	virtual BOOL SetupSSLContext(int iVerifyMode = SSL_VM_NONE, LPCTSTR lpszPemCertFile = nullptr, LPCTSTR lpszPemKeyFile = nullptr, LPCTSTR lpszKeyPasswod = nullptr, LPCTSTR lpszCAPemCertFileOrPath = nullptr) = 0;

	/*
	* ���ƣ�����ͨ����� SSL ���л���
	* ����������ͨ����� SSL ���л��������� SSL ����ڴ�
	*		1��ͨ���������ʱ���Զ����ñ�����
	*		2����Ҫ��������ͨ����� SSL ��������ʱ����Ҫ�ȵ��ñ���������ԭ�ȵĻ�������
	*
	* ������	��
	*
	* ����ֵ����
	*/
	virtual void CleanupSSLContext() = 0;

	/*
	* ���ƣ����� SSL ����
	* ��������ͨ���������Ϊ���Զ�����ʱ����Ҫ���ñ��������� SSL ����
	*
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� SYS_GetLastError() ��ȡʧ��ԭ��
	*/
	virtual BOOL StartSSLHandShake(CONNID dwConnID) = 0;

	/* ����ͨ��������ַ�ʽ��Ĭ�ϣ�TRUE���Զ����֣� */
	virtual void SetSSLAutoHandShake(BOOL bAutoHandShake) = 0;
	/* ��ȡͨ��������ַ�ʽ */
	virtual BOOL IsSSLAutoHandShake() = 0;
#endif

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* �����Ƿ����õ�ַ���û��ƣ�Ĭ�ϣ������ã� */
	virtual void SetReuseAddress(BOOL bReuseAddress) = 0;
	/* ����Ƿ����õ�ַ���û��� */
	virtual BOOL IsReuseAddress() = 0;

	/* ����ͨ�����ݻ�������С������ƽ��ͨ�����ݰ���С�������ã�ͨ������Ϊ 1024 �ı����� */
	virtual void SetSocketBufferSize(DWORD dwSocketBufferSize) = 0;
	/* ����������������������룬0 �򲻷�����������Ĭ�ϣ�60 * 1000�� */
	virtual void SetKeepAliveTime(DWORD dwKeepAliveTime) = 0;
	/* �����쳣��������������룬0 ����������������Ĭ�ϣ�20 * 1000������������ɴ� [Ĭ�ϣ�WinXP 5 ��, Win7 10 ��] ��ⲻ������ȷ�ϰ�����Ϊ�Ѷ��ߣ� */
	virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval) = 0;

	/* ��ȡͨ�����ݻ�������С */
	virtual DWORD GetSocketBufferSize() = 0;
	/* ��ȡ������������� */
	virtual DWORD GetKeepAliveTime() = 0;
	/* ��ȡ�쳣��������� */
	virtual DWORD GetKeepAliveInterval() = 0;
};

/************************************************************************
���ƣ�ͨ�ſͻ�������ӿ�
����������ͨ�ſͻ�����������в������������Է��ʷ���
************************************************************************/
class IClient
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ�����ͨ�����
	* �����������ͻ���ͨ����������ӷ���ˣ�������ɺ�ɿ�ʼ�շ�����
	*
	* ������		lpszRemoteAddress	-- ����˵�ַ
	*			usPort				-- ����˶˿�
	*			bAsyncConnect		-- �Ƿ�����첽 Connect
	*			lpszBindAddress		-- �󶨵�ַ��Ĭ�ϣ�nullptr��TcpClient/UdpClient -> ��ִ�а󶨲�����UdpCast �� -> �����ַ��
	*			usLocalPort			-- ���ض˿ڣ�Ĭ�ϣ�0��
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Start(LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect = TRUE, LPCTSTR lpszBindAddress = nullptr, USHORT usLocalPort = 0) = 0;

	/*
	* ���ƣ��ر�ͨ�����
	* �������رտͻ���ͨ��������ر���ɺ�Ͽ������˵����Ӳ��ͷ�������Դ
	*
	* ������
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� GetLastError() ��ȡ�������
	*/
	virtual BOOL Stop() = 0;

	/*
	* ���ƣ���������
	* �����������˷�������
	*
	* ������		pBuffer		-- ���ͻ�����
	*			iLength		-- ���ͻ���������
	*			iOffset		-- ���ͻ�����ָ��ƫ����
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ��ϵͳ API ���� ::GetLastError() ��ȡ�������
	*/
	virtual BOOL Send(const BYTE* pBuffer, int iLength, int iOffset = 0) = 0;

	/*
	* ���ƣ����Ͷ�������
	* �����������˷��Ͷ�������
	*		TCP - ˳�����������ݰ�
	*		UDP - ���������ݰ���ϳ�һ�����ݰ����ͣ����ݰ����ܳ��Ȳ��ܴ������õ� UDP ����󳤶ȣ�
	*
	* ������		pBuffers	-- ���ͻ���������
	*			iCount		-- ���ͻ�������Ŀ
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ��ϵͳ API ���� ::GetLastError() ��ȡ�������
	*/
	virtual BOOL SendPackets(const WSABUF pBuffers[], int iCount) = 0;

	/*
	* ���ƣ���ͣ/�ָ�����
	* ��������ͣ/�ָ�ĳ�����ӵ����ݽ��չ���
	*
	*			bPause	-- TRUE - ��ͣ, FALSE - �ָ�
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ��
	*/
	virtual BOOL PauseReceive(BOOL bPause = TRUE) = 0;

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* �������ӵĸ������� */
	virtual void SetExtra(PVOID pExtra) = 0;

	/* ��ȡ���ӵĸ������� */
	virtual PVOID GetExtra() = 0;

	/* ����Ƿ�Ϊ��ȫ���ӣ�SSL/HTTPS�� */
	virtual BOOL IsSecure() = 0;
	/* ���ͨ������Ƿ������� */
	virtual BOOL HasStarted() = 0;
	/* �鿴ͨ�������ǰ״̬ */
	virtual EnServiceState	GetState() = 0;
	/* ��ȡ���һ��ʧ�ܲ����Ĵ������ */
	virtual EnSocketError	GetLastError() = 0;
	/* ��ȡ���һ��ʧ�ܲ����Ĵ������� */
	virtual LPCTSTR			GetLastErrorDesc() = 0;
	/* ��ȡ�������������� ID */
	virtual CONNID			GetConnectionID() = 0;
	/* ��ȡ Client Socket �ĵ�ַ��Ϣ */
	virtual BOOL GetLocalAddress(TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
	/* ��ȡ���ӵ�Զ��������Ϣ */
	virtual BOOL GetRemoteHost(TCHAR lpszHost[], int& iHostLen, USHORT& usPort) = 0;
	/* ��ȡ������δ�������ݵĳ��� */
	virtual BOOL GetPendingDataLength(int& iPending) = 0;
	/* ��ȡ���ӵ����ݽ���״̬ */
	virtual BOOL IsPauseReceive(BOOL& bPaused) = 0;
	/* ����Ƿ���Ч���� */
	virtual BOOL IsConnected() = 0;

	/* �����ڴ�黺��ش�С��ͨ������Ϊ -> PUSH ģ�ͣ�5 - 10��PULL ģ�ͣ�10 - 20 �� */
	virtual void SetFreeBufferPoolSize(DWORD dwFreeBufferPoolSize) = 0;
	/* �����ڴ�黺��ػ��շ�ֵ��ͨ������Ϊ�ڴ�黺��ش�С�� 3 ���� */
	virtual void SetFreeBufferPoolHold(DWORD dwFreeBufferPoolHold) = 0;

	/* ��ȡ�ڴ�黺��ش�С */
	virtual DWORD GetFreeBufferPoolSize() = 0;
	/* ��ȡ�ڴ�黺��ػ��շ�ֵ */
	virtual DWORD GetFreeBufferPoolHold() = 0;

public:
	virtual ~IClient() {}
};

/************************************************************************
���ƣ�TCP ͨ�ſͻ�������ӿ�
���������� TCP ͨ�ſͻ�����������в������������Է��ʷ���
************************************************************************/
class ITcpClient : public IClient
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

	/*
	* ���ƣ�����С�ļ�
	* �����������˷��� 4096 KB ���µ�С�ļ�
	*
	* ������		lpszFileName	-- �ļ�·��
	*			pHead			-- ͷ����������
	*			pTail			-- β����������
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ��ϵͳ API ���� ::GetLastError() ��ȡ�������
	*/
	virtual BOOL SendSmallFile(LPCTSTR lpszFileName, const LPWSABUF pHead = nullptr, const LPWSABUF pTail = nullptr) = 0;

#ifdef _SSL_SUPPORT
	/*
	* ���ƣ���ʼ��ͨ����� SSL ��������
	* ������SSL �������������� SSL ͨ���������ǰ��ɳ�ʼ������������ʧ��
	*
	* ������		iVerifyMode				-- SSL ��֤ģʽ���ο� EnSSLVerifyMode��
	*			lpszPemCertFile			-- ֤���ļ����ͻ��˿�ѡ��
	*			lpszPemKeyFile			-- ˽Կ�ļ����ͻ��˿�ѡ��
	*			lpszKeyPasswod			-- ˽Կ���루û��������Ϊ�գ�
	*			lpszCAPemCertFileOrPath	-- CA ֤���ļ���Ŀ¼��������֤��ͻ��˿�ѡ��
	*
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� SYS_GetLastError() ��ȡʧ��ԭ��
	*/
	virtual BOOL SetupSSLContext(int iVerifyMode = SSL_VM_NONE, LPCTSTR lpszPemCertFile = nullptr, LPCTSTR lpszPemKeyFile = nullptr, LPCTSTR lpszKeyPasswod = nullptr, LPCTSTR lpszCAPemCertFileOrPath = nullptr) = 0;

	/*
	* ���ƣ�����ͨ����� SSL ���л���
	* ����������ͨ����� SSL ���л��������� SSL ����ڴ�
	*		1��ͨ���������ʱ���Զ����ñ�����
	*		2����Ҫ��������ͨ����� SSL ��������ʱ����Ҫ�ȵ��ñ���������ԭ�ȵĻ�������
	*
	* ������	��
	*
	* ����ֵ����
	*/
	virtual void CleanupSSLContext() = 0;

	/*
	* ���ƣ����� SSL ����
	* ��������ͨ���������Ϊ���Զ�����ʱ����Ҫ���ñ��������� SSL ����
	*
	* ����ֵ��	TRUE	-- �ɹ�
	*			FALSE	-- ʧ�ܣ���ͨ�� SYS_GetLastError() ��ȡʧ��ԭ��
	*/
	virtual BOOL StartSSLHandShake() = 0;

	/* ����ͨ��������ַ�ʽ��Ĭ�ϣ�TRUE���Զ����֣� */
	virtual void SetSSLAutoHandShake(BOOL bAutoHandShake) = 0;
	/* ��ȡͨ��������ַ�ʽ */
	virtual BOOL IsSSLAutoHandShake() = 0;
#endif

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* ����ͨ�����ݻ�������С������ƽ��ͨ�����ݰ���С�������ã�ͨ������Ϊ��(N * 1024) - sizeof(TBufferObj)�� */
	virtual void SetSocketBufferSize(DWORD dwSocketBufferSize) = 0;
	/* ����������������������룬0 �򲻷�����������Ĭ�ϣ�60 * 1000�� */
	virtual void SetKeepAliveTime(DWORD dwKeepAliveTime) = 0;
	/* �����쳣��������������룬0 ����������������Ĭ�ϣ�20 * 1000������������ɴ� [Ĭ�ϣ�WinXP 5 ��, Win7 10 ��] ��ⲻ������ȷ�ϰ�����Ϊ�Ѷ��ߣ� */
	virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval) = 0;

	/* ��ȡͨ�����ݻ�������С */
	virtual DWORD GetSocketBufferSize() = 0;
	/* ��ȡ������������� */
	virtual DWORD GetKeepAliveTime() = 0;
	/* ��ȡ�쳣��������� */
	virtual DWORD GetKeepAliveInterval() = 0;
};

/************************************************************************
���ƣ�UDP ͨ�ſͻ�������ӿ�
���������� UDP ͨ�ſͻ�����������в������������Է��ʷ���
************************************************************************/
class IUdpClient : public IClient
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* �������ݱ�����󳤶ȣ������ھ����������²����� 1472 �ֽڣ��ڹ����������²����� 548 �ֽڣ� */
	virtual void SetMaxDatagramSize(DWORD dwMaxDatagramSize) = 0;
	/* ��ȡ���ݱ�����󳤶� */
	virtual DWORD GetMaxDatagramSize() = 0;

	/* ���ü������Դ�����0 �򲻷��ͼ�������������������Դ�������Ϊ�Ѷ��ߣ� */
	virtual void SetDetectAttempts(DWORD dwDetectAttempts) = 0;
	/* ���ü������ͼ�����룬0 �����ͼ����� */
	virtual void SetDetectInterval(DWORD dwDetectInterval) = 0;
	/* ��ȡ���������� */
	virtual DWORD GetDetectAttempts() = 0;
	/* ��ȡ��������� */
	virtual DWORD GetDetectInterval() = 0;
};

/************************************************************************
���ƣ�UDP ��������ӿ�
���������� UDP �������鲥��㲥����������в������������Է��ʷ���
************************************************************************/
class IUdpCast : public IClient
{
public:

	/***********************************************************************/
	/***************************** ����������� *****************************/

public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* �������ݱ�����󳤶ȣ������ھ����������²����� 1472 �ֽڣ��ڹ����������²����� 548 �ֽڣ� */
	virtual void SetMaxDatagramSize(DWORD dwMaxDatagramSize) = 0;
	/* ��ȡ���ݱ�����󳤶� */
	virtual DWORD GetMaxDatagramSize() = 0;

	/* �����Ƿ����õ�ַ���û��ƣ�Ĭ�ϣ������ã� */
	virtual void SetReuseAddress(BOOL bReuseAddress) = 0;
	/* ����Ƿ����õ�ַ���û��� */
	virtual BOOL IsReuseAddress() = 0;

	/* ���ô���ģʽ���鲥��㲥�� */
	virtual void SetCastMode(EnCastMode enCastMode) = 0;
	/* ��ȡ����ģʽ */
	virtual EnCastMode GetCastMode() = 0;

	/* �����鲥���ĵ� TTL��0 - 255�� */
	virtual void SetMultiCastTtl(int iMCTtl) = 0;
	/* ��ȡ�鲥���ĵ� TTL */
	virtual int GetMultiCastTtl() = 0;

	/* �����Ƿ������鲥��·��TRUE or FALSE�� */
	virtual void SetMultiCastLoop(BOOL bMCLoop) = 0;
	/* ����Ƿ������鲥��· */
	virtual BOOL IsMultiCastLoop() = 0;

	/* ��ȡ��ǰ���ݱ���Զ�̵�ַ��Ϣ��ͨ���� OnReceive �¼��е��ã� */
	virtual BOOL GetRemoteAddress(TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
};

/************************************************************************
���ƣ�˫�ӿ�ģ����
����������˫�ӿ�ת������
************************************************************************/
template<class F, class S> class DualInterface : public F, public S
{
public:

	/* this ת��Ϊ F* */
	inline static F* ToF(DualInterface* pThis)
	{
		return (F*)(pThis);
	}

	/* F* ת��Ϊ this */
	inline static DualInterface* FromF(F* pF)
	{
		return (DualInterface*)(pF);
	}

	/* this ת��Ϊ S* */
	inline static S* ToS(DualInterface* pThis)
	{
		return (S*)(F2S(ToF(pThis)));
	}

	/* S* ת��Ϊ this */
	inline static DualInterface* FromS(S* pS)
	{
		return FromF(S2F(pS));
	}

	/* S* ת��Ϊ F* */
	inline static F* S2F(S* pS)
	{
		return (F*)((char*)pS - sizeof(F));
	}

	/* F* ת��Ϊ S* */
	inline static S* F2S(F* pF)
	{
		return (S*)((char*)pF + sizeof(F));
	}

public:
	~DualInterface() {}
};

/************************************************************************
���ƣ�Server/Agent PULL ģ������ӿ�
���������� Server/Agent ����� PULL ģ����������в�������
************************************************************************/
class IPullSocket
{
public:

	/*
	* ���ƣ�ץȡ����
	* �������û�ͨ���÷����� Socket �����ץȡ����
	*
	* ������		dwConnID	-- ���� ID
	*			pData		-- ץȡ������
	*			iLength		-- ץȡ���ݳ���
	* ����ֵ��	EnFetchResult
	*/
	virtual EnFetchResult Fetch(CONNID dwConnID, BYTE * pData, int iLength) = 0;

	/*
	* ���ƣ���̽���ݣ������Ƴ����������ݣ�
	* �������û�ͨ���÷����� Socket ����п�̽����
	*
	* ������		dwConnID	-- ���� ID
	*			pData		-- ��̽������
	*			iLength		-- ��̽���ݳ���
	* ����ֵ��	EnFetchResult
	*/
	virtual EnFetchResult Peek(CONNID dwConnID, BYTE * pData, int iLength) = 0;

public:
	virtual ~IPullSocket() {}
};

/************************************************************************
���ƣ�Client PULL ģ������ӿ�
���������� Client ����� PULL ģ����������в�������
************************************************************************/
class IPullClient
{
public:

	/*
	* ���ƣ�ץȡ����
	* �������û�ͨ���÷����� Socket �����ץȡ����
	*
	* ������		pData		-- ץȡ������
	*			iLength		-- ץȡ���ݳ���
	* ����ֵ��	EnFetchResult
	*/
	virtual EnFetchResult Fetch(BYTE * pData, int iLength) = 0;

	/*
	* ���ƣ���̽���ݣ������Ƴ����������ݣ�
	* �������û�ͨ���÷����� Socket ����п�̽����
	*
	* ������		pData		-- ��̽������
	*			iLength		-- ��̽���ݳ���
	* ����ֵ��	EnFetchResult
	*/
	virtual EnFetchResult Peek(BYTE * pData, int iLength) = 0;
public:
	virtual ~IPullClient() {}
};

/************************************************************************
���ƣ�TCP PULL ģ������ӿ�
�������̳��� PULL �� Socket �ӿ�
************************************************************************/
typedef	DualInterface<IPullSocket, ITcpServer>	ITcpPullServer;
typedef	DualInterface<IPullSocket, ITcpAgent>	ITcpPullAgent;
typedef	DualInterface<IPullClient, ITcpClient>	ITcpPullClient;


/************************************************************************
���ƣ�Server/Agent PACK ģ������ӿ�
���������� Server/Agent ����� PACK ģ����������в�������
************************************************************************/
class IPackSocket
{
public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* �������ݰ���󳤶ȣ���Ч���ݰ���󳤶Ȳ��ܳ��� 4194303/0x3FFFFF �ֽڣ�Ĭ�ϣ�262144/0x40000�� */
	virtual void SetMaxPackSize(DWORD dwMaxPackSize) = 0;
	/* ���ð�ͷ��ʶ����Ч��ͷ��ʶȡֵ��Χ 0 ~ 1023/0x3FF������ͷ��ʶΪ 0 ʱ��У���ͷ��Ĭ�ϣ�0�� */
	virtual void SetPackHeaderFlag(USHORT usPackHeaderFlag) = 0;

	/* ��ȡ���ݰ���󳤶� */
	virtual DWORD GetMaxPackSize() = 0;
	/* ��ȡ��ͷ��ʶ */
	virtual USHORT GetPackHeaderFlag() = 0;
public:
	virtual ~IPackSocket() {}
};

/************************************************************************
���ƣ�Client PACK ģ������ӿ�
���������� Client ����� PACK ģ����������в�������
************************************************************************/
class IPackClient
{
public:

	/***********************************************************************/
	/***************************** ���Է��ʷ��� *****************************/

	/* �������ݰ���󳤶ȣ���Ч���ݰ���󳤶Ȳ��ܳ��� 4194303/0x3FFFFF �ֽڣ�Ĭ�ϣ�262144/0x40000�� */
	virtual void SetMaxPackSize() = 0;
	/* ���ð�ͷ��ʶ����Ч��ͷ��ʶȡֵ��Χ 0 ~ 1023/0x3FF������ͷ��ʶΪ 0 ʱ��У���ͷ��Ĭ�ϣ�0�� */
	virtual void SetPackHeaderFlag(USHORT usPackHeaderFlag) = 0;

	/* ��ȡ���ݰ���󳤶� */
	virtual DWORD GetMaxPackSize() = 0;
	/* ��ȡ��ͷ��ʶ */
	virtual USHORT GetPackHeaderFlag() = 0;

public:
	virtual ~IPackClient() {}
};

/************************************************************************
���ƣ�TCP PACK ģ������ӿ�
�������̳��� PACK �� Socket �ӿ�
************************************************************************/
typedef DualInterface<IPackSocket, ITcpServer>	ITcpPackServer;
typedef DualInterface<IPackSocket, ITcpAgent>	ITcpPackAgent;
typedef DualInterface<IPackSocket, ITcpClient>	ITcpPackClient;

/************************************************************************
���ƣ�Socket ���������ӿ�
��������������������Ĺ�������
************************************************************************/
template<class T> class ISocketListenerT
{
public:

	/*
	* ���ƣ��������֪ͨ
	* �����������������ʱ��Socket ���������յ���֪ͨ�����������յ���֪ͨ����ܿ�ʼ
	*		�����շ�����
	*
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ���� OnClose() �¼����ر�����
	*/
	virtual EnHandleResult OnHandShake(T* pSender, CONNID dwConnID) = 0;

	/*
	* ���ƣ��ѷ�������֪ͨ
	* �������ɹ��������ݺ�Socket ���������յ���֪ͨ
	*
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	*			pData		-- �ѷ������ݻ�����
	*			iLength		-- �ѷ������ݳ���
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ��֪ͨ�������� HR_ERROR������ģʽ���������Դ���
	*/
	virtual EnHandleResult OnSend(T* pSneder, CONNID dwConnID, const BYTE* pData, int iLength) = 0;

	/*
	* ���ƣ����ݵ���֪ͨ��PUSH ģ�ͣ�
	* ���������� PUSH ģ�͵� Socket ͨ��������ɹ��������ݺ��� Socket ���������͸�֪ͨ
	*
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	*			pData		-- �ѽ������ݻ�����
	*			iLength		-- �ѽ������ݳ���
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ���� OnClose() �¼����ر�����
	*/
	virtual EnHandleResult OnReceive(T* pSender, CONNID dwConnID, const BYTE * pData, int iLength) = 0;

	/*
	* ���ƣ����ݵ���֪ͨ��PULL ģ�ͣ�
	* ���������� PULL ģ�͵� Socket ͨ��������ɹ��������ݺ��� Socket ���������͸�֪ͨ
	*
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	*			iLength		-- �ѽ������ݳ���
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ���� OnClose() �¼����ر�����
	*/
	virtual EnHandleResult OnReceive(T* pSender, CONNID dwConnID, int iLength) = 0;

	/*
	* ���ƣ�ͨ�Ŵ���֪ͨ
	* ������ͨ�ŷ��������Socket ���������յ���֪ͨ�����ر�����
	*
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	*			enOperation	-- Socket ��������
	*			iErrorCode	-- �������
	* ����ֵ��	���Է���ֵ
	*/
	virtual EnHandleResult OnClose(T* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) = 0;

public:
	virtual ~ISocketListenerT() {}
};

template<class T> 
class IComplexSocketListenerT : public ISocketListenerT<T>
{
public:

	/*
	* ���ƣ��ر�ͨ�����֪ͨ
	* ������ͨ������ر�ʱ��Socket ���������յ���֪ͨ
	*
	* ������		pSender		-- �¼�Դ����
	* ����ֵ�����Է���ֵ
	*/
	virtual EnHandleResult OnShutdown(T* pSender) = 0;
};

/************************************************************************
���ƣ������ Socket �������ӿ�
�������������� Socket �������������¼�
************************************************************************/
template<class T>
class IServerListenerT : public IComplexSocketListenerT<T>
{
public:

	/*
	* ���ƣ�׼������֪ͨ
	* ������ͨ�ŷ�����������ʱ���ڼ��� Socket ������ɲ���ʼִ�м���ǰ��Socket ����
	*		�����յ���֪ͨ��������������֪ͨ��������ִ�� Socket ѡ�����õȶ��⹤��
	*
	* ������		pSender		-- �¼�Դ����
	*			soListen	-- ���� Socket
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ��ֹ����ͨ�ŷ������
	*/
	virtual EnHandleResult OnPerpareListen(T* pSender, SOCKET soListen) = 0;

	/*
	* ���ƣ���������֪ͨ
	* ���������յ��ͻ�����������ʱ��Socket ���������յ���֪ͨ��������������֪ͨ����
	*		����ִ�� Socket ѡ�����û�ܾ��ͻ������ӵȶ��⹤��
	*
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	*			soClient	-- TCP: �ͻ��� Socket �����UDP: �ͻ��� Socket SOCKADDR ָ��
	* ����ֵ��	HR_OK / HR_IGNORE	-- ��������
	*			HR_ERROR			-- �ܾ�����
	*/
	virtual EnHandleResult OnAccept(T* pSender, CONNID dwConnID, UINT_PTR soClient) = 0;
};

/************************************************************************
���ƣ�TCP ����� Socket �������ӿ�
���������� TCP ����� Socket �������������¼�
************************************************************************/
class ITcpServerListener : public IServerListenerT<ITcpServer>
{

};

/************************************************************************
���ƣ�PUSH ģ�ͷ���� Socket �������������
����������ĳЩ�¼���Ĭ�ϴ������������¼���
************************************************************************/
class CTcpServerListener : public ITcpServerListener
{
public:
	virtual EnHandleResult OnPrepareListen(ITcpServer * pSender, SOCKET soListen) { return HR_IGNORE; }
	virtual EnHandleResult OnAccept(ITcpServer * pSender, CONNID dwConnID, UINT_PTR soClient) { return HR_IGNORE; }
	virtual EnHandleResult OnHandShake(ITcpServer * pSender, CONNID dwConnID) { return HR_IGNORE; }
	virtual EnHandleResult OnReceive(ITcpServer * pSender, CONNID dwConnID, int iLength) { return HR_IGNORE; }
	virtual EnHandleResult OnSend(ITcpServer * pSender, CONNID dwConnID, const BYTE * pData, int iLength) { return HR_IGNORE; }
	virtual EnHandleResult OnShutdown(ITcpServer * pSender) { return HR_IGNORE; }
};

/************************************************************************
���ƣ�PULL ģ�ͷ���� Socket �������������
����������ĳЩ�¼���Ĭ�ϴ������������¼���
************************************************************************/
class CTcpPullServerListener : public CTcpServerListener
{
public:
	virtual EnHandleResult OnReceive(ITcpServer * pSender, CONNID dwConnID, int iLength) = 0;
	virtual EnHandleResult OnReceive(ITcpServer * pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return HR_IGNORE; }
};

/************************************************************************
���ƣ�UDP ����� Socket �������ӿ�
���������� UDP ����� Socket �������������¼�
************************************************************************/
class IUdpServerListener : public IServerListenerT<IUdpServer>
{

};

/************************************************************************
���ƣ�UDP ����� Socket �������������
����������ĳЩ�¼���Ĭ�ϴ������������¼���
************************************************************************/
class CUdpServerListener : public IUdpServerListener
{
public:
	virtual EnHandleResult OnPrepareListen(IUdpServer* pSender, SOCKET soListen) { return HR_IGNORE; }
	virtual EnHandleResult OnAccept(IUdpServer* pSender, CONNID dwConnID, UINT_PTR pSockAddr) { return HR_IGNORE; }
	virtual EnHandleResult OnHandShake(IUdpServer* pSender, CONNID dwConnID) { return HR_IGNORE; }
	virtual EnHandleResult OnReceive(IUdpServer* pSender, CONNID dwConnID, int iLength) { return HR_IGNORE; }
	virtual EnHandleResult OnSend(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return HR_IGNORE; }
	virtual EnHandleResult OnShutdown(IUdpServer* pSender) { return HR_IGNORE; }
};

/************************************************************************
���ƣ�ͨ�Ŵ��� Socket �������ӿ�
���������� ͨ�Ŵ��� Socket �������������¼�
************************************************************************/
template<class T> class IAgentListenerT : public IComplexSocketListenerT<T>
{
public:

	/*
	* ���ƣ�׼������֪ͨ
	* ������ͨ�ſͻ����������ʱ���ڿͻ��� Socket ������ɲ���ʼִ������ǰ��Socket ����
	*		�����յ���֪ͨ��������������֪ͨ��������ִ�� Socket ѡ�����õȶ��⹤��
	*
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	*			socket		-- �ͻ��� Socket
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ��ֹ����ͨ�ſͻ������
	*/
	virtual EnHandleResult OnPrepareConnect(T* pSender, CONNID dwConnID, SOCKET socket) = 0;

	/*
	* ���ƣ��������֪ͨ
	* �����������˳ɹ���������ʱ��Socket ���������յ���֪ͨ
	*
	* ������		pSender		-- �¼�Դ����
	*			dwConnID	-- ���� ID
	* ����ֵ��	HR_OK / HR_IGNORE	-- ����ִ��
	*			HR_ERROR			-- ͬ�����ӣ���ֹ����ͨ�ſͻ������
	*								   �첽���ӣ��ر�����
	*/
	virtual EnHandleResult OnConnect(T* pSender, CONNID dwConnID) = 0;
};

