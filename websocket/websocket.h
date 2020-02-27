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

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

/*******************************************************************************  
 *                                 宏定义  
 ******************************************************************************/
#define RLOGE(fmt,args...) {printf("\033[0;31mERROR %s ",LOG_TAG);printf(fmt,##args);printf(" \033[0m\n");}
#define RLOGD(fmt,args...) {printf("\033[0;34mDEBUG %s ",LOG_TAG);printf(fmt,##args);printf(" \033[0m\n");}
#define RLOGI(fmt,args...) {printf("\033[0;32mINFO  %s ",LOG_TAG);printf(fmt,##args);printf(" \033[0m\n");}

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


/*******************************************************************************  
 *                                 外部函数声明  
 ******************************************************************************/
void *websocketDealThread(void *);
int lwsWebsocketSendData(char *pStr, int pStrSize);

#endif /* WEBSOCKET_H */

