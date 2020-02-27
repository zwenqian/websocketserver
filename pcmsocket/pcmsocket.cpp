/*******************************************************************************
* 版权所有(C)2019, 广州海格通信集团股份有限公司.
* 项目代号:     无
* 文件名称:     pcmsocket.cpp
* 运行平台:     Linux
* 编译环境:     C编译器--GCC
* 程序语言:     C++
* 作   者：     张问谦
* 内容摘要:     pcm数据传输模块
* 完成日期:     2019年12月
*******************************************************************************/
/*******************************************************************************  
 *                                 头文件  
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "pcmsocket.h"
#include "../websocket/websocket.h"
#include "../IPComm/udpComm.h"

/*******************************************************************************  
 *                                 宏定义  
 ******************************************************************************/
#define LOG_TAG "PCMSOCKET: "
#define AUDIOSOCKPORT 9997
#define REMODTEPORT 9013
#define REMODTEIP "172.20.1.1"

/*******************************************************************************  
 *                                 数据结构  
 ******************************************************************************/

/*******************************************************************************  
 *                                 全局变量  
 ******************************************************************************/
extern bool USER_DEBUG;
Csp::CommUdpServer *g_pcmSocket = NULL;
extern int g_toCPiCount;
extern int g_toWebiCount;

/*******************************************************************************  
 *                                 内部函数声明  
 ******************************************************************************/
int WriteFile(char *file, char *text, int text_length);

/*******************************************************************************  
 *                                 函数原型  
 ******************************************************************************/
/*******************************************************************************
* 函数名称： pcmSocketDealThread;
* 功能描述： pcm数据传输模块处理函数;
* 输入参数： 无;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 无;
*******************************************************************************/
void *pcmSocketDealThread(void *)
{
    int MaxFd, sel, len, i;
    int audioSockFd = -1;
    fd_set rfds;
    char pcmData[2048] = {0};
    int pcmLen;
    
    g_pcmSocket = new Csp::CommUdpServer();
    g_pcmSocket->initialize(NULL, AUDIOSOCKPORT);    
    audioSockFd = g_pcmSocket->getFd();
    
    MaxFd = audioSockFd;
    while(1)
    {
        FD_ZERO(&rfds);
        FD_SET(audioSockFd, &rfds);
        sel = select(MaxFd + 1, &rfds, NULL, NULL, 0);        
        if(sel <= 0) continue;    
        if(FD_ISSET(audioSockFd, &rfds))
        {
            len = g_pcmSocket->recvData((char *)pcmData, 1000);
            if(len <= 0) continue;
            pcmLen = (pcmData[4] << 8) + pcmData[5];
            if(USER_DEBUG)
            {
                printf("g_AudioSocket recvData %d Bytes: ", len); 
                for(i = 0; i < 20; i++) printf(" %02x", pcmData[i]); printf("\n");
            } 
//            WriteFile((char *)"recordToWeb", &pcmData[6], pcmLen);
            lwsWebsocketSendData(&pcmData[6], pcmLen);  
        }
    }
    
    delete(g_pcmSocket);
}

/*******************************************************************************
* 函数名称： pcmSocketSendData;
* 功能描述： pcm发送数据函数;
* 输入参数： 无;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 无;
*******************************************************************************/
void pcmSocketSendData(char *pStr, int pStrSize)
{
    if((NULL == g_pcmSocket) ||  (pStrSize <= 0)) return;
    if(2000 < pStrSize) return;
    char sendData[2048] = {'P', 'D', 'A', 0x0E};
    
//    WriteFile((char *)"recordToLi1", pStr, pStrSize);
    sendData[4] = (pStrSize >> 8) & 0xFF;
    sendData[5] = pStrSize & 0xFF;
    sendData[pStrSize + 6] = 0;//结束符;
    memcpy(&sendData[6], pStr, pStrSize);
    g_pcmSocket->sendData((const char *)sendData, pStrSize + 6 + 1, REMODTEIP, REMODTEPORT);
    if(USER_DEBUG)
        RLOGI("pcmSocketSendData %d", pStrSize + 6 + 1);
}

/******************************************************************************
* 函数名称： WriteFile()
* 功能描述： 写文件；
* 输入参数： 文件路径 写入数据 数据长度
* 输出参数： 无
* 返 回 值： 无
* 其它说明： 无
*******************************************************************************/
int WriteFile(char *file, char *text, int text_length)
{
    FILE *fp;
    
    fp = fopen(file, "a+");
    if(NULL == fp)
    {
        perror("file open fail!\n");
        fclose(fp);
        return -1;
    }
    if(0 == fwrite(text, text_length, 1, fp))
    {
        perror("file write fail!\n");
        fclose(fp);
        return -1;
    }   
    //printf("write file done!\n");
    fclose(fp);
    return 0;	
}