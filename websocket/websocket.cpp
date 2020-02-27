/*******************************************************************************
* 版权所有(C)2019, 广州海格通信集团股份有限公司.
* 项目代号:     无
* 文件名称:     websocket.cpp
* 运行平台:     Linux
* 编译环境:     C编译器--GCC
* 程序语言:     C++
* 作   者：     张问谦
* 内容摘要:     websocket通信模块
* 完成日期:     2019年12月
*******************************************************************************/
/*******************************************************************************  
 *                                 头文件  
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <libwebsockets.h>
#include "websocket.h"
#include "../pcmsocket/pcmsocket.h"
#include "../IPComm/udpComm.h"

/*******************************************************************************  
 *                                 宏定义  
 ******************************************************************************/
#define LOG_TAG "WEBSOCKET: "
#define SERVERPORT 9998

/*******************************************************************************  
 *                                 数据结构  
 ******************************************************************************/

/*******************************************************************************  
 *                                 全局变量  
 ******************************************************************************/
bool g_websockConFlag = false;
struct lws *pWsi = NULL;
extern bool USER_DEBUG;

/*******************************************************************************  
 *                                 内部函数声明  
 ******************************************************************************/
static int lwsCallbackService(struct lws *wsi, enum lws_callback_reasons reason, 
        void *user, void *in, size_t len);

/*******************************************************************************  
 *                                 函数原型  
 ******************************************************************************/
/*******************************************************************************
* 函数名称： websocketDealThread;
* 功能描述： websocket通信模块处理函数;
* 输入参数： 无;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 无;
*******************************************************************************/
void *websocketDealThread(void *)
{
    struct lws_protocols protocols = {"audioProtocols", lwsCallbackService, 0, 0};
    struct lws_context_creation_info info;
    struct lws_context *context;
    const char *interface = NULL;
    char cert_path[256] = "/myapp/TTwebhome/self.crt.pem";//NULL;
    char key_path[256] = "/myapp/TTwebhome/self.key.pem";//NULL;
    int opts = 0;
    int lwsRet = 0;
    
    //setup websocket context info;
    memset(&info, 0, sizeof info);
    info.port = SERVERPORT;
    info.iface = interface;
    info.protocols = &protocols;
//    info.extensions = lws_get_internal_extensions();
    info.ssl_cert_filepath = cert_path;
    info.ssl_private_key_filepath = key_path;
    info.gid = -1;
    info.uid = -1;
    opts |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;//增加SSL支持;
    info.options = opts; 
    info.timeout_secs = 5;
    
    //create libwebsocket context.
    context = lws_create_context(&info);
    if(NULL == context) 
    {
        RLOGE("Websocket context create error!");
        return NULL;
    }
    RLOGI("Websocket context create success.");   

    //* websocket service */
    char pBuf[640] = {0};
    while(1) 
    {
        lws_service(context, 0);
    }
    
    sleep(1);
    lws_context_destroy(context);   
}

/*******************************************************************************
* 函数名称： lws_callback_http;
* 功能描述： websocket协议回调函数;
* 输入参数： 无;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 无;
*******************************************************************************/
static int lwsCallbackService(struct lws *wsi, enum lws_callback_reasons reason, 
        void *user, void *in, size_t len)
{
    int i;
    char *pBuf = (char *)in;
    
    switch(reason)
    {
        case LWS_CALLBACK_ESTABLISHED:
            pWsi = wsi;
            g_websockConFlag = true;
            RLOGI("%d websocket connection established!", reason);
            break;
        case LWS_CALLBACK_RECEIVE:
            if(USER_DEBUG)
            {
                printf("websocket recvData %d Bytes: ", len); 
                for(i = 0; i < len; i++) printf(" %02x", pBuf[i]); printf("\n");                
            }           
            pcmSocketSendData(pBuf, len);            
//            lwsWebsocketSendData(pBuf, len);
            break;
        case LWS_CALLBACK_CLOSED:
            pWsi = NULL;
            g_websockConFlag = false;
            RLOGI("%d websocket connection closed!", reason);
            break;
        case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:
            RLOGI("%d LWS_CALLBACK_FILTER_NETWORK_CONNECTION!", reason);
            break;     
        case LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED:
            RLOGI("%d LWS_CALLBACK_SERVER_NEW_CLIENT_INSTANTIATED!", reason);
            break;            
        case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS:
            RLOGI("%d LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS!", reason);
            break;
        case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS:
            RLOGI("%d LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS!", reason);
            break;
        case LWS_CALLBACK_PROTOCOL_INIT:
            RLOGI("%d LWS_CALLBACK_PROTOCOL_INIT!", reason);
            break;
        case LWS_CALLBACK_WSI_CREATE:
            RLOGI("%d LWS_CALLBACK_WSI_CREATE!", reason);
            break;   
        case LWS_CALLBACK_WSI_DESTROY:
            RLOGI("%d LWS_CALLBACK_WSI_DESTROY!", reason);
            break;            
        case LWS_CALLBACK_GET_THREAD_ID:
            RLOGI("%d LWS_CALLBACK_GET_THREAD_ID!", reason);
            break;
        case LWS_CALLBACK_EVENT_WAIT_CANCELLED:
            RLOGI("%d LWS_CALLBACK_EVENT_WAIT_CANCELLED!", reason);
            break;    
        default:
            RLOGI("%d websocket nothing to do!", reason);
            break;        
    }
    return 0;
}

/*******************************************************************************
* 函数名称： lwsWebsocketSendData;
* 功能描述： websocket发送数据函数;
* 输入参数： 无;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 无;
*******************************************************************************/
int lwsWebsocketSendData(char *pStr, int pStrSize) 
{
    if((NULL == pWsi) || (NULL == pStr) || (pStrSize <= 0)) return -1;
    if(!g_websockConFlag) return 0;//没有连接，返回不发送数据;
    unsigned char *pOut = NULL;
    int ret;
    
    pOut = (unsigned char *)malloc(sizeof(unsigned char)*(LWS_SEND_BUFFER_PRE_PADDING 
            + pStrSize + LWS_SEND_BUFFER_POST_PADDING));
    memcpy(pOut + LWS_SEND_BUFFER_PRE_PADDING, pStr, pStrSize);
    ret = lws_write(pWsi, pOut + LWS_SEND_BUFFER_PRE_PADDING, pStrSize, LWS_WRITE_BINARY);
    if(USER_DEBUG)
        RLOGI("lwsWebsocketSendData ret = %d", ret);
    free(pOut);
    pOut = NULL;
    
    return ret;
}