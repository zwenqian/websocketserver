/*********************************************************************
* 版权所有(C)2014, 广州海格通信集团股份有限公司.
* 项目代号:     ZX195
* 文件名称:     commUdpSock.cpp
* 运行平台:     Linux
* 编译环境:     C编译器--GCC
* 程序语言:     c++
* 作   者：     	      林广荣
* 内容摘要:     UdpSock相关操作
* 完成日期:     2014年11月18日
***********************************************************************/
/**********************************************************************
*                       头文件
**********************************************************************/
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "udpComm.h"

namespace Csp
{
/**********************************************************************
*               宏定义
**********************************************************************/
#define CHECK_SOCKET_NORMAL(x)  do{\
    if(-1 == x)\
    {\
        BL_LOG_INFO("socket is error.\n");\
        return APP_STATUS_ERROR;\
    }\
}while(0)
    
#define CHECK_POINTER_VALID(x) \
        if(NULL == x) return APP_STATUS_ERROR;\

/**********************************************************************
*               函数原型
**********************************************************************/
/**********************************************************************
* 函数名称：CommTcpServer::CommTcpServer()
* 功能描述：CommTcpServer构造函数
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
CommUdpServer::CommUdpServer()
:_sockFd(-1)
{
    //生成套接字类型
    _sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1 == _sockFd)
    {
        BL_LOG_INFO("CommUdpServer=>socket create fail: %s\n", strerror(errno));
    }
}

/**********************************************************************
* 函数名称：CommTcpServer::getFd()
* 功能描述：获取文件描述符
* 输入参数：
* 输出参数：
* 返 回 值：文件描述符
* 其它说明：
***********************************************************************/
int CommUdpServer::getFd()
{
    return _sockFd;
}

/**********************************************************************
* 函数名称：CommTcpServer::CommTcpServer()
* 功能描述：CommTcpServer析构函数
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
CommUdpServer::~CommUdpServer()
{
    closeSock();
}

/**********************************************************************
* 函数名称：Int32 CommUdpServer::initialize(const char* IpAddr , Uint16 nPort )
* 功能描述：初始化函数
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::initialize(const char* IpAddr /*NULL*/, __uint16_t nPort /*50005*/)
{
    CHECK_SOCKET_NORMAL(_sockFd);

    __int32_t res = setSockOption();
    if(APP_STATUS_ERROR == res)
    {
        BL_LOG_INFO("CommUdpServer=>setSockOption is error: %s\n", strerror(errno));
        closeSock();
        return  APP_STATUS_ERROR;
    }

    struct sockaddr_in sockSer;
    memset(&sockSer, 0, sizeof(sockSer));
    sockSer.sin_family = AF_INET;
    sockSer.sin_port = htons(nPort);

    NULL == IpAddr ? (sockSer.sin_addr.s_addr = INADDR_ANY):(sockSer.sin_addr.s_addr = inet_addr(IpAddr));

    //绑定本地端口
    res = bind(_sockFd, (struct sockaddr*)&sockSer, sizeof(sockSer));
    if(APP_STATUS_ERROR == res)
    {
        BL_LOG_INFO("CommUdpServer=>bind is error: %s\n", strerror(errno));
        closeSock();
        return  APP_STATUS_ERROR;
    }

    return APP_STATUS_SUCCESS;
}
/**********************************************************************
* 函数名称：Int32 CommUdpServer::initializeRemote( const char * rempteIP = NULL, __uint16_t rPort = 8080)
* 功能描述：初始化函数
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::initializeRemote( const char * rempteIP , __uint16_t rPort )
{
    memset(&__pSockClient, 0, sizeof(__pSockClient));
    __pSockClient.sin_family = AF_INET;
    __pSockClient.sin_port = htons(rPort);
    __pSockClient.sin_addr.s_addr = inet_addr(rempteIP);
    return 0;
}
/**********************************************************************
* 函数名称：Int32 CommUdpServer::setSockOption()
* 功能描述：设置Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::setSockOption()
{
    CHECK_SOCKET_NORMAL(_sockFd);
    __int32_t tmp = 1;

    if(APP_STATUS_ERROR == setsockopt(_sockFd,SOL_SOCKET,SO_REUSEADDR,(char*)&tmp,sizeof(tmp)))
    {
        BL_LOG_INFO("service:set data socket option error\n");
        closeSock();
        return APP_STATUS_ERROR;
    }

    __int32_t nZero = 0;
    //设置缓冲区大小
    if(setsockopt(_sockFd, SOL_SOCKET, SO_SNDBUF,(char*)&nZero, sizeof(__int32_t)) < 0)
    {
        BL_LOG_INFO("set data socket option error\n");
        closeSock();
            return APP_STATUS_ERROR;
     }

    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：Int32 CommUdpServer::recvData(char* buff, Int32 len, struct
* sockaddr_in* pSockClient)
* 功能描述：Udp服务
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::recvData(char* buff, __int32_t len, struct sockaddr_in* pSockClient)
{
    CHECK_SOCKET_NORMAL(_sockFd);
    CHECK_POINTER_VALID(buff);

    __int32_t nCount = -1;
    socklen_t nlen = sizeof(sockaddr);

    nCount = recvfrom(_sockFd, buff, len, 0, (struct sockaddr*)pSockClient, &nlen);
    if(APP_STATUS_ERROR == nCount)
    {
        //BL_LOG_INFO("CommUdpServer=>recv datas fail: %s\n", strerror(errno));
        return APP_STATUS_ERROR;
    }

    //BL_LOG_INFO("CommUdpServer=>recv %d bytes data.\n", nCount);
    return nCount;
}
/**********************************************************************
* 函数名称：Int32 CommUdpServer::recvData(char* buff, Int32 len)
* 功能描述：Udp服务
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::recvData(char* buff, __int32_t len)
{
    CHECK_SOCKET_NORMAL(_sockFd);
    CHECK_POINTER_VALID(buff);

    __int32_t nCount = -1;
    socklen_t nlen = sizeof(sockaddr);

    nCount = recvfrom(_sockFd, buff, len, 0, (struct sockaddr*)&__pSockClient, &nlen);
    if(APP_STATUS_ERROR == nCount)
    {
        //BL_LOG_INFO("CommUdpServer=>recv datas fail: %s\n", strerror(errno));
        return APP_STATUS_ERROR;
    }

    //BL_LOG_INFO("CommUdpServer=>recv %d bytes data.\n", nCount);
    return nCount;
}
/**********************************************************************
* 函数名称：Int32 CommUdpServer::sendData(const char* buff, Int32 len,
* uint32_t dstAddr,uint16_t nPort)
* 功能描述：发送数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::sendData(const char* buff, __int32_t len, uint32_t dstAddr,uint16_t nPort)
{
    CHECK_SOCKET_NORMAL(_sockFd);
    CHECK_POINTER_VALID(buff);

    __int32_t nCount = 0;
    struct sockaddr_in SockClient;
    memset(&SockClient, 0, sizeof(SockClient));
    SockClient.sin_family = AF_INET;
    SockClient.sin_addr.s_addr =  htonl(dstAddr);
    SockClient.sin_port = htons(nPort);
    nCount = sendto(_sockFd, buff, len, 0, (struct sockaddr*)&SockClient, sizeof(sockaddr));
    if(-1 == nCount)
    {
        if(101 != errno) { BL_LOG_INFO("CommUdpServer=>send datas fail: %s, errno: %d\n", strerror(errno), errno); } // 网络不可到达就不打印了
        return APP_STATUS_ERROR;
    }

    //BL_LOG_INFO("CommUdpServer=>send %d bytes data.\n", nCount);
    return nCount;
}
/**********************************************************************
* 函数名称：Int32 CommUdpServer::sendData(const char* buff, Int32 len,
* const char * dstIP,uint16_t nPort)
* 功能描述：发送数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::sendData(const char* buff, __int32_t len, const char * dstIP,uint16_t nPort)
{
    return sendData(buff,len,ntohl(inet_addr(dstIP)),nPort);
}
/**********************************************************************
* 函数名称：Int32 CommUdpServer::sendData(const char* buff, Int32 len,
*  struct sockaddr_in* pSockClient)
* 功能描述：发送数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::sendData(const char* buff, __int32_t len, struct sockaddr_in* pSockClient)
{
    return sendData(buff,len,ntohl(pSockClient->sin_addr.s_addr),ntohs(pSockClient->sin_port));
}
/**********************************************************************
* 函数名称：Int32 CommUdpServer::sendData(const char* buff, Int32 len)
* 功能描述：发送数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::sendData(const char* buff, __int32_t len)
{
    return sendData(buff,len,ntohl(__pSockClient.sin_addr.s_addr),ntohs(__pSockClient.sin_port));
}
/**********************************************************************
* 函数名称：Int32 CommUdpServer::closeSock()
* 功能描述：关闭Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpServer::closeSock()
{
    CHECK_SOCKET_NORMAL(_sockFd);
    close(_sockFd);
    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：CommUdpClient::CommUdpClient()
* 功能描述：CommUdpClient构造函数
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
CommUdpClient::CommUdpClient()
:_sockFd(-1)
{
    _sockFd = socket(AF_INET, SOCK_DGRAM, 0);
    if(APP_STATUS_ERROR == _sockFd)
    {
        BL_LOG_INFO("CommUdpClient=>socket create fail: %s\n", strerror(errno));
    }
    memset(&_addrLocal, 0, sizeof(_addrLocal));
    memset(&_addrSer, 0, sizeof(_addrSer));
}

/**********************************************************************
* 函数名称：CommUdpClient::~CommUdpClient()
* 功能描述：CommUdpClient析构函数
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
CommUdpClient::~CommUdpClient()
{
    closeSock();
}

/**********************************************************************
* 函数名称：CommUdpClient::getFd()
* 功能描述：获取文件描述符
* 输入参数：
* 输出参数：
* 返 回 值：文件描述符
* 其它说明：
***********************************************************************/
int CommUdpClient::getFd()
{
    return _sockFd;
}

/**********************************************************************
* 函数名称：Int32 CommUdpClient::bindClient()
* 功能描述：绑定客户端
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpClient::bindClient()
{
    CHECK_SOCKET_NORMAL(_sockFd);

    __int32_t res = setSockOption();
    if(APP_STATUS_ERROR == res)
    {
        BL_LOG_INFO("CommUdpClient=>setSockOption is error: %s\n", strerror(errno));
        closeSock();
        return  APP_STATUS_ERROR;
    }
    res = bind(_sockFd, (struct sockaddr*)&_addrLocal, sizeof(_addrLocal));
    if(-1 == res)
    {
        BL_LOG_INFO("CommUdpServer=>bind is error: %s\n", strerror(errno));
        closeSock();
        return  APP_STATUS_ERROR;
    }

    return  APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：Int32 CommUdpClient::setHostAddr(const char* IpHostAddr, Int32 nHostPort)
* 功能描述：设置主机地址
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpClient::setHostAddr(const char* IpHostAddr, __int32_t nHostPort)
{
    bzero((char *)&(_addrLocal.sin_zero), 0);
    _addrLocal.sin_family = AF_INET;
    _addrLocal.sin_port = htons(nHostPort);

    (NULL == IpHostAddr) ? (_addrLocal.sin_addr.s_addr = inet_addr("127.0.0.1")):(_addrLocal.sin_addr.s_addr = inet_addr(IpHostAddr));

    return bindClient();
}

/**********************************************************************
* 函数名称：Int32 CommUdpClient::setRemoteAddr(const char* IpRemoteAddr,
* Int32 nRemotePort)
* 功能描述：设置远程地址
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpClient::setRemoteAddr(const char* IpRemoteAddr, __int32_t nRemotePort)
{
    _addrSer.sin_family = AF_INET;
    _addrSer.sin_port = htons(nRemotePort);

    (NULL == IpRemoteAddr) ? (_addrSer.sin_addr.s_addr = inet_addr("127.0.0.1")):(_addrSer.sin_addr.s_addr = inet_addr(IpRemoteAddr));
    connect(_sockFd,(sockaddr *)&_addrSer,sizeof(_addrSer));
    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：Int32 CommUdpClient::recvData(char* buff, Int32 len)
* 功能描述：接收数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpClient::recvData(char* buff, __int32_t len)
{
    CHECK_SOCKET_NORMAL(_sockFd);

    __int32_t nCount = 0;
    socklen_t nlen = sizeof(_addrSer);

    nCount = recvfrom(_sockFd, buff, len, 0, (struct sockaddr*)&_addrSer, &nlen);
    if(-1 == nCount)
    {
        BL_LOG_INFO("CommUdpClient=>recv datas fail: %s\n", strerror(errno));
        return APP_STATUS_ERROR;
    }

    return nCount;
}

/**********************************************************************
* 函数名称：Int32 CommUdpClient::sendData(const char* buff, Int32 len)
* 功能描述：发送数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpClient::sendData(const char* buff, __int32_t len)
{
    CHECK_SOCKET_NORMAL(_sockFd);

    __int32_t nCount = APP_STATUS_ERROR;
    nCount = sendto(_sockFd, buff, len, 0, (struct sockaddr*)&_addrSer, sizeof(_addrSer));
    if(-1 == nCount)
    {
        BL_LOG_INFO("CommUdpClient=>send datas fail: %s\n", strerror(errno));
        return APP_STATUS_ERROR;
    }

    return nCount;
}

/**********************************************************************
* 函数名称：Int32 CommUdpClient::setSockOption()
* 功能描述：设置Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpClient::setSockOption()
{
    CHECK_SOCKET_NORMAL(_sockFd);
    __int32_t tmp = 1;

    if(APP_STATUS_ERROR == setsockopt(_sockFd,SOL_SOCKET,SO_REUSEADDR,(char*)&tmp,sizeof(tmp)))
    {
        BL_LOG_INFO("CommUdpClient:set data socket option error\n");
        closeSock();
        return APP_STATUS_ERROR;
    }

    __int32_t nZero = APP_STATUS_SUCCESS;
    if(setsockopt(_sockFd, SOL_SOCKET, SO_SNDBUF,(char*)&nZero, sizeof(__int32_t)) < APP_STATUS_SUCCESS)
    {
        BL_LOG_INFO("set data socket option error\n");
        closeSock();
        return APP_STATUS_ERROR;
     }

    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：Int32 CommUdpClient::closeSock()
* 功能描述：关闭Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommUdpClient::closeSock()
{
    CHECK_SOCKET_NORMAL(_sockFd);
    close(_sockFd);
    return APP_STATUS_SUCCESS;
}

}

/* end of file */
