#ifndef __DBG_MSG_PRINT_H_
#define __DBG_MSG_PRINT_H_


#include <stdio.h>
#include "ossLog.h"

//#pragma comment(lib, "lib/Log.lib")
#pragma comment(lib, "ossLib.lib")
// extern "C" void LogInit(char *pcFileName, unsigned char ucConsolePrint);
// extern "C" void OutDebugInfo(const char* fmt, ...);

typedef void (*DbgFunc)(const char* fmt, ...);
extern DbgFunc OutDebugInfo;

//
// 调试信息输出方式 
// 1-UDP打印输出 
// 2-本地保存 
//  
#define DBG_MSG_TYPE            1

#if DBG_MSG_TYPE==1     // --------------------------------------------

int DbgMsgUDPInit(int port);
int DbgMsgUDPPrint(char *szFormat, ...);
int DbgMsgUDPUnInit(void);

//#define DbgLogInit                  LogInit
#define DbgLog                      OutDebugInfo
//#define DbgLogUnInit()              DbgMsgUDPUnInit()

// #define DbgLogInit(n)               DbgMsgUDPInit(n)
// #define DbgLog                      DbgMsgUDPPrint
// #define DbgLogUnInit()              DbgMsgUDPUnInit()

#define DBG_LOG_FILE_SIZE           (1024*1024*8)
#define DBG_LOG_FILE_NUM            (8)
#define DBG_LOG_FILE_PATH           "log//"
#define DBG_LOG_FILE_NAME           "DB.log"

#elif DBG_MSG_TYPE==2   // --------------------------------------------

int DbgMsgSaveInit(int para);
int DbgMsgSave(char *szFormat, ...);
int DbgMsgSaveUnInit(void);

#define DBG_LOG_FILE_SIZE           (1024*1024*8)
#define DBG_LOG_FILE_NUM            (8)
#define DBG_LOG_FILE_PATH           "log//"
#define DBG_LOG_FILE_NAME           "DB.log"
//#define DbgLogInit(n)               DbgMsgSaveInit(n)
#define DbgLog                      DbgMsgSave
#define DbgLogUnInit()              DbgMsgSaveUnInit

#else                   // --------------------------------------------                         

//#define DbgLogInit(n) 
#define DbgLog                      DbgLog /*do nothing */
#define DbgLogUninit()

#endif
void OutDebugNone(const char* fmt, ...);

#endif   /* __DBG_MSG_PRINT_H_ */


