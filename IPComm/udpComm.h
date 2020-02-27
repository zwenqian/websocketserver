/*********************************************************************
* 版权所有(C)2014, 广州海格通信集团股份有限公司.
* 项目代号:     ZX195
* 文件名称:     CommUdpSock.h
* 运行平台:     Linux
* 编译环境:     C编译器--GCC
* 程序语言:     c++
* 作   者：     林广荣
* 内容摘要:     CommTcpSock相关操作；
* 完成日期:     2014年11月18日
***********************************************************************/
#ifndef BL_COMMUDPSOCK_H_
#define BL_COMMUDPSOCK_H_
/**********************************************************************
*                       头文件
**********************************************************************/
#include <stddef.h>
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
*                       CommUdpServer类
**********************************************************************/
class CommUdpServer
{
public:
    CommUdpServer();
    ~CommUdpServer();

    int getFd();
    __int32_t initialize(const char* IpAddr = NULL, __uint16_t nPort = 50005);
    __int32_t initializeRemote(const char * rempteIP = NULL, __uint16_t rPort = 8080);
    __int32_t recvData(char* buff, __int32_t len);
    __int32_t recvData(char* buff, __int32_t len, struct sockaddr_in* pSockClient);
    __int32_t sendData(const char* buff, __int32_t len);
    __int32_t sendData(const char* buff, __int32_t len, struct sockaddr_in* pSockClient);
    __int32_t sendData(const char* buff, __int32_t len, uint32_t dstAddr,uint16_t nPort);
    __int32_t sendData(const char* buff, __int32_t len, const char * dstIP,uint16_t nPort);

private:
    struct sockaddr_in __pSockClient;
    __int32_t setSockOption();
    __int32_t closeSock();
    __int32_t _sockFd;
};

/**********************************************************************
*                       CommUdpClient类
**********************************************************************/
class CommUdpClient
{
public:
    CommUdpClient();
    ~CommUdpClient();

    int getFd();
    __int32_t recvData(char* buff, __int32_t len);
    __int32_t sendData(const char* buff, __int32_t len);
    __int32_t setHostAddr(const char* IpHostAddr = "0.0.0.0", __int32_t nHostPort = 50006);
    __int32_t setRemoteAddr(const char* IpRemoteAddr, __int32_t nRemotePort);

private:
    __int32_t bindClient();
    __int32_t setSockOption();
    __int32_t closeSock();
    __int32_t _sockFd;
    struct sockaddr_in  _addrSer;
    struct sockaddr_in  _addrLocal;
};

}

#endif /* BL_COMMUDPSOCK_H_ */

/* end of file */
