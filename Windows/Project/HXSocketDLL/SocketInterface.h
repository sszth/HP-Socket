#pragma once

#include <winsock2.h>

#include "HPTypeDef.h"

/*****************************************************************************************************************************************************/
/***************************************************************** TCP/UDP Interfaces ****************************************************************/
/*****************************************************************************************************************************************************/

/************************************************************************
名称：复合 Socket 组件接口
描述：定义复合 Socket 组件的所有操作方法和属性访问方法，复合 Socket 组件同时管理多个 Socket 连接
************************************************************************/
class IComplexSocket
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：关闭通信组件
	* 描述：关闭通信组件，关闭完成后断开所有连接并释放所有资源
	*
	* 参数：
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Stop() = 0;

	/*
	* 名称：发送数据
	* 描述：向指定连接发送数据
	*
	* 参数：		dwConnID	-- 连接 ID
	*			pBuffer		-- 发送缓冲区
	*			iLength		-- 发送缓冲区长度
	*			iOffset		-- 发送缓冲区指针偏移量
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过系统 API 函数 ::GetLastError() 获取错误代码
	*/
	virtual BOOL Send(CONNID dwConnID, const BYTE* pBuffer, int iLength, int iOffset = 0) = 0;

	/*
	* 名称：发送多组数据
	* 描述：向指定连接发送多组数据
	*		TCP - 顺序发送所有数据包
	*		UDP - 把所有数据包组合成一个数据包发送（数据包的总长度不能大于设置的 UDP 包最大长度）
	*
	* 参数：		dwConnID	-- 连接 ID
	*			pBuffers	-- 发送缓冲区数组
	*			iCount		-- 发送缓冲区数目
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过系统 API 函数 ::GetLastError() 获取错误代码
	*/
	virtual BOOL SendPackets(CONNID dwConnID, const WSABUF pBuffers[], int iCount) = 0;

	/*
	* 名称：暂停/恢复接收
	* 描述：暂停/恢复某个连接的数据接收工作
	*
	* 参数：		dwConnID	-- 连接 ID
	*			bPause		-- TRUE - 暂停, FALSE - 恢复
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL PauseReceive(CONNID dwConnID, BOOL bPause = TRUE) = 0;

	/*
	* 名称：断开连接
	* 描述：断开某个连接
	*
	* 参数：		dwConnID	-- 连接 ID
	*			bForce		-- 是否强制断开连接
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL Disconnect(CONNID dwConnID, BOOL bForce = TRUE) = 0;

	/*
	* 名称：断开超时连接
	* 描述：断开超过指定时长的连接
	*
	* 参数：		dwPeriod	-- 时长（毫秒）
	*			bForce		-- 是否强制断开连接
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL DisconnectLongConnections(DWORD dwPeriod, BOOL bForce = TRUE) = 0;

	/*
	* 名称：断开静默连接
	* 描述：断开超过指定时长的静默连接
	*
	* 参数：		dwPeriod	-- 时长（毫秒）
	*			bForce		-- 是否强制断开连接
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL DisconnectSilenceConnections(DWORD dwPeriod, BOOL bForce = TRUE) = 0;

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/*
	* 名称：设置连接的附加数据
	* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序自身决定
	*
	* 参数：		dwConnID	-- 连接 ID
	*			pv			-- 数据
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败（无效的连接 ID）
	*/
	virtual BOOL SetConnectionExtra(CONNID dwConnID, PVOID pExtra) = 0;

	/*
	* 名称：获取连接的附加数据
	* 描述：是否为连接绑定附加数据或者绑定什么样的数据，均由应用程序自身决定
	*
	* 参数：		dwConnID	-- 连接 ID
	*			ppv			-- 数据指针
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败（无效的连接 ID）
	*/
	virtual BOOL GetConnectionExtra(CONNID dwConnID, PVOID* ppExtra) = 0;

	/* 检测是否为安全连接（SSL/HTTPS） */
	virtual BOOL IsSecure() = 0;
	/* 检查通信组件是否已启动 */
	virtual BOOL HasStarted() = 0;
	/* 查看通信组件当前状态 */
	virtual EnServiceState GetState() = 0;
	/* 获取连接数 */
	virtual DWORD GetConnectionCount() = 0;
	/* 获取所有连接的 CONNID */
	virtual BOOL GetAllConnectionIDs(CONNID pIDs[], DWORD& dwCount) = 0;
	/* 获取某个连接时长（毫秒） */
	virtual BOOL GetConnectPeriod(CONNID dwConnID, DWORD& dwPeriod) = 0;
	/* 获取某个连接静默时间（毫秒） */
	virtual BOOL GetSilencePeriod(CONNID dwConnID, DWORD& dwPeriod) = 0;
	/* 获取某个连接的本地地址信息 */
	virtual BOOL GetLocalAddress(CONNID dwConnID, TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
	/* 获取某个连接的远程地址信息 */
	virtual BOOL GetRemoteAddress(CONNID dwConnID, TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
	/* 获取最近一次失败操作的错误代码 */
	virtual EnSocketError GetLastError() = 0;
	/* 获取最近一次失败操作的错误描述 */
	virtual LPCTSTR GetLastErrorDesc() = 0;
	/* 获取连接中未发出数据的长度 */
	virtual BOOL GetPendingDataLength(CONNID dwConnID, int& iPending) = 0;
	/* 获取连接的数据接收状态 */
	virtual BOOL IsPauseReceive(CONNID dwConnID, BOOL& bPaused) = 0;
	/* 检测是否有效连接 */
	virtual BOOL IsConnected(CONNID dwConnID) = 0;

	/* 设置数据发送策略 */
	virtual void SetSendPolicy(EnSendPolicy enSendPolicy) = 0;
	/* 设置 OnSend 事件同步策略（默认：OSSP_NONE，不同步） */
	virtual void SetOnSendSyncPolicy(EnOnSendSyncPolicy enSyncPolicy) = 0;
	/* 设置最大连接数（组件会根据设置值预分配内存，因此需要根据实际情况设置，不宜过大）*/
	virtual void SetMaxConnectionCount(DWORD dwMaxConnectionCount) = 0;
	/* 设置 Socket 缓存对象锁定时间（毫秒，在锁定期间该 Socket 缓存对象不能被获取使用） */
	virtual void SetFreeSocketObjLockTime(DWORD dwFreeSocketObjLockTime) = 0;
	/* 设置 Socket 缓存池大小（通常设置为平均并发连接数的 1/3 - 1/2） */
	virtual void SetFreeSocketObjPool(DWORD dwFreeSocketObjPool) = 0;
	/* 设置内存块缓存池大小（通常设置为 Socket 缓存池大小的 2 - 3 倍） */
	virtual void SetFreeBufferObjPool(DWORD dwFreeBufferObjPool) = 0;
	/* 设置 Socket 缓存池回收阀值（通常设置为 Socket 缓存池大小的 3 倍） */
	virtual void SetFreeSocketObjHold(DWORD dwFreeSocketObjHold) = 0;
	/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
	virtual void SetFreeBufferObjHold(DWORD dwFreeBufferObjHold) = 0;
	/* 设置工作线程数量（通常设置为 2 * CPU + 2） */
	virtual void SetWorkerThreadCount(DWORD dwWorkerThreadCount) = 0;
	/* 设置是否标记静默时间（设置为 TRUE 时 DisconnectSilenceConnections() 和 GetSilencePeriod() 才有效，默认：TRUE） */
	virtual void SetMarkSilence(BOOL bMarkSilence) = 0;

	/* 获取数据发送策略 */
	virtual EnSendPolicy GetSendPolicy() = 0;
	/* 获取 OnSend 事件同步策略 */
	virtual EnOnSendSyncPolicy GetOnSendSyncPolicy() = 0;
	/* 获取最大连接数 */
	virtual DWORD GetMaxConnectionCount() = 0;
	/* 获取 Socket 缓存对象锁定时间 */
	virtual DWORD GetFreeSocketObjLockTime() = 0;
	/* 获取 Socket 缓存池大小 */
	virtual DWORD GetFreeSocketObjPool() = 0;
	/* 获取内存块缓存池大小 */
	virtual DWORD GetFreeBufferObjPool() = 0;
	/* 获取 Socket 缓存池回收阀值 */
	virtual DWORD GetFreeSocketObjHold() = 0;
	/* 获取内存块缓存池回收阀值 */
	virtual DWORD GetFreeBufferObjHold() = 0;
	/* 获取工作线程数量 */
	virtual DWORD GetWorkerThreadCount() = 0;
	/* 检测是否标记静默时间 */
	virtual BOOL IsMarkSilence() = 0;

public:
	virtual ~IComplexSocket() {}
};

/************************************************************************
名称：通信服务端组件接口
描述：定义通信服务端组件的所有操作方法和属性访问方法
************************************************************************/
class IServer : public IComplexSocket
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：启动通信组件
	* 描述：启动服务端通信组件，启动完成后可开始接收客户端连接并收发数据
	*
	* 参数：		lpszBindAddress	-- 监听地址
	*			usPort			-- 监听端口
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Start(LPCTSTR lpszBindAddress, USHORT usPort) = 0;

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 获取监听 Socket 的地址信息 */
	virtual BOOL GetListenAddress(TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
};

/************************************************************************
名称：TCP 通信服务端组件接口
描述：定义 TCP 通信服务端组件的所有操作方法和属性访问方法
************************************************************************/
class ITcpServer : public IServer
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：发送小文件
	* 描述：向指定连接发送 4096 KB 以下的小文件
	*
	* 参数：		dwConnID		-- 连接 ID
	*			lpszFileName	-- 文件路径
	*			pHead			-- 头部附加数据
	*			pTail			-- 尾部附加数据
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过系统 API 函数 ::GetLastError() 获取错误代码
	*/
	virtual BOOL SendSmallFile(CONNID dwConnID, LPCTSTR lpszFileName, const LPWSABUF pHead = nullptr, const LPWSABUF pTail = nullptr) = 0;

#ifdef _SSL_SUPPORT
	/*
	* 名称：初始化通信组件 SSL 环境参数
	* 描述：SSL 环境参数必须在 SSL 通信组件启动前完成初始化，否则启动失败
	*
	* 参数：		iVerifyMode				-- SSL 验证模式（参考 EnSSLVerifyMode）
	*			lpszPemCertFile			-- 证书文件
	*			lpszPemKeyFile			-- 私钥文件
	*			lpszKeyPasswod			-- 私钥密码（没有密码则为空）
	*			lpszCAPemCertFileOrPath	-- CA 证书文件或目录（单向验证或客户端可选）
	*			fnServerNameCallback	-- SNI 回调函数指针（可选）
	*
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
	*/
	virtual BOOL SetupSSLContext(int iVerifyMode = SSL_VM_NONE, LPCTSTR lpszPemCertFile = nullptr, LPCTSTR lpszPemKeyFile = nullptr, LPCTSTR lpszKeyPasswod = nullptr, LPCTSTR lpszCAPemCertFileOrPath = nullptr, Fn_SNI_ServerNameCallback fnServerNameCallback = nullptr) = 0;

	/*
	* 名称：增加 SNI 主机证书
	* 描述：SSL 服务端在 SetupSSLContext() 成功后可以调用本方法增加多个 SNI 主机证书
	*
	* 参数：		iVerifyMode				-- SSL 验证模式（参考 EnSSLVerifyMode）
	*			lpszPemCertFile			-- 证书文件
	*			lpszPemKeyFile			-- 私钥文件
	*			lpszKeyPasswod			-- 私钥密码（没有密码则为空）
	*			lpszCAPemCertFileOrPath	-- CA 证书文件或目录（单向验证可选）
	*
	* 返回值：	正数		-- 成功，并返回 SNI 主机证书对应的索引，该索引用于在 SNI 回调函数中定位 SNI 主机
	*			负数		-- 失败，可通过 SYS_GetLastError() 获取失败原因
	*/
	virtual BOOL AddSSLContext(int iVerifyMode = SSL_VM_NONE, LPCTSTR lpszPemCertFile = nullptr, LPCTSTR lpszPemKeyFile = nullptr, LPCTSTR lpszKeyPasswod = nullptr, LPCTSTR lpszCAPemCertFileOrPath = nullptr) = 0;

	/*
	* 名称：清理通信组件 SSL 运行环境
	* 描述：清理通信组件 SSL 运行环境，回收 SSL 相关内存
	*		1、通信组件析构时会自动调用本方法
	*		2、当要重新设置通信组件 SSL 环境参数时，需要先调用本方法清理原先的环境参数
	*
	* 参数：	无
	*
	* 返回值：无
	*/
	virtual void CleanupSSLContext() = 0;

	/*
	* 名称：启动 SSL 握手
	* 描述：当通信组件设置为非自动握手时，需要调用本方法启动 SSL 握手
	*
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
	*/
	virtual BOOL StartSSLHandShake(CONNID dwConnID) = 0;

	/* 设置通信组件握手方式（默认：TRUE，自动握手） */
	virtual void SetSSLAutoHandShake(BOOL bAutoHandShake) = 0;
	/* 获取通信组件握手方式 */
	virtual BOOL IsSSLAutoHandShake() = 0;
#endif

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置 Accept 预投递数量（根据负载调整设置，Accept 预投递数量越大则支持的并发连接请求越多） */
	virtual void SetAcceptSocketCount(DWORD dwAcceptSocketCount) = 0;
	/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为 1024 的倍数） */
	virtual void SetSocketBufferSize(DWORD dwSocketBufferSize) = 0;
	/* 设置监听 Socket 的等候队列大小（根据并发连接数量调整设置） */
	virtual void SetSocketListenQueue(DWORD dwSocketListenQueue) = 0;
	/* 设置正常心跳包间隔（毫秒，0 则不发送心跳包，默认：60 * 1000） */
	virtual void SetKeepAliveTime(DWORD dwKeepAliveTime) = 0;
	/* 设置异常心跳包间隔（毫秒，0 不发送心跳包，，默认：20 * 1000，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
	virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval) = 0;

	/* 获取 Accept 预投递数量 */
	virtual DWORD GetAcceptSocketCount() = 0;
	/* 获取通信数据缓冲区大小 */
	virtual DWORD GetSocketBufferSize() = 0;
	/* 获取监听 Socket 的等候队列大小 */
	virtual DWORD GetSocketListenQueue() = 0;
	/* 获取正常心跳包间隔 */
	virtual DWORD GetKeepAliveTime() = 0;
	/* 获取异常心跳包间隔 */
	virtual DWORD GetKeepAliveInterval() = 0;
};


/************************************************************************
名称：UDP 通信服务端组件接口
描述：定义 UDP 通信服务端组件的所有操作方法和属性访问方法
************************************************************************/
class IUdpServer : public IServer
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置数据报文最大长度（建议在局域网环境下不超过 1472 字节，在广域网环境下不超过 548 字节） */
	virtual void SetMaxDatagramSize(DWORD dwMaxDatagramSize) = 0;
	/* 获取数据报文最大长度 */
	virtual DWORD GetMaxDatagramSize() = 0;

	/* 设置 Receive 预投递数量（根据负载调整设置，Receive 预投递数量越大则丢包概率越小） */
	virtual void SetPostReceiveCount(DWORD dwPostReceiveCount) = 0;
	/* 获取 Receive 预投递数量 */
	virtual DWORD GetPostReceiveCount() = 0;

	/* 设置监测包尝试次数（0 则不发送监测跳包，如果超过最大尝试次数则认为已断线） */
	virtual void SetDetectAttempts(DWORD dwDetectAttempts) = 0;
	/* 设置监测包发送间隔（秒，0 不发送监测包） */
	virtual void SetDetectInterval(DWORD dwDetectInterval) = 0;
	/* 获取心跳检查次数 */
	virtual DWORD GetDetectAttempts() = 0;
	/* 获取心跳检查间隔 */
	virtual DWORD GetDetectInterval() = 0;
};

/************************************************************************
名称：通信代理组件接口
描述：定义通信代理组件的所有操作方法和属性访问方法，代理组件本质是一个同时连接多个服务器的客户端组件
************************************************************************/
class IAgent : public IComplexSocket
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：启动通信组件
	* 描述：启动通信代理组件，启动完成后可开始连接远程服务器
	*
	* 参数：		lpszBindAddress	-- 绑定地址（默认：nullptr，绑定任意地址）
	*			bAsyncConnect	-- 是否采用异步 Connect
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Start(LPCTSTR lpszBindAddress = nullptr, BOOL bAsyncConnect = TRUE) = 0;

	/*
	* 名称：连接服务器
	* 描述：连接服务器，连接成功后 IAgentListener 会接收到 OnConnect() / OnHandShake() 事件
	*
	* 参数：		lpszRemoteAddress	-- 服务端地址
	*			usPort				-- 服务端端口
	*			pdwConnID			-- 连接 ID（默认：nullptr，不获取连接 ID）
	*			pExtra				-- 连接附加数据（默认：nullptr）
	*			usLocalPort			-- 本地端口（默认：0）
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过系统 API 函数 ::GetLastError() 获取错误代码
	*/
	virtual BOOL Connect(LPCTSTR lpszRemoteAddress, USHORT usPort, CONNID* pdwConnID = nullptr, PVOID pExtra = nullptr, USHORT usLocalPort = 0) = 0;

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 获取某个连接的远程主机信息 */
	virtual BOOL GetRemoteHost(CONNID dwConnID, TCHAR lpszHost[], int& iHostLen, USHORT& usPort) = 0;

};


/************************************************************************
名称：TCP 通信代理组件接口
描述：定义 TCP 通信代理组件的所有操作方法和属性访问方法
************************************************************************/
class ITcpAgent : public IAgent
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：发送小文件
	* 描述：向指定连接发送 4096 KB 以下的小文件
	*
	* 参数：		dwConnID		-- 连接 ID
	*			lpszFileName	-- 文件路径
	*			pHead			-- 头部附加数据
	*			pTail			-- 尾部附加数据
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过系统 API 函数 ::GetLastError() 获取错误代码
	*/
	virtual BOOL SendSmallFile(CONNID dwConnID, LPCTSTR lpszFileName, const LPWSABUF pHead = nullptr, const LPWSABUF pTail = nullptr) = 0;

#ifdef _SSL_SUPPORT
	/*
	* 名称：初始化通信组件 SSL 环境参数
	* 描述：SSL 环境参数必须在 SSL 通信组件启动前完成初始化，否则启动失败
	*
	* 参数：		iVerifyMode				-- SSL 验证模式（参考 EnSSLVerifyMode）
	*			lpszPemCertFile			-- 证书文件（客户端可选）
	*			lpszPemKeyFile			-- 私钥文件（客户端可选）
	*			lpszKeyPasswod			-- 私钥密码（没有密码则为空）
	*			lpszCAPemCertFileOrPath	-- CA 证书文件或目录（单向验证或客户端可选）
	*
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
	*/
	virtual BOOL SetupSSLContext(int iVerifyMode = SSL_VM_NONE, LPCTSTR lpszPemCertFile = nullptr, LPCTSTR lpszPemKeyFile = nullptr, LPCTSTR lpszKeyPasswod = nullptr, LPCTSTR lpszCAPemCertFileOrPath = nullptr) = 0;

	/*
	* 名称：清理通信组件 SSL 运行环境
	* 描述：清理通信组件 SSL 运行环境，回收 SSL 相关内存
	*		1、通信组件析构时会自动调用本方法
	*		2、当要重新设置通信组件 SSL 环境参数时，需要先调用本方法清理原先的环境参数
	*
	* 参数：	无
	*
	* 返回值：无
	*/
	virtual void CleanupSSLContext() = 0;

	/*
	* 名称：启动 SSL 握手
	* 描述：当通信组件设置为非自动握手时，需要调用本方法启动 SSL 握手
	*
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
	*/
	virtual BOOL StartSSLHandShake(CONNID dwConnID) = 0;

	/* 设置通信组件握手方式（默认：TRUE，自动握手） */
	virtual void SetSSLAutoHandShake(BOOL bAutoHandShake) = 0;
	/* 获取通信组件握手方式 */
	virtual BOOL IsSSLAutoHandShake() = 0;
#endif

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置是否启用地址重用机制（默认：不启用） */
	virtual void SetReuseAddress(BOOL bReuseAddress) = 0;
	/* 检测是否启用地址重用机制 */
	virtual BOOL IsReuseAddress() = 0;

	/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为 1024 的倍数） */
	virtual void SetSocketBufferSize(DWORD dwSocketBufferSize) = 0;
	/* 设置正常心跳包间隔（毫秒，0 则不发送心跳包，默认：60 * 1000） */
	virtual void SetKeepAliveTime(DWORD dwKeepAliveTime) = 0;
	/* 设置异常心跳包间隔（毫秒，0 不发送心跳包，，默认：20 * 1000，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
	virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval) = 0;

	/* 获取通信数据缓冲区大小 */
	virtual DWORD GetSocketBufferSize() = 0;
	/* 获取正常心跳包间隔 */
	virtual DWORD GetKeepAliveTime() = 0;
	/* 获取异常心跳包间隔 */
	virtual DWORD GetKeepAliveInterval() = 0;
};

/************************************************************************
名称：通信客户端组件接口
描述：定义通信客户端组件的所有操作方法和属性访问方法
************************************************************************/
class IClient
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：启动通信组件
	* 描述：启动客户端通信组件并连接服务端，启动完成后可开始收发数据
	*
	* 参数：		lpszRemoteAddress	-- 服务端地址
	*			usPort				-- 服务端端口
	*			bAsyncConnect		-- 是否采用异步 Connect
	*			lpszBindAddress		-- 绑定地址（默认：nullptr，TcpClient/UdpClient -> 不执行绑定操作，UdpCast 绑定 -> 任意地址）
	*			usLocalPort			-- 本地端口（默认：0）
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Start(LPCTSTR lpszRemoteAddress, USHORT usPort, BOOL bAsyncConnect = TRUE, LPCTSTR lpszBindAddress = nullptr, USHORT usLocalPort = 0) = 0;

	/*
	* 名称：关闭通信组件
	* 描述：关闭客户端通信组件，关闭完成后断开与服务端的连接并释放所有资源
	*
	* 参数：
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 GetLastError() 获取错误代码
	*/
	virtual BOOL Stop() = 0;

	/*
	* 名称：发送数据
	* 描述：向服务端发送数据
	*
	* 参数：		pBuffer		-- 发送缓冲区
	*			iLength		-- 发送缓冲区长度
	*			iOffset		-- 发送缓冲区指针偏移量
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过系统 API 函数 ::GetLastError() 获取错误代码
	*/
	virtual BOOL Send(const BYTE* pBuffer, int iLength, int iOffset = 0) = 0;

	/*
	* 名称：发送多组数据
	* 描述：向服务端发送多组数据
	*		TCP - 顺序发送所有数据包
	*		UDP - 把所有数据包组合成一个数据包发送（数据包的总长度不能大于设置的 UDP 包最大长度）
	*
	* 参数：		pBuffers	-- 发送缓冲区数组
	*			iCount		-- 发送缓冲区数目
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过系统 API 函数 ::GetLastError() 获取错误代码
	*/
	virtual BOOL SendPackets(const WSABUF pBuffers[], int iCount) = 0;

	/*
	* 名称：暂停/恢复接收
	* 描述：暂停/恢复某个连接的数据接收工作
	*
	*			bPause	-- TRUE - 暂停, FALSE - 恢复
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败
	*/
	virtual BOOL PauseReceive(BOOL bPause = TRUE) = 0;

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置连接的附加数据 */
	virtual void SetExtra(PVOID pExtra) = 0;

	/* 获取连接的附加数据 */
	virtual PVOID GetExtra() = 0;

	/* 检测是否为安全连接（SSL/HTTPS） */
	virtual BOOL IsSecure() = 0;
	/* 检查通信组件是否已启动 */
	virtual BOOL HasStarted() = 0;
	/* 查看通信组件当前状态 */
	virtual EnServiceState	GetState() = 0;
	/* 获取最近一次失败操作的错误代码 */
	virtual EnSocketError	GetLastError() = 0;
	/* 获取最近一次失败操作的错误描述 */
	virtual LPCTSTR			GetLastErrorDesc() = 0;
	/* 获取该组件对象的连接 ID */
	virtual CONNID			GetConnectionID() = 0;
	/* 获取 Client Socket 的地址信息 */
	virtual BOOL GetLocalAddress(TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
	/* 获取连接的远程主机信息 */
	virtual BOOL GetRemoteHost(TCHAR lpszHost[], int& iHostLen, USHORT& usPort) = 0;
	/* 获取连接中未发出数据的长度 */
	virtual BOOL GetPendingDataLength(int& iPending) = 0;
	/* 获取连接的数据接收状态 */
	virtual BOOL IsPauseReceive(BOOL& bPaused) = 0;
	/* 检测是否有效连接 */
	virtual BOOL IsConnected() = 0;

	/* 设置内存块缓存池大小（通常设置为 -> PUSH 模型：5 - 10；PULL 模型：10 - 20 ） */
	virtual void SetFreeBufferPoolSize(DWORD dwFreeBufferPoolSize) = 0;
	/* 设置内存块缓存池回收阀值（通常设置为内存块缓存池大小的 3 倍） */
	virtual void SetFreeBufferPoolHold(DWORD dwFreeBufferPoolHold) = 0;

	/* 获取内存块缓存池大小 */
	virtual DWORD GetFreeBufferPoolSize() = 0;
	/* 获取内存块缓存池回收阀值 */
	virtual DWORD GetFreeBufferPoolHold() = 0;

public:
	virtual ~IClient() {}
};

/************************************************************************
名称：TCP 通信客户端组件接口
描述：定义 TCP 通信客户端组件的所有操作方法和属性访问方法
************************************************************************/
class ITcpClient : public IClient
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

	/*
	* 名称：发送小文件
	* 描述：向服务端发送 4096 KB 以下的小文件
	*
	* 参数：		lpszFileName	-- 文件路径
	*			pHead			-- 头部附加数据
	*			pTail			-- 尾部附加数据
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过系统 API 函数 ::GetLastError() 获取错误代码
	*/
	virtual BOOL SendSmallFile(LPCTSTR lpszFileName, const LPWSABUF pHead = nullptr, const LPWSABUF pTail = nullptr) = 0;

#ifdef _SSL_SUPPORT
	/*
	* 名称：初始化通信组件 SSL 环境参数
	* 描述：SSL 环境参数必须在 SSL 通信组件启动前完成初始化，否则启动失败
	*
	* 参数：		iVerifyMode				-- SSL 验证模式（参考 EnSSLVerifyMode）
	*			lpszPemCertFile			-- 证书文件（客户端可选）
	*			lpszPemKeyFile			-- 私钥文件（客户端可选）
	*			lpszKeyPasswod			-- 私钥密码（没有密码则为空）
	*			lpszCAPemCertFileOrPath	-- CA 证书文件或目录（单向验证或客户端可选）
	*
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
	*/
	virtual BOOL SetupSSLContext(int iVerifyMode = SSL_VM_NONE, LPCTSTR lpszPemCertFile = nullptr, LPCTSTR lpszPemKeyFile = nullptr, LPCTSTR lpszKeyPasswod = nullptr, LPCTSTR lpszCAPemCertFileOrPath = nullptr) = 0;

	/*
	* 名称：清理通信组件 SSL 运行环境
	* 描述：清理通信组件 SSL 运行环境，回收 SSL 相关内存
	*		1、通信组件析构时会自动调用本方法
	*		2、当要重新设置通信组件 SSL 环境参数时，需要先调用本方法清理原先的环境参数
	*
	* 参数：	无
	*
	* 返回值：无
	*/
	virtual void CleanupSSLContext() = 0;

	/*
	* 名称：启动 SSL 握手
	* 描述：当通信组件设置为非自动握手时，需要调用本方法启动 SSL 握手
	*
	* 返回值：	TRUE	-- 成功
	*			FALSE	-- 失败，可通过 SYS_GetLastError() 获取失败原因
	*/
	virtual BOOL StartSSLHandShake() = 0;

	/* 设置通信组件握手方式（默认：TRUE，自动握手） */
	virtual void SetSSLAutoHandShake(BOOL bAutoHandShake) = 0;
	/* 获取通信组件握手方式 */
	virtual BOOL IsSSLAutoHandShake() = 0;
#endif

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置通信数据缓冲区大小（根据平均通信数据包大小调整设置，通常设置为：(N * 1024) - sizeof(TBufferObj)） */
	virtual void SetSocketBufferSize(DWORD dwSocketBufferSize) = 0;
	/* 设置正常心跳包间隔（毫秒，0 则不发送心跳包，默认：60 * 1000） */
	virtual void SetKeepAliveTime(DWORD dwKeepAliveTime) = 0;
	/* 设置异常心跳包间隔（毫秒，0 不发送心跳包，，默认：20 * 1000，如果超过若干次 [默认：WinXP 5 次, Win7 10 次] 检测不到心跳确认包则认为已断线） */
	virtual void SetKeepAliveInterval(DWORD dwKeepAliveInterval) = 0;

	/* 获取通信数据缓冲区大小 */
	virtual DWORD GetSocketBufferSize() = 0;
	/* 获取正常心跳包间隔 */
	virtual DWORD GetKeepAliveTime() = 0;
	/* 获取异常心跳包间隔 */
	virtual DWORD GetKeepAliveInterval() = 0;
};

/************************************************************************
名称：UDP 通信客户端组件接口
描述：定义 UDP 通信客户端组件的所有操作方法和属性访问方法
************************************************************************/
class IUdpClient : public IClient
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置数据报文最大长度（建议在局域网环境下不超过 1472 字节，在广域网环境下不超过 548 字节） */
	virtual void SetMaxDatagramSize(DWORD dwMaxDatagramSize) = 0;
	/* 获取数据报文最大长度 */
	virtual DWORD GetMaxDatagramSize() = 0;

	/* 设置监测包尝试次数（0 则不发送监测跳包，如果超过最大尝试次数则认为已断线） */
	virtual void SetDetectAttempts(DWORD dwDetectAttempts) = 0;
	/* 设置监测包发送间隔（秒，0 不发送监测包） */
	virtual void SetDetectInterval(DWORD dwDetectInterval) = 0;
	/* 获取心跳检查次数 */
	virtual DWORD GetDetectAttempts() = 0;
	/* 获取心跳检查间隔 */
	virtual DWORD GetDetectInterval() = 0;
};

/************************************************************************
名称：UDP 传播组件接口
描述：定义 UDP 传播（组播或广播）组件的所有操作方法和属性访问方法
************************************************************************/
class IUdpCast : public IClient
{
public:

	/***********************************************************************/
	/***************************** 组件操作方法 *****************************/

public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置数据报文最大长度（建议在局域网环境下不超过 1472 字节，在广域网环境下不超过 548 字节） */
	virtual void SetMaxDatagramSize(DWORD dwMaxDatagramSize) = 0;
	/* 获取数据报文最大长度 */
	virtual DWORD GetMaxDatagramSize() = 0;

	/* 设置是否启用地址重用机制（默认：不启用） */
	virtual void SetReuseAddress(BOOL bReuseAddress) = 0;
	/* 检测是否启用地址重用机制 */
	virtual BOOL IsReuseAddress() = 0;

	/* 设置传播模式（组播或广播） */
	virtual void SetCastMode(EnCastMode enCastMode) = 0;
	/* 获取传播模式 */
	virtual EnCastMode GetCastMode() = 0;

	/* 设置组播报文的 TTL（0 - 255） */
	virtual void SetMultiCastTtl(int iMCTtl) = 0;
	/* 获取组播报文的 TTL */
	virtual int GetMultiCastTtl() = 0;

	/* 设置是否启用组播环路（TRUE or FALSE） */
	virtual void SetMultiCastLoop(BOOL bMCLoop) = 0;
	/* 检测是否启用组播环路 */
	virtual BOOL IsMultiCastLoop() = 0;

	/* 获取当前数据报的远程地址信息（通常在 OnReceive 事件中调用） */
	virtual BOOL GetRemoteAddress(TCHAR lpszAddress[], int& iAddressLen, USHORT& usPort) = 0;
};

/************************************************************************
名称：双接口模版类
描述：定义双接口转换方法
************************************************************************/
template<class F, class S> class DualInterface : public F, public S
{
public:

	/* this 转换为 F* */
	inline static F* ToF(DualInterface* pThis)
	{
		return (F*)(pThis);
	}

	/* F* 转换为 this */
	inline static DualInterface* FromF(F* pF)
	{
		return (DualInterface*)(pF);
	}

	/* this 转换为 S* */
	inline static S* ToS(DualInterface* pThis)
	{
		return (S*)(F2S(ToF(pThis)));
	}

	/* S* 转换为 this */
	inline static DualInterface* FromS(S* pS)
	{
		return FromF(S2F(pS));
	}

	/* S* 转换为 F* */
	inline static F* S2F(S* pS)
	{
		return (F*)((char*)pS - sizeof(F));
	}

	/* F* 转换为 S* */
	inline static S* F2S(F* pF)
	{
		return (S*)((char*)pF + sizeof(F));
	}

public:
	~DualInterface() {}
};

/************************************************************************
名称：Server/Agent PULL 模型组件接口
描述：定义 Server/Agent 组件的 PULL 模型组件的所有操作方法
************************************************************************/
class IPullSocket
{
public:

	/*
	* 名称：抓取数据
	* 描述：用户通过该方法从 Socket 组件中抓取数据
	*
	* 参数：		dwConnID	-- 连接 ID
	*			pData		-- 抓取缓冲区
	*			iLength		-- 抓取数据长度
	* 返回值：	EnFetchResult
	*/
	virtual EnFetchResult Fetch(CONNID dwConnID, BYTE * pData, int iLength) = 0;

	/*
	* 名称：窥探数据（不会移除缓冲区数据）
	* 描述：用户通过该方法从 Socket 组件中窥探数据
	*
	* 参数：		dwConnID	-- 连接 ID
	*			pData		-- 窥探缓冲区
	*			iLength		-- 窥探数据长度
	* 返回值：	EnFetchResult
	*/
	virtual EnFetchResult Peek(CONNID dwConnID, BYTE * pData, int iLength) = 0;

public:
	virtual ~IPullSocket() {}
};

/************************************************************************
名称：Client PULL 模型组件接口
描述：定义 Client 组件的 PULL 模型组件的所有操作方法
************************************************************************/
class IPullClient
{
public:

	/*
	* 名称：抓取数据
	* 描述：用户通过该方法从 Socket 组件中抓取数据
	*
	* 参数：		pData		-- 抓取缓冲区
	*			iLength		-- 抓取数据长度
	* 返回值：	EnFetchResult
	*/
	virtual EnFetchResult Fetch(BYTE * pData, int iLength) = 0;

	/*
	* 名称：窥探数据（不会移除缓冲区数据）
	* 描述：用户通过该方法从 Socket 组件中窥探数据
	*
	* 参数：		pData		-- 窥探缓冲区
	*			iLength		-- 窥探数据长度
	* 返回值：	EnFetchResult
	*/
	virtual EnFetchResult Peek(BYTE * pData, int iLength) = 0;
public:
	virtual ~IPullClient() {}
};

/************************************************************************
名称：TCP PULL 模型组件接口
描述：继承了 PULL 和 Socket 接口
************************************************************************/
typedef	DualInterface<IPullSocket, ITcpServer>	ITcpPullServer;
typedef	DualInterface<IPullSocket, ITcpAgent>	ITcpPullAgent;
typedef	DualInterface<IPullClient, ITcpClient>	ITcpPullClient;


/************************************************************************
名称：Server/Agent PACK 模型组件接口
描述：定义 Server/Agent 组件的 PACK 模型组件的所有操作方法
************************************************************************/
class IPackSocket
{
public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置数据包最大长度（有效数据包最大长度不能超过 4194303/0x3FFFFF 字节，默认：262144/0x40000） */
	virtual void SetMaxPackSize(DWORD dwMaxPackSize) = 0;
	/* 设置包头标识（有效包头标识取值范围 0 ~ 1023/0x3FF，当包头标识为 0 时不校验包头，默认：0） */
	virtual void SetPackHeaderFlag(USHORT usPackHeaderFlag) = 0;

	/* 获取数据包最大长度 */
	virtual DWORD GetMaxPackSize() = 0;
	/* 获取包头标识 */
	virtual USHORT GetPackHeaderFlag() = 0;
public:
	virtual ~IPackSocket() {}
};

/************************************************************************
名称：Client PACK 模型组件接口
描述：定义 Client 组件的 PACK 模型组件的所有操作方法
************************************************************************/
class IPackClient
{
public:

	/***********************************************************************/
	/***************************** 属性访问方法 *****************************/

	/* 设置数据包最大长度（有效数据包最大长度不能超过 4194303/0x3FFFFF 字节，默认：262144/0x40000） */
	virtual void SetMaxPackSize() = 0;
	/* 设置包头标识（有效包头标识取值范围 0 ~ 1023/0x3FF，当包头标识为 0 时不校验包头，默认：0） */
	virtual void SetPackHeaderFlag(USHORT usPackHeaderFlag) = 0;

	/* 获取数据包最大长度 */
	virtual DWORD GetMaxPackSize() = 0;
	/* 获取包头标识 */
	virtual USHORT GetPackHeaderFlag() = 0;

public:
	virtual ~IPackClient() {}
};

/************************************************************************
名称：TCP PACK 模型组件接口
描述：继承了 PACK 和 Socket 接口
************************************************************************/
typedef DualInterface<IPackSocket, ITcpServer>	ITcpPackServer;
typedef DualInterface<IPackSocket, ITcpAgent>	ITcpPackAgent;
typedef DualInterface<IPackSocket, ITcpClient>	ITcpPackClient;

/************************************************************************
名称：Socket 监听器基接口
描述：定义组件监听器的公共方法
************************************************************************/
template<class T> class ISocketListenerT
{
public:

	/*
	* 名称：握手完成通知
	* 描述：连接完成握手时，Socket 监听器将收到该通知，监听器接收到该通知后才能开始
	*		数据收发操作
	*
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 引发 OnClose() 事件并关闭连接
	*/
	virtual EnHandleResult OnHandShake(T* pSender, CONNID dwConnID) = 0;

	/*
	* 名称：已发送数据通知
	* 描述：成功发送数据后，Socket 监听器将收到该通知
	*
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	*			pData		-- 已发送数据缓冲区
	*			iLength		-- 已发送数据长度
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 该通知不允许返回 HR_ERROR（调试模式下引发断言错误）
	*/
	virtual EnHandleResult OnSend(T* pSneder, CONNID dwConnID, const BYTE* pData, int iLength) = 0;

	/*
	* 名称：数据到达通知（PUSH 模型）
	* 描述：对于 PUSH 模型的 Socket 通信组件，成功接收数据后将向 Socket 监听器发送该通知
	*
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	*			pData		-- 已接收数据缓冲区
	*			iLength		-- 已接收数据长度
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 引发 OnClose() 事件并关闭连接
	*/
	virtual EnHandleResult OnReceive(T* pSender, CONNID dwConnID, const BYTE * pData, int iLength) = 0;

	/*
	* 名称：数据到达通知（PULL 模型）
	* 描述：对于 PULL 模型的 Socket 通信组件，成功接收数据后将向 Socket 监听器发送该通知
	*
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	*			iLength		-- 已接收数据长度
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 引发 OnClose() 事件并关闭连接
	*/
	virtual EnHandleResult OnReceive(T* pSender, CONNID dwConnID, int iLength) = 0;

	/*
	* 名称：通信错误通知
	* 描述：通信发生错误后，Socket 监听器将收到该通知，并关闭连接
	*
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	*			enOperation	-- Socket 操作类型
	*			iErrorCode	-- 错误代码
	* 返回值：	忽略返回值
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
	* 名称：关闭通信组件通知
	* 描述：通信组件关闭时，Socket 监听器将收到该通知
	*
	* 参数：		pSender		-- 事件源对象
	* 返回值：忽略返回值
	*/
	virtual EnHandleResult OnShutdown(T* pSender) = 0;
};

/************************************************************************
名称：服务端 Socket 监听器接口
描述：定义服务端 Socket 监听器的所有事件
************************************************************************/
template<class T>
class IServerListenerT : public IComplexSocketListenerT<T>
{
public:

	/*
	* 名称：准备监听通知
	* 描述：通信服务端组件启动时，在监听 Socket 创建完成并开始执行监听前，Socket 监听
	*		器将收到该通知，监听器可以在通知处理方法中执行 Socket 选项设置等额外工作
	*
	* 参数：		pSender		-- 事件源对象
	*			soListen	-- 监听 Socket
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 终止启动通信服务组件
	*/
	virtual EnHandleResult OnPerpareListen(T* pSender, SOCKET soListen) = 0;

	/*
	* 名称：接收连接通知
	* 描述：接收到客户端连接请求时，Socket 监听器将收到该通知，监听器可以在通知处理方
	*		法中执行 Socket 选项设置或拒绝客户端连接等额外工作
	*
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	*			soClient	-- TCP: 客户端 Socket 句柄，UDP: 客户端 Socket SOCKADDR 指针
	* 返回值：	HR_OK / HR_IGNORE	-- 接受连接
	*			HR_ERROR			-- 拒绝连接
	*/
	virtual EnHandleResult OnAccept(T* pSender, CONNID dwConnID, UINT_PTR soClient) = 0;
};

/************************************************************************
名称：TCP 服务端 Socket 监听器接口
描述：定义 TCP 服务端 Socket 监听器的所有事件
************************************************************************/
class ITcpServerListener : public IServerListenerT<ITcpServer>
{

};

/************************************************************************
名称：PUSH 模型服务端 Socket 监听器抽象基类
描述：定义某些事件的默认处理方法（忽略事件）
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
名称：PULL 模型服务端 Socket 监听器抽象基类
描述：定义某些事件的默认处理方法（忽略事件）
************************************************************************/
class CTcpPullServerListener : public CTcpServerListener
{
public:
	virtual EnHandleResult OnReceive(ITcpServer * pSender, CONNID dwConnID, int iLength) = 0;
	virtual EnHandleResult OnReceive(ITcpServer * pSender, CONNID dwConnID, const BYTE* pData, int iLength) { return HR_IGNORE; }
};

/************************************************************************
名称：UDP 服务端 Socket 监听器接口
描述：定义 UDP 服务端 Socket 监听器的所有事件
************************************************************************/
class IUdpServerListener : public IServerListenerT<IUdpServer>
{

};

/************************************************************************
名称：UDP 服务端 Socket 监听器抽象基类
描述：定义某些事件的默认处理方法（忽略事件）
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
名称：通信代理 Socket 监听器接口
描述：定义 通信代理 Socket 监听器的所有事件
************************************************************************/
template<class T> class IAgentListenerT : public IComplexSocketListenerT<T>
{
public:

	/*
	* 名称：准备连接通知
	* 描述：通信客户端组件启动时，在客户端 Socket 创建完成并开始执行连接前，Socket 监听
	*		器将收到该通知，监听器可以在通知处理方法中执行 Socket 选项设置等额外工作
	*
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	*			socket		-- 客户端 Socket
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 终止启动通信客户端组件
	*/
	virtual EnHandleResult OnPrepareConnect(T* pSender, CONNID dwConnID, SOCKET socket) = 0;

	/*
	* 名称：连接完成通知
	* 描述：与服务端成功建立连接时，Socket 监听器将收到该通知
	*
	* 参数：		pSender		-- 事件源对象
	*			dwConnID	-- 连接 ID
	* 返回值：	HR_OK / HR_IGNORE	-- 继续执行
	*			HR_ERROR			-- 同步连接：终止启动通信客户端组件
	*								   异步连接：关闭连接
	*/
	virtual EnHandleResult OnConnect(T* pSender, CONNID dwConnID) = 0;
};

