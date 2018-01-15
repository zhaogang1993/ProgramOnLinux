#include <stdio.h>
#include <assert.h>

#include "../../../PUBILIC/include/basetype.h"

#include "../../../PUBILIC/include/lib_list.h"

VOID LIST_SL_Init(SL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);
    pstHead->pstFirst = NULL;
    return;
}
SL_NODE_S *LIST_SL_First(SL_HEAD_S *pstHead)
{
    return pstHead->pstFirst;
}
SL_NODE_S *LIST_SL_Next(SL_NODE_S *pstCurNode)
{
    return pstCurNode->pstNext;
}
SL_NODE_S* LIST_SL_DelHead(SL_HEAD_S *pstHead)
{
    SL_NODE_S *pstTmp = NULL;
    pstTmp = pstHead->pstFirst;
    if(NULL != pstTmp)
    {
        pstHead->pstFirst = pstTmp->pstNext;
    }
    return pstTmp;
}

VOID LIST_SL_AddHead(SL_HEAD_S *pstHead, SL_NODE_S *pstNode)
{
    assert(NULL != pstHead);
    assert(NULL != pstNode);
    if (!pstHead->pstFirst)
    {
        pstHead->pstFirst = pstNode;
    }
    else
    {
        pstNode->pstNext = pstHead->pstFirst;
        pstHead->pstFirst = pstNode;
    }
    return;
}
VOID LIST_SL_AddBefore(SL_NODE_S *pstCurNode, SL_NODE_S *pstNode)
{
    assert(NULL != pstCurNode);
    assert(NULL != pstNode);

    if (!pstCurNode->pstNext)
    {
        pstCurNode->pstNext = pstNode;
    }
    else
    {
        pstNode->pstNext = pstCurNode->pstNext;
        pstCurNode->pstNext = pstNode;
    }
    return;
}
VOID LIST_SL_FREEALL(SL_HEAD_S *pstHead, VOID (*pFunc)(SL_NODE_S *pstCurNode))
{
    SL_NODE_S *pstCurNode = NULL;
    SL_NODE_S *pstTemNode = NULL;

    /* 这里出现free空指针 */
    LIST_SL_FOREACH_SAFE(pstHead, pstCurNode, pstTemNode)
    {
        pFunc(pstCurNode);
        pstCurNode = NULL;
    }
    return;
}

VOID LIST_STQ_Init(STQ_HEAD_S *pstHead)
{
    assert(NULL != pstHead);
    pstHead->pstFirst = NULL;
    pstHead->pstLast = NULL;
    return;
}
VOID LIST_STQ_AddHead(STQ_HEAD_S *pstHead, STQ_NODE_S *pstNode)
{
    if (NULL == pstHead->pstFirst)
    {
        /* 说明链表为空 */
        pstHead->pstFirst = pstNode;
        pstHead->pstLast = pstNode;
    }
    else
    {
        pstNode->pstNext = pstHead->pstFirst;
        pstHead->pstFirst = pstNode;
    }
    return;
}
VOID LIST_STQ_AddTail(STQ_HEAD_S *pstHead, STQ_NODE_S *pstNode)
{
    if (NULL == pstHead->pstLast)
    {
        /* 说明链表为空 */
        pstHead->pstFirst = pstNode;
        pstHead->pstLast = pstNode;
    }
    else
    {
        pstHead->pstLast->pstNext = pstNode;
        pstHead->pstLast = pstNode;
    }
    return;
}
STQ_NODE_S *LIST_STQ_DelHead(STQ_HEAD_S *pstHead)
{
    STQ_NODE_S *pstTmp = NULL;
    if (pstHead->pstFirst == pstHead->pstLast)
    {
        if (pstHead->pstFirst == NULL)
        {   
            /* 空 */
            pstTmp = NULL;
        }
        else 
        {
            /* 说明只有一个元素 */
            pstTmp = pstHead->pstFirst;
            pstHead->pstFirst = NULL;
            pstHead->pstLast = NULL;

        }
    }
    else
    {
        pstTmp = pstHead->pstFirst;
        pstHead->pstFirst = pstTmp->pstNext;
    }
    return pstTmp;
}

VOID LIST_DL_Init(DL_HEAD_S *pstHead)
{
    assert(NULL != pstHead);
    pstHead->pstFirst = NULL;
    return;
}
VOID LIST_DL_AddHead(DL_HEAD_S *pstHead, DL_NODE_S *pstNode)
{
    assert(NULL != pstHead);
    assert(NULL != pstNode);
    if (!pstHead->pstFirst)
    {
        pstHead->pstFirst = pstNode;
        pstNode->ppstPrev = &pstHead->pstFirst;
    }
    else
    {
        pstNode->pstNext = pstHead->pstFirst;
        pstNode->ppstPrev = &pstHead->pstFirst;
        pstHead->pstFirst->ppstPrev = &pstNode;
        pstHead->pstFirst = pstNode;
    }
    return;
}
VOID LIST_DL_DelNode(DL_NODE_S *pstNode)
{
    assert(NULL != pstNode);
    if (NULL == pstNode->pstNext)
    {
        (*pstNode->ppstPrev)->pstNext = NULL;
    }
    else 
    {
        (*pstNode->ppstPrev)->pstNext = pstNode->pstNext;
        pstNode->pstNext->ppstPrev = pstNode->ppstPrev;
    }
    return;
}
VOID LIST_DL_FREEALL(DL_HEAD_S *pstHead, VOID (*pFunc)())
{
    DL_NODE_S *pstCurNode = NULL;
    DL_NODE_S *pstTemNode = NULL;

    /* 这里出现free空指针 */
    LIST_DL_FOREACH_SAFE(pstHead, pstCurNode, pstTemNode)
    {
        pFunc(pstCurNode);
        pstCurNode = NULL;
    }
    return;

}
VOID LIST_DTQ_Init(DTQ_HEAD_S *pstHead)
{
    assert(NULL != pstHead);
    pstHead->stHead.pstNext = &pstHead->stHead;          /* 这里要检查是否写正确了 */
    pstHead->stHead.pstPrev = &pstHead->stHead;
    return;
}
