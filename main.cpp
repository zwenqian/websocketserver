/*******************************************************************************
* 版权所有(C)2019, 广州海格通信集团股份有限公司.
* 项目代号:     
* 文件名称:     main.cpp
* 运行平台:     Linux
* 编译环境:     C编译器--GCC
* 程序语言:     C++
* 作   者：     张问谦
* 内容摘要:     主函数
* 完成日期:     2019年10月
*******************************************************************************/

/*******************************************************************************  
 *                                 头文件  
 ******************************************************************************/
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <execinfo.h>
#include "websocket/websocket.h"
#include "pcmsocket/pcmsocket.h"

using namespace std;
/*******************************************************************************  
 *                                 宏定义  
 ******************************************************************************/
#define RLOGE(fmt,args...) {printf("\033[0;31mERROR %s ",LOG_TAG);printf(fmt,##args);printf(" \033[0m\n");}
#define RLOGD(fmt,args...) {printf("\033[0;34mDEBUG %s ",LOG_TAG);printf(fmt,##args);printf(" \033[0m\n");}
#define RLOGI(fmt,args...) {printf("\033[0;32mINFO  %s ",LOG_TAG);printf(fmt,##args);printf(" \033[0m\n");}
#define LOG_TAG "MAIN: "
#define SIZE 1000

/*******************************************************************************  
 *                                 全局变量  
 ******************************************************************************/
void *buffer[SIZE];
bool USER_DEBUG = false;

/*******************************************************************************  
 *                                 内部函数声明  
 ******************************************************************************/
void setuptrap();
void fault_trap(int n, siginfo_t *siginfo, void *myact);
int inputParaDeal(int argc, char** argv);

/*******************************************************************************  
 *                                 函数原型  
 ******************************************************************************/
/*******************************************************************************
* 函数名称： main;
* 功能描述： 主函数;
* 输入参数： 无;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 无;
*******************************************************************************/
int main(int argc, char** argv) 
{
    int ret;
    pthread_t websocketDealTask, pcmSocketDealTask;

    setuptrap();
    inputParaDeal(argc, argv);
    
    ret = pthread_create(&websocketDealTask, NULL, websocketDealThread, NULL);//websocket通信模块处理线程;
    if(0 != ret)
    {        
        printf("Create CPDealTask pthread error!\n"); 
        exit(-1); 
    }  
    
    ret = pthread_create(&pcmSocketDealTask, NULL, pcmSocketDealThread, NULL);//pcm数据传输模块处理线程;
    if(0 != ret)
    {        
        printf("Create CPDealTask pthread error!\n"); 
        exit(-1); 
    }
    
    pthread_join(websocketDealTask, NULL);
    pthread_join(pcmSocketDealTask, NULL);
    
    return 0;
}

/*******************************************************************************
* 函数名称： setuptrap;
* 功能描述： 注册SIGSEGV信号;
* 输入参数： 无;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 调试段错误用;
*******************************************************************************/
void setuptrap()
{ 
    struct sigaction act; 
    
    sigemptyset(&act.sa_mask);   
    act.sa_flags = SA_SIGINFO;     
    act.sa_sigaction = fault_trap; 
    sigaction(SIGSEGV, &act, NULL); 
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGKILL, &act, NULL);
}

/*******************************************************************************
* 函数名称： fault_trap;
* 功能描述： 打印段错误信息;
* 输入参数： SIGSEGV信号传入的信号信息;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 无;
*******************************************************************************/
void fault_trap(int n, siginfo_t *siginfo, void *myact)
{ 
    int i, num; 
    char **calls; 
    
    if(siginfo->si_signo == SIGINT) exit(1);
    if(siginfo->si_signo == SIGKILL) exit(1);
    
//    RLOGE("Fault address:%x", (unsigned int)siginfo->si_addr);  
    RLOGE("Fault address:0x%08x", (unsigned long)siginfo->si_addr);
    num = backtrace(buffer, SIZE); 
    calls = backtrace_symbols(buffer, num); 
    for (i = 0; i < num; i++) 
        printf("%s\n", calls[i]);
    
    exit(1); 
} 

/*******************************************************************************
* 函数名称： inputParaDeal;
* 功能描述： 输入参数处理;
* 输入参数： 输入的参数;
* 输出参数： 无;
* 返 回 值： 无;
* 其它说明： 无;
*******************************************************************************/
int inputParaDeal(int argc, char** argv)
{
    int n = 0;
    
    while(0 <= n)
    {
        n = getopt(argc, argv, "dh");
        switch(n)
        {
            case 'd':
                RLOGI("Debug");
                USER_DEBUG = true;
                break;
            case 'h':
                RLOGI("Help");
                break;
            default:
                break;
        }        
    }
    
    return 0;
}