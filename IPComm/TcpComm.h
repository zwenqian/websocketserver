/*********************************************************************
* 版权所有(C)2014, 广州海格通信集团股份有限公司.
* 项目代号:     ZX195
* 文件名称:     CommTcpSock.h
* 运行平台:     Linux
* 编译环境:     C编译器--GCC
* 程序语言:     c++
* 作   者：     	      林广荣
* 内容摘要:     CommTcpSock相关操作；
* 完成日期:     2014年11月18日
***********************************************************************/
#ifndef BL_COMMTCPSOCK_H_
#define BL_COMMTCPSOCK_H_
/**********************************************************************
*                       头文件
**********************************************************************/
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace Csp
{
#ifndef BL_LOG_INFO
#define BL_LOG_INFO        printf("BL_LOG_INFO:    ");printf
#endif  
#ifndef APP_STATUS_ERROR
#define APP_STATUS_ERROR   -1
#endif

#ifndef APP_STATUS_SUCCESS
#define APP_STATUS_SUCCESS  0
#endif
/**********************************************************************
*                  CommTcpServer类
**********************************************************************/
class CommTcpServer
{
public:
    CommTcpServer();
    virtual ~CommTcpServer();

    __int32_t initialize(const char* pStrAddr = NULL, __uint16_t nPort = 55000, __int32_t maxListen = 5);
    __int32_t acceptSock();
    __int32_t recvData(__int32_t sockFd, char* pBuf, __int32_t len);
    __int32_t sendData(__int32_t sockFd, const char* pBuf, __int32_t len);
    __int32_t sendData(const char* pBuf, __int32_t len);

    bool isConnected() const;

private:
    __int32_t   setSockOption();
    __int32_t   closeSock();
    __int32_t   _sockFd;
    __int32_t	_accsockFD;
    bool        _bConneted;
};

/**********************************************************************
*                  CommTcpClient类
**********************************************************************/
class CommTcpClient
{
public:
    CommTcpClient(__uint16_t nLocalPort);
    virtual ~CommTcpClient();

    __int32_t connectSock(const char* pStrAddr = NULL, __uint16_t nPort = 55000);
    __int32_t recvData(char* pBuf, __int32_t len);
    __int32_t sendData(const char* pBuf, __int32_t len);
    int getFd();
    int connectedState();
    
    bool isConnected() const;

private:
    __int32_t setSockOption();
    __int32_t closeSock();
    __uint16_t _nPort;
    __int32_t  _sockFd;
    bool   _bConneted;
};

}
#endif /* COMMTCPSOCK_H_ */

/* end of file */

