/*********************************************************************
* 版权所有(C)2014, 广州海格通信集团股份有限公司.
* 项目代号:     ZX195
* 文件名称:     commTcpSock.cpp
* 运行平台:     Linux
* 编译环境:     C编译器--GCC
* 程序语言:     c++
* 作   者：     	      林广荣
* 内容摘要:     TcpSock相关操作
* 完成日期:     2014年11月18日
***********************************************************************/
/**********************************************************************
*                       头文件
**********************************************************************/
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <netinet/tcp.h>
#include "TcpComm.h"

namespace Csp
{
/**********************************************************************
				宏定义
***********************************************************************/
#define CHECK_SOCKET_NORMAL(x)  do{\
    if(-1 == x)\
    {\
            BL_LOG_INFO("socket is error.\n");\
            return -1;\
    }\
}while(0)

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
CommTcpServer::CommTcpServer()
:_sockFd(-1),_bConneted(false)
{
    _sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == _sockFd)
    {
    	BL_LOG_INFO("CommTcpServer=>socket create fail: %s\n", strerror(errno));
    }

}

/**********************************************************************
* 函数名称：CommTcpServer::~CommTcpServer()
* 功能描述：CommTcpServer析构函数
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
CommTcpServer::~CommTcpServer()
{
    closeSock();
}


/**********************************************************************
* 函数名称：Int32 CommTcpServer::initialize(const char* pStrAddr ,
* Uint16 nPort , Int32 maxListen )
* 功能描述：初始化
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpServer::initialize(const char* pStrAddr /*NULL*/, __uint16_t nPort /*55000*/, __int32_t maxListen /*5*/)
{
    CHECK_SOCKET_NORMAL(_sockFd);

    if( APP_STATUS_ERROR == setSockOption())
    {
        BL_LOG_INFO("setSockOption is error!!!\n");
        return -1;
    }
    __int32_t res = -1;
    struct sockaddr_in addrSer;
    memset(&addrSer, 0, sizeof(addrSer));
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(nPort);
    (NULL == pStrAddr) ? (addrSer.sin_addr.s_addr = INADDR_ANY) : (addrSer.sin_addr.s_addr = inet_addr(pStrAddr));

    bzero(&(addrSer.sin_zero), 8);

    //绑定地址
    res = bind(_sockFd, (struct sockaddr*)&addrSer, sizeof(addrSer));
    if(-1 == res)
    {
        BL_LOG_INFO("CommTcpServer=>bind is error: %s", strerror(errno));
        closeSock();
        return  APP_STATUS_ERROR;
    }

    //监听连接
    res = listen(_sockFd, maxListen);
    if(-1 == res)
    {
        BL_LOG_INFO("CommTcpServer=>listen is error: %s", strerror(errno));
        closeSock();
        return APP_STATUS_ERROR;
    }
    return APP_STATUS_SUCCESS;

}

/**********************************************************************
* 函数名称：Int32 CommTcpServer::acceptSock()
* 功能描述：接收sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpServer::acceptSock()
{
    CHECK_SOCKET_NORMAL(_sockFd);

    struct sockaddr_in addrClient;
    socklen_t len = sizeof(sockaddr);
    __int32_t sockAcc = -1;

    //接收连接
    sockAcc = accept(_sockFd, (struct sockaddr*)&addrClient, &len);
    if(-1 == sockAcc)
    {
        BL_LOG_INFO("CommTcpServer=>accept is error: %s", strerror(errno));
        closeSock();
        return -1;
    }

    _bConneted = true;
    _accsockFD = sockAcc;
    BL_LOG_INFO("accept socket = %d\n",sockAcc);

    return sockAcc;
}

/**********************************************************************
* 函数名称：Int32 CommTcpServer::recvData(Int32 sockFd, char* pBuf, Int32 len)
* 功能描述：接收数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpServer::recvData(__int32_t sockFd, char* pBuf, __int32_t len)
{
    CHECK_SOCKET_NORMAL(sockFd);

    __int32_t nCount = -1;

    //接收数据包
    nCount = recv(sockFd, pBuf, len, 0);
    if(APP_STATUS_ERROR == nCount)
    {
        BL_LOG_INFO("CommTcpServer=>recv datas fail: %s\n", strerror(errno));
        return APP_STATUS_ERROR;
    }

    BL_LOG_INFO("CommTcpServer=>recv %d bytes data.\n", nCount);
    return nCount;
}

/**********************************************************************
* 函数名称：Int32 CommTcpServer::sendData(const char* pBuf, Int32 len)
* 功能描述：发送数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpServer::sendData(const char* pBuf, __int32_t len)
{
    __int32_t nCount = -1;

    //发送数据包
    nCount = send(_accsockFD, pBuf, len, 0);
    if(APP_STATUS_ERROR == nCount)
    {
        BL_LOG_INFO("CommTcpServer->send datas fail: %s\n", strerror(errno));
        return APP_STATUS_ERROR;
    }

    BL_LOG_INFO("CommTcpServer->send %d bytes data.\n", nCount);
    return nCount;
}

/**********************************************************************
* 函数名称：Int32 CommTcpServer::sendData(Int32 sockFd, const char* pBuf, Int32 len)
* 功能描述：发送数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpServer::sendData(__int32_t sockFd, const char* pBuf, __int32_t len)
{
    CHECK_SOCKET_NORMAL(sockFd);

    __int32_t nCount = -1;

    //在指定套接字发送数据
    nCount = send(sockFd, pBuf, len, 0);
    if(APP_STATUS_ERROR == nCount)
    {
        BL_LOG_INFO("CommTcpServer=>send datas fail: %s\n", strerror(errno));
        return APP_STATUS_ERROR;
    }

    BL_LOG_INFO("CommTcpServer=>send %d bytes data.\n", nCount);
    return nCount;
}

/**********************************************************************
* 函数名称：bool CommTcpServer::isConnected() const
* 功能描述：判断是否连接
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
bool CommTcpServer::isConnected() const
{
    BL_LOG_INFO("CommTcpServer=>connect state = %d\n", _bConneted);
    return _bConneted;
}

/**********************************************************************
* 函数名称：Int32 CommTcpServer::closeSock()
* 功能描述：关闭Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpServer::closeSock()
{
    CHECK_SOCKET_NORMAL(_sockFd);
    close(_sockFd);
    _bConneted = false;
    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：Int32 CommTcpServer::setSockOption()
* 功能描述：配置Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpServer::setSockOption()
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
    if(setsockopt(_sockFd, SOL_SOCKET, SO_SNDBUF,(char*)&nZero, sizeof(__int32_t)) < 0)
    {
        BL_LOG_INFO("set data socket option error\n");
        closeSock();
            return APP_STATUS_ERROR;
    }

    struct linger li;
    li.l_onoff = 1;
    li.l_linger = 0;

    if(setsockopt(_sockFd, SOL_SOCKET, SO_LINGER, &li, sizeof(li)) < 0)
    {
        BL_LOG_INFO("set SO_LINGER error\n");
        closeSock();
            return APP_STATUS_ERROR;
     }
    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：CommTcpClient::CommTcpClient(Uint16 nLocalPort)
* 功能描述：连接Tcp客服端
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
CommTcpClient::CommTcpClient(__uint16_t nLocalPort)
:_sockFd(-1),_bConneted(false)
{

    _sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == _sockFd)
    {
    	BL_LOG_INFO("CommTcpClient=>socket create fail: %s\n", strerror(errno));
    }
    _nPort = htons(nLocalPort);
}

/**********************************************************************
* 函数名称：CommTcpClient::~CommTcpClient()
* 功能描述：CommTcpClient析构函数
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
CommTcpClient::~CommTcpClient()
{
    closeSock();
}

/**********************************************************************
* 函数名称：Int32 CommTcpClient::connectSock(const char* pStrAddr, Uint16 nPort)
* 功能描述：连接Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpClient::connectSock(const char* pStrAddr, __uint16_t nPort)
{
    CHECK_SOCKET_NORMAL(_sockFd);
    if(APP_STATUS_ERROR == setSockOption())
    {
        BL_LOG_INFO("CommTcpClient=>setSockOption  fail: %s\n", strerror(errno));
        closeSock();
        return APP_STATUS_ERROR;
    }
    sockaddr_in addrClient;
    addrClient.sin_family = AF_INET;
    addrClient.sin_addr.s_addr = inet_addr("0.0.0.0");  /* 本机IP */
    addrClient.sin_port = _nPort;
    /*lint -save -e(545)*/
    bzero((char *)&(addrClient.sin_zero), 8);
    int dataAddrLen = sizeof(sockaddr);
    if(-1 == bind(_sockFd, (const struct sockaddr*)((void *)&addrClient),dataAddrLen))
    {
        BL_LOG_INFO("addrClient:error: bind data, errorinfo[%s]\n",strerror(errno));
        return APP_STATUS_ERROR;
    }

    addrClient.sin_family = AF_INET;
    addrClient.sin_port = htons(nPort);
    NULL == pStrAddr ? (addrClient.sin_addr.s_addr = inet_addr("127.0.0.1")):(addrClient.sin_addr.s_addr = inet_addr(pStrAddr));

    /*lint -save -e(545)*/
    bzero((char*)&(addrClient.sin_zero),8);

    if(APP_STATUS_ERROR == connect(_sockFd,(struct sockaddr*)&addrClient, sizeof(addrClient)))
    {
        BL_LOG_INFO("CommTcpClient=>connect server fail: %s\n", strerror(errno));
        closeSock();
        return APP_STATUS_ERROR;
    }
    
    _bConneted = true;
    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：Int32 CommTcpClient::recvData(char* pBuf, Int32 len)
* 功能描述：接收数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpClient::recvData(char* pBuf, __int32_t len)
{
    CHECK_SOCKET_NORMAL(_sockFd);

    __int32_t nCount = -1;

    nCount = recv(_sockFd, pBuf, len, 0);
    if(-1 == nCount)
    {
        BL_LOG_INFO("CommTcpClient=>recv datas fail: %s\n", strerror(errno));
        return -1;
    }

//    BL_LOG_INFO("CommTcpClient=>recv %d bytes data.\n", nCount);
    return nCount;
}

/**********************************************************************
* 函数名称：Int32 CommTcpClient::sendData(const char* pBuf, Int32 len)
* 功能描述：发送数据
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpClient::sendData(const char* pBuf, __int32_t len)
{
    CHECK_SOCKET_NORMAL(_sockFd);

    __int32_t nCount = -1;

    nCount = send(_sockFd, pBuf, len, 0);
    if(-1 == nCount)
    {
        BL_LOG_INFO("CommTcpClient=>send datas fail: %s\n", strerror(errno));
        return APP_STATUS_ERROR;
    }
//    BL_LOG_INFO("CommTcpClient=>send %d bytes data.\n", nCount);
    return nCount;
}

/**********************************************************************
* 函数名称：bool CommTcpClient::isConnected() const
* 功能描述：判断Sock是否连接
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
bool CommTcpClient::isConnected() const
{    
    BL_LOG_INFO("CommTcpClient=>connect state = %d\n", _bConneted);
    return _bConneted;
}

/**********************************************************************
* 函数名称：Int32 CommTcpClient::closeSock()
* 功能描述：关闭Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpClient::closeSock()
{
    CHECK_SOCKET_NORMAL(_sockFd);
    close(_sockFd);
    _bConneted = false;
    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：Int32 CommTcpClient::setSockOption()
* 功能描述：配置Sock
* 输入参数：
* 输出参数：
* 返 回 值：
* 其它说明：
***********************************************************************/
__int32_t CommTcpClient::setSockOption()
{
    CHECK_SOCKET_NORMAL(_sockFd);

    __int32_t reuse = 1;
    //设置本地地址可重用
    if(APP_STATUS_ERROR == setsockopt(_sockFd,SOL_SOCKET,SO_REUSEADDR,(char*)&reuse,sizeof(reuse)))
    {
        BL_LOG_INFO("service:set data socket option error\n");
        closeSock();
        return APP_STATUS_ERROR;
    }

    struct linger li;
    li.l_onoff = 1;
    li.l_linger = 0;

    //设置关闭套接字时的延迟时间
    if(setsockopt(_sockFd, SOL_SOCKET, SO_LINGER, &li, sizeof(li)) < 0)
    {
        BL_LOG_INFO("set SO_LINGER error\n");
        closeSock();
            return APP_STATUS_ERROR;
     }
    
//    int keepalive = 1;//开启keepalive属性;
//    if(setsockopt(_sockFd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive)) < 0)
//    {
//        BL_LOG_INFO("set SO_KEEPALIVE error\n");
//        closeSock();
//            return APP_STATUS_ERROR;
//     }
//    
//    int keepIdle = 60;//如果该链接在60s内没有任何数据往来，则进行探测;
//    if(setsockopt(_sockFd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle)) < 0)
//    {
//        BL_LOG_INFO("set TCP_KEEPIDLE error\n");
//        closeSock();
//            return APP_STATUS_ERROR;
//     }
//    
//    int keepInterval = 5;//探测时发包的时间间隔为5s;
//    if(setsockopt(_sockFd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval)) < 0)
//    {
//        BL_LOG_INFO("set TCP_KEEPINTVL error\n");
//        closeSock();
//            return APP_STATUS_ERROR;
//     }
//    
//    int keepCount = 3;//探测尝试的次数，如果第1次有响应则后2次不再发;
//    if(setsockopt(_sockFd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount)) < 0)
//    {
//        BL_LOG_INFO("set TCP_KEEPCNT error\n");
//        closeSock();
//            return APP_STATUS_ERROR;
//     }

    return APP_STATUS_SUCCESS;
}

/**********************************************************************
* 函数名称：CommTcpClient::getFd()
* 功能描述：获取文件描述符
* 输入参数：
* 输出参数：
* 返 回 值：文件描述符
* 其它说明：
***********************************************************************/
int CommTcpClient::getFd()
{
    return _sockFd;
}

/**********************************************************************
* 函数名称：CommTcpClient::getFd()
* 功能描述：获取文件描述符
* 输入参数：
* 输出参数：
* 返 回 值：文件描述符
* 其它说明：
***********************************************************************/
int CommTcpClient::connectedState()
{
    struct tcp_info info;
    int len = sizeof(info);
    
    getsockopt(_sockFd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
    if(TCP_ESTABLISHED == info.tcpi_state)
    {
        _bConneted = true;
    }
    else
    {
        closeSock();
        _bConneted = false;
    }
    
    return _bConneted;
}

}

/* end of file */
