#ifndef _IPC_SEM_H
#define _IPC_SEM_H

#define IPC_SEM_LEY    1234L


ULONG IPC_Sem_Init();
VOID IPC_Sem_Fini();
VOID IPC_Sem_Up();
VOID IPC_Sem_Down();


#endif
