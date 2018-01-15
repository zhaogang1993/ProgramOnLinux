#include <stdio.h>
#include <sys/sem.h>
#include "../../../PUBILIC/include/basetype.h"
#include "ipc_sem.h"

struct sembuf g_stopDown[1] = {0, -1, 0};
struct sembuf g_stopUp[1] = {0, 1, 0};
INT g_iSemId = -1;

ULONG IPC_Sem_Init()
{
    INT iSemId = -1;
    INT iRet;
    ULONG ulRet = ERROR_FAILED;
    /* 首先打开信号量 */
    iSemId = semget(IPC_SEM_LEY, 0, IPC_CREAT | 0666);
    if (iSemId < 0)
    {
        /* 打开失败再创建 */
        printf("Sem(%ld) does not exist, then create it.\n", IPC_SEM_LEY);
        iSemId = semget(IPC_SEM_LEY, 1, IPC_CREAT | 0666);
        if (iSemId < 0)
        {
            printf("Create sem(%ld) failed.", IPC_SEM_LEY);
            return ERROR_FAILED;
        }
    }

    /* 设置信号量值 */
    iRet = semctl(iSemId, 0, SETVAL, 1);
    if (iRet == 0)
    {
        g_iSemId = iSemId;
        ulRet = ERROR_SUCCESS;
    }
    return ulRet;
}

VOID IPC_Sem_Up()
{
    semop(g_iSemId, &g_stopUp[0], 1);
    return;
}
VOID IPC_Sem_Down()
{
    semop(g_iSemId, &g_stopDown[0], 1);
    return;
}

VOID IPC_Sem_Fini()
{
    /* 删除信号量 */
    if (g_iSemId > 0)
    {
        semctl(g_iSemId, 0, IPC_RMID);
    }
    return;
}
