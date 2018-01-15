#include <stdio.h>
#include <getopt.h>
#include <pthread.h>
#include "../../../PUBILIC/include/basetype.h"
#include "../../../PUBILIC/include/lib_algorithm.h" /* 测试lib */

#include "ipc_sem.h"

#define IPC_THREAD_NUM  10
#define IPC_MAX_ADD_NUM  10000

BOOL_T g_bIsHaveLock = BOOL_FALSE;
UINT g_uiTestUint = 0;
pthread_t g_astThreadID[IPC_THREAD_NUM];
    
ULONG ipc_ParseParam(INT argc, CHAR **argv)
{
    ULONG ulRet = ERROR_SUCCESS;
    INT iOption = 0;
    while (-1 != (iOption = getopt(argc, argv, "l:")))
    {
        switch (iOption)
        {
            case 'l':
            {
                g_bIsHaveLock = BOOL_TRUE;
                break;
            }
            default:
            {
                ulRet = ERROR_FAILED;
                break;
            }
        }
    }
    
    return ulRet;
}

VOID *ipc_thread_Func(VOID *pParam)
{
    UINT uiIndex;
    
    pthread_t stSelf;
    stSelf = pthread_self();
    if (BOOL_TRUE  == g_bIsHaveLock)
    {
        IPC_Sem_Down();
    }
    lib_algo_Test();
    for (uiIndex = 0; uiIndex < IPC_MAX_ADD_NUM; uiIndex++)
    {
        g_uiTestUint++;
    }

    /* 加完之后再打印值 */
    printf("This is thread(%lu): UINT(%u)\n", stSelf, g_uiTestUint);
    if (BOOL_TRUE  == g_bIsHaveLock)
    {
        IPC_Sem_Up();
    }

    return NULL;
}
ULONG ipc_Init()
{
    ULONG ulRet = ERROR_SUCCESS;
    UINT uiIndex;
    
    ulRet = IPC_Sem_Init();
    if (ERROR_SUCCESS != ulRet)
    {
        return ulRet;
    }
    /* 创建10个线程同时对全局变量进行操作 */
    for (uiIndex = 0; uiIndex < IPC_THREAD_NUM; uiIndex++)
    {
        pthread_create(&g_astThreadID[uiIndex], NULL, ipc_thread_Func, NULL);
    }

    return ulRet;
}

VOID ipc_Fini()
{
    UINT uiIndex;
    for (uiIndex = 0; uiIndex < IPC_THREAD_NUM; uiIndex++)
    {
        pthread_join(g_astThreadID[uiIndex], NULL);
    }

    IPC_Sem_Fini();
    return;
}

INT main(INT argc, CHAR **argv)
{
    ULONG ulRet = ERROR_SUCCESS;

    /* 解析参数 */
    ulRet = ipc_ParseParam(argc, argv);
    if (ERROR_SUCCESS != ERROR_SUCCESS)
    {
        return -1;
    }
    
    ulRet = ipc_Init();
    if (ERROR_SUCCESS != ulRet)
    {
        ipc_Fini();
    }

    ipc_Fini();

    return 0;
}
